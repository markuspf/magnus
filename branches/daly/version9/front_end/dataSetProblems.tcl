#  added by xuxw
proc setEntryValue { entryCtl strValue } {
  set strTemp [$entryCtl get]
  $entryCtl delete 0 [string length $strTemp]
  $entryCtl insert 0 $strValue
}

proc setGramType {w type} {
  setEntryValue $w.frame.graphicType $type
}
proc getSetMax {defination} {
  set defn [lindex $defination 0]
  set max [lindex $defn 0]
  for { set i 0 } { $i < [llength $defination] } {incr i 1} {
     set defn [lindex $defination $i]
     for {set index 0 } { $index < [llength $defn] } {incr index 1} {
         if { [lindex $defn $index] > $max } {
             set max [lindex $defn $index]
         }
     }
  }
  return $max
}
proc getSetMin {defination} {
  set defn [lindex $defination 0]
  set min [lindex $defn 0]
  for { set i 0 } { $i < [llength $defination] } {incr i 1} {
     set defn [lindex $defination $i]
     for {set index 1 } { $index < [llength $defn] } {incr index 1} {
         if { [lindex $defn $index] < $min } {
             set min [lindex $defn $index]
         }
     }
  }
  return $min
}

proc setGraphicDefaultValue {defination w} {
#  set defination [lindex $defination [$w.frame.setNum get]]
  set startpoint [getSetMin $defination]
  set maxValue [getSetMax $defination]
  if {$startpoint==$maxValue}  {
      incr maxValue
      set cell 1
  }  else {
      set cell 8
  }
  set width [expr ceil(double($maxValue - $startpoint) / $cell)]
  setEntryValue $w.frameParamCell.entryCell $cell
  setEntryValue $w.frameParamWidth.entryWidth $width
  setEntryValue $w.frameParamStart.entryStart $startpoint
}

proc makeControlPanel { w } {
  frame $w.frameParamCell -relief raised -bd 2
  frame $w.frameParamWidth -relief raised -bd 2
  frame $w.frameParamStart -relief raised -bd 2

  label $w.frameParamCell.labelCell -text "Number of Cells:"
  entry $w.frameParamCell.entryCell -width 5 -relief sunken
  label $w.frameParamWidth.labelWidth -text "Width of Cells:"
  entry $w.frameParamWidth.entryWidth -width 5 -relief sunken
  label $w.frameParamStart.labelStart -text "Starting Point:"
  entry $w.frameParamStart.entryStart -width 5 -relief sunken

  pack $w.frameParamCell.labelCell -side left -padx 2m
  pack $w.frameParamCell.entryCell -side right -padx 2m 
  pack $w.frameParamWidth.labelWidth -side left -padx 2m
  pack $w.frameParamWidth.entryWidth -side right -padx 2m 
  pack $w.frameParamStart.labelStart -side left -padx 2m
  pack $w.frameParamStart.entryStart -side right -padx 2m 
  pack $w.frameParamCell -side top -fill x
  pack $w.frameParamWidth -side top -fill x
  pack $w.frameParamStart -side top -fill x
}

proc makeGraphicViewMenu { w setSize} {
  frame $w.mbar -relief raised -bd 2

  menubutton $w.mbar.gramType -text {Graph Type} -menu $w.mbar.gramType.menu
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack $w.mbar.gramType -side left
  pack $w.mbar.help -side right

  menu $w.mbar.gramType.menu 
    $w.mbar.gramType.menu add command -label "Histogram" \
      -command "setGramType $w H"

    $w.mbar.gramType.menu add command -label "Frequency Polygon" \
      -command "setGramType $w F"

    $w.mbar.gramType.menu add command -label "Comparision with Normal" \
      -command "setGramType $w C"

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Graphical Analysis" \
      -command "showHelp {graphicAnalysis::How Graphical Analysis Works}"
  pack $w.mbar -side top -fill x
}

proc graphicView {viewID} {

  global A tcl_path
  set w [uniqueDialogName]
  set iconID [get_viewIcon $viewID]
  set strDef [get_objectDefinition $iconID]
  toplevel $w
  eval wm title $w "\"[get_viewTitle $viewID]\""
  eval wm iconname $w "\"[get_viewTitle $viewID]\""
  #@db wm minsize $w 300 300
  # Make menu bar

  makeGraphicViewMenu $w [llength $strDef]

  # The banner frame:

  eval set banner "\"[get_viewBanner $viewID]\""

  set height [expr [string length $banner] / 20]

  frame $w.frame -relief raised -bd 2
  set heritage [get_viewHeritage $viewID]
  label $w.frame.descr -relief flat -text $heritage
  #save the graphic type in this hidden entry
  entry $w.frame.graphicType -width 1 -relief sunken
  setGramType $w H
  #save the analysis type in this hidden entry
  entry $w.frame.setNum -width 1 -relief sunken
  setEntryValue $w.frame.setNum 0

  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x

  # The control panel:
  #cell,width,startpoint
  makeControlPanel $w

  # ok button
  frame $w.frame3 -relief raised -bd 2
  set f $w.frame3
  button $f.cancel -text Dismiss -state active -width 11 \
    -command "problem_Cancel $w $viewID"
  button $f.ok -text Recalculate -state active -width 11 \
    -command "drawGraph {$strDef} $w"

  pack $f.ok -side left -padx 2m -pady 1m
  pack $f.cancel -side right -padx 2m -pady 1m

  pack $w.frame3 -side top -fill x

  wm protocol $w WM_DELETE_WINDOW  "problem_Cancel $w $viewID"
#set the default value
  eval "setGraphicDefaultValue {$strDef} $w"
 eval "drawGraph {$strDef} $w"
  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetGraphicalAnalysis"
  }
  return $w
}

proc checkGraphicParam {cell width startPoint} {
   if {[regexp {^([1-9][0-9]*)$} $cell]==0} {
     alert "You must give a positive integer for cell number."
     return "F"
   }

   if {[regexp {^([0-9]*[.]?[0-9]*)$} $width]==0} {
     alert "You must give a positive value for width."
     return "F"
   }
   if {[regexp {^([-]*[0-9]*[.]?[0-9]*)$} $startPoint]==0} {
     alert "You must give a float value for startPoint."
     return "F"
   }

   return "T"
}

proc drawGraghOnFrame { graphicType cell width startpoint defination f } {
  for {set index 0 } { $index< [llength $defination] } {incr index 1} {
     set chartName "$f.b$index"
     destroy $chartName
     set defn [lindex $defination $index]
     switch $graphicType {
         H {
            drawHistogram $cell $width $startpoint $defn $chartName
         }
         F {
            drawFrequencygram $cell $width $startpoint $defn $chartName
         }
         C {
            drawCompgram $cell $width $startpoint $defn $chartName
         }
         #frequency summary
         S {
            drawFrequencyTable $cell $width $startpoint $defn $chartName
         }
         #percentage summary
         P {
            drawPercentageTable $cell $width $startpoint $defn $chartName
         }
    }
  }
}
 
proc drawGraph { defination w} {
#     set defination [lindex $defination [$w.frame.setNum get]]
     set graphicType [$w.frame.graphicType get]
     set cell [$w.frameParamCell.entryCell get]
     set width [$w.frameParamWidth.entryWidth get]
     set startpoint [$w.frameParamStart.entryStart get]
    if { [checkGraphicParam $cell $width $startpoint]=="F"} {
      return
    }

    drawGraghOnFrame $graphicType $cell $width $startpoint $defination $w
}

proc drawGramInit { cell width startpoint defn chart} {
  $chart configure -title "  "
  set strx [getGraphicX "$defn" $cell $width $startpoint]
  set stry [getGraphicY "$defn" $cell $width $startpoint]
  set strexp "$chart element create HistoLine -xdata {$strx} -ydata {$stry} "
  eval $strexp
  $chart element configure HistoLine -label " Frequency Line "
  $chart axis configure x -stepsize $width -title "value" -min $startpoint
  $chart axis configure y -title "Number"
  pack $chart -side left -expand 1
}

proc drawHistogram { cell width startpoint defn chartName} {
    barchart $chartName -plotbackground gray 
    drawGramInit $cell $width $startpoint $defn $chartName
    $chartName element configure HistoLine -barwidth $width -foreground blue
}

proc drawFrequencygram { cell width startpoint defn chartName} {
    graph $chartName -plotbackground gray 
    drawGramInit $cell $width $startpoint $defn $chartName
}

proc drawCompgram { cell width startpoint defination chartName} {
    barchart $chartName -plotbackground gray
    # add the standard line here
    set strx [getGraphicX "$defination" $cell $width $startpoint]
    set stry [getStandardGraphicY $cell $width $startpoint "$defination"]
    set strexp "$chartName line create StandardLine -xdata {$strx} -ydata {$stry} -smooth quadratic"
    eval $strexp

    drawGramInit $cell $width $startpoint $defination $chartName
    $chartName element configure HistoLine -barwidth $width -foreground blue

}

proc getGraphicX { defn cell width start } {
  set strX ""
  for {set i 1 } { $i<= $cell } {incr i 1} {
     set x [expr $start + $width * ($i - 1.0/2.0) ]
     lappend strX $x
  }
  return $strX
}

proc getGraphicY { defn cell width start } {
  set strY ""
  for {set i 1 } { $i <= $cell } {incr i 1} {
     set y 0
     for {set index 0 } { $index < [llength $defn] } {incr index 1} {
        set curValue [lindex $defn $index]
        if { $curValue>=[expr $start + $width * ($i-1)] && $curValue<[expr $start + $width * $i] } {
            incr y 1
        }
     }
     lappend strY $y
  }
  return $strY
}

proc GetMean { defn } {
  set sum 0
  for {set i 0 } { $i< [llength $defn] } {incr i 1} {
     set x [lindex $defn $i]
     set sum [expr $x+$sum]
  }
  return [expr $sum/[llength $defn]]
}

proc GetStandardDeviation { defn } {
  set mean [GetMean $defn]
  set len [llength $defn]
  set sum 0
  if {$len==1} {
     return 0;
  } 
  for {set i 0 } { $i< $len } {incr i 1} {
     set x [lindex $defn $i]
     set sum [expr $sum+pow(($x-$mean),2)]
  }
  return [expr sqrt($sum/($len-1))]
}

proc getStandardGraphicY { cell width startpoint defn } {
  set mean [GetMean $defn]
  set len [llength $defn]
  set dv [GetStandardDeviation $defn]
  for {set i 1 } { $i <= $cell } {incr i 1} {
     set x [expr $startpoint+$width*($i-1)]
     set strExec "exp(-pow(($mean-$x)/$dv,2)/2)/($dv*sqrt(2*acos(-1)))"
     if {$dv==0} {
        set y 0
     } else {
        set y [expr $strExec*$width]
        set y [expr $y*$len]
     }
     lappend strY $y
  }
  return $strY
}


#
#  set s 1
#}
proc makeResultViewHelpMenu { w heritage } {
   frame $w.mbar -relief raised -bd 2
   menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu
   pack $w.mbar.help -side right
   set helpTitle "No Help So far!"
   set helpTopic "fullAnalysis::How Full Analysis Works"

   switch $heritage {
      "DataSet Full Analysis" {
          set helpTitle "How Full Analysis Works"
          set helpTopic "fullAnalysis::How Full Analysis Works"
       }
      "Analysis of the Mean for a One Valriable Data Set" {
          set helpTitle "How Mean Analysis Works"
          set helpTopic "meanAnalysis::How Mean Analysis Works"
      }
      "DataSet Variance Analysis" {
          set helpTitle "How Variance Analysis Works"
          set helpTopic "varianceAnalysis::How Variance Analysis Works"
      }
      "Goodness of Fit Analysis" {
          set helpTitle "How Goodness Of Fit Works"
          set helpTopic "goodnessOfFit::How Goodness Of Fit Works"
      }

      "DataSet Compare Mean Analysis" {
          set helpTitle "How Compare Mean Analysis Works"
          set helpTopic "descriptionOfCompareMean::Description of Compare Mean Analysis"
      }

      "DataSet Compare Variance Analysis" {
          set helpTitle "How Compare Variance Analysis Works"
          set helpTopic "descriptionOfCompareVariance::Description of Compare Variance Analysis"
      }

      "DataSet Non-Parametric Analysis" {
          set helpTitle "How Non_Parametric Testing Works"
          set helpTopic "descriptionOfNonParametricTest::Description of Non-Parametric Testing"
      }

      "DataSet Full Regression Analysis" {
          set helpTitle "How Full Regression Analysis Works"
          set helpTopic "descriptionOfFullRegression::Description of Full Regression Analysis"
      }

      "DataSet Prediction and Parameter Testing" {
          set helpTitle "How Prediction and Parameter Testing Works"
          set helpTopic "descriptionOfPredictionParameterTest::Description of Prediction and Parameter Testing"
      }

      "DataSet Alternate Model Analysis" {
          set helpTitle "How Alternate Model Analysis Works"
          set helpTopic "descriptionOfAlternateModel::Description of Alternate Model Testing"
      }

      "DataSet Correlation Analysis" {
          set helpTitle "How Correlation Analysis Works"
          set helpTopic "descriptionOfCorrelationAnalysis::Description of Correlation Analysis"
      }

  }
  menu $w.mbar.help.menu
  $w.mbar.help.menu add command -label $helpTitle \
      -command "showHelp {$helpTopic}"
  pack $w.mbar -side top -fill x

}

#resultView
proc resultView {viewID} {
  global tcl_path
  set w [uniqueDialogName]
  toplevel $w
  eval wm title $w "\"[get_viewTitle $viewID]\""
  eval wm iconname $w "\"[get_viewTitle $viewID]\""
  eval set banner "\"[get_viewBanner $viewID]\""
    # The log:

  frame $w.frame4 -relief raised -bd 2
  frame $w.frame4.frameImage -relief flat
  frame $w.frame4.frame -relief flat
  set heritage [get_viewHeritage $viewID]
  #make help menu according to heritage.
  makeResultViewHelpMenu $w $heritage
  label $w.frame4.frameImage.label -fg blue -text $heritage
#  label $w.frame4.frameImage.picture -bitmap @$tcl_path/bitmaps/analysis.xbm
  text $w.frame4.frame.log -relief sunken -bd 2 -height 25 -width 70 \
    -wrap word -yscrollcommand "$w.frame4.frame.s set"
  scrollbar $w.frame4.frame.s -relief sunken \
    -command "$w.frame4.frame.log yview"
  pack $w.frame4.frameImage.label -side left -anchor nw
  pack $w.frame4.frame.log -side left -fill both -expand 1
  pack $w.frame4.frame.s -side right -fill y
  pack $w.frame4.frameImage -side top -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frame4.frame -side top -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frame4 -fill both -expand 1

  frame $w.frame4.frameButton -relief flat
  button $w.frame4.frameButton.cancel -text Close -state active -width 11 \
    -command "problem_Cancel $w $viewID"

  button $w.frame4.frameButton.print -text Print -state active -width 11 \
     -command "printText $w.frame4.frame.log"

  pack $w.frame4.frameButton.print -side left -padx 2m -pady 1m
  pack $w.frame4.frameButton.cancel -side right -padx 2m -pady 1m
  pack $w.frame4.frameButton -side top -fill both -expand 1 -padx 2m -pady 1m
 
  viewLogInsert $w.frame4.frame.log [get_viewLog $viewID]

  wm protocol $w WM_DELETE_WINDOW  "problem_Cancel $w $viewID"

  return $w

}

proc makeSummaryViewMenu { w setSize} {

  frame $w.mbar -relief raised -bd 2

  menubutton $w.mbar.summaryType -text {Summary Type} -menu $w.mbar.summaryType.menu
  menubutton $w.mbar.datasetNum -text {Which dataset} -menu $w.mbar.datasetNum.menu
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu
  
  pack $w.mbar.summaryType -side left
  pack $w.mbar.help -side right

  menu $w.mbar.summaryType.menu 
  $w.mbar.summaryType.menu add command -label "Frequency" \
      -command "setGramType $w S"

    $w.mbar.summaryType.menu add command -label "Percentage" \
      -command "setGramType $w P"

  menu $w.mbar.help.menu
    $w.mbar.help.menu add command -label "about Data Summary" \
      -command "showHelp {dataSummary::How Data Summary Works}"
  pack $w.mbar -side top -fill x
}

proc summaryView {viewID} {
  global A tcl_path
  set w [uniqueDialogName]
  set iconID [get_viewIcon $viewID]
  set strDef [get_objectDefinition $iconID]
  toplevel $w
  eval wm title $w "\"[get_viewTitle $viewID]\""
  eval wm iconname $w "\"[get_viewTitle $viewID]\""
  #@db wm minsize $w 300 300

  # Make menu bar
  makeSummaryViewMenu  $w [llength $strDef]

  # The banner frame:

  eval set banner "\"[get_viewBanner $viewID]\""
  set heritage [get_viewHeritage $viewID]

  set height [expr [string length $banner] / 20]

  frame $w.frame -relief raised -bd 2
  text $w.frame.descr -height $height -width 20 -relief flat -wrap word
  $w.frame.descr insert end $heritage
  $w.frame.descr configure -state disabled
  #save the summary type in this hidden entry
  entry $w.frame.graphicType -width 1 -relief sunken
  setGramType $w S
  #save the analysis type in this hidden entry
  entry $w.frame.setNum -width 1 -relief sunken
  setEntryValue $w.frame.setNum 0

  pack $w.frame.descr -side top -fill x -padx 2m -pady 1m
  pack $w.frame -fill x

  # The control panel:
  #cell,width,startpoint
  makeControlPanel $w

  # ok button
  frame $w.frame3 -relief raised -bd 2
  set f $w.frame3
  button $f.cancel -text Dismiss -state active -width 11 \
    -command "problem_Cancel $w $viewID"

  button $f.ok -text Recalculate -state active -width 11 \
     -command "drawGraph {$strDef} $w"

  pack $f.ok -side left -padx 2m -pady 1m

  pack $f.cancel -side right -padx 2m -pady 1m

  pack $w.frame3 -side top -fill x -expand 1

  wm protocol $w WM_DELETE_WINDOW  "problem_Cancel $w $viewID"
  eval "setGraphicDefaultValue {$strDef} $w"
  eval "drawGraph {$strDef} $w"
  if { [inNoviceMode] } {
    makeDefnHelp $w "DataSetDataSummary"
  }
  return $w
}

proc drawFrequencyTable { cell width startpoint defn chartName } {
  drawTableInit $cell $width $startpoint $defn $chartName "F"
}

proc drawPercentageTable { cell width startpoint defn chartName } {
  drawTableInit $cell $width $startpoint $defn $chartName "P"
}

proc fillOutFirstColumn { cell width startpoint defn w} {

  set c C0
  for {set i 0} {$i < $cell} {incr i 1} {
     set strStart [expr $startpoint+$i*$width]
     set strStop [expr $strStart+$width]
     set strValue "$strStart - $strStop"
     set strEntry "$w.b.entry$i$c"
     set strCommand "setEntryValue $strEntry {$strValue}"
     eval $strCommand
  }
}
proc getFirstColumnDef { cell width startpoint defn} {
  set str " "
  for {set i 0} {$i < $cell} {incr i 1} {
     set strStart [expr $startpoint+$i*$width]
     set strStop [expr $strStart+$width]
     set strValue "$strStart---$strStop"
     set str " $str $strValue"
  }
  return $str
}
proc getSecondColumnDef { cell width startpoint defn graphicType} {
  set str " "
  for {set i 1 } { $i <= $cell } {incr i 1} {
     set iNum 0
     for {set index 0 } { $index < [llength $defn] } {incr index 1} {
        set curValue [lindex $defn $index]
        if { $curValue>=[expr $startpoint + $width * ($i-1)] && $curValue<[expr $startpoint + $width * $i] } {
            incr iNum 1
        }
     }
     set iValue $iNum
     if { $graphicType == "P" } {
        set iValue [expr double($iValue)/[llength $defn]*100]
        set iValue [format "%.2f" $iValue]
        set iValue "$iValue%"
     } 
     set str "$str $iValue"
  }
  return $str
}

proc fillOutSecondColumn { cell width startpoint defn w} {
  set c C1
  set index 0
  set curValue [lindex $defn $index]
  for {set i 0} {$i < $cell} {incr i 1} {
     set iStart [expr $startpoint+$i*$width]
     set iStop [expr $iStart+$width]
     set iNum 0
     while { $curValue < $iStop } {
        incr index 1
        incr iNum 1
        if { $index < [llength $defn] } {
              set curValue [lindex $defn $index]
        } else {
             # set maximum value represent there is no more data
             set curValue [expr $iStart + $width * $cell]
        }
     }
     set strEntry "$w.b.entry$i$c"
     set iValue $iNum
     set graphicType [$w.frame.graphicType get]
     if { $graphicType == "P" } {
        set iValue [expr double($iValue)/[llength $defn]*100]
        set iValue "$iValue%"
     } 
     
     set strCommand "setEntryValue $strEntry {$iValue}"
     eval $strCommand
  }
}

proc drawTableInit { cell width startpoint defn chartName graphicType} {
   set strFirst [getFirstColumnDef $cell $width $startpoint $defn]
   set strSecond [getSecondColumnDef $cell $width $startpoint $defn $graphicType]
   frame $chartName -relief raised -bd 2 -width 20
   createTable $chartName $cell 2 " {$strFirst} {$strSecond}"
   pack $chartName -side left -expand 1
}

#fitView
proc fitView {viewID} {
  set w [uniqueDialogName]
  toplevel $w
  eval wm title $w "\"[get_viewTitle $viewID]\""
  eval wm iconname $w "\"[get_viewTitle $viewID]\""
  eval set banner "\"[get_viewBanner $viewID]\""
    # The log:

  frame $w.frame4 -relief raised -bd 2
  frame $w.frame4.frame -relief flat
  label $w.frame4.label -text "Goodness of Fit"
  text $w.frame4.frame.log -relief sunken -bd 2 -height 25 -width 30 \
    -wrap word -yscrollcommand "$w.frame4.frame.s set"
  scrollbar $w.frame4.frame.s -relief sunken \
    -command "$w.frame4.frame.log yview"
  pack $w.frame4.label -side top -anchor nw
  pack $w.frame4.frame.log -side left -fill both -expand 1
  pack $w.frame4.frame.s -side right -fill y
  pack $w.frame4.frame -side top -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frame4 -fill both -expand 1
  viewLogInsert $w.frame4.frame.log [get_viewLog $viewID]

  wm protocol $w WM_DELETE_WINDOW  "problem_Cancel $w $viewID"

  return $w
}

