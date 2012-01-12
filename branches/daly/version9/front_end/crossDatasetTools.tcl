proc CrossDSConfidenceOneAnalysisDialog_OK {w title message} {
  set level [$w.frameDef.frame.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
  destroy $w
  set message "$message $level"
  eval [concat toSessionManager "\"$message\""]
}

proc CrossDataSetConfidenceOneAnalysisMenu  { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.help -side right

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Confidence Analysis for One Proportion" \
      -command "showHelp {confidenceOneAnalysis::How Confidence for One Proportion Works}"
  pack $w.mbar -side top -fill x
}

proc CrossDataSetConfidenceOneControlDialog {title} {
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  CrossDataSetConfidenceOneAnalysisMenu $w

  frame $w.frame -relief raised -bd 2
  label $w.frame.descr -fg blue  -text "Confidence for One Proportion Analysis:"
 
  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x

  frame $w.frameDef -relief raised -bd 2
  frame $w.frameDef.frame -relief flat
  label $w.frameDef.frame.labelTitle -fg blue -text "Confidence for One Proportion"
  label $w.frameDef.frame.labelLevel -fg blue -text "What level of Confidence is desired?"
  entry $w.frameDef.frame.textLevel -relief sunken -bd 2  

  pack $w.frameDef.frame.labelTitle -side top -anchor nw
  pack $w.frameDef.frame.labelLevel -side top -anchor nw
  pack $w.frameDef.frame.textLevel -side left -fill both -expand 1
  pack $w.frameDef.frame -side top
  pack $w.frameDef -fill both -expand 1

  return $w  
}

proc defineCrossDSConfidenceOneDialog  {title message } {
  # create the widgets
  set w [CrossDataSetConfidenceOneControlDialog $title]

  # Make OK and Cancel buttons
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "Confidence for One Proportion Analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "CrossDSConfidenceOneAnalysisDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -fill x
}

proc CrossDSHypothesisOneAnalysisDialog_OK {w title message} {
  global MuType
  set level [$w.frameDef.frame.textLevel get]
  if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
   set nullHypo [$w.frameHypo.frame.nullHypo get]
   if {[regexp {^([0]?[.][0-9]*)$} $nullHypo]==0} {
       alert "You must give a value of MU0 between 0 and 1"
       return
    }
    set muType $MuType
  
  destroy $w
  set message "$message $level $nullHypo $muType"
  eval [concat toSessionManager "\"$message\""]
}

proc CrossDataSetHypothesisOneAnalysisMenu  { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.help -side right

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Hypothesis Testing for One Proportion" \
      -command "showHelp {hypothesisOneAnalysis::How Hypothesis for One Proportion Works}"
  pack $w.mbar -side top -fill x
}

proc CrossDataSetHypothesisOneControlDialog {title} {
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  CrossDataSetHypothesisOneAnalysisMenu $w

  frame $w.frame -relief raised -bd 2
  label $w.frame.descr -fg blue  -text "Confidence for One Proportion Analysis:"
 
  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x

  frame $w.frameDef -relief raised -bd 2
  frame $w.frameDef.frame -relief flat
  label $w.frameDef.frame.labelTitle -fg blue -text "Hypothesis Testing for One Proportion"
  label $w.frameDef.frame.labelLevel -fg blue -text "What level of Significance is desired?"
  entry $w.frameDef.frame.textLevel -relief sunken -bd 2  

  pack $w.frameDef.frame.labelTitle -side top -anchor nw
  pack $w.frameDef.frame.labelLevel -side top -anchor nw
  pack $w.frameDef.frame.textLevel -side left -fill both -expand 1
  pack $w.frameDef.frame -side top
  pack $w.frameDef -fill both -expand 1

  frame $w.frameHypo -relief flat
  frame $w.frameHypo.frame -relief flat
  frame $w.frameHypo.frame.muType -relief flat
  label $w.frameHypo.frame.label1 -fg blue -text "What is the Null Hypothesis"
  entry $w.frameHypo.frame.nullHypo -width 10 -relief sunken
  label $w.frameHypo.frame.label2 -fg blue -text "What is the alternative?"
  radiobutton $w.frameHypo.frame.muType.muType1 -value "1" -variable "MuType" -text "p!=p0"
  radiobutton $w.frameHypo.frame.muType.muType2 -value "2" -variable "MuType" -text "p>p0"
  radiobutton $w.frameHypo.frame.muType.muType3 -value "3" -variable "MuType" -text "p<p0"
  $w.frameHypo.frame.muType.muType1 select 

  pack $w.frameHypo.frame.label1 -side top -fill x -padx 2m -pady 1m
  pack $w.frameHypo.frame.nullHypo  -side top -fill x -padx 2m -pady 1m
  pack $w.frameHypo.frame.label2 -side top -fill x -padx 2m -pady 1m
  pack $w.frameHypo.frame.muType.muType1 -side left -fill x -padx 2m -pady 1m
  pack $w.frameHypo.frame.muType.muType2 -side left -fill x -padx 2m -pady 1m
  pack $w.frameHypo.frame.muType.muType3 -side left -fill x -padx 2m -pady 1m
  pack $w.frameHypo.frame.muType -side top -fill x -padx 2m -pady 1m
  pack $w.frameHypo.frame -side top -fill x -padx 2m -pady 1m
  pack $w.frameHypo -side top -fill x -padx 2m -pady 1m

  return $w  
}

proc defineCrossDSHypothesisOneDialog  {title message } {
  # create the widgets
  set w [CrossDataSetHypothesisOneControlDialog $title]

  # Make OK and Cancel buttons
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "Confidence for One Proportion Analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "CrossDSHypothesisOneAnalysisDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -fill x
}


proc defineCrossDSConfidenceTwoDialog  {title message } {
  defineCrossDSConfidenceOneDialog  $title $message 
}

proc defineCrossDSHypothesisTwoDialog  {title message } {
  defineCrossDSHypothesisOneDialog  $title $message 
}

proc CrossDSTableAnalysisDialog_OK {w title message} {
  set level [$w.frameDef.frame.textLevel get]
  if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
  set rowFactor [$w.frameDef.frame.textRowFactor get]
  if {[regexp {^([a-zA-Z])} $rowFactor]==0} {
    alert "You must give a name for row factor"
    return
  }
  set columnFactor [$w.frameDef.frame.textColumnFactor get]
  if {[regexp {^([a-zA-Z])} $columnFactor]==0} {
    alert "You must give a value for column factor"
    return
  }

  destroy $w
  set message "$message $level $rowFactor $columnFactor"
  eval [concat toSessionManager "\"$message\""]
}

proc CrossDataSetTableAnalysisMenu  { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.help -side right

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Contingency Table Analysis" \
      -command "showHelp {tableAnalysis::How Contingency Table Analysis Works}"
  pack $w.mbar -side top -fill x
}

proc CrossDataSetTableAnalysisControlDialog {title} {
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  CrossDataSetTableAnalysisMenu $w

  frame $w.frame -relief raised -bd 2
  label $w.frame.descr -fg blue  -text "Confidence for One Proportion Analysis:"
 
  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x

  frame $w.frameDef -relief raised -bd 2
  frame $w.frameDef.frame -relief flat
  label $w.frameDef.frame.labelTitle -fg blue -text "Contingency Table Analysis"
  label $w.frameDef.frame.labelLevel -fg blue -text "What level of Significance is desired?"
  entry $w.frameDef.frame.textLevel -relief sunken -bd 2  
  label $w.frameDef.frame.labelRowFactor -fg blue -text "What is then name of the row factor?"
  entry $w.frameDef.frame.textRowFactor -relief sunken -bd 2  
  label $w.frameDef.frame.labelColumnFactor -fg blue -text "What is the name of the column factor?"
  entry $w.frameDef.frame.textColumnFactor -relief sunken -bd 2  

  pack $w.frameDef.frame.labelTitle -side top -anchor nw
  pack $w.frameDef.frame.labelLevel -side top -anchor nw
  pack $w.frameDef.frame.textLevel -side top -fill both -expand 1
  pack $w.frameDef.frame.labelRowFactor -side top -anchor nw
  pack $w.frameDef.frame.textRowFactor -side top -anchor nw
  pack $w.frameDef.frame.labelColumnFactor -side top -anchor nw
  pack $w.frameDef.frame.textColumnFactor  -side top -anchor nw

  pack $w.frameDef.frame -side top
  pack $w.frameDef -fill both -expand 1

  return $w  
}

proc defineCrossDSTableAnalysisDialog  {title message } {
  # create the widgets
  set w [CrossDataSetTableAnalysisControlDialog $title]

  # Make OK and Cancel buttons
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "Contingency Table Analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "CrossDSTableAnalysisDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -fill x
}

proc defineCrossDSFollowUpAnalysisDialog  {title message } {
  eval [concat toSessionManager "\"$message\""]
}