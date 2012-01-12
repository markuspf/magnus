
load "./tkhtml/tkhtml.so"

proc ReadFile {name} {
  if {[catch {open $name r} fp]} {
    tk_messageBox -icon error -message $fp -type ok
    return {}
  } else {
    fconfigure $fp -translation binary
    set r [read $fp [file size $name]]
    close $fp
    return $r
  }
}

proc ImageCmd {args} {
  global Images, htmlHelpRoot
  set fn [lindex $args 0]
  set fn $htmlHelpRoot$fn
  if {[catch {image create photo -file $fn} img]} {
    return smgray
  }

  set Images($fn) $img
  return $img
}

#proc ImageCmd {args} {
#  global OldImages Images showImages rt
#  if {!$showImages} {
#    return smgray
#  }
#  set fn [lindex $args 0]
#  set fn "$rt$fn"
#  puts $fn
#  if {[info exists OldImages($fn)]} {
#    set Images($fn) $OldImages($fn)
#    unset OldImages($fn)
#    return $Images($fn)
#  }
#  if {[catch {image create photo -file $fn} img]} {
#    return smgray
#  }
#  if {[image width $img]*[image height $img]>20000} {
#    global BigImages
#    set b [image create photo -width [image width $img] \
#           -height [image height $img]]
#    set BigImages($b) $img
#    set img $b
#    after idle "MoveBigImage $b"
#  }
#  set Images($fn) $img
#  return $img
#}
# proc MoveBigImage b {
#   global BigImages
#   if {![info exists BigImages($b)]} return
#   $b copy $BigImages($b)
#   image delete $BigImages($b)
#   unset BigImages($b)
#   update
# }


# This procedure is called when the user clicks on a hyperlink.
# See the "bind .h.h.x" below for the binding that invokes this
# procedure
#
proc HrefBinding {x y} {
  global htmlHelpRoot

  set new [.help.frame.frame1.html href $x $y]
  # puts "link to [list $new]"; return
  if {$new!=""} {
      
    fetchHelpPage "$new\::Html Help" "" 1
    #global LastFile
    #set pattern $LastFile#
    #set len [string length $pattern]
    #incr len -1
    #if {[string range $new 0 $len]==$pattern} {
    #  incr len
      #.h.h yview [string range $new $len end]
    #} else {
    #  LoadFile $new
    #}
  }
}


# This binding changes the cursor when the mouse move over
# top of a hyperlink.
#
bind HtmlClip <Motion> {
  set parent [winfo parent %W]
  set url [$parent href %x %y] 
  if {[string length $url] > 0} {
    $parent configure -cursor hand2
  } else {
    $parent configure -cursor {}
  }
}

proc helpHTML { w file} {
 
    global htmlHelpRoot

    bind .help.frame.frame1.html.x <1> {HrefBinding %x %y}

    regexp {(.+)\/(.+)\.(.+)} $file dum path name ext

    set htmlHelpRoot "$path/" 

    $w clear
    $w parse [ReadFile $file] 
}