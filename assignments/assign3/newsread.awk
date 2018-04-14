#!/usr/bin/awk -f

BEGIN {
	printf("%45s\n\n", "News Reader Summary")
	printf("%-16s%-16s%-16s%-16s%-16s\n\n", "", "lonestar", "runner", "ringer", "rings")
	token = "lonestar runner ringer rings"
	split(token, tokens, " ")
	for (i = 1; i <= length(tokens); i++) {
		articles[tokens[i]] = 0
		groups[tokens[i]] = 0
		cs2413[tokens[i]] = 0
		cs2413d[tokens[i]] = 0
		usertime[tokens[i]] = 0
	}
}

NR == 1 { t_start = $1 " " $2 " " $3 }

$7 == "exit" {
	if ($6 == "lonestar.jpl.utsa.edu") {
		articles["lonestar"] += $9
		groups["lonestar"] += $11
	} else if ($6 == "runner.jpl.utsa.edu") {
		articles["runner"] += $9
		groups["runner"] += $11
	} else if ($6 == "ringer.cs.utsa.edu") {
                articles["ringer"] += $9
                groups["ringer"] += $11
	} else if ($6 ~ /ring[0-4][0-9].cs.utsa.edu/) {
		articles["rings"] += $9
		groups["rings"] += $11
	}
}

$7 == "group" {
	if ($8 == "utsa.cs.2413") {
		if ($6 == "lonestar.jpl.utsa.edu") cs2413["lonestar"] += $9
        	else if ($6 == "runner.jpl.utsa.edu") cs2413["runner"] += $9
        	else if ($6 == "ringer.cs.utsa.edu") cs2413["ringer"] += $9
        	else if ($6 ~ /ring[0-4][0-9].cs.utsa.edu/) cs2413["rings"] += $9
	} else if ($8 == "utsa.cs.2413.d") {
		if ($6 == "lonestar.jpl.utsa.edu") cs2413d["lonestar"] += $9
        	else if ($6 == "runner.jpl.utsa.edu") cs2413d["runner"] += $9
        	else if ($6 == "ringer.cs.utsa.edu") cs2413d["ringer"] += $9
        	else if ($6 ~ /ring[0-4][0-9].cs.utsa.edu/) cs2413d["rings"] += $9
	}

}

$7 == "times" {
	if ($6 == "lonestar.jpl.utsa.edu") usertime["lonestar"] += $13
        else if ($6 == "runner.jpl.utsa.edu") usertime["runner"] += $13
        else if ($6 == "ringer.cs.utsa.edu") usertime["ringer"] += $13
        else if ($6 ~ /ring[0-4][0-9].cs.utsa.edu/) usertime["rings"] += $13
}

END {
	printf("%-16s%-16s%-16s%-16s%-16s\n", "Articles:", articles["lonestar"], articles["runner"], articles["ringer"], articles["rings"])
	printf("%-16s%-16s%-16s%-16s%-16s\n", "Groups:", groups["lonestar"], groups["runner"], groups["ringer"], groups["rings"])
	printf("%-16s%-16s%-16s%-16s%-16s\n", "Cs2413:", cs2413["lonestar"], cs2413["runner"], cs2413["ringer"], cs2413["rings"])
	printf("%-16s%-16s%-16s%-16s%-16s\n", "Cs2413.d:", cs2413d["lonestar"], cs2413d["runner"], cs2413d["ringer"], cs2413d["rings"])
	printf("%-16s%-16s%-16s%-16s%-16s\n", "User Time:", usertime["lonestar"], usertime["runner"], usertime["ringer"], usertime["rings"])
	print "\nStart Time = " t_start "\tEnd Time = " $1 " " $2 " " $3
}
