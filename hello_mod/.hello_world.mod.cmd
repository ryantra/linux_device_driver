savedcmd_/home/ubuntu/ldd/hello_mod/hello_world.mod := printf '%s\n'   hello_world.o | awk '!x[$$0]++ { print("/home/ubuntu/ldd/hello_mod/"$$0) }' > /home/ubuntu/ldd/hello_mod/hello_world.mod
