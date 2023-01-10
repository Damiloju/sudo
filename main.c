#include <stdbool.h>

#include "chkpw.h"

int main(int argc, char *argv[])
{
    bool password_is_correct = false;

    while (!password_is_correct)
    {
        password_is_correct = chkpw();
    }

    return 0;
}