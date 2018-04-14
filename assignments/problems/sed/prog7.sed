#!/bin/sed -f
/Invalid user/!d
s/.*: Invalid user //
s/ from / /
