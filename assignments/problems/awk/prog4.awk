BEGIN {
	OFS = "\t"
	for (i = 0; i < 5; i++) range[i] = 0 
}

/Ascend-Data-Rate/ {
	if ($3 >= 0 && $3 <= 14400) range[0]++
	else if ($3 >= 14401 && $3 <= 19200) range[1]++
	else if ($3 >= 19201 && $3 <= 28800) range[2]++
	else if ($3 >= 28801 && $3 <= 33600) range[3]++
	else if ($3 > 33600) range[4]++
}

END {
	print "0-14400	", range[0]
	print "14401-19200", range[1]
	print "19201-28800", range[2]
	print "28801-33600", range[3]
	print "above 33600", range[4]
}
