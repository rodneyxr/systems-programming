BEGIN {
	# array of names to match order of your output
	sortedNames = "daven917 chris rmckeel cunder rebates edvargas shadow dvorak wkuechle wynng cstaples pansut0 mustang alcestys tweety kiri"
	split(sortedNames, users, " ")

}

/User-Name/ { user = $3; gsub(/"/, "", user); username[user] = user }
/Acct-Status-Type/ { if ($3 ~ /Stop/) sessions[user]++ }
/Acct-Session-Time/ { sessionTime[user] += $3 }
/Acct-Input-Packets/ { inputPackets[user] += $3 }
/Acct-Output-Packets/ { outputPackets[user] += $3; user = "" }

END {
	for (i = 0; i < length(users); i++) {
		user = users[i]
		print "User:			", username[user] ? "\"" username[user] "\"" : username[user]
		print "Number of Sessions:	", sessions[user]
		print "Total Connect Time:	", sessionTime[user]
		print "Input Bandwidth Usage:	", inputPackets[user]
		print "Output Bandwidth Usage:	", outputPackets[user]
		print ""
		
	}
}
