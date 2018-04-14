#!/bin/sed -f

# 1: replace every "\" with "\verb+\+"
s/\\/\\verb+\\+/g

# 2: replace every "%" with "\%"
s/%/\\%/g

# 3: replace every "^" with "\^"
s/\^/\\^/g

# 4: replace every "--" with "-\hspace{.01cm}-"
s/--/-\\hspace{.01cm}-/g

# 5: LaTeX file must start with these two lines
1i\
\\documentstyle[11pt]{article}\
\\begin{document}

# 6: center the title (first line of the ascii file)
1s/.*/\\begin{center} {\\bf\
&\
} \\end{center}/

# 7: surround description list with begin and end
1a\
\\begin{description}
$a\
\\end{description}

# 8: surround every header line with "\item[" and "] \hfill \\"
s/^[A-Z].*/\\item[&] \\hfill \\\\/

# 9: first non-blank char is either a "+" or "-" must terminate with " \\"
s/^[ \t]*[-+].*/& \\\\/

# 5: LaTeX file must end with this line
$a\
\\end{document}
