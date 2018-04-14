/^[^ \t]/ { time = $4 }

/User-Name/ { user = $3; gsub(/"/, "", user) }

/Acct-Status-Type/ { type = ($3 ~ /Stop/) }

/Acct-Session-Id/ { sessionId = $3; gsub(/"/, "", sessionId) }

/Acct-Session-Time/ { sessionTime = $3 }

/Ascend-First-Dest/ { firstDest = $3 }

/Framed-Address/ { 
	framedAddress = $3
	
	if (type) {
		print "User:			" user
		print "Session ID:		" sessionId
		print "Framed Address:		" framedAddress
		print "Disconnect Time:	" time
		print "Session Time:		" sessionTime
		print "First Address:		" firstDest
		print ""
	}

	user = sessionId = framedAddress = time = sessionTime = firstDest = ""
}
