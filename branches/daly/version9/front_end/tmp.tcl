bind .workspace.mbar.testing.menu  <Motion> { 
    
    set dif [expr [%W yposition 2]-[%W yposition 1]];
    set n [expr %y - [%W yposition 0]]
    set num [expr $n / $dif]

    set y1 [%W yposition 1]
    set y2 [%W yposition 2]
    set y3 [%W yposition 3]
    set y4 [%W yposition 4]
    set y5 [%W yposition 5]

    if { $y5 <= %y } {
	%W postcascade 5
    } elseif { $y4 <= %y } {
	%W postcascade 4
    } elseif { $y3 <= %y } {
	%W postcascade 3
    } elseif { $y2 <= %y } {
	%W postcascade 2
    } elseif { $y1 <= %y } {
	%W postcascade 1
    } else {
	%W postcascade 0
    }
    
   # puts "%y $y1 $y2 $y3 $y4" 
}
