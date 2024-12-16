savedcmd_/home/ubuntu/ldd/chardev/chardev.mod := printf '%s\n'   chardev.o | awk '!x[$$0]++ { print("/home/ubuntu/ldd/chardev/"$$0) }' > /home/ubuntu/ldd/chardev/chardev.mod
