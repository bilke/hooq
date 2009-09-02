#!/bin/sh
NOTICE_HEADER="is free software; you can redistribute it"
TMPFILE=$(mktemp)
for file in $(find .. -name *.cpp -o -name *.h); do
	if ! grep -q "$NOTICE_HEADER" "$file"; then
		echo "/*" > $TMPFILE
		cat COPYING.HEADER >> $TMPFILE
		echo "*/" >> $TMPFILE
		cat "$file" >> $TMPFILE
		cat $TMPFILE > "$file"
	fi
done
rm $TMPFILE
