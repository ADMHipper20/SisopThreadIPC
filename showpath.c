#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>

#define BUFFER_SIZE 1024

int main() {
    int fd[2];
    pid_t pid;

    char *argv1[] = {"pwd", NULL};
    execv("/usr/bin/pwd", argv1);

	if (pipe(fd)==-1) 
	{ 
		fprintf(stderr, "Pipe Gagal"); 
		return 1; 
	}

    pid = fork();

	if (pid < 0) 
	{ 
		fprintf(stderr, "fork Gagal"); 
		return 1; 
	}

    if (pid > 0) { 
		close(fd[0]);
        char pathDir[BUFFER_SIZE];
        if (getcwd(pathDir, sizeof(pathDir) == NULL)){
            perror("getcwd gagal");
            return 1;
        }
		close(fd[1]); 

		// Wait for child to send a string
		wait(NULL); 

		write(fd[1], pathDir, strlen(pathDir) + 1);
		close(fd[1]);
	}

    else {
        close(fd[1]); // Close writing end of first pipe 

		// Read a string using first pipe 
		char pathDir[BUFFER_SIZE];
        read(fd[0], pathDir, sizeof(pathDir));
        close(fd[0]);

        printf("Current Path/Dir: %s\n", pathDir);
        DIR *dir = opendir(pathDir);
        if (dir == NULL) {
            perror("Direktori tidak dapat dibuka/ditemukan!");
            return 1;
        }

        struct dirent *entry;
        int count = 0;
        printf("3 File Pertama:\n");
        while ((entry = readdir(dir)) != NULL && count < 3)
        {
            if (entry->d_type == DT_REG) {
                printf("%s\n", entry->d_name);
                count++;
            }
        }
        
        closedir(dir);
		exit(0);
    }
}