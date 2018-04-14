BEGIN {
	packetsIn = 0
	packetsOut = 0
}

/Acct-Input-Packets/ { packetsIn += $3 }
/Acct-Output-Packets/ { packetsOut += $3 }

END {
	print "Total Input Bandwidth Usage  =", packetsIn, "packets"
	print "Total Output Bandwidth Usage =", packetsOut, "packets"
}
