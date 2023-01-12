/**
 * @license SPDX-License-Identifier: MIT
 * @file chkpw.c
 * @brief Check password of current user
 * @details
 * This program checks the password of the current user
 * by reading the /etc/passwd and /etc/shadow files.
 * most of the code copied/modified from  https://gitlab.cs.ttu.ee/henvas/its8020-materials/-/tree/master/assignments/chkpw
 */
#define _DEFAULT_SOURCE
#include <pwd.h>
#include <shadow.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

#include "chkpw.h"

static struct passwd *getCurrentUserPassword(uid_t uid);
static struct spwd *getShadowPassword(char *hash_real, char *name);
static char *getPasswordFromUser(const char *prompt);
static char *getHashedPassword(char *pass_try, char *hash_real);
static bool comparePasswords(char *user_entered_hashed_password, char *user_password_hased_password);

bool chkpw(void)
{

    /**
     * Even if the program is running as setuid root, getuid() will
     * return the normal user's ID that ran the program.
     * To get the "effective UID" there is another function geteuid()
     * */
    uid_t uid = getuid();

    // Read the /etc/passwd file for the current user's information
    struct passwd *pwd = getCurrentUserPassword(uid);
    char *name = pwd->pw_name;
    char *hash_real = pwd->pw_passwd;

    hash_real = getShadowPassword(hash_real, name)->sp_pwdp;

    char *pass_try = getPasswordFromUser("Your password: ");

    char *hash_try = getHashedPassword(pass_try, hash_real);

    return comparePasswords(hash_try, hash_real);
}

static struct passwd *getCurrentUserPassword(uid_t uid)
{
    // Read the /etc/passwd file for the current user's information
    static struct passwd *pwd;
    pwd = getpwuid(uid);

    if (pwd == NULL)
    {
        fprintf(stderr, "Can't get passwd entry for UID %u\n", uid);
        _exit(1);
    }

    return pwd;
}

static struct spwd *getShadowPassword(char *hash_real, char *name)
{
    // If the password field in the /etc/passwd file is 'x' that means
    // that the password (hashed) is stored in /etc/shadow, instead.
    // Different functions and structures are used for that file.
    // At this point we should fail without setuid root permissions.
    if (strcmp(hash_real, "x") == 0)
    {
        static struct spwd *shadow;
        shadow = getspnam(name);

        if (shadow == NULL)
        {
            fprintf(stderr, "Can't get shadow entry for user '%s'\n", name);
            _exit(1);
        }

        return shadow;
    }
}

static char *getPasswordFromUser(const char *prompt)
{
    // Read from the terminal the password to check from the user.
    // This function is considered deprecated but it's simple
    // enough for our purpose.
    static char *pass_try;
    pass_try = getpass(prompt);

    if (pass_try == NULL)
    {
        fprintf(stderr, "Can't get password\n");
        _exit(1);
    }

    return pass_try;
}

static char *getHashedPassword(char *pass_try, char *hash_real)
{
    // The crypt() function takes the plain text password and
    // some data in the second argument that specifies the
    // hash algorithm and random salt to use.
    // This data can be easily read from the existing hash
    // that we got from passwd/shadow, it is prepended
    // to the hashed password.
    // It will also be prepended to the output.
    static char *hash_try;

    hash_try = crypt(pass_try, hash_real);

    if (hash_try == NULL)
    {
        fprintf(stderr, "crypt() failed\n");
        _exit(1);
    }

    return hash_try;
}

static bool comparePasswords(char *user_entered_hashed_password, char *user_password_hased_password)
{
    // Since the output of crypt() will be in the same
    // format as in the system password files we can
    // just compare them to see if the typed password was
    // correct.
    if (strcmp(user_entered_hashed_password, user_password_hased_password) == 0)
    {
        printf("OK\n");
        return true;
    }
    else
    {
        printf("ERR: Wrong password\n");
        return false;
    }
}
