#  added by xuxw

# to response the dataset defination input.
proc getDataSetObjectDef { w } {
  if {[$w.frame1.formatType get]=="ST"} {
     if {[$w.frame1.dimension get]=="2"} {
        set def1 [$w.frame2.frame.text get 1.0 end-1char]
	set def2 [$w.frame2.frame2.text get 1.0 end-1char]
	return "$def1 $def2"
     } else {
        return [$w.frame2.frame.text get 1.0 end-1char]
     }
  } else {
     return [getTableValue $w.frame2.frame.table]
  }
}

proc defineDataSetObjectDialog_OK {w dialog title message} {

  # $w is the dialog widget which was OK'd

  set name [verifyDialogName [$w.frame1.entry get]]
  if { $name == "" } { return }

  set defn [getDataSetObjectDef $w]
  if { $defn == "" } {
    alert "You must give a definition of $name."
    return
  }

  destroy $w

  save_pendingCheckin $name $dialog "$defn" $title $message
  if {[inNoviceMode] == 1} {
    postToMinilog "Check-in may be delayed if the session manager is busy."
  }

  # The defn must be on a single line for the SM:
  set defn "{ $defn }"
  regsub -all "\n" $defn " " defn
  set message "$message \$name \$defn"
  eval [concat toSessionManager "\"$message\""]
}

proc DataSetsetSpreadSheetFormat { w help {defn ""} } {
   if {$defn==""} {
     set defn [getDataSetObjectDef $w] 
   }

   regsub -all "," $defn " " defn
   setEntryValue $w.frame1.formatType SP
   destroy $w.frame2.frame
   if {[$w.frame1.dimension get]=="2"} {
       destroy $w.frame2.frame2
   }
   frame $w.frame2.frame -relief flat
   pack $w.frame2.frame -side top -fill both -expand 1 -padx 2m -pady 1m
   set dimension [$w.frame1.dimension get]
   createTable $w.frame2.frame 1000 $dimension $defn
   makeDefnHelp $w [lindex $help 1] 1
}

proc DataSetsetStringFormat { w help {defn ""} } {
   if {$defn==""} {
     set defn [getDataSetObjectDef $w] 
   }
  setEntryValue $w.frame1.formatType ST
  destroy $w.frame2.frame
  destroy $w.frame2.frame2
  frame $w.frame2.frame -relief flat
  text $w.frame2.frame.text -relief sunken -bd 2 -height 5 -width 50 \
     -wrap word -yscrollcommand "$w.frame2.frame.s set"
  scrollbar $w.frame2.frame.s -relief sunken \
     -command "$w.frame2.frame.text yview"
  pack $w.frame2.frame.text -side left -fill both -expand 1
  pack $w.frame2.frame.s -side right -fill y
  pack $w.frame2.frame -side top -fill both -expand 1 -padx 2m -pady 1m
  bind $w.frame2.frame.text <Tab> "focus $w.frame1.entry"
  bind $w.frame2.frame.text <Meta-o> "focus $w.frame1.entry"
  $w.frame2.frame.text insert 1.0 [lrange $defn 0 0]
  focus $w.frame2.frame.text
  $w.frame2.frame.text mark set insert "1.2"
  if { [$w.frame1.dimension get]==2 } {
     frame $w.frame2.frame2 -relief flat
     text $w.frame2.frame2.text -relief sunken -bd 2 -height 5 -width 50 \
       -wrap word -yscrollcommand "$w.frame2.frame2.s set"
     scrollbar $w.frame2.frame2.s -relief sunken \
       -command "$w.frame2.frame2.text yview"
     $w.frame2.frame2.text insert 1.0 [lrange $defn 1 1]
     pack $w.frame2.frame2.text -side left -fill both -expand 1
     pack $w.frame2.frame2.s -side right -fill y
     pack $w.frame2.frame2 -side top -fill both -expand 1 -padx 2m -pady 1m
  }
#  makeDefnHelp $w [lindex $help 1] 2
}
proc getDataSetDefnFromFile { w help } {
  set fileName [openfile [uniqueDialogName]]
  if {$fileName != ""} {
    set defn [read [open $fileName]]
    if {[$w.frame1.formatType get]=="ST"} {
       DataSetsetStringFormat $w $help $defn
    } else {
       DataSetsetSpreadSheetFormat $w $help $defn
    }
  }

  makeDefnHelp $w [lindex $help 1] 3
}

proc showCheckInHelp {w topic content} {
   makeDefnHelp $w $content 1
   showHelp "checkInHelp::$topic"
}

proc makeDataSetCheckInMenu { DialogWindow helps cathegory} {
  set m $DialogWindow.mbar
  menubutton $m.checkIn -text "Check In" -menu $m.checkIn.menu
  menu $m.checkIn.menu
  $m.checkIn.menu add command -label "Enter from Screen SpreadSheet Format" \
      -command "DataSetsetSpreadSheetFormat $DialogWindow \"[lindex $helps 1]\" "
  $m.checkIn.menu add command -label "Enter from Screen String Format" \
      -command "DataSetsetStringFormat $DialogWindow \"[lindex $helps 2]\" "
  $m.checkIn.menu add command -label "Enter from an Existing File " \
      -command "getDataSetDefnFromFile $DialogWindow \"[lindex $helps 3]\" "
# so far discard database check in
#  $m.checkIn.menu add command -label "Enter from MAGNUSSTAT Database " \
#      -command "getDefnFromDatabase $cathegory $DialogWindow.frame1.entry $DialogWindow.frame2.frame.text "

#  menubutton $m.tools -text "Tools" -menu $m.tools.menu
#  menu $m.tools.menu
  
  makeEditMenu $m.edit
  menubutton $m.help -text Help -menu $m.help.menu

  pack $m.checkIn $m.edit -side left
  pack $m.help -side right

  menu $m.help.menu
  foreach pair $helps {
    $m.help.menu add command -label [lindex $pair 0] \
      -command "showCheckInHelp $DialogWindow \"[lindex $pair 0]\" [lindex $pair 1]"
  }

  pack $m -side top -fill x
}

proc defineDataSetObjectDialog {message dialog title name helps defn \
			 errorMsg cathegory {dimension 1} {cursorPos "1.2"} } {

  # $message is the SM message template
  # $dialog is the name of the calling proc
  # $title is the dialog window title
  # $name is the default name of the object
  # $help is the help string to display
  # $defn is the default/erroneous definition
  # $errorMsg is the syntax error mess	age (empty if none)
  # $dimension is the data set variable dimension

  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"

  if {$errorMsg != ""} {
    defineDataSetObjectDialog2 $w $message $dialog $title $name $helps $defn \
			 $errorMsg $cathegory $dimension $cursorPos "ST" 
    return
  }

  #@db wm minsize $w 200 200
  frame $w.mbar -relief raised -bd 2
  set m $w.mbar
  menubutton $m.help -text Help -menu $m.help.menu
  pack $m.help -side right
  menu $m.help.menu
  foreach pair $helps {
    $m.help.menu add command -label [lindex $pair 0] \
      -command "showCheckInHelp $w \"[lindex $pair 0]\" [lindex $pair 1]"
  }

  pack $m -side top -fill x

  # the header
  frame $w.frame1 -relief flat -bd 2 
  frame $w.frame1.frame -relief flat 
  label $w.frame1.frame.title -text "CHECK-IN A DATASET"

  label $w.frame1.frame.procedure -text "CHECK-IN PROCEDURE (CLICK ON ONE)"

  pack $w.frame1.frame.title -side top
  pack $w.frame1.frame.procedure  -side top
  pack $w.frame1.frame  -side top
  pack $w.frame1  -side top

  # Make Choice buttons

  frame $w.frame3 -relief raised -bd 2
  frame $w.frame3.frame -relief flat

  button $w.frame3.frame.string -foreground blue -text "Enter from Screen String Format" -relief flat \
             -command "defineDataSetObjectDialog2 $w \"$message\" $dialog \"$title\" $name \"$helps\" \"$defn\" \
			 \"$errorMsg\" \"$cathegory\" $dimension $cursorPos \"STRING\" "
  button $w.frame3.frame.spread -foreground blue -text "Enter from Screen SpreadSheet Format" -relief flat \
             -command "defineDataSetObjectDialog2 $w \"$message\" $dialog \"$title\" $name \"$helps\" \"$defn\" \
			 \"$errorMsg\" \"$cathegory\" $dimension $cursorPos \"SPREAD\" "
  button $w.frame3.frame.exitfile -foreground blue -text "Enter from an Existing File" -relief flat \
             -command "defineDataSetObjectDialog2 $w \"$message\" $dialog \"$title\" $name \"$helps\" \"$defn\" \
			 \"$errorMsg\" \"$cathegory\" $dimension $cursorPos \"FILE\" "

  #@db 02/27 changed the order of Ok and Cancel buttons.

  pack $w.frame3.frame.string  -side top\
    -expand 1 -padx 3m -pady 1m 
  pack $w.frame3.frame.spread  -side top\
    -expand 1 -padx 3m -pady 1m 
  pack $w.frame3.frame.exitfile -side top\
    -expand 1 -padx 3m -pady 1m 

  pack $w.frame3.frame -fill x
  pack $w.frame3 -fill x

  # Make the help field?

  if { [inNoviceMode] } {
    makeDefnHelp $w [lindex [lindex $helps 0] 1]
  }

}

proc defineDataSetObjectDialog2 {preWin message dialog title name helps defn \
			 errorMsg cathegory {dimension 1} {cursorPos "1.2"} {format "ST"} } {

  # $message is the SM message template
  # $dialog is the name of the calling proc
  # $title is the dialog window title
  # $name is the default name of the object
  # $help is the help string to display
  # $defn is the default/erroneous definition
  # $errorMsg is the syntax error mess	age (empty if none)
  # $dimension is the data set variable dimension
  destroy $preWin
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200

  # Make menu bar

  frame $w.mbar -relief raised -bd 2

# @am change the look of File menu
  makeDataSetCheckInMenu $w $helps $cathegory

  frame $w.frame0 -relief flat -bd 2 
  label $w.frame0.title -text "CHECK-IN A DATASET"

  pack $w.frame0.title -side top
  pack $w.frame0  -side top

  # Make name field

  frame $w.frame1 -relief raised -bd 2
  label $w.frame1.label -text "Name"
  entry $w.frame1.entry -relief sunken
  $w.frame1.entry insert 0 $name
  pack $w.frame1.label -side left
  pack $w.frame1.entry -fill x -padx 2m -pady 1m
  pack $w.frame1 -fill x
  bind $w.frame1.entry <Tab> "focus $w.frame2.frame.text"
  bind $w.frame1.entry <Meta-o> "focus $w.frame2.frame.text"
  bind $w.frame1.entry <Return> "focus $w.frame2.frame.text"
  #focus $w.frame1.entry
  #save the format type in this hidden entry: ST:string SP:spreadsheet
  entry $w.frame1.formatType -width 2 -relief sunken
  setEntryValue $w.frame1.formatType ST
  entry $w.frame1.dimension -width 2 -relief sunken
  setEntryValue $w.frame1.dimension $dimension

  # Make definition field

  frame $w.frame2 -relief raised -bd 2
  label $w.frame2.label -text "Definition"
  pack $w.frame2.label -side top -anchor nw
  pack $w.frame2 -fill both -expand 1

  frame $w.frame2.frame -relief flat
  text $w.frame2.frame.text -relief sunken -bd 2 -height 5 -width 50 \
     -wrap word -yscrollcommand "$w.frame2.frame.s set"
  scrollbar $w.frame2.frame.s -relief sunken \
     -command "$w.frame2.frame.text yview"
  pack $w.frame2.frame.text -side left -fill both -expand 1
  pack $w.frame2.frame.s -side right -fill y
  pack $w.frame2.frame -side top -fill both -expand 1 -padx 2m -pady 1m
  bind $w.frame2.frame.text <Tab> "focus $w.frame1.entry"
  bind $w.frame2.frame.text <Meta-o> "focus $w.frame1.entry"
  $w.frame2.frame.text insert 1.0 [lrange "$defn" 0 0]
  focus $w.frame2.frame.text
  $w.frame2.frame.text mark set insert $cursorPos
  #
  if { $dimension==2 } {
     frame $w.frame2.frame2 -relief flat
     text $w.frame2.frame2.text -relief sunken -bd 2 -height 5 -width 50 \
       -wrap word -yscrollcommand "$w.frame2.frame2.s set"
     scrollbar $w.frame2.frame2.s -relief sunken \
       -command "$w.frame2.frame2.text yview"
     $w.frame2.frame2.text insert 1.0 [lrange "$defn" 1 1]
     pack $w.frame2.frame2.text -side left -fill both -expand 1
     pack $w.frame2.frame2.s -side right -fill y
     pack $w.frame2.frame2 -side top -fill both -expand 1 -padx 2m -pady 1m
  }
  # Make error field if necessary

  if {$errorMsg != ""} {
    frame $w.frame5 -relief raised -bd 2
    frame $w.frame5.frame -relief flat
    label $w.frame5.label -text "Problem with check-in" -foreground red
    text $w.frame5.frame.text -relief sunken -bd 2 -height 5 -width 50 \
      -wrap word -yscrollcommand "$w.frame5.frame.s set"
    scrollbar $w.frame5.frame.s -relief sunken \
      -command "$w.frame5.frame.text yview"
    pack $w.frame5.label -side top -anchor nw
    pack $w.frame5.frame.text -side left -fill both -expand 1
    pack $w.frame5.frame.s -side right -fill y
    pack $w.frame5.frame -side top -fill both -expand 1 -padx 2m -pady 1m
    pack $w.frame5 -fill both -expand 1

    # The message string has the form {{n} {mesg}}, where n is the index
    # into the defn of the problem
    set index [lindex $errorMsg 0]
    set mesg [lindex $errorMsg 1]
    $w.frame5.frame.text insert 1.0 \
       "$errorMsg"
#      "[string range $defn 0 $index]\n\n$mesg"
 
    $w.frame5.frame.text yview -pickplace end
    $w.frame5.frame.text configure -state disabled

    # Put cursor at error in definition field
#    $w.frame2.frame.text mark set insert "1.0 + $index chars"
    $w.frame2.frame.text yview -pickplace insert
    focus $w.frame2.frame.text
  }

  # Make OK and Cancel buttons

  frame $w.frame3 -relief raised -bd 2
  frame $w.frame3.frame -relief flat
  button $w.frame3.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  button $w.frame3.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "defineDataSetObjectDialog_OK $w $dialog \{$title\} \{$message\}"

  #@db 02/27 changed the order of Ok and Cancel buttons.

  pack $w.frame3.frame.ok $w.frame3.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frame3.frame -fill x
  pack $w.frame3 -fill x

  # Make the help field?

  if { [inNoviceMode] } {
    makeDefnHelp $w [lindex [lindex $helps 0] 1]
  }

  if { $format=="SPREAD" } {
     DataSetsetSpreadSheetFormat $w [lindex $helps 1]
  }

  if { $format=="FILE" } {
     getDataSetDefnFromFile $w [lindex $helps 1]
  }

  # Blink error field label
  if {$errorMsg != ""} {
    blinkWidget $w.frame5.label 0 red OldLace
  }
}

