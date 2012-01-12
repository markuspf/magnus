
source [file join [file dirname [info script]] loadtable.tcl]
proc createTable { f row column defn } {
set t $f.table
table $t -rows $row -cols $column -cache 1 -selecttype row \
	-yscrollcommand "$f.sy set" \
	-xscrollcommand "$f.sx set" \
	-height 8 -colstretch unset -rowstretch unset \
	-autoclear 1 -colwidth 15
$t tag config unset -fg \#008811
for {set i 0} {$i<[llength $defn]} {incr i} {
  set strDef [lindex $defn $i]
  $t set col 0,$i $strDef
}

scrollbar $f.sy -command [list $t yview]
scrollbar $f.sx -command [list $t xview] -orient horizontal
grid $t    $f.sy     -sticky news
grid $f.sx            -sticky ew
}

proc getTableValue {t {type "column" } } {
  set row [$t cget -rows]
  set col [$t cget -cols]
  if {$type=="column"} {
    for {set i 0} {$i<$col} {incr i} {
      set value [$t get 0,$i $row,$i]
      set value [eval concat $value]
      if {[llength $value]>1} {
        lappend str $value
      } 
      if {[llength $value]==1} {
        set str "{ $value }"
      }
    }
  } else {
    for {set i 0} {$i<$row} {incr i} {
      set value [$t get $i,0 $i,$col]
      set value [eval concat $value]
      if {[llength $value]>0} {
        lappend str $value
      }
    }
  }
# check the data format
  foreach dataset $str {
     foreach value $dataset {
        if { ![VerifyValueType $value "D" $value] } {return ""}
     }
  }
   
  if {![info exists str]} {
    alert "You must give a definition of $name."
    return "{ }"
  }
  return $str
}

# draw graph on frame with data format as {2,3,4,5}{2,4,5,6}
# type could be frequency 'F'
proc drawGraphFromData {f data type title} {
   graph $f.graph -plotbackground gray 
   $f.graph configure -title $title
   set strx [lindex $data 0]
   set stry [lindex $data 1]
   set strexp "$f.graph element create HistoLine -xdata {$strx} -ydata {$stry} -smooth quadratic"
   eval $strexp
   $f.graph element configure HistoLine -label "  "
   $f.graph axis configure x -title "X" 
   $f.graph axis configure y -title "Y"
   pack $f.graph -side top -expand 1
}
