proc getAlternateModelParametersDialog_ok  {f} {
     global AMType
     set curParameters "M \{$AMType\}"
     return $curParameters
}

proc getCorrelationConfidenceParametersDialog_ok  {f} {
     set level [$f.paramChoose.level get]
     if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
        alert "You must give a value of level between 0 and 1 for correlation confidence analysis"
        return ""
     }
     set curParameters "C \{C $level\}"
     return $curParameters
}

proc getCorrelationHypothesisParametersDialog_ok  {f} {
     global MuTypeCorrelationReport
     set level [$f.paramChoose.textLevel get]
     if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
        alert "You must give a value of level between 0 and 1 for correlation hypothesis analysis"
        return ""
     }
     set nullHypo [$f.paramChoose.nullHypo get]
     if {[regexp {^([-]?[0-9]*[.]?[0-9]*)$} $nullHypo]==0} {
       alert "You must give a value of MU0 for correlation hypothesis analysis"
       return ""
     }
     set curParameters "c \{h $nullHypo $level $MuTypeCorrelationReport\}"
     return $curParameters
}

proc TVDSReportRegressionDialog_OK {w title message} {
  global fullRegressionReport graphicalRegressionReport residualAnalysisReport alternateModelReport 
  global correlationConfidenceReport correlationHypothesisReport
  set Parameters ""

  if { $fullRegressionReport=="1" } {
    set curParameters "\{ F \{ \} \}"
    set Parameters [concat $Parameters $curParameters]
  }
 
  if { $graphicalRegressionReport=="1" } {
    set curParameters "\{ G \{ \} \}"
    set Parameters [concat $Parameters $curParameters]
  }

  if { $residualAnalysisReport=="1" } {
    set curParameters "\{ R \{ \} \}"
    set Parameters [concat $Parameters $curParameters]
  }

  if { $alternateModelReport=="1" } {
    set curParameters [getAlternateModelParametersDialog_ok $w.choiceFrm1.modelAnalysisF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }

  if { $correlationConfidenceReport=="1" } {
    set curParameters [getCorrelationConfidenceParametersDialog_ok $w.choiceFrm2.correlationAnalysisF.confidenceF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }

  if { $correlationHypothesisReport=="1" } {
    set curParameters [getCorrelationHypothesisParametersDialog_ok $w.choiceFrm2.correlationAnalysisF.hypothesisF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }
  destroy $w
  set message "$message \{$Parameters\}"
  postToLog $message
  eval [concat toSessionManager "\"$message\""]
}

proc MakeTVDSReportRegressionMenu  { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu
  pack $w.mbar.help -side right
  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about One Variable Dattaset Report" \
      -command "showHelp {descriptionOfTVDSRegressionReport::Description of Two Variable Regression Report}"
  pack $w.mbar -side top -fill x
}

proc makeWidgetsOnFullRegressionF  {f} {
    frame $f.mainchoose -relief sunken -bd 2 
    checkbutton $f.mainchoose.choosenReport -variable fullRegressionReport -text "Full Regression Analysis" -foreground blue -justify left
    pack $f.mainchoose -side left 
    pack $f.mainchoose.choosenReport -side left 
}

proc makeWidgetsOnGraphicalRegressionAnalysisF  {f} {
    frame $f.mainchoose -relief sunken -bd 2  
    checkbutton $f.mainchoose.choosenReport -variable graphicalRegressionReport -text "Graphical Analysis" -foreground blue -justify left
    pack $f.mainchoose -side left 
    pack $f.mainchoose.choosenReport -side left 
}

proc makeWidgetsOnResidualAnalysisF  {f} {
    frame $f.mainchoose -relief sunken -bd 2  
    checkbutton $f.mainchoose.choosenReport -variable residualAnalysisReport -text "Residual Analysis" -foreground blue  -justify left
    pack $f.mainchoose -side left 
    pack $f.mainchoose.choosenReport -side left 
}

proc makeWidgetsOnTVDSAlternateModelF  {f} {
    frame $f.mainchoose -relief sunken -bd 2  
    frame $f.paramChoose -relief flat 
    checkbutton $f.mainchoose.choosenReport -variable alternateModelReport -text "Alternate Model Analysis" -foreground blue  -justify left

    label $f.paramChoose.label1 -text "Which Alternate Model would you like to consider"
    radiobutton $f.paramChoose.muType1 -value "1" -variable "AMType" -text "Exponential?"
    radiobutton $f.paramChoose.muType2 -value "2" -variable "AMType" -text "Power Model?"
    $f.paramChoose.muType1 select 

    pack $f.paramChoose.label1 -side top -fill x -padx 2m -pady 1m
    pack $f.paramChoose.muType1 -side top -fill x -padx 2m -pady 1m
    pack $f.paramChoose.muType2 -side top -fill x -padx 2m -pady 1m

    pack $f.mainchoose -side top
    pack $f.paramChoose  -side top 
    pack $f.mainchoose.choosenReport -side left 
}

proc makeWidgetsOnTVDSCorrelationConfidenceF  {f} {
    frame $f.mainChoose -relief sunken -bd 2 
    frame $f.paramChoose -relief flat 
    label $f.paramChoose.labelLevel -text "What level of Confidence is desired?"
    label $f.paramChoose.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"

    entry $f.paramChoose.level -width 5 -relief sunken

    label $f.paramChoose.appendAlian -text "  " -pady 5

    checkbutton $f.mainChoose.choosenReport -variable correlationConfidenceReport -text "Correlation Confidence Analysis" -foreground blue -width 33

    pack $f.paramChoose.labelLevel -side top -padx 2m
    pack $f.paramChoose.labelLevelExplain -side top -padx 2m
    pack $f.paramChoose.level -side top -padx 2m 
    pack $f.paramChoose.appendAlian -side top -padx 2m 
    pack $f.mainChoose.choosenReport -side left
    pack $f.mainChoose -side top
    pack $f.paramChoose -side top
}

proc makeWidgetsOnTVDSCorrelationHypothesisF  {f} {
    frame $f.mainchoose -relief sunken -bd 2
    frame $f.typeChoose -relief flat
    frame $f.paramChoose -relief flat
    checkbutton $f.mainchoose.choosenReport -variable correlationHypothesisReport \
               -text "Correlation Hypothesis Analysis" -foreground blue -width 55

    label $f.paramChoose.labelLevel  -text "What level of Significance is desired?"
    label $f.paramChoose.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
    entry $f.paramChoose.textLevel -relief sunken -bd 2  
    label $f.paramChoose.label1  -text "What is the Null Hypothesis"
    entry $f.paramChoose.nullHypo -width 10 -relief sunken
    label $f.paramChoose.label2  -text "What is the alternative?"
  
    pack $f.paramChoose.labelLevel -side top -anchor nw
    pack $f.paramChoose.labelLevelExplain -side top -anchor nw
    pack $f.paramChoose.textLevel -side top -fill both -expand 1
    pack $f.paramChoose.label1 -side top -anchor nw
    pack $f.paramChoose.nullHypo -side top -fill both -expand 1
    pack $f.paramChoose.label2 -side top -fill both -expand 1

    radiobutton $f.typeChoose.muType1 -value "1" -variable "MuTypeCorrelationReport" -text "p!=p0"
    radiobutton $f.typeChoose.muType2 -value "2" -variable "MuTypeCorrelationReport" -text "p>p0"
    radiobutton $f.typeChoose.muType3 -value "3" -variable "MuTypeCorrelationReport" -text "p<p0"
    $f.typeChoose.muType1 select 
    pack $f.typeChoose.muType1 -side left -fill x -padx 2m -pady 1m
    pack $f.typeChoose.muType2 -side left -fill x -padx 2m -pady 1m
    pack $f.typeChoose.muType3 -side left -fill x -padx 2m -pady 1m

    pack $f.mainchoose.choosenReport -side left
    pack $f.mainchoose -side top
    pack $f.paramChoose -side top
    pack $f.typeChoose -side top    
}

proc defineTVDSRegressionReportAnalysisDialog {title message} {
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
  MakeTVDSReportRegressionMenu $w

  frame $w.frame -relief raised -bd 2
  label $w.frame.descr -fg blue  -text "Dataset Report:"
  pack $w.frame.descr -side top
  pack $w.frame -side top

  # make choice frames
  frame $w.choiceFrm1 -relief raised -bd 2 -width 400
  frame $w.choiceFrm2 -relief raised -bd 2 -width 400
  frame $w.choiceFrm1.flatAnalysisF  -relief raised -bd 2 -width 400
  frame $w.choiceFrm1.flatAnalysisF.fullAnalysisF  -relief flat -bd 2 -width 200
  frame $w.choiceFrm1.flatAnalysisF.graphicalAnalysisF  -relief flat -bd 2 -width 200
  frame $w.choiceFrm1.flatAnalysisF.residualAnalysisF  -relief flat -bd 2 -width 200

  frame $w.choiceFrm1.modelAnalysisF  -relief raised -bd 2 -width 200

  frame $w.choiceFrm2.correlationAnalysisF  -relief raised -bd 2 -width 400
  frame $w.choiceFrm2.correlationAnalysisF.confidenceF  -relief raised -bd 2 -width 200
  frame $w.choiceFrm2.correlationAnalysisF.hypothesisF  -relief raised -bd 2 -width 200

  #make widgets on frame
  makeWidgetsOnFullRegressionF $w.choiceFrm1.flatAnalysisF.fullAnalysisF
  makeWidgetsOnGraphicalRegressionAnalysisF $w.choiceFrm1.flatAnalysisF.graphicalAnalysisF
  makeWidgetsOnResidualAnalysisF $w.choiceFrm1.flatAnalysisF.residualAnalysisF
  makeWidgetsOnTVDSAlternateModelF $w.choiceFrm1.modelAnalysisF
  makeWidgetsOnTVDSCorrelationConfidenceF $w.choiceFrm2.correlationAnalysisF.confidenceF
  makeWidgetsOnTVDSCorrelationHypothesisF  $w.choiceFrm2.correlationAnalysisF.hypothesisF

  pack $w.choiceFrm1.flatAnalysisF  -side left
  pack $w.choiceFrm1.flatAnalysisF.fullAnalysisF  -side top
  pack $w.choiceFrm1.flatAnalysisF.graphicalAnalysisF  -side top
  pack $w.choiceFrm1.flatAnalysisF.residualAnalysisF  -side top

  pack $w.choiceFrm1.modelAnalysisF  -side right
  pack $w.choiceFrm2.correlationAnalysisF -side top
  pack $w.choiceFrm2.correlationAnalysisF.confidenceF  -side left
  pack $w.choiceFrm2.correlationAnalysisF.hypothesisF  -side right
  pack $w.choiceFrm1 -side top
  pack $w.choiceFrm2 -side top

  # Make OK and Cancel buttons

  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "compare variance analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "TVDSReportRegressionDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame.cancel -side right
  pack $w.frameBtn.frame -side top -fill x
  pack $w.frameBtn -side top -fill x
}

proc drawTVDSRegressionGraphicalReport { f strDef Params } {
   postToLog $Params 
   set reportParam [lindex $Params 0]
   set slopeParam [lindex $Params 1]
   set slope [lindex $slopeParam 0]
   set intercept [lindex $slopeParam 1]
   postToLog "$reportParam"
   for {set index 0 } { $index < [llength $reportParam] } {incr index 1} {
      postToLog "$reportParam"
      set reportType [lindex $reportParam $index]
      set fName "$f.graphicalFrame$index"
      set fGraph [frame $fName  -relief flat -bd 2]
      if { $reportType=="G" } {
        postToLog "test1"
        drawRegressionGraph $strDef $fGraph $slope $intercept
      }
      if { $reportType=="R" } {
        postToLog "test2"
        drawResidualPlot $fGraph $strDef $slope $intercept
      }
      pack $fGraph -side left -fill x
  }
}

proc TVDSReportRegressionView {viewID} {
  set iconID [get_viewIcon $viewID]
  set strDef [get_objectDefinition $iconID]
  set parameters [get_viewParameter $viewID]
  set w [uniqueDialogName]
  toplevel $w
  eval wm title $w "\"[get_viewTitle $viewID]\""
  eval wm iconname $w "\"[get_viewTitle $viewID]\""
  eval set banner "\"[get_viewBanner $viewID]\""
# datasummary and graphical
  MakeTVDSReportRegressionMenu $w

  frame $w.graphframe -relief flat -bd 2
  drawTVDSRegressionGraphicalReport $w.graphframe $strDef $parameters
  pack $w.graphframe -side top -fill both -expand 1
# The log:
  frame $w.frame1 -relief raised -bd 2
  frame $w.frame1.frame -relief flat
  label $w.frame1.label -text "Report"
  pack $w.frame1.label -side top -anchor nw
  pack $w.frame1.frame -side top -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frame1 -fill both -expand 1

# log text
  frame $w.frame4 -relief raised -bd 2
  frame $w.frame4.frame -relief flat
  text $w.frame4.frame.log -relief sunken -bd 2 -height 15 -width 30 \
    -wrap word -yscrollcommand "$w.frame4.frame.s set"
  scrollbar $w.frame4.frame.s -relief sunken \
    -command "$w.frame4.frame.log yview"
  pack $w.frame4.frame.log -side left -fill both -expand 1
  pack $w.frame4.frame.s -side right -fill y
  pack $w.frame4.frame -side top -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frame4 -fill both -expand 1
  viewLogInsert $w.frame4.frame.log [get_viewLog $viewID]

# ok button
  frame $w.frame3 -relief raised -bd 2
  set f $w.frame3

  button $f.cancel -text Close -state active -width 11 \
    -command "problem_Cancel $w $viewID"

  button $f.ok -text Print -state active -width 11 \
     -command "printTVDSReportRegression $w"

  pack $f.ok -side left -padx 2m -pady 1m

  pack $f.cancel -side right -padx 2m -pady 1m

  pack $w.frame3 -side top -fill x

  wm protocol $w WM_DELETE_WINDOW  "problem_Cancel $w $viewID"

  return $w
}

proc printTVDSReportRegression { w } {
   printText $w.frame4.frame.log
}
