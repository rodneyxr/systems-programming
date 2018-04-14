#!/bin/sed -f
s///g
s/_//g
/^[A-Z]/s/\([A-Z]\)\1/\1/g
