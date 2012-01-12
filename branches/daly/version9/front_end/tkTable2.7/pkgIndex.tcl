if {[catch {package require Tcl 8.1}]} return
package ifneeded Tktable 2.7 "package require Tk 8.1; [list load [file join $dir Tktable.so.2.7] Tktable]"
