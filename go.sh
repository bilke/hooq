#!/bin/sh
SCRIPT=$(mktemp)
cat > $SCRIPT <<EOF
break _start
run
call __dlopen("hook/libhook.so.1.0.0", 2)
call registerFredhookCallback()
c
EOF
gdb hooked/hooked -x $SCRIPT
rm $SCRIPT
