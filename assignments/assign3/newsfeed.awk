#!/usr/bin/awk -f

BEGIN {	
	OFS = "\t"
	printf("%48s\n\n\n", "Incoming News Feed Summary")
}

NR == 1 { t_start = $1 " " $2 " " $3 }

{
	if ($4 == "+" || $4 == "j") accepted[$5] += 1
	else if ($4 == "-") rejected[$5] += 1
	else if ($4 == "c") {
		accepted[$5] += 1
		canceled[$5] += 1
	}
	
	attempted[$5] += 1
}

END { 
	printf("%-16s%-16s%-16s%-16s\n", "", "accepted", "rejected", "canceled")
	
	for (K in attempted) {
		if (!accepted[K]) accepted[K] = 0
		if (!rejected[K]) rejected[K] = 0
		if (!canceled[K]) canceled[K] = 0	
	}

	printf("%-16s%-16s%-16s%-16s\n", "swrinde:", accepted["swrinde"], rejected["swrinde"], canceled["swrinde"])
	printf("%-16s%-16s%-16s%-16s\n", "news.cais.net:", accepted["news.cais.net"], rejected["news.cais.net"], canceled["news.cais.net"])
	printf("%-16s%-16s%-16s%-16s\n", "?:", accepted["?"], rejected["?"], canceled["?"])

	print "\nStart Time = " t_start, "End Time = " $1 " " $2 " " $3
}
