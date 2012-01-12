

proc getDataSummaryParametersDialog_ok { f } {
     global dataFrequency dataPercentage
     set cell [$f.paramChoose.entryCell get]
     set width [$f.paramChoose.entryWidth get]
     set startpoint [$f.paramChoose.entryStart get]
     if {[checkGraphicParam $cell $width $startpoint]=="F" } {
        return ""
     }
     set curParameters ""
     if {$dataFrequency=="1" } {
         lappend curParameters "d \{S, $cell, $width, $startpoint\}"
     }
     if {$dataPercentage=="1" } {
         lappend curParameters "d \{P, $cell, $width, $startpoint\}"
     }
          
     if {$curParameters==""} {
          alert "You must choose one data summary analysis."
      }
     return $curParameters
}

proc getGraphicalAnalysisParametersDialog_ok { f } {
     global histogramGraphic frequencyGraphic compareGraphic
     set cell [$f.paramChoose.entryCell get]
     set width [$f.paramChoose.entryWidth get]
     set startpoint [$f.paramChoose.entryStart get]
     if {[checkGraphicParam $cell $width $startpoint]=="F" } {
        return ""
     }
     set curParameters ""
     if {$histogramGraphic=="1" } {
         lappend curParameters "g \{H, $cell, $width, $startpoint\}"
     }
     if {$frequencyGraphic=="1" } {
         lappend curParameters "g \{F, $cell, $width, $startpoint\}"
     }
     if {$compareGraphic=="1" } {
         lappend curParameters "g \{C, $cell, $width, $startpoint\}"
     }          
     if {$curParameters==""} {
          alert "You must choose one graphical analysis."
      }
     return $curParameters
}

proc getMeanConfidenceParametersDialog_ok {f} {
     set level [$f.paramChoose.level get]
     if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
        alert "You must give a value of level between 0 and 1 for mean confidence analysis"
        return ""
     }
     set curParameters "m \{C $level\}"
     return $curParameters
}


proc getVarianceConfidenceParametersDialog_ok {f} {
     set level [$f.paramChoose.level get]
     if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
        alert "You must give a value of level between 0 and 1 for variance confidence analysis"
        return ""
     }
     set curParameters "v \{C $level\}"
     return $curParameters
}

proc getMeanHypothesisParametersDialog_ok {f} {
     global MuTypeMeanReport
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
     set curParameters "m \{h $nullHypo $level $MuTypeMeanReport\}"
     return $curParameters
}


proc getVarianceHypothesisParametersDialog_ok {f} {
     global MuTypeVarianceReport
     set level [$f.paramChoose.textLevel get]
     if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
        alert "You must give a value of level between 0 and 1 for variance hypothesis analysis"
        return ""
     }
     set nullHypo [$f.paramChoose.nullHypo get]
     if {[regexp {^([-]?[0-9]*[.]?[0-9]*)$} $nullHypo]==0} {
       alert "You must give a value of MU0 for variance hypothesis analysis"
       return ""
     }
     set curParameters "v \{h $nullHypo $level $MuTypeVarianceReport\}"
     return $curParameters
}


proc getFitAnalysisParametersDialog_ok {f} {
     global MuTypeFitReport
     set level [$f.paramChoose.textLevel get]
     if {[regexp {^([0]?[.][0-9]*)$} $level]==0} {
        alert "You must give a value of level between 0 and 1 for goofness of fit analysis"
        return ""
     }
     set curParameters "f \{$MuTypeFitReport $level \}"
     return $curParameters
}

proc OVDSReportAnalysisDialog_OK {w title message} {
  global dataSummaryReport graphicalAnalysisReport meanConfidenceReport fullAnalysisReport
  global varianceConfidenceReport meanHypothesisReport varianceHypothesisReport fitAnalysisReport
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

  if { $varianceConfidenceReport=="1" } {
    set curParameters [getVarianceConfidenceParametersDialog_ok $w.choiceFrm.confidenceF.varianceAnalysisF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }
  
  if { $meanHypothesisReport=="1" } {
    set curParameters [getMeanHypothesisParametersDialog_ok $w.choiceFrm.hypothesisF.meanAnalysisF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }

  if { $varianceHypothesisReport=="1" } {
    set curParameters [getVarianceHypothesisParametersDialog_ok $w.choiceFrm.hypothesisF.varianceAnalysisF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }
  
  if { $fitAnalysisReport=="1" } {
    set curParameters [getFitAnalysisParametersDialog_ok $w.choiceFrm.confidenceF.fitAnalysisF]
    if {$curParameters==""} { return }
    lappend Parameters $curParameters
  }
  destroy $w
  set message "$message \{$Parameters\}"
  postToLog $message
  eval [concat toSessionManager "\"$message\""]
}

proc setOVDSReportAnalysisType { w type } {
  set reportType $type
  if { $reportType == "F" } {
    $w.choiceFrm.fullAnalysis.mainchoose.choosenReport configure -state enabled
    $w.choiceFrm.summaryGraphF.dataSummaryF.choosenReport  configure -state enabled
    $w.choiceFrm.summaryGraphF.graphicalAnalysisF.choosenReport  configure -state enabled
    $w.choiceFrm.confidenceF.meanAnalysisF.choosenReport   configure -state enabled
    $w.choiceFrm.confidenceF.varianceAnalysisF.choosenReport   configure -state enabled
    $w.choiceFrm.confidenceF.fitAnalysisF.choosenReport   configure -state enabled    
    $w.choiceFrm.hypothesisF.meanAnalysisF.choosenReport   configure -state enabled
    $w.choiceFrm.hypothesisF.varianceAnalysisF.choosenReport   configure -state enabled
    
  } else {
    $w.choiceFrm.fullAnalysis.mainchoose.choosenReport configure -state disabled
    $w.choiceFrm.summaryGraphF.dataSummaryF.choosenReport  configure -state disabled
    $w.choiceFrm.summaryGraphF.graphicalAnalysisF.choosenReport  configure -state disabled
    $w.choiceFrm.confidenceF.meanAnalysisF.choosenReport   configure -state disabled
    $w.choiceFrm.confidenceF.varianceAnalysisF.choosenReport   configure -state disabled
    $w.choiceFrm.confidenceF.fitAnalysisF.choosenReport   configure -state disabled    
    $w.choiceFrm.hypothesisF.meanAnalysisF.choosenReport   configure -state disabled
    $w.choiceFrm.hypothesisF.varianceAnalysisF.choosenReport   configure -state disabled
 }
}

proc MakeOVDSReportAnalysisMenu { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.analysisType -text { Type } -menu $w.mbar.analysisType.menu
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.analysisType -side left
  pack $w.mbar.help -side right

  menu $w.mbar.analysisType.menu 
    $w.mbar.analysisType.menu add command -label "select all" \
      -command "setOVDSReportAnalysisType $w F"

    $w.mbar.analysisType.menu add command -label "unselect all" \
      -command "setOVDSReportAnalysisType $w P"

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Dataset Report" \
      -command "showHelp {OVDSReport::How Dataset Report Works}"
  pack $w.mbar -side top -fill x

}

proc  makeWidgetsOnFullAnalysisF {f} {
    frame $f.mainchoose -relief sunken -bd 2  -width 400
    checkbutton $f.mainchoose.choosenReport -variable fullAnalysisReport -text "Full Analysis" -foreground blue -width 114 -justify left
    pack $f.mainchoose -side left 
    pack $f.mainchoose.choosenReport -side left 
}

proc  makeWidgetsOnDataSummaryF {f} {
    frame $f.mainchoose -relief sunken -bd 2  -width 200
    frame $f.typeChoose -relief flat
    frame $f.paramChoose -relief flat
    checkbutton $f.mainchoose.choosenReport -variable "dataSummaryReport" -text "Data Summary" -foreground blue -width 55
    checkbutton $f.typeChoose.dataFrequency -text "Frequency Summary"
    checkbutton $f.typeChoose.dataPercentage -text "Percentage Summary"
    pack $f.typeChoose.dataFrequency -side left
    pack $f.typeChoose.dataPercentage -side right
  #cell,width,startpoint
    label $f.paramChoose.labelCell -text "Cell Number:"
    entry $f.paramChoose.entryCell -width 5 -relief sunken
    label $f.paramChoose.labelWidth -text "Cell Width:"
    entry $f.paramChoose.entryWidth -width 5 -relief sunken
    label $f.paramChoose.labelStart -text "Starting Point:"
    entry $f.paramChoose.entryStart -width 5 -relief sunken

    pack $f.paramChoose.labelCell -side left 
    pack $f.paramChoose.entryCell -side left 
    pack $f.paramChoose.labelWidth -side left
    pack $f.paramChoose.entryWidth -side left
    pack $f.paramChoose.labelStart -side left
    pack $f.paramChoose.entryStart -side right 

    
    pack $f.mainchoose.choosenReport -side left
    pack $f.mainchoose -side top
    pack $f.typeChoose -side top    
    pack $f.paramChoose -side top
}

proc  makeWidgetsOnGraphicalAnalysisF {f} {
    frame $f.mainchoose -relief sunken -bd 2
    frame $f.typeChoose -relief flat
    frame $f.paramChoose -relief flat 
    checkbutton $f.mainchoose.choosenReport -variable graphicalAnalysisReport -text "Graphical Analysis" \
          -foreground blue -width 55
    checkbutton $f.typeChoose.histogramGraphic -text "Histogram"
    checkbutton $f.typeChoose.frequencyGraphic -text "Frequency Polygon"
    checkbutton $f.typeChoose.compareGraphic -text "Compare with Normal"
    pack $f.typeChoose.histogramGraphic -side left
    pack $f.typeChoose.frequencyGraphic -side left
    pack $f.typeChoose.compareGraphic -side right

  #cell,width,startpoint
    label $f.paramChoose.labelCell -text "Cell Number:"
    entry $f.paramChoose.entryCell -width 5 -relief sunken
    label $f.paramChoose.labelWidth -text "Cell Width:"
    entry $f.paramChoose.entryWidth -width 5 -relief sunken
    label $f.paramChoose.labelStart -text "Starting Point:"
    entry $f.paramChoose.entryStart -width 5 -relief sunken

    pack $f.paramChoose.labelCell -side left 
    pack $f.paramChoose.entryCell -side left 
    pack $f.paramChoose.labelWidth -side left 
    pack $f.paramChoose.entryWidth -side left 
    pack $f.paramChoose.labelStart -side left 
    pack $f.paramChoose.entryStart -side right 

    
    pack $f.mainchoose.choosenReport -side left
    pack $f.mainchoose -side top
    pack $f.typeChoose -side top    
    pack $f.paramChoose -side top

}

proc  makeWidgetsOnMeanConfidenceF {f} {
    frame $f.mainChoose -relief sunken -bd 2 
    frame $f.paramChoose -relief flat 
    label $f.paramChoose.labelLevel -text "What level of Confidence is desired?"
    label $f.paramChoose.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"

    entry $f.paramChoose.level -width 5 -relief sunken

    label $f.paramChoose.appendAlian -text "  " -pady 5

    checkbutton $f.mainChoose.choosenReport -variable meanConfidenceReport -text "Mean Confidence Analysis" -foreground blue -width 33

    pack $f.paramChoose.labelLevel -side top -padx 2m
    pack $f.paramChoose.labelLevelExplain -side top -padx 2m
    pack $f.paramChoose.level -side top -padx 2m 
    pack $f.paramChoose.appendAlian -side top -padx 2m 
    pack $f.mainChoose.choosenReport -side left
    pack $f.mainChoose -side top
    pack $f.paramChoose -side top

}

proc  makeWidgetsOnVarianceConfidenceF {f} {
    frame $f.mainchoose -relief sunken -bd 2
    frame $f.paramChoose -relief flat
    label $f.paramChoose.labelLevel -text "What level of Confidence is desired?"
    label $f.paramChoose.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
    label $f.paramChoose.appendAlian -text "  "

    entry $f.paramChoose.level -width 5 -relief sunken

    checkbutton $f.mainchoose.choosenReport -variable varianceConfidenceReport \
                -text "Variance Confidence Analysis" -foreground blue -width 33

    pack $f.paramChoose.labelLevel -side top -padx 2m
    pack $f.paramChoose.labelLevelExplain -side top -padx 2m
    pack $f.paramChoose.level -side top -padx 2m 
    pack $f.paramChoose.appendAlian -side top -padx 2m -pady 5
    
    pack $f.mainchoose.choosenReport -side left
    pack $f.mainchoose -side top
    pack $f.paramChoose -side top

}

proc  makeWidgetsOnMeanHypothesisF {f} {
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
    label $f.paramChoose.label2  -text "What is the alternative?"
  
    pack $f.paramChoose.labelLevel -side top -anchor nw
    pack $f.paramChoose.labelLevelExplain -side top -anchor nw
    pack $f.paramChoose.textLevel -side top -fill both -expand 1
    pack $f.paramChoose.label1 -side top -anchor nw
    pack $f.paramChoose.nullHypo -side top -fill both -expand 1
    pack $f.paramChoose.label2 -side top -fill both -expand 1

    radiobutton $f.typeChoose.muType1 -value "1" -variable "MuTypeMeanReport" -text "MU!=MU0"
    radiobutton $f.typeChoose.muType2 -value "2" -variable "MuTypeMeanReport" -text "MU>MU0"
    radiobutton $f.typeChoose.muType3 -value "3" -variable "MuTypeMeanReport" -text "MU<MU0"
    $f.typeChoose.muType1 select 
    pack $f.typeChoose.muType1 -side left -fill x -padx 2m -pady 1m
    pack $f.typeChoose.muType2 -side left -fill x -padx 2m -pady 1m
    pack $f.typeChoose.muType3 -side left -fill x -padx 2m -pady 1m

    
    pack $f.mainchoose.choosenReport -side left
    pack $f.mainchoose -side top
    pack $f.paramChoose -side top
    pack $f.typeChoose -side top    
}

proc  makeWidgetsOnVarianceHypothesisF {f} {
    frame $f.mainchoose -relief sunken -bd 2
    frame $f.typeChoose -relief flat
    frame $f.paramChoose -relief flat
    checkbutton $f.mainchoose.choosenReport -variable varianceHypothesisReport \
              -text "Variance Hypothesis Analysis" -foreground blue -width 55


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

    radiobutton $f.typeChoose.muType1 -value "1" -variable "MuTypeVarianceReport" -text "MU!=MU0"
    radiobutton $f.typeChoose.muType2 -value "2" -variable "MuTypeVarianceReport" -text "MU>MU0"
    radiobutton $f.typeChoose.muType3 -value "3" -variable "MuTypeVarianceReport" -text "MU<MU0"
    $f.typeChoose.muType1 select 
    pack $f.typeChoose.muType1 -side left -fill x -padx 2m -pady 1m
    pack $f.typeChoose.muType2 -side left -fill x -padx 2m -pady 1m
    pack $f.typeChoose.muType3 -side left -fill x -padx 2m -pady 1m

    
    pack $f.mainchoose.choosenReport -side left
    pack $f.mainchoose -side top
    pack $f.paramChoose -side top
    pack $f.typeChoose -side top   

}

proc  makeWidgetsOnFitAnalysisF {f} {
    frame $f.mainchoose -relief sunken -bd 2
    frame $f.typeChoose -relief flat
    frame $f.paramChoose -relief flat

    checkbutton $f.mainchoose.choosenReport -variable fitAnalysisReport -text "Goodness of Fit Analysis" -foreground blue -width 35

    label $f.paramChoose.labelLevel -fg blue -text "What level of Significance is desired?"
    label $f.paramChoose.labelLevelExplain -fg green -text "(must be a number between 0 and 1)"
    entry $f.paramChoose.textLevel -relief sunken -bd 2  
  
    pack $f.paramChoose.labelLevel -side top -anchor nw
    pack $f.paramChoose.labelLevelExplain -side top -anchor nw
    pack $f.paramChoose.textLevel -side left -fill both -expand 1

    radiobutton $f.typeChoose.muType1 -value "1" -variable "MuTypeFitReport" -text "Normal?"
    radiobutton $f.typeChoose.muType2 -value "2" -variable "MuTypeFitReport" -text "Exponential?"
    radiobutton $f.typeChoose.muType3 -value "3" -variable "MuTypeFitReport" -text "Uniform?"
    $f.typeChoose.muType1 select 
    pack $f.typeChoose.muType1 -side left -fill x -padx 2m -pady 1m
    pack $f.typeChoose.muType2 -side left -fill x -padx 2m -pady 1m
    pack $f.typeChoose.muType3 -side left -fill x -padx 2m -pady 1m

    
    pack $f.mainchoose.choosenReport -side left
    pack $f.mainchoose -side top
    pack $f.paramChoose -side top
    pack $f.typeChoose -side top   

}

proc defineOVDSReportAnalysisDialog {title message} {
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
  MakeODVSReportViewMenu $w

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
  frame $w.choiceFrm.hypothesisF  -relief raised -bd 2 -width 400
  frame $w.choiceFrm.hypothesisF.meanAnalysisF  -relief raised -bd 2 -width 200
  frame $w.choiceFrm.hypothesisF.varianceAnalysisF  -relief raised -bd 2 -width 200
  frame $w.choiceFrm.confidenceF.fitAnalysisF   -relief raised -bd 2 -width 400
  #make widgets on frame
  makeWidgetsOnFullAnalysisF $w.choiceFrm.fullAnalysisF
  makeWidgetsOnDataSummaryF $w.choiceFrm.summaryGraphF.dataSummaryF
  makeWidgetsOnGraphicalAnalysisF $w.choiceFrm.summaryGraphF.graphicalAnalysisF
  makeWidgetsOnMeanConfidenceF $w.choiceFrm.confidenceF.meanAnalysisF
  makeWidgetsOnVarianceConfidenceF $w.choiceFrm.confidenceF.varianceAnalysisF
  makeWidgetsOnMeanHypothesisF $w.choiceFrm.hypothesisF.meanAnalysisF
  makeWidgetsOnVarianceHypothesisF $w.choiceFrm.hypothesisF.varianceAnalysisF
  makeWidgetsOnFitAnalysisF $w.choiceFrm.confidenceF.fitAnalysisF

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
  pack $w.choiceFrm.hypothesisF.varianceAnalysisF  -side right
  pack $w.choiceFrm -side top

  # Make OK and Cancel buttons

  frame $w.frameBtn -relief raised -bd 2
  frame $w.frameBtn.frame -relief flat
  button $w.frameBtn.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  set title "compare variance analysis"
  button $w.frameBtn.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "OVDSReportAnalysisDialog_OK $w \{$title\} \{$message\}"
  
  pack $w.frameBtn.frame.ok $w.frameBtn.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frameBtn.frame.cancel -side right
  pack $w.frameBtn.frame -side top -fill x
  pack $w.frameBtn -side top -fill x
}



## {type cell width startpoint} 
proc drawGraphicalReport { f strDef Params } {
   frame $f.graphicalFrame -relief flat -bd 2
   frame $f.summaryFrame -relief flat -bd 2
   pack $f.summaryFrame -side top -fill x
   pack $f.graphicalFrame -side top -fill x
   for {set index 0 } { $index < [llength $Params] } {incr index 1} {
      set perParam [lindex $Params $index]
      set type [lindex $perParam 0]
      set cell [lindex $perParam 1]
      set width [lindex $perParam 2]
      set startpoint [lindex $perParam 3]
      if {$index<2} {
         set fName "$f.summaryFrame.graphic$index"
      } else {
         set fName "$f.graphicalFrame.graphc$index"
      }
      set fGraph [frame $fName  -relief flat -bd 2]
      drawGraghOnFrame $type $cell $width $startpoint $strDef $fGraph 
      pack $fGraph -side left -fill x
  }
}

proc MakeODVSReportViewMenu { w } {
  frame $w.mbar -relief raised -bd 2
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu
  pack $w.mbar.help -side right
  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about One Variable Dattaset Report" \
      -command "showHelp {OVDSReport::How Dataset Report Works}"
  pack $w.mbar -side top -fill x
}

proc ODVSReportView {viewID} {
  set iconID [get_viewIcon $viewID]
  set strDef [get_objectDefinition $iconID]
  set parameters [get_viewParameter $viewID]
  set w [uniqueDialogName]
  toplevel $w
  eval wm title $w "\"[get_viewTitle $viewID]\""
  eval wm iconname $w "\"[get_viewTitle $viewID]\""
  eval set banner "\"[get_viewBanner $viewID]\""
  MakeODVSReportViewMenu $w
# datasummary and graphical
  frame $w.graphframe -relief flat -bd 2
  drawGraphicalReport $w.graphframe $strDef $parameters
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
     -command "printOVDSReport $w \{$parameters\} $strDef"

  pack $f.ok -side left -padx 2m -pady 1m

  pack $f.cancel -side right -padx 2m -pady 1m

  pack $w.frame3 -side top -fill x

  wm protocol $w WM_DELETE_WINDOW  "problem_Cancel $w $viewID"

  return $w
}

proc printOVDSReport { w Params defn} {
   printText $w.frame4.frame.log
#  print summary
   for {set index 0 } { $index < [llength $Params] } {incr index 1} {
      set perParam [lindex $Params $index]
      set type [lindex $perParam 0]
      set cell [lindex $perParam 1]
      set width [lindex $perParam 2]
      set startpoint [lindex $perParam 3]
      if {$type=="S"||$type=="P"} {
         set strFirst [getFirstColumnDef $cell $width $startpoint $defn]
         set strSecond [getSecondColumnDef $cell $width $startpoint $defn $type]
         printSummary $strFirst $strSecond
      } else {
           if {$index<2} {
                 set fName "$w.graphframe.summaryFrame.graphic$index"
           } else {
                 set fName "$w.graphframe.graphicalFrame.graphc$index"
           }
           printGraph $fName.b0
      }
    }
}

proc printGraph {graphicWidget} {
  global tmp_dir
  $graphicWidget postscript output $tmp_dir/graphicOutput.ps
  if { [catch {exec lpr $tmp_dir/graphicOutput.ps} ] } {
	alert "sorry, can not print the result. please check your print configure!"
  } 
}
