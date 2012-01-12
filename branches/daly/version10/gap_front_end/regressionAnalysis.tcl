proc MakeRegressionGraphicMenu { w } {
  frame $w.mbar -relief raised -bd 2

  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.help -side right

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Graphical Regression Analysis" \
      -command "showHelp {descriptionOfGraphicalRegression::Description of Graphical Regression Analysis}"
  pack $w.mbar -side top -fill x
}

proc regressionGraphicView {viewID} {

  global A tcl_path
  set w [uniqueDialogName]
  set iconID [get_viewIcon $viewID]
  set strDef [get_objectDefinition $iconID]
  set parameters [get_viewParameter $viewID]

  toplevel $w
  eval wm title $w "\"[get_viewTitle $viewID]\""
  eval wm iconname $w "\"[get_viewTitle $viewID]\""
  #@db wm minsize $w 300 300
  # Make menu bar

  MakeRegressionGraphicMenu $w
  # The banner frame:

  eval set banner "\"[get_viewBanner $viewID]\""

  set height [expr [string length $banner] / 20]

  frame $w.frame -relief raised -bd 2
  text $w.frame.descr -height $height -width 20 -relief flat -wrap word
  $w.frame.descr insert end $banner
  $w.frame.descr configure -state disabled


  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x

  frame $w.mainF -relief flat -bd 2
  pack $w.mainF -fill x -side top
  
  # ok button
  frame $w.frame3 -relief raised -bd 2
  set f $w.frame3
  button $f.cancel -text Close -state active -width 11 \
    -command "problem_Cancel $w $viewID"
  #button $f.ok -text Redraw -state active -width 11 \
  #  -command "drawGraph {$strDef} $w"

  #pack $f.ok -side left -padx 2m -pady 1m
  pack $f.cancel -side right -padx 2m -pady 1m

  pack $w.frame3 -side top -fill x

  wm protocol $w WM_DELETE_WINDOW  "problem_Cancel $w $viewID"
#set the default value
  #eval "setGraphicDefaultValue {$strDef} $w"
  eval "drawRegressionGraph {$strDef} $w.mainF [lindex $parameters 0] [lindex $parameters 1]"
  return $w
}

proc drawRegressionGraph { defn w slope intercept} {
  destroy $w.b
  graph $w.b -plotbackground gray 
  $w.b configure -title "  "
  set strx [getRegressionGraphicX "$defn"]
  set stry [lindex $defn 1]
  set strRegressionY [getRegressionGraphicY "$defn" $slope $intercept]

  set strexp "$w.b element create HistoLine -xdata {$strx} -ydata {$stry} -linewidth 0 -label \"data\""
  eval $strexp
  set strexp "$w.b element create RegressionLine -xdata {$strx} -ydata {$strRegressionY} -pixels 0 -color blue "
  eval $strexp

  $w.b element configure RegressionLine -label " Regression Line "
  $w.b axis configure x -title "y" 
  $w.b axis configure y -title "x"
  pack $w.b
}

proc getRegressionGraphicX { defn } {
  return [lindex $defn 0]
}

proc getRegressionGraphicY { defn slope intercept} {
  set listX [lindex $defn 0]
  set strY ""
  for {set i 0 } { $i <[llength $listX] } {incr i 1} {
     set x [lindex $listX $i]
     set y [expr $x*$slope+$intercept]
     lappend strY $y
  }
  return $strY
}

proc defineTVDSRegressionFullAnalysisDialog {title message} {
  eval [concat toSessionManager "\"$message\""]
}

proc TVDSPredictionHypothesisSlopeDialog_OK {w title message} {
  global BTypeTVDSSlope
  set level [$w.frameDef.frame.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
  set nullHypo [$w.frameDef.frame.hypoF.nullHypo get]
  if {[regexp {^([-]?[0-9]*[.]?[0-9]*)$} $nullHypo]==0} {
       alert "You must give a value of B0"
       return
  }
  set muType $BTypeTVDSSlope
  destroy $w
  set analysisType S
  set message "$message $analysisType $level $nullHypo $muType"
  eval [concat toSessionManager "\"$message\""]
}

proc TVDSPredictionHypothesisSlope { w title message } {
  destroy $w.frameDef.frameDesc
  destroy $w.frameDef.frame

# Make definition field
  frame $w.frameDef.frame -relief flat
  makeWidgetsOnSlopeHypotheis  $w.frameDef.frame
  pack $w.frameDef.frame -side top -fill both -expand 1 -padx 2m -pady 1m

# Make OK and Cancel buttons
  destroy $w.frameBtn
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "TVDSPredictionHypothesisSlopeDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -fill x

  makeDefnHelp $w "DataSetPredictionHypothesisSlopeAnalysis"

}

proc TVDSPredictionConfidenceSlopeDialog_OK { w title message} {
  set level [$w.frameDef.frame.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
  destroy $w
  set analysisType s
  set message "$message $analysisType $level"
  eval [concat toSessionManager "\"$message\""]

}

proc TVDSPredictionConfidenceSlope  { w  title message} {
  destroy $w.frameDef.frameDesc
  destroy $w.frameDef.frame
  # Make definition field
  frame $w.frameDef.frame -relief flat
  makeWidgetsOnSlopeConfidence $w.frameDef.frame
  pack $w.frameDef.frame -side top -fill both -expand 1 -padx 2m -pady 1m
#  $w.frameDef.frame.textLevel insert 1.0 "0.5"
  focus $w.frameDef.frame.textLevel

    # Make OK and Cancel buttons
  destroy $w.frameBtn
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "TVDSPredictionConfidenceSlopeDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -fill x
  makeDefnHelp $w "DataSetPredictionConfidenceSlopeAnalysis"

}
proc TVDSPredictionHypothesisInterceptDialog_OK {w title message} {
  global ATypeTVDSIntercept
  set level [$w.frameDef.frame.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
  set nullHypo [$w.frameDef.frame.hypoF.nullHypo get]
  if {[regexp {^([-]?[0-9]*[.]?[0-9]*)$} $nullHypo]==0} {
       alert "You must give a value of B0"
       return
  }
  set muType $ATypeTVDSIntercept
  destroy $w
  set analysisType I
  set message "$message $analysisType $level $nullHypo $muType"
  eval [concat toSessionManager "\"$message\""]
}

proc TVDSPredictionHypothesisIntercept  { w  title message} {
  destroy $w.frameDef.frameDesc
  destroy $w.frameDef.frame
# Make definition field

  frame $w.frameDef.frame -relief flat
  makeWidgetsOnInterceptHypotheis $w.frameDef.frame
  pack $w.frameDef.frame -side top -fill both -expand 1 -padx 2m -pady 1m

# Make OK and Cancel buttons
  destroy $w.frameBtn
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "TVDSPredictionHypothesisInterceptDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -fill x
  makeDefnHelp $w "DataSetPredictionHypothesisInterceptAnalysis"
}

proc TVDSPredictionConfidenceInterceptDialog_OK {w title message} {
  set level [$w.frameDef.frame.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value o`f level between 0 and 1"
    return
  }
  destroy $w
  set analysisType i
  set message "$message $analysisType $level"
  eval [concat toSessionManager "\"$message\""]
}

proc TVDSPredictionConfidenceIntercept  { w  title message} {
  destroy $w.frameDef.frameDesc
  destroy $w.frameDef.frame

# Make definition field
  frame $w.frameDef.frame -relief flat
  makeWidgetsOnInterceptConfidence $w.frameDef.frame
  pack $w.frameDef.frame -side top -fill both -expand 1 -padx 2m -pady 1m
#  $w.frameDef.frame.textLevel insert 1.0 "0.5"
  focus $w.frameDef.frame.textLevel

# Make OK and Cancel buttons
  destroy $w.frameBtn
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "TVDSPredictionConfidenceInterceptDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -fill x
  makeDefnHelp $w "DataSetPredictionConfidenceInterceptAnalysis"
}

proc TVDSPredictionModelFitDialog_OK {w title message} {
  set level [$w.frameDef.frame.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
  destroy $w
  set analysisType M
  set message "$message $analysisType $level"
  eval [concat toSessionManager "\"$message\""]
}

proc TVDSPredictionModelFit  { w  title message} {
  destroy $w.frameDef.frameDesc
  destroy $w.frameDef.frame
  # Make definition field
  frame $w.frameDef.frame -relief flat
  makeWidgetsOnModelFitPrediction $w.frameDef.frame
  pack $w.frameDef.frame -side top -fill both -expand 1 -padx 2m -pady 1m
#  $w.frameDef.frame.textLevel insert 1.0 "0.5"
  focus $w.frameDef.frame.textLevel

    # Make OK and Cancel buttons
  destroy $w.frameBtn
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "TVDSPredictionModelFitDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -fill x
  makeDefnHelp $w "DataSetPredictionModelFitAnalysis"

}

proc TVDSPredictionIntervalDialog_OK {w title message} {
  set level [$w.frameDef.frame.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1"
    return
  }
  set X0 [$w.frameDef.frame.textX0 get]
  if {[regexp {^([-]?[0-9]*[.]?[0-9]*)$} $X0]==0} {
       alert "You must give a value of B0"
       return
  }
  destroy $w
  set analysisType P
  set message "$message $analysisType $level $X0"
  eval [concat toSessionManager "\"$message\""]
}

proc TVDSPredictionInterval  { w  title message} {
  destroy $w.frameDef.frameDesc
  destroy $w.frameDef.frame
  # Make definition field
  frame $w.frameDef.frame -relief flat
  makeWidgetsOnPredictionTest $w.frameDef.frame
  pack $w.frameDef.frame -side top -fill both -expand 1 -padx 2m -pady 1m
#  $w.frameDef.frame.textLevel insert 1.0 "0.5"

  focus $w.frameDef.frame.textLevel

    # Make OK and Cancel buttons
  destroy $w.frameBtn
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "TVDSPredictionIntervalDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -fill x

  makeDefnHelp $w "DataSetPredictionIntervalAnalysis"

}

proc getPredictSlopeConfidenceParametersDialog_ok { f } {
  set level [$f.textLevel get]
  if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1 for slope confidence!"
    return
  }
  set curParameters "S \{C $level\}"
  return $curParameters
}

proc getPredictSlopeHypothesisParametersDialog_ok { f } {
  global BTypeTVDSSlope
  set level [$f.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1 for slope hypothesis!"
    return
  }
  set nullHypo [$f.hypoF.nullHypo get]
  if {[regexp {^([-]?[0-9]*[.]?[0-9]*)$} $nullHypo]==0} {
       alert "You must give a value of B0"
       return
  }
  set muType $BTypeTVDSSlope
  set curParameters "S \{H $level $nullHypo $muType\}"
  return $curParameters
}
proc getPredictInterceptConfidenceParametersDialog_ok { f } {
  set level [$f.textLevel get]
  if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1 for intercept confidence!"
    return
  }
  set curParameters "I \{C $level\}"
  return $curParameters
}

proc getPredictInterceptHypothesisParametersDialog_ok { f } {
  global ATypeTVDSIntercept
  set level [$f.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1 for intercept hypothesis!"
    return
  }
  set nullHypo [$f.hypoF.nullHypo get]
  if {[regexp {^([-]?[0-9]*[.]?[0-9]*)$} $nullHypo]==0} {
       alert "You must give a value of B0"
       return
  }
  set curParameters "I \{H $level $nullHypo $ATypeTVDSIntercept\}"
  return $curParameters
}
proc getPredictModelFitParametersDialog_ok { f } {
  set level [$f.textLevel get]
  if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1 for model fit!"
    return
  }
  set curParameters "M \{ $level\}"
  return $curParameters
}
proc getPredictX0ParametersDialog_ok { f } {
  set level [$f.textLevel get]
   if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
    alert "You must give a value of level between 0 and 1 for prediction!"
    return
  }
  set X0 [$f.textX0 get]
  if {[regexp {^([-]?[0-9]*[.]?[0-9]*)$} $X0]==0} {
       alert "You must give a value of B0"
       return
  }
  set curParameters "P \{$level $X0 \}"
  return $curParameters
}
# produce A { {S { C Level} } {S{ H Level Mu0 Type } } {I {C Level} } {I{ H Level Mu0 Type} } 
#           { M { Level} } { P {Level X0 } } }

proc TVDSAllPredictionDialog_OK { w title message } {
  global BTypeTVDSSlope ATypeTVDSIntercept
  set Parameters ""
  set curParameter [getPredictSlopeConfidenceParametersDialog_ok $w.frameDef.frame.confidenceF.slopeF]
  if {$curParameter==""} { return }
  lappend Parameters $curParameter

  set curParameter [getPredictSlopeHypothesisParametersDialog_ok $w.frameDef.frame.hypothesisF.slopeF]
  if {$curParameter==""} { return }
  lappend Parameters $curParameter

  set curParameter [getPredictInterceptConfidenceParametersDialog_ok $w.frameDef.frame.confidenceF.interceptF]
  if {$curParameter==""} { return }
  lappend Parameters $curParameter

  set curParameter [getPredictInterceptHypothesisParametersDialog_ok $w.frameDef.frame.hypothesisF.interceptF]
  if {$curParameter==""} { return }
  lappend Parameters $curParameter

  set curParameter [getPredictModelFitParametersDialog_ok $w.frameDef.frame.otherF.modelF]
  if {$curParameter==""} { return }
  lappend Parameters $curParameter

  set curParameter [getPredictX0ParametersDialog_ok $w.frameDef.frame.otherF.predictF]
  if {$curParameter==""} { return }
  lappend Parameters $curParameter

  destroy $w
  set analysisType A
  set message "$message $analysisType \{ $Parameters \}"
  eval [concat toSessionManager "\"$message\""]
}

proc makeWidgetsOnSlopeConfidence {f} {
  label $f.labelTitle -fg blue -text "Confidence Interval for Slope"
  label $f.labelLevel -text "What level of Significance is desired?   "
  label $f.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  entry $f.textLevel -relief sunken -bd 2  
  
  pack $f.labelTitle -side top -anchor nw
  pack $f.labelLevel -side top -anchor nw
  pack $f.labelLevelExplain -side top -anchor nw
  pack $f.textLevel -side left -fill both -expand 1
}

proc makeWidgetsOnSlopeHypotheis  {f} {
  label $f.labelTitle -fg blue -text "Hypothesis test for the regression slope"
  label $f.labelLevel -text "What level of Significance is desired?   "
  label $f.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  entry $f.textLevel -relief sunken -bd 2  
  
  pack $f.labelTitle -side top -anchor nw
  pack $f.labelLevel -side top -anchor nw
  pack $f.labelLevelExplain -side top -anchor nw
  pack $f.textLevel -side top -fill both -expand 1

  focus $f.textLevel

  frame $f.hypoF -relief flat
  frame $f.hypoF.muType -relief flat
  label $f.hypoF.label1 -text "What is the Null Hypothesis"
  entry $f.hypoF.nullHypo -width 10 -relief sunken
  label $f.hypoF.label2 -text "What is the alternative?"
  radiobutton $f.hypoF.muType.muType1 -value "1" -variable "BTypeTVDSSlope" -text "B\u2260B0"
  radiobutton $f.hypoF.muType.muType2 -value "2" -variable "BTypeTVDSSlope" -text "B>B0"
  radiobutton $f.hypoF.muType.muType3 -value "3" -variable "BTypeTVDSSlope" -text "B<B0"
  $f.hypoF.muType.muType1 select 
  pack $f.hypoF.label1 -side top -fill x -padx 2m -pady 1m
  pack $f.hypoF.nullHypo  -side top -fill x -padx 2m -pady 1m
  pack $f.hypoF.label2 -side top -fill x -padx 2m -pady 1m
  pack $f.hypoF.muType.muType1 -side left -fill x -padx 2m -pady 1m
  pack $f.hypoF.muType.muType2 -side left -fill x -padx 2m -pady 1m
  pack $f.hypoF.muType.muType3 -side left -fill x -padx 2m -pady 1m
  pack $f.hypoF.muType -side top -fill x -padx 2m -pady 1m
  pack $f.hypoF -side top -fill x -padx 2m -pady 1m
}

proc makeWidgetsOnInterceptConfidence  {f} {
  label $f.labelTitle -fg blue -text "Confidence Interval for Intercept"
  label $f.labelLevel  -text "What level of Significance is desired?   "
  label $f.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  entry $f.textLevel -relief sunken -bd 2  
  
  pack $f.labelTitle -side top -anchor nw
  pack $f.labelLevel -side top -anchor nw
  pack $f.labelLevelExplain -side top -anchor nw
  pack $f.textLevel -side left -fill both -expand 1
}

proc makeWidgetsOnInterceptHypotheis  {f} {
  label $f.labelTitle -fg blue -text "Hypothesis test for the Intercept"
  label $f.labelLevel  -text "What level of Significance is desired?   "
  label $f.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  entry $f.textLevel -relief sunken -bd 2  
  
  pack $f.labelTitle -side top -anchor nw
  pack $f.labelLevel -side top -anchor nw
  pack $f.labelLevelExplain -side top -anchor nw
  pack $f.textLevel -side top -fill both -expand 1

  focus $f.textLevel
#  hypothesis widgets
  frame $f.hypoF -relief flat
  frame $f.hypoF.muType -relief flat
  label $f.hypoF.label1  -text "What is the Null Hypothesis"
  entry $f.hypoF.nullHypo -width 10 -relief sunken
  label $f.hypoF.label2  -text "What is the alternative?"
  radiobutton $f.hypoF.muType.muType1 -value "1" -variable "ATypeTVDSIntercept" -text "a\u2260a0"
  radiobutton $f.hypoF.muType.muType2 -value "2" -variable "ATypeTVDSIntercept" -text "a>a0"
  radiobutton $f.hypoF.muType.muType3 -value "3" -variable "ATypeTVDSIntercept" -text "a<a0"
  $f.hypoF.muType.muType1 select 
  pack $f.hypoF.label1 -side top -fill x -padx 2m -pady 1m
  pack $f.hypoF.nullHypo  -side top -fill x -padx 2m -pady 1m
  pack $f.hypoF.label2 -side top -fill x -padx 2m -pady 1m
  pack $f.hypoF.muType.muType1 -side left -fill x -padx 2m -pady 1m
  pack $f.hypoF.muType.muType2 -side left -fill x -padx 2m -pady 1m
  pack $f.hypoF.muType.muType3 -side left -fill x -padx 2m -pady 1m
  pack $f.hypoF.muType -side top -fill x -padx 2m -pady 1m
  pack $f.hypoF -side top -fill x -padx 2m -pady 1m

}

proc makeWidgetsOnModelFitPrediction  {f} {
  label $f.labelTitle -fg blue -text "Test of Model Fit"
  label $f.labelLevel -text "What level of Significance is desired?   "
  label $f.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  entry $f.textLevel -relief sunken -bd 2  
  
  pack $f.labelTitle -side top -anchor nw
  pack $f.labelLevel -side top -anchor nw
  pack $f.labelLevelExplain -side top -anchor nw
  pack $f.textLevel -side left -fill both -expand 1
}

proc makeWidgetsOnPredictionTest  {f} {
  label $f.labelTitle -fg blue -text "Test of Model Fit"
  label $f.labelLevel  -text "What level of Significance is desired?   "
  label $f.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
  entry $f.textLevel -relief sunken -bd 2  

  label $f.labelX0  -text "Please input a predicted point X0:"
  entry $f.textX0 -relief sunken -bd 2  
  
  pack $f.labelTitle -side top -anchor nw
  pack $f.labelLevel -side top -anchor nw
  pack $f.labelLevelExplain -side top -anchor nw
  pack $f.textLevel -side top -fill both -expand 1
  pack $f.labelX0 -side top -anchor nw
  pack $f.textX0 -side top -fill both -expand 1
}

proc TVDSPredictionTotal  { w  title message } {
  destroy $w.frameDef.frameDesc
  destroy $w.frameHypo.frame
  destroy $w.frameDef.frame
  # Make definition field
  frame $w.frameDef.frame -relief flat
  frame $w.frameDef.frame.confidenceF -relief flat
  frame $w.frameDef.frame.hypothesisF -relief flat
  frame $w.frameDef.frame.otherF -relief flat

  frame $w.frameDef.frame.confidenceF.slopeF -relief raised -bd 2 -width 300
  frame $w.frameDef.frame.confidenceF.interceptF -relief raised -bd 2 -width 300
  frame $w.frameDef.frame.hypothesisF.slopeF -relief raised -bd 2 -width 300
  frame $w.frameDef.frame.hypothesisF.interceptF -relief raised -bd 2 -width 300
  frame $w.frameDef.frame.otherF.modelF -relief raised -bd 2 -width 300
  frame $w.frameDef.frame.otherF.predictF -relief raised -bd 2 -width 300
  makeWidgetsOnSlopeConfidence $w.frameDef.frame.confidenceF.slopeF
  makeWidgetsOnSlopeHypotheis $w.frameDef.frame.hypothesisF.slopeF
  makeWidgetsOnInterceptConfidence $w.frameDef.frame.confidenceF.interceptF
  makeWidgetsOnInterceptHypotheis $w.frameDef.frame.hypothesisF.interceptF
  makeWidgetsOnModelFitPrediction $w.frameDef.frame.otherF.modelF
  makeWidgetsOnPredictionTest $w.frameDef.frame.otherF.predictF
  pack $w.frameDef.frame.confidenceF.slopeF -side left -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frameDef.frame.confidenceF.interceptF -side left -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frameDef.frame.hypothesisF.slopeF -side left -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frameDef.frame.hypothesisF.interceptF -side left -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frameDef.frame.otherF.modelF -side left -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frameDef.frame.otherF.predictF -side left -fill both -expand 1 -padx 2m -pady 1m

  pack $w.frameDef.frame.confidenceF -side top -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frameDef.frame.hypothesisF -side top -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frameDef.frame.otherF -side top -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frameDef.frame -side top -fill both -expand 1 -padx 2m -pady 1m

  # Make OK and Cancel buttons
  destroy $w.frameBtn
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "TVDSAllPredictionDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -fill x
  makeDefnHelp $w "DataSetPredictionTotalAnalysis"

}

proc MakeTVDSPredictionAnalysisMenu { w title message } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.analysisType -text { Type } -menu $w.mbar.analysisType.menu
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.analysisType -side left
  pack $w.mbar.help -side right

  menu $w.mbar.analysisType.menu 
    $w.mbar.analysisType.menu  add cascade -label {Inferences Concerning Regression Slope} \
            -menu $w.mbar.analysisType.menu.slope
        menu $w.mbar.analysisType.menu.slope
        $w.mbar.analysisType.menu.slope  add command -label "Hypothesis Test for Regression Slope" \
            -command "TVDSPredictionHypothesisSlope $w \{$title\} \{$message\}"
        $w.mbar.analysisType.menu.slope  add command -label "Confidence Interval for Regression Slope" \
            -command "TVDSPredictionConfidenceSlope $w \{$title\} \{$message\}"
    
    $w.mbar.analysisType.menu  add cascade -label {Inferences Concerning Regression Intercept} \
            -menu $w.mbar.analysisType.menu.intercept
        menu $w.mbar.analysisType.menu.intercept
        $w.mbar.analysisType.menu.intercept  add command -label "Hypothesis Test for Regression Intercept" \
            -command "TVDSPredictionHypothesisIntercept $w \{$title\} \{$message\}" 
        $w.mbar.analysisType.menu.intercept  add command -label "Confidence Interval for Regression Intercept" \
            -command "TVDSPredictionConfidenceIntercept $w \{$title\} \{$message\}"
         
    $w.mbar.analysisType.menu add command -label "Test of Model Fit" \
      -command "TVDSPredictionModelFit $w \{$title\} \{$message\}"
    $w.mbar.analysisType.menu add command -label "Prediction Intervals" \
      -command "TVDSPredictionInterval $w \{$title\} \{$message\}"
    $w.mbar.analysisType.menu add command -label "Total Prediction and Parameter Test" \
      -command "TVDSPredictionTotal $w \{$title\} \{$message\}"

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Prediction and Parameter Testing" \
      -command "showHelp {descriptionOfPredictionParameterTest::Description of Prediction and Parameter Testing}"
  pack $w.mbar -side top -fill x
  
}

proc defineTVDSPredictionAnalysisDialog {title message} {
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  MakeTVDSPredictionAnalysisMenu $w $title $message

  frame $w.frame -relief raised -bd 2
#  label $w.frame.descr -fg blue  -text "The Prediction and Parameter Testing"
#  label $w.frame.descr -fg blue -text "The Prediction and Parameter Testing"
 
  #save the analysis type in this hidden entry

#  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x

  frame $w.frameDef -relief raised -bd 2
  frame $w.frameDef.frameDesc -relief flat
  label $w.frameDef.frameDesc.labelTitle -text "PREDICTION AND PARAMETER TEST"
  label $w.frameDef.frameDesc.labelLevel -text "ANALYSIS TYPE (CLICK ON ONE)"

  frame $w.frameDef.frameDesc.frameCommand -relief raised -bd 2

  button $w.frameDef.frameDesc.frameCommand.btn1 -foreground blue -text "Hypothesis Test for Regression Slope" -relief flat \
             -command "TVDSPredictionHypothesisSlope $w \{$title\} \{$message\}"
  button $w.frameDef.frameDesc.frameCommand.btn2 -foreground blue -text "Confidence Interval for Regression Slope" -relief flat \
             -command "TVDSPredictionConfidenceSlope $w \{$title\} \{$message\}"
  button $w.frameDef.frameDesc.frameCommand.btn3 -foreground blue -text "Hypothesis Test for Regression Intercept" -relief flat \
             -command "TVDSPredictionHypothesisIntercept $w \{$title\} \{$message\}"
  button $w.frameDef.frameDesc.frameCommand.btn4 -foreground blue -text "Confidence Interval for Regression Intercept" -relief flat \
             -command "TVDSPredictionConfidenceIntercept $w \{$title\} \{$message\}"
  button $w.frameDef.frameDesc.frameCommand.btn5 -foreground blue -text "Test of Model Fit" -relief flat \
             -command "TVDSPredictionModelFit $w \{$title\} \{$message\}"
  button $w.frameDef.frameDesc.frameCommand.btn6 -foreground blue -text "Prediction Intervals" -relief flat \
             -command "TVDSPredictionInterval $w \{$title\} \{$message\}"
  button $w.frameDef.frameDesc.frameCommand.btn7 -foreground blue -text "Total Prediction and Parameter Test" -relief flat \
             -command "TVDSPredictionTotal $w \{$title\} \{$message\}"

  pack $w.frameDef.frameDesc.labelTitle -side top -expand 1
  pack $w.frameDef.frameDesc.labelLevel -side top -expand 1
  pack $w.frameDef.frameDesc.frameCommand.btn1 -side top -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand.btn2 -side top -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand.btn3 -side top -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand.btn4 -side top -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand.btn5 -side top -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand.btn6 -side top -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand.btn7 -side top -padx 2m -pady 1m -expand 1

  pack $w.frameDef.frameDesc -side top  -padx 2m -pady 1m -expand 1
  pack $w.frameDef.frameDesc.frameCommand -side top -padx 2m -pady 1m -expand 1

  pack $w.frameDef -fill both -expand 1

  pack $w.frameDef -fill both -expand 1

  frame $w.frameHypo -relief raised -bd 2
  pack $w.frameHypo -fill x

  frame $w.frame3 -relief raised -bd 2
  pack $w.frame3 -side top -fill x

  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetPredictionAnalysis"
  }

  return $w  
}

proc getRedidualPlotZeroY { defn } {
  set listY [lindex $defn 1]
  set strY ""
  for {set i 0 } { $i <[llength $listY] } {incr i 1} {
     set e 0
     lappend strY $e
  }
  return $strY
}

proc getRedidualPlotY { defn slope intercept } {
  set listX [lindex $defn 0]
  set listY [lindex $defn 1]
  set strY ""
  for {set i 0 } { $i <[llength $listX] } {incr i 1} {
     set x [lindex $listX $i]
     set y [lindex $listY $i]
     set e [expr $y-($intercept+$slope*$x)]
     lappend strY $e
  }
  return $strY
}

proc drawResidualPlot {f defn slope intercept} {
  set chart $f.b
  graph $chart -plotbackground gray 
  $chart configure -title "  "
  set strx [lindex $defn 0]
  set stry [getRedidualPlotY $defn $slope $intercept]
  set str0 [getRedidualPlotZeroY $defn]
  set strexp "$chart element create HistoLine -xdata {$strx} -ydata {$stry} -linewidth 0 -label \"data\""
  eval $strexp
  set strexp "$chart element create RegressionLine -xdata {$strx} -ydata {$str0} -pixels 0 -color blue "
  eval $strexp
  $chart element configure RegressionLine -label " Zero Line "
  $chart axis configure x -title "x"
  $chart axis configure y -title "y"
  pack $chart -side left
}

proc MakeResidualViewMenu { w } {
  frame $w.mbar -relief raised -bd 2

  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.help -side right

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Residual Analysis" \
      -command "showHelp {descriptionOfResidualAnalysis::Description of Residual Analysis}"
  pack $w.mbar -side top -fill x
}

proc residualView {viewID} {
  set iconID [get_viewIcon $viewID]
  set strDef [get_objectDefinition $iconID]
  set parameters [get_viewParameter $viewID]
  set w [uniqueDialogName]
  toplevel $w
  eval wm title $w "\"[get_viewTitle $viewID]\""
  eval wm iconname $w "\"[get_viewTitle $viewID]\""
  eval set banner "\"[get_viewBanner $viewID]\""

    # The log:
  MakeResidualViewMenu $w

  frame $w.frame4 -relief raised -bd 2
  frame $w.frame4.frame -relief flat
  text $w.frame4.frame.log -relief sunken -bd 2 -height 15 -width 40 \
    -wrap word -yscrollcommand "$w.frame4.frame.s set"
  scrollbar $w.frame4.frame.s -relief sunken \
    -command "$w.frame4.frame.log yview"
  pack $w.frame4.frame.log -side left -fill both -expand 1
  pack $w.frame4.frame.s -side right -fill y
  pack $w.frame4.frame -side top -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frame4 -fill both -expand 1

  frame $w.frame4.framePlot  -relief flat
  pack $w.frame4.framePlot -side top
  drawResidualPlot $w.frame4.framePlot $strDef [lindex $parameters 0] [lindex $parameters 1]

  #button
  frame $w.frame4.frameButton -relief flat
  button $w.frame4.frameButton.cancel -text Close -state active -width 11 \
    -command "problem_Cancel $w $viewID"

  pack $w.frame4.frameButton.cancel -side right -padx 2m -pady 1m
  pack $w.frame4.frameButton -side top -fill both -expand 1 -padx 2m -pady 1m
 
  viewLogInsert $w.frame4.frame.log [get_viewLog $viewID]

  wm protocol $w WM_DELETE_WINDOW  "problem_Cancel $w $viewID"
  return $w
}

proc DataSetTVDSResidualAnalysisDialog_OK {w title message} {

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

proc setTVDSResidualAnalysisType { w type} {
  setEntryValue $w.frame.analysisType $type
  destroy $w.frameDef.frameDesc
  destroy $w.frameDef.frame
  destroy $w.frameHypo.frame
  # Make definition field
  frame $w.frameDef.frame -relief flat
  if { $type == "C" } {
    label $w.frameDef.frame.labelTitle -text "Confidence Interval for Correlation Coefficient"
    label $w.frameDef.frame.labelLevel -fg blue -text "What level of Confidence is desired?"
    makeDefnHelp $w "DataSetConfidenceCorrelationAnalysis"

  } else {
    label $w.frameDef.frame.labelTitle -text "Hypothesis test for Correlation Coefficient"
    label $w.frameDef.frame.labelLevel -fg blue -text "What level of Significance is desired?"
    makeDefnHelp $w "DataSetHypothesisCorrelationAnalysis"

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
     label $w.frameHypo.frame.label2 -fg blue -text "What is the alternative?"
     radiobutton $w.frameHypo.frame.muType.muType1 -value "1" -variable "MuType" -text "p\u2260p0"
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

  }

  pack $w.frameBtn -fill x
}

proc MakeTVDSResidualAnalysisMenu { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.analysisType -text { Type } -menu $w.mbar.analysisType.menu
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.analysisType -side left
  pack $w.mbar.help -side right

  menu $w.mbar.analysisType.menu 
    $w.mbar.analysisType.menu add command -label "Confidence Interval for the Correlation" \
      -command "setTVDSResidualAnalysisType $w C"

    $w.mbar.analysisType.menu add command -label "Hypothesis Test Concerning the Correlation" \
      -command "setTVDSResidualAnalysisType $w H"

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Correlation Analysis" \
      -command "showHelp {descriptionOfCorrelationAnalysis::Description of Correlation Analysis}"
  pack $w.mbar -side top -fill x
}

proc DataSetTVDSResidualAnalysisControlDialog {title} {
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  MakeTVDSResidualAnalysisMenu $w

  frame $w.frame -relief raised -bd 2
#  label $w.frame.descr -fg blue  -text "Residual Analysis:"
 
  #save the analysis type in this hidden entry
  entry $w.frame.analysisType -width 1 -relief sunken

#  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x

  frame $w.frameDef -relief raised -bd 2
  frame $w.frameDef.frameDesc -relief flat
  label $w.frameDef.frameDesc.labelTitle -text "DATASET CORRELATION ANALYSIS"
  label $w.frameDef.frameDesc.labelLevel -text "CORRELATION ANALYSIS TYPE (CLICK ON ONE)"

  frame $w.frameDef.frameDesc.frameCommand -relief raised -bd 2

  button $w.frameDef.frameDesc.frameCommand.confidence -foreground blue -text "Confidence Interval for the Correlation" -relief flat \
             -command "setTVDSResidualAnalysisType $w C"

  button $w.frameDef.frameDesc.frameCommand.hypothesis -foreground blue -text "Hypothesis Test Concerning the Correlation" -relief flat \
             -command "setTVDSResidualAnalysisType $w H"
  
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

proc defineTVDSRegressionCorrelationAnalysisDialog {title message} {
  # create the widgets
  set w [DataSetTVDSResidualAnalysisControlDialog $title]

  # Make OK and Cancel buttons
  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "mean analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "DataSetTVDSResidualAnalysisDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left \
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x

  frame $w.frame3 -relief raised -bd 2
  pack $w.frame3 -side top -fill x

  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetRegressionCorrelationAnalysis"
  }
}

proc defineTVDSResidualAnalysisDialog {title message} {
  eval [concat toSessionManager "\"$message\""]
}

proc TVDSAlternateModelAnalysisDialog_OK { w title message} {

  # $w is the dialog widget which was OK'd
  global AMType
 
  destroy $w
  set message "$message $AMType"
  eval [concat toSessionManager "\"$message\""]
}

proc MakeTVDSAlternateModelMenu { w } {
  frame $w.mbar -relief raised -bd 2

  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.help -side right

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Alternate Model Analysis" \
      -command "showHelp {descriptionOfAlternateModel::Description of Alternate Model Testing}"
  pack $w.mbar -side top -fill x
}

proc defineTVDSAlternateModelAnalysisDialog {title message} {
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  MakeTVDSAlternateModelMenu $w 

  frame $w.frame -relief raised -bd 2
#  label $w.frame.descr -fg blue  -text "Alternate Model Anlysis"
 
#  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x
  
  frame $w.frameDef -relief flat
  frame $w.frameDef.frame -relief flat
  label $w.frameDef.frame.label1 -fg blue -text "Which Alternate Model would you like to consider"
  radiobutton $w.frameDef.frame.muType1 -value "1" -variable "AMType" -text "Exponential?"
  radiobutton $w.frameDef.frame.muType2 -value "2" -variable "AMType" -text "Power Model?"
  $w.frameDef.frame.muType1 select 

  pack $w.frameDef.frame.label1 -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame.muType1 -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame.muType2 -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef.frame -side top -fill x -padx 2m -pady 1m
  pack $w.frameDef -side top -fill x -padx 2m -pady 1m
  # Make OK and Cancel buttons

  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "Alternate Model Analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "TVDSAlternateModelAnalysisDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame -fill x
  pack $w.frameBtn -side top -fill x -padx 2m -pady 1m

  frame $w.frame3 -relief raised -bd 2
  pack $w.frame3 -side top -fill x

  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetAlternateModelAnalysis"
  }

}

#proc defineTVDSRegressionReportDialog {title message} {
#  eval [concat toSessionManager "\"$message\""]
#}

