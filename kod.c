#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void createChildren(int level, int depth, int check) {
    int value = level % 2 + 1;
    pid_t children_pids[value];
    
    for (int i = 0; i < value; ++i) {
    if(check == 0){
    	int variable = (level % 4 == 1) ? 0 : 1;
    	if(level%2 == 0)
    		variable = 0;
    	else
    		variable = (variable + i) % 2;
        pid_t pid = fork();
        //printf("This pid: %d check: %d level: %d, I: %d\n", getpid(), check, level, i);
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            printf("Level %d Child process %d pid: %d ppid: %d\n", level, i + 1, getpid(), getppid());
            if (level >= depth && i == 0) {
             	//printf("PS pid: %d \n", getpid());
                execlp("ps", "-u kacper", "-l", "--forest", NULL);
                perror("execlp");
                exit(EXIT_FAILURE);
            } else {
            	printf("Level process pid: %d  checkNew: %d\n", getpid(), variable);
                createChildren(level + 1, depth, variable); // (level+i)%2
                exit(EXIT_SUCCESS);
            }
        } else {
            children_pids[i] = pid; // Store child PID
        }
        }
    }
    
    // Parent process waits for its children to finish
    for (int i = 0; i < value; ++i) {
        waitpid(children_pids[i], NULL, 0);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <depth>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int depth = atoi(argv[1]) - 1;
    printf("Level 0 Root process pid: %d \n", getpid());
    if(depth > 1) {
    	createChildren(1, depth, 0);
    }
    return 0;
}