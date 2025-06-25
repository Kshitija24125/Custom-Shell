#include "../include/shell.h"
#include <sys/stat.h>  
#include <fcntl.h>   
#include <limits.h> 

#ifndef PATH_MAX
#define PATH_MAX 4096  
#endif

int shell_cd(char **args) {
    char *target = args[1];
    char *oldpwd = getenv("PWD");
    char cwd[PATH_MAX];
    
    // Get current directory
    if (!getcwd(cwd, sizeof(cwd))) {
        fprintf(stderr, "cd: %s\n", strerror(errno));
        return 1;
    }

    // Handle 'cd' without arguments
    if (!target) {
        target = getenv("HOME");
        if (!target) {
            fprintf(stderr, "cd: No home directory\n");
            return 1;
        }
    }
    // Handle 'cd -'
    else if (strcmp(target, "-") == 0) {
        target = getenv("OLDPWD");
        if (!target) {
            fprintf(stderr, "cd: No previous directory\n");
            return 1;
        }
        printf("%s\n", target);
    }

    // Build full path for relative paths
    if (target[0] != '/') {
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", cwd, target);
        
        if (chdir(full_path) == 0) {
            setenv("OLDPWD", oldpwd, 1);
            setenv("PWD", full_path, 1);
            return 1;
        }
    }

    // Try direct path
    if (chdir(target) == 0) {
        setenv("OLDPWD", oldpwd, 1);
        
        // Update PWD with canonical path
        char new_pwd[PATH_MAX];
        if (getcwd(new_pwd, sizeof(new_pwd))) {
            setenv("PWD", new_pwd, 1);
        }
        return 1;
    }

    fprintf(stderr, "cd: %s: %s\n", target, strerror(errno));
    return 1;
}

int shell_pwd(char **args) {

    (void)args;  
    char cwd[1024];
    printf("%s\n", getcwd(cwd, sizeof(cwd)));
    return 1;
}

int shell_exit(char **args) {

    (void)args;  
    return 0;
}

int shell_help(char **args) {
    (void)args;  
    printf("Simple POSIX Shell\n");
    printf("Built-in commands: cd, pwd, exit, help,history ,touch ,head,tail,cat,mkdir,echo,chmod\n");
    return 1;
}

int handle_builtins(char **args) {
    struct builtin {
        char *name;
        int (*func)(char **);
    } builtins[] = {
        {"cd", shell_cd},
        {"pwd", shell_pwd},
        {"exit", shell_exit},
        {"help", shell_help},
        {"history", show_history},  
        {"touch", shell_touch},
        {"echo", shell_echo},
        {"mkdir", shell_mkdir},
        {"cat", shell_cat},
        {"head", shell_head},
        {"chmod", shell_chmod},
         {NULL, NULL},   


    };
    
    for (int i = 0; builtins[i].name; i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            return builtins[i].func(args);
        }
    }
    
    return 0;
}
// TOUCH - Create empty file
int shell_touch(char **args) {
    if (!args[1]) {
        fprintf(stderr, "touch: missing file operand\n");
        return 1;
    }
    int fd = open(args[1], O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("touch");
        return 1;
    }
    close(fd);
    return 1;
}
// ECHO - Print arguments
int shell_echo(char **args) {
    for (int i = 1; args[i]; i++) {
        printf("%s ", args[i]);
    }
    printf("\n");
    return 1;
}

// MKDIR - Create directory
int shell_mkdir(char **args) {
    if (!args[1]) {
        fprintf(stderr, "mkdir: missing operand\n");
        return 1;
    }
    if (mkdir(args[1], 0755) == -1) {
        perror("mkdir");
        return 1;
    }
    return 1;
}
// CAT - Concatenate files
int shell_cat(char **args) {
    if (!args[1]) {
        char buf[1024];
        while (fgets(buf, sizeof(buf), stdin)) {
            printf("%s", buf);
        }
        return 1;
    }

    for (int i = 1; args[i]; i++) {
        FILE *fp = fopen(args[i], "r");
        if (!fp) {
            perror("cat");
            continue;
        }
        char ch;
        while ((ch = fgetc(fp)) != EOF) {
            putchar(ch);
        }
        fclose(fp);
    }
    return 1;
}
// HEAD - Show first 10 lines
int shell_head(char **args) {
    int lines = 10;
    if (args[1] && strcmp(args[1], "-n") == 0 && args[2]) {
        lines = atoi(args[2]);
        args += 2;
    }

    FILE *fp = fopen(args[1] ? args[1] : "-", "r");
    if (!fp) {
        perror("head");
        return 1;
    }

    char buf[1024];
    for (int i = 0; i < lines && fgets(buf, sizeof(buf), fp); i++) {
        printf("%s", buf);
    }
    fclose(fp);
    return 1;
}



// CHMOD - Change file permissions
int shell_chmod(char **args) {
    if (!args[1] || !args[2]) {
        fprintf(stderr, "Usage: chmod <mode> <file>\n");
        return 1;
    }
    mode_t mode = strtol(args[1], NULL, 8);
    if (chmod(args[2], mode) == -1) {
        perror("chmod");
        return 1;
    }
    return 1;
}