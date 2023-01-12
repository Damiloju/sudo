#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "chkpw.h"
#include "runCommand.h"

#define EXIT_FAILURE 1

int setUserIdentity(uid_t uid);

int main(int argc, char *argv[])
{
    bool password_is_correct = false;
    int opt, user, numberOfPasswordTries, setUserIdentityReturn;
    numberOfPasswordTries = 0;
    setUserIdentityReturn = 0;
    user = -1;

    /**
     * Parse command line arguments
     * -h: help
     * -u: user uid
     */
    while ((opt = getopt(argc, argv, "hu:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            fprintf(stderr, "Usage: %s [-h help] [-u user] uid [command]\n", argv[0]);
            return 0;
        case 'u':
            user = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-h help] [-u user] uid [command]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    // Here we change the arguments so that the array starts from the
    // point after all the options have been parsed.
    argc -= optind;
    argv += optind;

    // `argc` will be 0 if there are no additional arguments after the
    // options or there were no options as well.
    if (argc == 0)
    {
        fprintf(stderr, "Usage: %s [-h help] [-u user] uid [command]\n", argv[0]);
        return EXIT_FAILURE;
    }

    while (!password_is_correct && numberOfPasswordTries < 3)
    {
        password_is_correct = chkpw();
        numberOfPasswordTries++;
    }

    if (!password_is_correct)
    {
        fprintf(stderr, "Incorrect password\n");
        return 1;
    }

    setUserIdentityReturn = setUserIdentity(user);

    if (setUserIdentityReturn == -1)
    {
        perror("Failed to set user identity");
        return 1;
    }

    setenv("PATH", "/usr/local/bin:/usr/bin:/bin", 1); // set path to safe value leaving out sbin

    return runCommand(argc, argv);
}

/**
 * @param uid: user id to set the identity to
 * @return: 0 if success, -1 if failed
 * Set user identity to root or the user provided in the command line
 * */
int setUserIdentity(uid_t uid)
{
    if (uid == -1) // check if uid is default value
    {
        return setuid(0);
    }

    return setuid(uid);
}