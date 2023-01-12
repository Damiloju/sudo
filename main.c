#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "chkpw.h"

int setUserIdentity(uid_t uid);

int main(int argc, char *argv[])
{
    bool password_is_correct = false;
    int numberOfPasswordTries = 0;
    int uid = -1;
    int setUserIdentityReturn = 0;

    while (!password_is_correct && numberOfPasswordTries < 3)
    {
        password_is_correct = chkpw();
        numberOfPasswordTries++;
    }

    setUserIdentityReturn = setUserIdentity(uid);

    if (setUserIdentityReturn == -1)
    {
        perror("Failed to set user identity");
        return 1;
    }

    return 0;
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