#!/bin/sh
CWD=$(pwd)
SCRIPT=$(mktemp)
cat > $SCRIPT <<EOF
break _start
run
call __dlopen("$CWD/hook/libhook.so.1.0.0", 2)
call registerFredhookCallback()
c
EOF
cd ~/git/desktop/b/
gdb ./mendeleydesktop.x86_64 -x $SCRIPT
rm $SCRIPT
