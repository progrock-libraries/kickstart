#!/bin/bash
TMPFILE=`mktemp /tmp/run-in-terminal.XXX`
echo "bash -c \"$@\";" $'read -p "\e[90mPress Enter to continue:\e[0m " dummy' >$TMPFILE
chmod +x $TMPFILE
gnome-terminal -t `basename "$@"` -- $TMPFILE

