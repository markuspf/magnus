
proc printSummary { firstColumn secondColumn} {
   global tcl_path
   if {[llength $firstColumn]!=[llength $secondColumn]} {
		alert "sorry, Column lengths are not equal. contact with the developer!"
                return
      }
   if { [catch {exec cp $tcl_path/template/summaryTemplate.ps $tcl_path/template/summary.ps} ] } {
		alert "sorry, can not copy the summaryTemplate.ps. contact with the developer!"
                return
      }
   set outF [open "$tcl_path/template/summary.ps" "a"]
   printColumns $outF $firstColumn $secondColumn
   puts $outF "pagelevel restore"
   puts $outF "showpage"
   puts $outF "%%EOF"
   close $outF

   if { [catch {exec lpr $tcl_path/template/summary.ps} ] } {
      alert "sorry, can not print the result. please check your print configure!"
      }
   
}

proc printColumns {outF firstColumn secondColumn} {
   for {set i 0 } { $i < [llength $firstColumn] } {incr i 1} {
      for {set j 0 } { $j < 2 } {incr j 1} {
            set x0 [expr 10+110*$j]
            set y0 [expr 740-16*$i]
	    puts $outF "gsave 0.7 setlinewidth "
	    puts $outF " $x0 $y0 moveto "
	    puts $outF "110 0 rlineto 0 -16 rlineto -110 0 rlineto closepath  stroke "
	    puts $outF "[expr $x0+10] [expr $y0-12] moveto"
	    puts $outF "12 f0 "
            if {$j==0} {
        	    puts $outF "([lindex $firstColumn $i]) show"
            } else {
        	    puts $outF "([lindex $secondColumn $i]) show"
            }
      }
   }
}

proc printText {logText} {
   global tmp_dir
   set textStr [$logText get 0.0 end]
   set testLen [string length $textStr]
   set textList [list]
   while { [string length $textStr] > 0} {
      set b [string first "\n" $textStr]
      if { $b > -1 } {
	    lappend textList [string range $textStr 0 $b]
	    incr b
	    set textStr [string range $textStr $b end]
       }
   }
   set outF [open "$tmp_dir/resultFromStats.prt" "w"]
   foreach  l $textList {
      if { [string length $l] > 76 } {
	 set tmpL $l
	 while { [string length $tmpL] > 76 } {
	    set i [string last " " $tmpL 76]
	    puts $outF [string range $tmpL 0 $i]
            incr i
            set tmpL [string range $tmpL $i end]
	 }
	 set nl [string last "\n" $tmpL]
	 puts $outF [string replace $tmpL $nl $nl]
       } else {
	  set nl [string last "\n" $l]
	  puts $outF [string replace $l $nl $nl]
       }
   }
   close $outF
   if { [catch { exec mpage -1 -P $header $tmp_dir/resultFromStats.prt }]} {
      if { [catch {exec lpr $tmp_dir/resultFromStats.prt} ] } {
		alert "sorry, can not print the result. please check your print configure!"
      }
   } 
}

