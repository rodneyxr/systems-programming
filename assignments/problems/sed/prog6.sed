#!/bin/sed -f
/ verification /!d
s/.* warning: \([^ ]*\): hostname \([^ ]*\).*/\1\t\2/
