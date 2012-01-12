# 
proc getTVDSVarianceHypothesisParametersDialog_ok {f} {
     set level [$f.paramChoose.textLevel get]
     if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
        alert "You must give a value of level between 0 and 1 for goofness of fit analysis"
        return ""
     }
     set curParameters "v \{ V $level \}"
     return $curParameters
}

proc getNonParametricParametersDialog_ok {f} {
     set level [$f.paramChoose.textLevel get]
     if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
        alert "You must give a value of level between 0 and 1 for goofness of fit analysis"
        return ""
     }
     set curParameters "n \{ $level \}"
     return $curParameters
}

proc getCompareMeanHypothesisParametersDialog_ok {f} {
     global MuTypeMeanReport equalAssumed
     set level [$f.paramChoose.textLevel get]
     if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
        alert "You must give a value of level between 0 and 1 for mean hypothesis analysis"
        return ""
     }
     set nullHypo [$f.paramChoose.nullHypo get]
     if {[regexp {^([-]?[0-9]*[.]?[0-9]*)$} $nullHypo]==0} {
       alert "You must give a value of MU0 for mean hypothesis analysis"
       return ""
     }
     set curParameters "m \{h $nullHypo $level $MuTypeMeanReport $equalAssumed\}"
     return $curParameters
}

proc TVDSReportIndependentDialog_OK {w title message} {
  global dataSummaryReport graphicalAnalysisReport meanConfidenceReport fullAnalysisReport
  global nonParametricReport meanHypothesisReport varianceHypothesisReport fitAnalysisReport
  set Parameters ""

  if { $fullAnalysisReport=="1" } {
    set curParameters "\{ F \{ \} \}"
    set Parameters [concat $Parameters $curParameters]
  }

  if { $dataSummaryReport=="1" } {
    set curParameters [getDataSummaryParametersDialog_ok $w.choiceFrm.summaryGraphF.dataSummaryF]
    if {$curParameters==""} { return }
    set Parameters [concat $Parameters $curParameters]
  }

  if { $graphicalAnalysisReport=="1" } {
    set curParameters [getGraphicalAnalysisParametersDialog_ok $w.choiceFrm.summaryGraphF.graphicalAnalysisF]
    if {$curParameters==""} { return }
    set Parameters [concat $Parameters $curParameters]
  }

  if { $meanConfidenceReport=="1" } {
    set curParameters [getMeanConfidenceParametersDialog_ok $w.choiceFrm.confidenceF.meanAnalysisF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }

  if { $meanHypothesisReport=="1" } {
    set curParameters [getCompareMeanHypothesisParametersDialog_ok $w.choiceFrm.hypothesisF.meanAnalysisF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }

  if { $varianceHypothesisReport=="1" } {
    set curParameters [getTVDSVarianceHypothesisParametersDialog_ok $w.choiceFrm.confidenceF.varianceAnalysisF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }
  
  if { $fitAnalysisReport=="1" } {
    set curParameters [getFitAnalysisParametersDialog_ok $w.choiceFrm.confidenceF.fitAnalysisF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }

  if { $nonParametricReport=="1" } {
    set curParameters [getNonParametricParametersDialog_ok $w.choiceFrm.hypothesisF.nonParametricAnalysisF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }

  destroy $w
  set message "$message \{$Parameters\}"
  postToLog $message
  eval [concat toSessionManager "\"$message\""]
}

proc makeWidgetsOnTVDSVarianceHypothesisF  { f } {
    frame $f.mainchoose -relief sunken -bd 2
    frame $f.typeChoose -relief flat
    frame $f.paramChoose -relief flat

    checkbutton $f.mainchoose.choosenReport -variable varianceHypothesisReport -text "Hypothesis Testing for Variance" -foreground blue -width 35

    label $f.paramChoose.labelLevel -fg blue -text "What level of Significance is desired?"
    label $f.paramChoose.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
    entry $f.paramChoose.textLevel -relief sunken -bd 2  
  
    pack $f.paramChoose.labelLevel -side top -anchor nw
    pack $f.paramChoose.labelLevelExplain -side top -anchor nw
    pack $f.paramChoose.textLevel -side left -fill both -expand 1

    label $f.paramChoose.appendAlian -text "  " -pady 5
    pack $f.paramChoose.appendAlian -side top -padx 2m 
   
    pack $f.mainchoose.choosenReport -side left
    pack $f.mainchoose -side top
    pack $f.paramChoose -side top
    pack $f.typeChoose -side top   

}

proc makeWidgetsOnNonParametricF  {f} {
    frame $f.mainchoose -relief sunken -bd 2
    frame $f.typeChoose -relief flat
    frame $f.paramChoose -relief flat

    checkbutton $f.mainchoose.choosenReport -variable nonParametricReport -text "Non-Parametric Tests" -foreground blue -width 35

    label $f.paramChoose.labelLevel -fg blue -text "What level of Significance is desired?"
    label $f.paramChoose.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
    entry $f.paramChoose.textLevel -relief sunken -bd 2  
  
    pack $f.paramChoose.labelLevel -side top -anchor nw
    pack $f.paramChoose.labelLevelExplain -side top -anchor nw
    pack $f.paramChoose.textLevel -side left -fill both -expand 1

    
    pack $f.mainchoose.choosenReport -side left
    pack $f.mainchoose -side top
    pack $f.paramChoose -side top
    pack $f.typeChoose -side top   

}

proc makeWidgetsOnCompareMeanHypothesisF {f} {
    frame $f.mainchoose -relief sunken -bd 2
    frame $f.typeChoose -relief flat
    frame $f.paramChoose -relief flat
    checkbutton $f.mainchoose.choosenReport -variable meanHypothesisReport \
               -text "Mean Hypothesis Analysis" -foreground blue -width 55

    label $f.paramChoose.labelLevel  -text "What level of Significance is desired?"
    label $f.paramChoose.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
    entry $f.paramChoose.textLevel -relief sunken -bd 2  
    label $f.paramChoose.label1  -text "What is the Null Hypothesis"
    entry $f.paramChoose.nullHypo -width 10 -relief sunken
    checkbutton $f.paramChoose.equalAssumed -variable "equalAssumed" -onvalue "T" -offvalue "F" \
              -text "Should equal variance be assumed"
    
    pack $f.paramChoose.labelLevel -side top -anchor nw
    pack $f.paramChoose.labelLevelExplain -side top -anchor nw
    pack $f.paramChoose.textLevel -side top -fill both -expand 1
    pack $f.paramChoose.label1 -side top -anchor nw
    pack $f.paramChoose.nullHypo -side top -fill both -expand 1
    pack $f.paramChoose.equalAssumed -side top -fill both -expand 1
    
    label $f.paramChoose.label2  -text "What is the alternative?"
    radiobutton $f.typeChoose.muType1 -value "1" -variable "MuTypeMeanReport" -text "MU!=MU0"
    radiobutton $f.typeChoose.muType2 -value "2" -variable "MuTypeMeanReport" -text "MU>MU0"
    radiobutton $f.typeChoose.muType3 -value "3" -variable "MuTypeMeanReport" -text "MU<MU0"
    $f.typeChoose.muType1 select 
    pack $f.paramChoose.label2 -side top -fill both -expand 1
    pack $f.typeChoose.muType1 -side left -fill x -padx 2m -pady 1m
    pack $f.typeChoose.muType2 -side left -fill x -padx 2m -pady 1m
    pack $f.typeChoose.muType3 -side left -fill x -padx 2m -pady 1m

    
    pack $f.mainchoose.choosenReport -side left
    pack $f.mainchoose -side top
    pack $f.paramChoose -side top
    pack $f.typeChoose -side top    

}

proc MakeTVDSReportIndependentMenu  { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu
  pack $w.mbar.help -side right
  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about One Variable Dattaset Report" \
      -command "showHelp {descriptionOfTVDSIndependentReport::Description of Two Variable Independent Report}"
  pack $w.mbar -side top -fill x
}

proc defineTVDSReportIndependentDialog {title message} {
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
  MakeTVDSReportIndependentMenu $w

  frame $w.frame -relief raised -bd 2
  label $w.frame.descr -fg blue  -text "Dataset Report:"
  pack $w.frame.descr -side top
  pack $w.frame -side top

  # make choice frames
  frame $w.choiceFrm -relief raised -bd 2 -width 400
  frame $w.choiceFrm.fullAnalysisF  -relief raised -bd 2 -width 400
  frame $w.choiceFrm.summaryGraphF  -relief raised -bd 2 -width 400
  frame $w.choiceFrm.summaryGraphF.dataSummaryF  -relief raised -bd 2 -width 200
  frame $w.choiceFrm.summaryGraphF.graphicalAnalysisF  -relief raised -bd 2 -width 200
  frame $w.choiceFrm.confidenceF  -relief raised -bd 2 -width 400 
  frame $w.choiceFrm.confidenceF.meanAnalysisF  -relief raised -bd 2 -width 200
  frame $w.choiceFrm.confidenceF.varianceAnalysisF  -relief raised -bd 2 -width 200
  frame $w.choiceFrm.confidenceF.fitAnalysisF   -relief raised -bd 2 -width 400
  frame $w.choiceFrm.hypothesisF  -relief raised -bd 2 -width 400
  frame $w.choiceFrm.hypothesisF.meanAnalysisF  -relief raised -bd 2 -width 200
  frame $w.choiceFrm.hypothesisF.nonParametricAnalysisF  -relief raised -bd 2 -width 200

  #make widgets on frame
  makeWidgetsOnFullAnalysisF $w.choiceFrm.fullAnalysisF
  makeWidgetsOnDataSummaryF $w.choiceFrm.summaryGraphF.dataSummaryF
  makeWidgetsOnGraphicalAnalysisF $w.choiceFrm.summaryGraphF.graphicalAnalysisF
  makeWidgetsOnMeanConfidenceF $w.choiceFrm.confidenceF.meanAnalysisF
  makeWidgetsOnTVDSVarianceHypothesisF $w.choiceFrm.confidenceF.varianceAnalysisF
  makeWidgetsOnFitAnalysisF $w.choiceFrm.confidenceF.fitAnalysisF
  makeWidgetsOnCompareMeanHypothesisF $w.choiceFrm.hypothesisF.meanAnalysisF
  makeWidgetsOnNonParametricF $w.choiceFrm.hypothesisF.nonParametricAnalysisF

  pack $w.choiceFrm.fullAnalysisF  -side top
  pack $w.choiceFrm.summaryGraphF  -side top
  pack $w.choiceFrm.summaryGraphF.dataSummaryF  -side left
  pack $w.choiceFrm.summaryGraphF.graphicalAnalysisF  -side right
  pack $w.choiceFrm.confidenceF  -side top
  pack $w.choiceFrm.confidenceF.meanAnalysisF  -side left
  pack $w.choiceFrm.confidenceF.varianceAnalysisF  -side left
  pack $w.choiceFrm.confidenceF.fitAnalysisF   -side right
  pack $w.choiceFrm.hypothesisF  -side top
  pack $w.choiceFrm.hypothesisF.meanAnalysisF  -side left
  pack $w.choiceFrm.hypothesisF.nonParametricAnalysisF  -side right
  pack $w.choiceFrm -side top

  # Make OK and Cancel buttons

  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "compare variance analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "TVDSReportIndependentDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame.cancel -side right
  pack $w.frameBtn.frame -side top -fill x
  pack $w.frameBtn -side top -fill x
}

## {type cell width startpoint} 
proc drawTVDSGraphicalReport { f strDef Params } {
   for {set index 0 } { $index < [llength $Params] } {incr index 1} {
      set perParam [lindex $Params $index]
      set type [lindex $perParam 0]
      set cell [lindex $perParam 1]
      set width [lindex $perParam 2]
      set startpoint [lindex $perParam 3]
      set fName "$f.graphicalFrame$index"
      set fGraph [frame $fName  -relief flat -bd 2]

      drawGraghOnFrame $type $cell $width $startpoint $strDef $fGraph 
      pack $fGraph -side top -fill x
  }
}

proc TVDSReportIndependentView {viewID} {
  set iconID [get_viewIcon $viewID]
  set strDef [get_objectDefinition $iconID]
  set parameters [get_viewParameter $viewID]
  set w [uniqueDialogName]
  toplevel $w
  eval wm title $w "\"[get_viewTitle $viewID]\""
  eval wm iconname $w "\"[get_viewTitle $viewID]\""
  eval set banner "\"[get_viewBanner $viewID]\""
# datasummary and graphical
  MakeTVDSReportIndependentMenu $w

  frame $w.graphframe -relief flat -bd 2
  drawTVDSGraphicalReport $w.graphframe $strDef $parameters
  pack $w.graphframe -side top -fill both -expand 1
# The log:
  postToLog $parameters
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
     -command "printTVDSReportIndependent $w"

  pack $f.ok -side left -padx 2m -pady 1m

  pack $f.cancel -side right -padx 2m -pady 1m

  pack $w.frame3 -side top -fill x

  wm protocol $w WM_DELETE_WINDOW  "problem_Cancel $w $viewID"

  return $w
}

proc printTVDSReportIndependent { w } {
   printText $w.frame4.frame.log
}
