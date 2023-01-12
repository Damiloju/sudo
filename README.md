# Program: sudo
This is a program that allows users to run commands as root or any other user. The goal is to be a mini `su` linux command. This program is written in C and uses the `setuid` system call to change the user ID of the process. This program is meant to be used as a learning tool/project for C class in [`TALTECH`](https://taltech.ee/)

# Command line options:

1. `-h` for help/usage
2. `-u` `uid` to select user to run the command as

# Building and running
Use Make to build.

There is a target `suid` that you can use to make the executable `setuid root` which is required for the proper permissions to access those files.

```sh
make
sudo make suid
./sudo
```


# Example commands
- `./sudo -u 0 ls -l`
- `./sudo -u 0 apt-get update`
- `./sudo -u 1000 chmod +x ${filename}`



# License
[MIT](LICENSE)
