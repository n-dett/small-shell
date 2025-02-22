#include "assignment4.h"

int main() {
    struct commandLine *currentCommand;
    int zero = 0;
    int* exitStatus = &zero;
    int* signalNum = &zero;
    bool falseValue = false;
    bool* termBySignal = &falseValue;

    int backgroundPids[100] = {0};
    int bgPidTerminated = 0;
    int bgPidStatus;


    while(true) {
        // Parse the entered command
        currentCommand = parse_input();

        if(!currentCommand) {
            // If currentCommand is NULL
            continue;
        } else if(!strcmp(currentCommand->argv[0], "exit")) {
            // If current command is "exit", then exit the process
            exit(*exitStatus);
        } else if(!strncmp(currentCommand->argv[0], "cd", 2)) {
            // If current command is "cd"
            cdCommand(currentCommand);
        } else if(!strncmp(currentCommand->argv[0], "status", 6)) {
            statusCommand(*exitStatus, *signalNum, *termBySignal);
        } else {
            // If not a built-in command, start a new process
            newProcess(currentCommand, exitStatus, termBySignal, signalNum, backgroundPids);
        } 


        // Check whether a bg child process has finished
        for(int i = 0; i < 100; i++) {
            // If the pid isn't 0, check to see if it has terminated
            if(backgroundPids[i]) {
                // If the pid is terminated, status is not 0
                bgPidTerminated = waitpid(backgroundPids[i], &bgPidStatus, WNOHANG);

                // If the background process has terminated, print it and remove from array
                if(bgPidTerminated){
                    if(WIFEXITED(bgPidStatus)) {
                        // If process terminated normally
                        *termBySignal = false;
                        *exitStatus = WEXITSTATUS(bgPidStatus);
                        printf("background pid %d is done: exit value %d\n", backgroundPids[i], *exitStatus);
                        fflush(stdout);
                    } else {
                        // If process was terminated by a signal
                        *termBySignal = true;
                        *signalNum = WTERMSIG(bgPidStatus);
                        printf("background pid %d is done: terminated by signal %d\n", backgroundPids[i], *signalNum);
                        fflush(stdout);                        
                    }

                    // Remove pid from array
                    backgroundPids[i] = 0;
                }
            }
        }
    }

    
    return EXIT_SUCCESS;
}
