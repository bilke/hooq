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

PROGRAM=${1:-hooked/hooked}
cd $(dirname "$PROGRAM")
gdb ./$(basename "$PROGRAM") -x $SCRIPT
rm $SCRIPT
