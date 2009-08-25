#!/bin/sh
PROGRAM=${1:-hooked/hooked}
LOGFILE=${2:-/dev/stdin}
BREAKPOINT=${3:-QApplication::exec()}

CWD=$(pwd)
SCRIPT=$(mktemp)

cat > $SCRIPT <<EOF
break _start
run
break ${BREAKPOINT}
continue
call __dlopen("$CWD/player/libplayer.so.1.0.0", 2)
call setFredhookLogFile("$LOGFILE")
call startFredhookPlayback()
continue
quit
EOF

cat $SCRIPT

cd $(dirname "$PROGRAM")
gdb ./$(basename "$PROGRAM") -x $SCRIPT
rm $SCRIPT