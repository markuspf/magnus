
proc DataSetMeanAnalysisDialog_OK {w title message} {

  # $w is the dialog widget which was OK'd
  global MuType
  set analysisType [$w.frame.analysisType get]
  set level [$w.frameDef.frame.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
  if { $analysisType == "H" } {
    set nullHypo [$w.frameHypo.frame.nullHypo get]
   if {[regexp {^([-]?[0-9]*[.]?[0-9]*)$} $nullHypo]==0} {
       alert "You must give a value of MU0"
       return
    }
    set muType $MuType
  }
  destroy $w

  if { $analysisType == "C" } {
     set message "$message $analysisType $level"
  }

  if { $analysisType == "H" } {
     set message "$message $analysisType $level $nullHypo $muType"
  }
  eval [concat toSessionManager "\"$message\""]
}

proc setMeanAnalysisType { w type} {
  setEntryValue $w.frame.analysisType $type
  destroy $w.frameDef.frameDesc
  destroy $w.frameDef.frame
  destroy $w.frameHypo.frame
  # Make definition field
  frame $w.frameDef.frame -relief flat
  if { $type == "C" } {
    label $w.frameDef.frame.labelTitle  -text "CONFIDENCE INTERVAL FOR THE MEAN"
    label $w.frameDef.frame.labelLevel -fg blue -text "What level of Confidence is desired?"
    destroy $w.mbar.help.menu
    menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Confidence Interval for Mean Analysis" \
      -command "showHelp {meanConfidenceAnalysis::How Confidence Interval for Mean Analysis Works}"
    makeDefnHelp $w "DataSetMeanConfidenceAnalysis"
  } else {
    label $w.frameDef.frame.labelTitle  -text "HYPOTHESIS TEST FOR THE MEAN"
    label $w.frameDef.frame.labelLevel -fg blue -text "What level of Significance is desired?"
    destroy $w.mbar.help.menu
    menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Hypothesis Testing for Mean Analysis" \
      -command "showHelp {meanHypothesisAnalysis::How Hypothesis Testing for Mean Analysis Works}"
    makeDefnHelp $w "DataSetMeanHypothesisAnalysis"
  }
  label $w.frameDef.frame.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  
  entry $w.frameDef.frame.textLevel -relief sunken -bd 2  
  
  pack $w.frameDef.frame.labelTitle -side top 
  pack $w.frameDef.frame.labelLevel -side top -anchor nw
  pack $w.frameDef.frame.labelLevelExplain -side top -anchor nw
  pack $w.frameDef.frame.textLevel -side left -fill both -expand 1
  pack $w.frameDef.frame -side top -fill both -expand 1 -padx 2m -pady 1m
#  $w.frameDef.frame.textLevel insert 1.0 "0.5"
  focus $w.frameDef.frame.textLevel

  if { $type == "H" } {
     frame $w.frameHypo.frame -relief flat
     frame $w.frameHypo.frame.muType -relief flat
     label $w.frameHypo.frame.label1 -fg blue -text "What is the Null Hypothesis"
     entry $w.frameHypo.frame.nullHypo -width 10 -relief sunken
     label $w.frameHypo.frame.label2 -fg blue -text "What is the alternative?"
     radiobutton $w.frameHypo.frame.muType.muType1 -value "1" -variable "MuType" -text "MU\u2260MU0"
     radiobutton $w.frameHypo.frame.muType.muType2 -value "2" -variable "MuType" -text "MU>MU0"
     radiobutton $w.frameHypo.frame.muType.muType3 -value "3" -variable "MuType" -text "MU<MU0"
     $w.frameHypo.frame.muType.muType1 select 
     pack $w.frameHypo.frame.label1 -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.nullHypo  -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.label2 -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType.muType1 -side left -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType.muType2 -side left -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType.muType3 -side left -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame -side top -fill x -padx 2m -pady 1m

  }

  pack $w.frameBtn -fill x
}

proc MakeMeanAnalysisMenu { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.analysisType -text { Type } -menu $w.mbar.analysisType.menu
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.analysisType -side left
  pack $w.mbar.help -side right

  menu $w.mbar.analysisType.menu 
    $w.mbar.analysisType.menu add command -label "Confidence Interval for the Mean" \
      -command "setMeanAnalysisType $w C"

    $w.mbar.analysisType.menu add command -label "Hypothesis Test Concerning the Mean" \
      -command "setMeanAnalysisType $w H"

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Mean Analysis" \
      -command "showHelp {meanAnalysis::How Mean Analysis Works}"
  pack $w.mbar -side top -fill x
}

proc DataSetMeanAnalysisControlDialog {title} {
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  MakeMeanAnalysisMenu $w

  frame $w.frame -relief raised -bd 2
#  label $w.frame.descr -fg blue -text "Mean Analysis"
 
  #save the analysis type in this hidden entry
  entry $w.frame.analysisType -width 1 -relief sunken

#  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x -expand 1

  frame $w.frameDef -relief raised -bd 2
  frame $w.frameDef.frameDesc -relief flat
  label $w.frameDef.frameDesc.labelTitle -text "DATASET MEAN ANALYSIS"
  label $w.frameDef.frameDesc.labelLevel -text "MEAN ANALYSIS TYPE (CLICK ON ONE)"

  frame $w.frameDef.frameDesc.frameCommand -relief raised -bd 2

  button $w.frameDef.frameDesc.frameCommand.confidence -foreground blue -text "Confidence Interval for the Mean" -relief flat \
             -command "setMeanAnalysisType $w C"

  button $w.frameDef.frameDesc.frameCommand.hypothesis -foreground blue -text "Hypothesis Test Concerning the Mean" -relief flat \
             -command "setMeanAnalysisType $w H"
  
  pack $w.frameDef.frameDesc.labelTitle -side top -expand 1
  pack $w.frameDef.frameDesc.labelLevel -side top -expand 1
  pack $w.frameDef.frameDesc.frameCommand.confidence -side top -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand.hypothesis -side top -padx 2m -pady 1m -expand 1

  pack $w.frameDef.frameDesc -side top  -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand -side top -padx 2m -pady 1m -expand 1

  pack $w.frameDef -fill both -expand 1

  pack $w.frameDef -fill both -expand 1
  frame $w.frameHypo -relief raised -bd 2
  pack $w.frameHypo -fill x
  return $w  
}

proc defineDataSetMeanAnalysisDialog {message title} {
  # create the widgets
  set w [DataSetMeanAnalysisControlDialog $title]

  # Make OK and Cancel buttons
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "DataSetMeanAnalysisDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x

  frame $w.frame3 -relief raised -bd 2
  pack $w.frame3 -side top -fill x

  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetMeanAnalysis"
  }
}

## variance analysis tools

proc setVarianceAnalysisType { w type} {
  setEntryValue $w.frame.analysisType $type
  destroy $w.frameDef.frameDesc
  destroy $w.frameDef.frame
  destroy $w.frameHypo.frame
  # Make definition field
  frame $w.frameDef.frame -relief flat
  if { $type == "C" } {
    label $w.frameDef.frame.labelTitle  -text "CONFIDENCE INTERVAL FOR THE VARIANCE"
    label $w.frameDef.frame.labelLevel -fg blue -text "What level of Confidence is desired?"
    destroy $w.mbar.help.menu
    menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Confidence Interval for Variance Analysis" \
      -command "showHelp {varianceConfidenceAnalysis::How Confidence Interval for the Population Variance Works}"
    makeDefnHelp $w "DataSetVarianceConfidenceAnalysis"
  } else {
    label $w.frameDef.frame.labelTitle  -text "HYPOTHESIS TEST FOR THE VARIANCE"
    label $w.frameDef.frame.labelLevel -fg blue -text "What level of Significance is desired?"
    destroy $w.mbar.help.menu
    menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Hypothesis Testing for Variance Analysis" \
      -command "showHelp {varianceHypothesisAnalysis::How Hypothesis Testing for the Population Variance Works}"
    makeDefnHelp $w "DataSetVarianceHypothesisAnalysis"
  }

  label $w.frameDef.frame.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  
  entry $w.frameDef.frame.textLevel -relief sunken -bd 2  
  
  pack $w.frameDef.frame.labelTitle -side top 
  pack $w.frameDef.frame.labelLevel -side top -anchor nw
  pack $w.frameDef.frame.labelLevelExplain -side top -anchor nw
  pack $w.frameDef.frame.textLevel -side left -fill both -expand 1
  pack $w.frameDef.frame -side top -fill both -expand 1 -padx 2m -pady 1m
#  $w.frameDef.frame.textLevel insert 1.0 "0.5"
  focus $w.frameDef.frame.textLevel

  if { $type == "H" } {
     frame $w.frameHypo.frame -relief flat
     frame $w.frameHypo.frame.muType -relief flat
     label $w.frameHypo.frame.label1 -fg blue -text "What is the Null Hypothesis"
     entry $w.frameHypo.frame.nullHypo -width 10 -relief sunken
     label $w.frameHypo.frame.label2 -fg blue -text "What is the alternative?"
     radiobutton $w.frameHypo.frame.muType.muType1 -value "1" -variable "MuType" -text "sigma\u2260sigma0"
     radiobutton $w.frameHypo.frame.muType.muType2 -value "2" -variable "MuType" -text "sigma>sigma0"
     radiobutton $w.frameHypo.frame.muType.muType3 -value "3" -variable "MuType" -text "sigma<sigma0"
     $w.frameHypo.frame.muType.muType1 select 
     pack $w.frameHypo.frame.label1 -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.nullHypo  -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.label2 -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType.muType1 -side left -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType.muType2 -side left -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType.muType3 -side left -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame -side top -fill x -padx 2m -pady 1m

  }

  pack $w.frameBtn -fill x
}

proc MakeVarianceAnalysisMenu { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.analysisType -text { Type } -menu $w.mbar.analysisType.menu
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.analysisType -side left
  pack $w.mbar.help -side right

  menu $w.mbar.analysisType.menu 
    $w.mbar.analysisType.menu add command -label "Confidence Interval for the Variance" \
      -command "setVarianceAnalysisType $w C"

    $w.mbar.analysisType.menu add command -label "Hypothesis Test Concerning the Variance" \
      -command "setVarianceAnalysisType $w H"

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Variance Analysis" \
      -command "showHelp {varianceAnalysis::How Variance Analysis Works}"
  pack $w.mbar -side top -fill x
}

proc DataSetVarianceAnalysisControlDialog {title} {
  # $message is the SM message template
  # $dialog is the name of the calling proc
  # $title is the dialog window title
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  MakeVarianceAnalysisMenu $w

  frame $w.frame -relief raised -bd 2
#  label $w.frame.descr -fg blue -text "Variance Analysis"
 
  #save the analysis type in this hidden entry
  entry $w.frame.analysisType -width 1 -relief sunken

#  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x


  frame $w.frameDef -relief raised -bd 2
  frame $w.frameDef.frameDesc -relief flat
  label $w.frameDef.frameDesc.labelTitle -text "DATASET VARIANCE ANALYSIS"
  label $w.frameDef.frameDesc.labelLevel -text "VARIANCE ANALYSIS TYPE (CLICK ON ONE)"

  frame $w.frameDef.frameDesc.frameCommand -relief raised -bd 2

  button $w.frameDef.frameDesc.frameCommand.confidence -foreground blue -text "Confidence Interval for the Variance" \
    -relief flat -command "setVarianceAnalysisType $w C"

  button $w.frameDef.frameDesc.frameCommand.hypothesis -foreground blue -text "Hypothesis Test Concerning the Variance" \
        -relief flat -command "setVarianceAnalysisType $w H"
  
  pack $w.frameDef.frameDesc.labelTitle -side top -expand 1
  pack $w.frameDef.frameDesc.labelLevel -side top -expand 1
  pack $w.frameDef.frameDesc.frameCommand.confidence -side top -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand.hypothesis -side top -padx 2m -pady 1m -expand 1

  pack $w.frameDef.frameDesc -side top  -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand -side top -padx 2m -pady 1m -expand 1

  pack $w.frameDef -fill both -expand 1
  frame $w.frameHypo -relief raised -bd 2
  pack $w.frameHypo -fill x
  return $w
}

proc defineVarianceAnalysisDialog {title message} {
  set w [DataSetVarianceAnalysisControlDialog $title]
  # Make OK and Cancel buttons

  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "DataSetMeanAnalysisDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x

  frame $w.frame3 -relief raised -bd 2
  pack $w.frame3 -side top -fill x

  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetVarianceAnalysis"
  }

}


proc DataSetGoodnessOfFitDialog_OK {w title message} {

  # $w is the dialog widget which was OK'd
  global MuType
  set level [$w.frameDef.frame.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
  set setNum [$w.frame.setNum get]
 
  destroy $w
  set message "$message $MuType $level $setNum"
  eval [concat toSessionManager "\"$message\""]
}

proc MakeGoodnessOfFitMenu { w dataSize} {
  frame $w.mbar -relief raised -bd 2

  if {$dataSize==2} {
     menubutton $w.mbar.type -text "Which Dataset" -menu $w.mbar.type.menu
     pack $w.mbar.type -side left
     menu $w.mbar.type.menu
     $w.mbar.type.menu add command -label "the first dataset" \
        -command "setEntryValue $w.frame.setNum 0"
     $w.mbar.type.menu add command -label "the second dataset" \
        -command "setEntryValue $w.frame.setNum 1"
  }

  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.help -side right

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Goodness Of Fit" \
      -command "showHelp {goodnessOfFit::How Goodness Of Fit Works}"
  pack $w.mbar -side top -fill x
}

proc defineGoodnessOfFitControlDialog { title dataSize} {
  # $message is the SM message template
  # $dialog is the name of the calling proc
  # $title is the dialog window title
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  MakeGoodnessOfFitMenu $w $dataSize

  frame $w.frame -relief raised -bd 2
  label $w.frame.descr -text "GOODNESS OF FIT ANALYSIS"
 
  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x
  #save the analysis type in this hidden entry
  entry $w.frame.setNum -width 1 -relief sunken
  setEntryValue $w.frame.setNum 0
  
#
  frame $w.frameDef -relief flat
  frame $w.frameDef.frame -relief flat
  label $w.frameDef.frame.label1 -fg blue -text "For what distribution do you wish to test the fit"
  radiobutton $w.frameDef.frame.muType1 -value "1" -variable "MuType" -text "Normal?"
  radiobutton $w.frameDef.frame.muType2 -value "2" -variable "MuType" -text "Exponential?"
  radiobutton $w.frameDef.frame.muType3 -value "3" -variable "MuType" -text "Uniform?"
  $w.frameDef.frame.muType1 select 
  label $w.frameDef.frame.labelLevel -fg blue -text "What's the desired level of significance"
  label $w.frameDef.frame.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  entry $w.frameDef.frame.textLevel -relief sunken -bd 2  

  pack $w.frameDef.frame.label1 -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame.muType1 -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame.muType2 -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame.muType3 -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame.labelLevel  -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame.labelLevelExplain  -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame.textLevel  -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef -side top -fill x -padx 2m -pady 1m
  return $w
}

proc defineGoodnessOfFitDialog {dataSize title message} {
  set w [defineGoodnessOfFitControlDialog $title $dataSize]
  # Make OK and Cancel buttons

  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "DataSetGoodnessOfFitDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -side top -fill x -padx 2m -pady 1m
  frame $w.frame3 -relief raised -bd 2
  pack $w.frame3 -side top -fill x

  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetGoodnessFitAnalysis"
  }
}

proc getRandomSample {size strParentDef} {
  set strOldDef [lindex $strParentDef 0]
  set strDef "{"
  for {set i 0 } { $i < $size } {incr i 1} {
    set index [rand 0 [llength $strOldDef]-1]
    set value [lindex $strOldDef $index]
    set strDef "$strDef $value"
  }
  set strDef "$strDef }"
  postToLog $strDef
  return $strDef
}

proc SampleFromDataSetDialog_OK {w title message} {

  # $w is the dialog widget which was OK'd
  set size [$w.frameDef.frame.textSize get]
  if {[regexp {^([1-9][0-9]*)$} $size]==0} {
    alert "You must give a non-negative integer for size"
    return
  }
  set name "Sample"
  set viewID [lindex $message 2]
  set iconID [get_viewIcon $viewID]
  set strParentDef [get_objectDefinition $iconID]
  regsub -all "," $strParentDef " " strParentDef
  set strOldDef [lindex $strParentDef 0]
  if {$size > [llength $strOldDef]} {
    alert "Your size can not be greater than the old dataset"
    return
  }
  destroy $w
  set def [getRandomSample $size $strParentDef]
  save_pendingCheckin $name "SampleDataSet" $def $title $message
  set message "$message $name \{$def\}"
  eval [concat toSessionManager "\"$message\""]
}
proc makeSampleViewMenu {w} {
  frame $w.mbar -relief raised -bd 2

  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.help -side right

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Random Sample CHECK-IN" \
      -command "showHelp {randomSample::How Random Sample CHECK-IN Works}"
  pack $w.mbar -side top -fill x

}

proc defineSampleFromDataSetDialog {title message} {

  # $message is the SM message template
  # $dialog is the name of the calling proc
  # $title is the dialog window title
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"

  makeSampleViewMenu $w

  frame $w.frame -relief raised -bd 2
#  label $w.frame.descr -fg blue -text "Goodness Of Fit:"
 
#  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x
  frame $w.frameDef -relief flat
  frame $w.frameDef.frame -relief flat
  label $w.frameDef.frame.labelSize -fg blue -text "How large a random sample do you want"
  entry $w.frameDef.frame.textSize -relief sunken -bd 2  
  pack $w.frameDef.frame.labelSize  -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame.textSize  -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef -side top -fill x -padx 2m -pady 1m

  # Make OK and Cancel buttons
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "SampleFromDataSetDialog_OK $w \{$title\} \{$message\}"
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -side top -fill x -padx 2m -pady 1m
  frame $w.frame3 -relief raised -bd 2
  pack $w.frame3 -side top -fill x

  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetRandomSampleAnalysis"
  }
}


proc DataSetCompareMeanAnalysisDialog_OK {w title message} {

  # $w is the dialog widget which was OK'd
  global MuType
  global equalAssumed
  set analysisType [$w.frame.analysisType get]
  set level [$w.frameDef.frame.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
  if { $analysisType == "H" } {
    set nullHypo [$w.frameHypo.frame.nullHypo get]
    if {[regexp {^([-]?[0-9]*[.]?[0-9]*)$} $nullHypo]==0} {
        alert "You must give a value of MU0"
        return
    }
    set muType $MuType
  }
  destroy $w

  if { $analysisType == "C" } {
     set message "$message $analysisType $level"
  }

  if { $analysisType == "H" } {
#    set message "$message $analysisType $level"

     set message "$message $analysisType $level $nullHypo $muType $equalAssumed"
  }
  eval [concat toSessionManager "\"$message\""]
}

proc setCompareMeanAnalysisType { w type} {
  setEntryValue $w.frame.analysisType $type
  destroy $w.frameDef.frameDesc
  destroy $w.frameDef.frame
  destroy $w.frameHypo.frame
  # Make definition field
  frame $w.frameDef.frame -relief flat
  if { $type == "C" } {
    label $w.frameDef.frame.labelTitle -text "Confidence Interval for Difference of Means"
    label $w.frameDef.frame.labelLevel -fg blue -text "What level of Confidence is desired?"
    if { [inNoviceMode] } {
       makeDefnHelp $w "DataSetComparisonMeanConfidenceAnalysis"
    }
  } else {
    label $w.frameDef.frame.labelTitle -text "Hypothesis Test Concerning Difference of means"
    label $w.frameDef.frame.labelLevel -fg blue -text "What level of Significance is desired?"
    if { [inNoviceMode] } {
       makeDefnHelp $w "DataSetComparisonMeanHypothesisAnalysis"
    }
  }
  label $w.frameDef.frame.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  
  entry $w.frameDef.frame.textLevel -relief sunken -bd 2  
  
  pack $w.frameDef.frame.labelTitle -side top -anchor nw
  pack $w.frameDef.frame.labelLevel -side top -anchor nw
  pack $w.frameDef.frame.labelLevelExplain -side top -anchor nw
  pack $w.frameDef.frame.textLevel -side left -fill both -expand 1
  pack $w.frameDef.frame -side top -fill both -expand 1 -padx 2m -pady 1m
#  $w.frameDef.frame.textLevel insert 1.0 "0.5"
  focus $w.frameDef.frame.textLevel

  if { $type == "H" } {
     frame $w.frameHypo.frame -relief flat
     frame $w.frameHypo.frame.muType -relief flat
     label $w.frameHypo.frame.label1 -fg blue -text "What is the Null Hypothesis"
     entry $w.frameHypo.frame.nullHypo -width 10 -relief sunken

     checkbutton $w.frameHypo.frame.equalAssumed -variable "equalAssumed" -onvalue "T" -offvalue "F" \
              -text "Should equal variance be assumed"

     label $w.frameHypo.frame.label2 -fg blue -text "What is the alternative?"
     radiobutton $w.frameHypo.frame.muType.muType1 -value "1" -variable "MuType" -text "MU\u2260MU0"
     radiobutton $w.frameHypo.frame.muType.muType2 -value "2" -variable "MuType" -text "MU>MU0"
     radiobutton $w.frameHypo.frame.muType.muType3 -value "3" -variable "MuType" -text "MU<MU0"
     $w.frameHypo.frame.muType.muType1 select 
     pack $w.frameHypo.frame.label1 -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.nullHypo  -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.equalAssumed -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.label2 -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType.muType1 -side left -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType.muType2 -side left -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType.muType3 -side left -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame.muType -side top -fill x -padx 2m -pady 1m
     pack $w.frameHypo.frame -side top -fill x -padx 2m -pady 1m

  }

  pack $w.frameBtn -fill x
}

proc MakeCompareMeanAnalysisMenu { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.analysisType -text { Type } -menu $w.mbar.analysisType.menu
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.analysisType -side left
  pack $w.mbar.help -side right

  menu $w.mbar.analysisType.menu 
    $w.mbar.analysisType.menu add command -label "Confidence Interval for the Difference of Means" \
      -command "setCompareMeanAnalysisType $w C"

    $w.mbar.analysisType.menu add command -label "Hypothesis Test Concerning the Difference of Means" \
      -command "setCompareMeanAnalysisType $w H"

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Compare Mean Analysis" \
      -command "showHelp {descriptionOfCompareMean::Description of Compare Mean Analysis}"
  pack $w.mbar -side top -fill x
}

proc defineCompareMeanAnalysisDialog { title message} {

  # $message is the SM message template
  # $dialog is the name of the calling proc
  # $title is the dialog window title
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  MakeCompareMeanAnalysisMenu $w

  frame $w.frame -relief raised -bd 2
#  label $w.frame.descr -fg blue  -text "Compare Mean Analysis:"
 
  #save the analysis type in this hidden entry
  entry $w.frame.analysisType -width 1 -relief sunken

#  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x

  frame $w.frameDef -relief raised -bd 2
  frame $w.frameDef.frameDesc -relief flat

  label $w.frameDef.frameDesc.labelTitle -text "COMPARISON OF MEANS"
  label $w.frameDef.frameDesc.labelLevel -text "MEAN COMPARISON ANALYSIS TYPE (CLICK ON ONE)"

  frame $w.frameDef.frameDesc.frameCommand -relief raised -bd 2

  button $w.frameDef.frameDesc.frameCommand.confidence -foreground blue -text "Confidence Interval for the Diffrence of Means" -relief flat \
             -command "setCompareMeanAnalysisType $w C"

  button $w.frameDef.frameDesc.frameCommand.hypothesis -foreground blue -text "Hypothesis Test Concerning the Difference of Means" -relief flat \
             -command "setCompareMeanAnalysisType $w H"
  
  pack $w.frameDef.frameDesc.labelTitle -side top -expand 1
  pack $w.frameDef.frameDesc.labelLevel -side top -expand 1
  pack $w.frameDef.frameDesc.frameCommand.confidence -side top -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand.hypothesis -side top -padx 2m -pady 1m -expand 1

  pack $w.frameDef.frameDesc -side top  -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand -side top -padx 2m -pady 1m -expand 1

  pack $w.frameDef -fill both -expand 1

  pack $w.frameDef -fill both -expand 1


  frame $w.frameHypo -relief raised -bd 2
  pack $w.frameHypo -fill x

  # Make OK and Cancel buttons

  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "DataSetCompareMeanAnalysisDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x

  frame $w.frame3 -relief raised -bd 2
  pack $w.frame3 -side top -fill x

  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetComparisonMeanAnalysis"
  }

}

## mean variance analysis tools

proc setCompareVarianceAnalysisType { w type} {
  setEntryValue $w.frame.analysisType $type
  destroy $w.frameDef.frameDesc
  destroy $w.frameDef.frame
  destroy $w.frameHypo.frame
  # Make definition field
  frame $w.frameDef.frame -relief flat
  if { $type == "C" } {
    label $w.frameDef.frame.labelTitle -text "Confidence Interval for the Variance"
    label $w.frameDef.frame.labelLevel -fg blue -text "What level of Confidence is desired?"
    if { [inNoviceMode] } {
       makeDefnHelp $w "DataSetComparisonVarianceHypothesisAnalysis"
    }
  } else {
    label $w.frameDef.frame.labelTitle -text "Hypothesis test for the Difference of Variances"
    label $w.frameDef.frame.labelLevel -fg blue -text "What level of Significance is desired?"
    if { [inNoviceMode] } {
       makeDefnHelp $w "DataSetComparisonVarianceHypothesisAnalysis"
    }
  }

  label $w.frameDef.frame.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  
  entry $w.frameDef.frame.textLevel -relief sunken -bd 2  
  
  pack $w.frameDef.frame.labelTitle -side top -anchor nw
  pack $w.frameDef.frame.labelLevel -side top -anchor nw
  pack $w.frameDef.frame.labelLevelExplain -side top -anchor nw
  pack $w.frameDef.frame.textLevel -side left -fill both -expand 1
  pack $w.frameDef.frame -side top -fill both -expand 1 -padx 2m -pady 1m
#  $w.frameDef.frame.textLevel insert 1.0 "0.5"
  focus $w.frameDef.frame.textLevel

  pack $w.frameBtn -fill x
}

proc MakeCompareVarianceAnalysisMenu { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.analysisType -text { Type } -menu $w.mbar.analysisType.menu
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.analysisType -side left
  pack $w.mbar.help -side right

  menu $w.mbar.analysisType.menu 
#    $w.mbar.analysisType.menu add command -label "Confidence Interval for the Variance" \
#      -command "setCompareVarianceAnalysisType $w C"

    $w.mbar.analysisType.menu add command -label "Hypothesis Test Concerning the Difference of Variances" \
      -command "setCompareVarianceAnalysisType $w C"

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Compare Variance Analysis" \
      -command "showHelp {descriptionOfCompareVariance::Description of Compare Variance Analysis}"
  pack $w.mbar -side top -fill x
}

proc defineCompareVarianceAnalysisDialog {title message} {

  # $message is the SM message template
  # $dialog is the name of the calling proc
  # $title is the dialog window title
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  MakeCompareVarianceAnalysisMenu $w

  frame $w.frame -relief raised -bd 2
#  label $w.frame.descr -fg blue -text "Compare  Variance Analysis:"
 
  #save the analysis type in this hidden entry
  entry $w.frame.analysisType -width 1 -relief sunken

#  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x

  frame $w.frameDef -relief raised -bd 2
  frame $w.frameDef.frameDesc -relief flat
  label $w.frameDef.frameDesc.labelTitle -text "COMPARISON OF VARIANCE ANALYSIS"
  label $w.frameDef.frameDesc.labelLevel -text "VARIANCE COMPARISON ANALYSIS TYPE (CLICK ON ONE)"

  frame $w.frameDef.frameDesc.frameCommand -relief raised -bd 2

#  button $w.frameDef.frameDesc.frameCommand.confidence -foreground blue -text "Confidence Interval" -relief flat \
#             -command "setVarianceAnalysisType $w C"

  button $w.frameDef.frameDesc.frameCommand.hypothesis -foreground blue -text "Hypothesis Test Concerning the Difference of Variances" -relief flat \
             -command "setCompareVarianceAnalysisType $w C"
  
  pack $w.frameDef.frameDesc.labelTitle -side top -expand 1
  pack $w.frameDef.frameDesc.labelLevel -side top -expand 1
#  pack $w.frameDef.frameDesc.frameCommand.confidence -side top -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand.hypothesis -side top -padx 2m -pady 1m -expand 1

  pack $w.frameDef.frameDesc -side top  -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand -side top -padx 2m -pady 1m -expand 1

  pack $w.frameDef -fill both -expand 1

  pack $w.frameDef -fill both -expand 1

  frame $w.frameHypo -relief raised -bd 2
  pack $w.frameHypo -fill x

  # Make OK and Cancel buttons

  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "compare variance analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "DataSetCompareMeanAnalysisDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x

  frame $w.frame3 -relief raised -bd 2
  pack $w.frame3 -side top -fill x

  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetComparisonVarianceAnalysis"
  }
}

proc MakeNonParametricTestMenu { w } {
  frame $w.mbar -relief raised -bd 2

  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.help -side right

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Non-Parametric Testing" \
      -command "showHelp {descriptionOfNonParametricTest::Description of Non-Parametric Testing}"
  pack $w.mbar -side top -fill x
}

proc defineNonParametricControlDialog { title } {
  # $message is the SM message template
  # $dialog is the name of the calling proc
  # $title is the dialog window title
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus

  MakeNonParametricTestMenu $w
  frame $w.frame -relief raised -bd 2
  label $w.frame.descr -text "Non-Parametric Analysis of Dataset"
 
  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x
  #save the analysis type in this hidden entry
  
#
  frame $w.frameDef -relief flat
  frame $w.frameDef.frame -relief flat
  label $w.frameDef.frame.labelLevel -fg blue -text "What's the desired level of significance"
  label $w.frameDef.frame.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  entry $w.frameDef.frame.textLevel -relief sunken -bd 2  

  pack $w.frameDef.frame.labelLevel  -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame.labelLevelExplain  -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame.textLevel  -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef -side top -fill x -padx 2m -pady 1m

  frame $w.frame3 -relief raised -bd 2
  pack $w.frame3 -side top -fill x

  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetNonParametricAnalysis"
  }

  return $w
}

proc TVDSNonParametricAnalysisDialog_OK { w title message } {
    # $w is the dialog widget which was OK'd
  set level [$w.frameDef.frame.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
 
  destroy $w
  set message "$message $level "
  eval [concat toSessionManager "\"$message\""]
}
proc defineTVDSNonParametricAnalysisDialog {title message} {
  set w [defineNonParametricControlDialog $title]
  # Make OK and Cancel buttons
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "TVDSNonParametricAnalysisDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -side top -fill x -padx 2m -pady 1m

}


