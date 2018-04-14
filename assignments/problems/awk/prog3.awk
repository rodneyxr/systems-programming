/Acct-Session-Time/ { totalTime += $3 }
END { print "Total User Session Time =", totalTime }
