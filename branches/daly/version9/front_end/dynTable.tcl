
source [file join [file dirname [info script]] loadtable.tcl]
proc createTable { f row column defn } {
set t $f.table
table $t -rows $row -cols $column -cache 1 -selecttype row \
	-yscrollcommand "$f.sy set" \
	-xscrollcommand "$f.sx set" \
	-height 8 -colstretch unset -rowstretch unset \
	-autoclear 1 -colwidth 20
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

proc getTableValue {t} {
  set row [$t cget -rows]
  set col [$t cget -cols]
  for {set i 0} {$i<$col} {incr i} {
    set value [$t get 0,$i $row,$i]
    set value [eval concat $value]
    if {[llength $value]>0} {
        lappend str $value
    }
  }
  if {![info exists str]} {
    return "{  }"
  }
  return $str
}
