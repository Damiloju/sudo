/**
 * This program demonstrates how to run a program in a child process
 * and wait for it to finish.
 * It also shows how to check the exit status of the child process
 * and return an exit code that reflects the success or failure of the
 * child process.
 * Code picked from https://gitlab.cs.ttu.ee/henvas/its8020-materials/-/tree/master/assignments/success
 * */
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int runCommand(int argc, char *argv)
{

    // After this the program is running in two processes, the `if`
    // below selects which branch we follow.
    pid_t pid = fork();

    if (pid == 0)
    {
        // This is the child process

        // `argv` should contain the command to run and its arguments.
        // By convention we have to give the program's own name as well
        // but this is easily available as the first element of the array.
        // execvp requires that the array end with a NULL pointer but
        // this is always the case with the argv passed to `main()` as well.
        execvp(argv[0], argv);

        // If `execvp` was successful, it will never return because
        // the running program has been replaced by the new program.
        // If it does return then there was some problem executing the
        // requested command.
        fprintf(stderr, "Command \"%s\" not found.\n", argv[0]);

        // Returning an error code here will give an exit code that is
        // nonzero to the waiting parent process below and indicate failure.
        return 1;
    }

    // This is the parent process.

    // We start waiting for the child process to finish.
    int status;
    waitpid(pid, &status, 0);

    // The `status` integer holds several pieces of information,
    // there are macros that can check for these conditions.
    // We are most interested in the fact that the program
    // exited in some way and that the code was successful (=0)
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        fprintf(stderr, "Program exited successfully\n");
        return 0;
    }

    fprintf(stderr, "Program didn't exit successfully\n");

    return WEXITSTATUS(status) || 1;
}
