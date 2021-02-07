# Detail — <br>**Running an executable by double-clicking in Ubuntu**

When the author explored double-clicking of modern g++-produced executables in the Nautilus file explorer in Gnome in Ubuntu, it turned out that “run a modern executable” functionality was not present by default (!), and *creating that functionality* involved some installations (which may or may not have been necessary) plus adding a file “[/usr/local/share/applications/run-in-terminal.desktop](examples/run-in-terminal/ubuntu/run-in-terminal.desktop)” that specified an “Open with” item that invoked a file “[/usr/local/bin/run-in-terminal.sh](examples/run-in-terminal/ubuntu/run-in-terminal.sh)” that, due to apparent bugs in `gnome-terminal`, dynamically created a “/tmp/” shell script to run the program, which script it in turn passed as “`--`”-command to `gnome-terminal`&hellip;

*File ([/usr/local/share/applications/run-in-terminal.desktop](examples/run-in-terminal/ubuntu/run-in-terminal.desktop)):*
~~~desktop
[Desktop Entry]
Encoding=UTF-8
Name=Run in terminal
Comment=Run a specified program in a console, with a stop at the end.
Exec=/usr/local/bin/run-in-terminal.sh %u
Terminal=true
Type=Application
Categories=Application;Utility;
StartupNotify=true
MimeType=application/x-sharedlib;application/x-executable;
~~~

*File ([/usr/local/bin/run-in-terminal.sh](examples/run-in-terminal/ubuntu/run-in-terminal.sh)):*
~~~sh
#!/bin/bash
TMPFILE=`mktemp /tmp/run-in-terminal.XXX`
echo "bash -c \"$@\";" $'read -p "\e[90mPress Enter to continue:\e[0m " dummy' >$TMPFILE
chmod +x $TMPFILE
gnome-terminal -t `basename "$@"` -- $TMPFILE
~~~

Disclaimer: the author usually works in Windows, and only dimly recalls some Unix stuff from the mid 1990’s, so there might be some much easier solution, perhaps even a single little behavior toggle configuration.

Also, for the same reason, the above quick-’n-dirty scripts may be very imperfect relative to the author’s general standards, e.g. as regards quoting.
