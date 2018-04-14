#!/bin/sed -f
/user=root$/!d
s/.*ruser=\([^ ]*\) rhost=.*$/\1/
