    graph .g1 
    for { set i -10 } { $i <= 10 } { incr i } {
	lappend x $i
	lappend y [expr {$i * $i}]
    }
    .g1 element create "e1" -label "One" -xdata $x -ydata $y -symbol none
    pack .g1
