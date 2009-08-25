#!/bin/sh
PROGRAM=${1:-hooked/hooked}
LOGFILE=${2:-/dev/stdout}

CWD=$(pwd)
SCRIPT=$(mktemp)

cat > $SCRIPT <<EOF
break _start
run
break QCoreApplication::sendPostedEvents(QObject*,int)
continue
delete 2
call __dlopen("$CWD/hook/libhook.so.1.0.0", 2)
call setHooqLogFile("$LOGFILE")
call startHooqRecording()
continue
quit
EOF

cat $SCRIPT

cd $(dirname "$PROGRAM")
gdb ./$(basename "$PROGRAM") -x $SCRIPT
rm $SCRIPT

echo "Written event log to: ${LOGFILE}"
