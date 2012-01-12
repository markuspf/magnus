#!/usr/bin/wish -f

proc convert { iname pagenum } {

    set oname "tex/$iname.tex"

    set in [open $iname r]
    set out [open $oname w]

    puts $out {\documentclass[12pt]{article}}
    puts $out {\pagestyle{myheadings}}
    puts $out "\\markright\{$pagenum\}"
    puts $out {\begin{document}}
    puts $out ""

    # 'start' becomes 1 after skiping all that CVS stuff
    set start 0    

    # 'type' can be 'text' or 'link'
    set type text  

    # 'comment' is 1 when the current line is inside /* ... */
    set comment 0

    while { ![eof $in] } {
	#global start
	gets $in s
	set s [string trimleft $s]

	if { $start == 0 && [regexp {Check-in type:} $s] != 0 } { set start 1 }
	if { $start == 1} { 
	    
	    # Emphasize object names

	    while { [regexp -nocase {[ ]([gwhms][0-9]*)([ .,:;?!]|$)} $s seg name] } {
		regsub -nocase "$name" $seg "\$$name\$" newSeg
		set c [string index $name 0]
		if { $c == "W" } { regsub W $newSeg w newSeg }
		if { $c == "M" } { regsub M $newSeg m newSeg }
		regsub -nocase {[ ]([gwhms][0-9]*)([ .,:;?!]|$)} $s $newSeg s
	    }

	    regsub -all {<|>} $s "\$&\$" s
	    regsub -all {[*]} $s "$*$" s
	    if { [regexp {/\*} $s] } { set comment 1}
	    
	    # Six lines at the top that must be present
	    
	    if { [regsub {Check-in type:} $s \
		      { \makebox[2in][l]{\textbf{Check-in type:}} } s] != 0 } { 
	    } elseif { [regsub {Objects highlighted:} $s \
			    { \makebox[2in][l]{\textbf{Objects highlighted:}} } s ] != 0 } {
	    } elseif { [regsub {Other objects present:} $s \
			    { \makebox[2in][l]{\textbf{Other objects present:}} } s] != 0 } {
	    } elseif { [regsub {Name in the menu:} $s \
			    { \makebox[2in][l]{\textbf{Name in the menu:}} } s] != 0 } {
	    } elseif { [regsub {Problem type:} $s \
			    { \makebox[2in][l]{\textbf{Problem type:}} } s] != 0 } {
	    } elseif { [regsub {Help file:} $s \
			    { \makebox[2in][l]{\textbf{Help file:}} } s] != 0 } { }

	    # Titles
	    
	    if { [regsub {Fast checks:} $s \
		      {\vspace{5 mm}\begin{center} \Large\textbf{ Fast checks:} \end{center}} s] != 0 } {
	    } elseif { [regsub {Algorithms:} $s \
			    {\vspace{5 mm}\begin{center} \Large\textbf{Algorithms:} \end{center}} s] != 0 } {
	    } elseif { [regsub {Links:} $s {\hspace{8mm}{\textbf{Links:}}} s] != 0 } {
		set type link
		#set s "$s\n\\begin\{itemize\}"

		# Main body

	    } elseif { $type == "link" } {
		if { [regexp {^[ ]*[0-9]+} $s] != 0 } { 
		    set type text
		    #set s "\\end\{itemize\}\n$s"
		} elseif { [regexp -nocase {[a-z]} $s] } {
		    if { [regexp {^[ ]*((//)|(/\*))} $s] == 0 && !$comment } {
			set s "\\hspace\{11mm\}\$\\bullet\$ \{\\em $s\}"
		    }
		    #set s "\\item \{\\em $s\}"
		}
	    }
	    
	    if { [regexp {\*/} $s] } { set comment 0}

	    #if { [regexp -nocase {[a-z]|[0-9]} $s] } {
	    puts $out $s 
	    #puts $out ""
	    #}
	}
    }

    puts $out {\end{document}}

    close $in
    close $out  
}

set groups { \
		 {Abelian "Abelian groups" } \
		 {FP "Finitely Presented groups" } \
		 {FreeGroups "Free groups" } \
		 {Nilpotent "Nilpotent groups" } \
		 {FreeNilpotent "Free nilpotent groups"} \
		 {OR "One relator groups"} \
		 {MSC "Small cancellation groups" } \
		 {AProducts "Amalgamated products" } \
		 {HNNExtensions " HNN extensions"} \
	     }

set objects {\
		 {group "Algorithms involving the whole group"} \
		 {subgroup "Algorithms involving a subgroup" }\
		 {word "Algorithms involving a word" } \
		 {word_subgroup "Algorithms involving words and subgroups" } \
		 {word_word "Algorithms involving two elements"}\
		 {subgroup_subgroup "Algorithms involving two subgroups"} \
		 {map "Algorithms involving maps"} \
		 {homomorphism "Algorithms involving homomorphisms"}\
		 {subgroup_map "Algorithms involving maps and subgroups" }\
		 {word_map "Algorithms involving maps and words" } \
		 {word_homomorphism "Algorithms involving homomorphisms and words"}\
		 {equation "Equations"} \
		 {quadratic_equation "Quadratic equation" } \
		 {set "Algorithms involving sets" }\
		 {tuple "Algorithms involving tuples"}
	     }

# main body 

if { ![file isdirectory tex] } { exec mkdir tex }

set dir [pwd]
cd ..
set dir1 [pwd]
cd ..
set dir2 [pwd]
cd $dir 
regsub $dir1 $dir "" name
regsub "/" $name "" objectName
regsub $dir2 $dir1 "" name
regsub "/" $name "" groupName

set i 1
set groupInd -1
foreach group $groups {
    if { [lsearch $group $groupName] >= 0 } { 
	set groupInd $i 
	break
    }
    incr i
}

set i 1
set objectInd -1
foreach object $objects {
    if { [lsearch $object $objectName] >= 0 } { 
	set objectInd $i 
	break
    }
    incr i
}

if { $groupInd < 0 || $objectInd < 0 } {
    puts file1 "Bad index"
    exit
}

set files [glob *]
set files [lsort $files]
set i 1

foreach fileName $files {
    if { ![file isdirectory $fileName] } {

	convert $fileName "$groupInd-$objectInd-$i"
	puts file1 $fileName
	incr i
    }
}

cd tex
set out [open "print" w]

set files [glob *.tex]
foreach fileName $files {
    exec latex $fileName >@ file1
    regsub ".tex" $fileName "" fn
    puts $out "dvips tex/$fn"
}

close $out
exec chmod u+x print

exit