# Program: sudo
This is a program that allows users to run commands as root or any other user. The goal is to be a mini `su` linux command. This program is written in C and uses the `setuid` system call to change the user ID of the process. This program is meant to be used as a learning tool/project for C class in [`TALTECH`](https://taltech.ee/)

# Command line options:

1. `-h` for help/usage
2. `-u` to select user to run the command as

# Example commands
- `./main -u 0 ls -l`
- `./main -u 0 apt-get update`
- `./main -u 1000 chmod +x ${filename}`



# License
MIT
