#!/usr/bin/wish -f

set files [glob *.C]
set list ""

foreach fileName $files {
    
    global list
    
    set f [open $fileName r]
    set s [read $f]
    
    while { 1 } {
	
	if {[regexp {ProblemView[^:;"]+\([^;"]*\"([^"]*)\"[^;]*;} $s a b]} {
	
            set helpID ""           
            regexp {helpID *\( *\"([^"]*)\"} $a t helpID
            set string "$b :  $helpID.help"
            lappend list $string
            puts file1 $string
            regsub {ProblemView[^:;"]*\([^;"]*\"([^"]*)\"[^;]*;} $s "" s

	 } else {
	     break
	 }
    
    }
    close $f
}

exit