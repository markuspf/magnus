## Copyright (C) 1995 The New York Group Theory Cooperative
## See magnus/doc/COPYRIGHT for the full notice.

## Contents: Code supporting the greeting window.
##
## Principal Author: Roger Needham, Dmitry Bormotov
##
## Status: In progress.
##
## Revision History:
##
## 03/96 Dmitry B. made porting to tcl7.4/tk4.0 
##

##done when group theory is chosen

proc makeGreetingCopyright   { bitMapName } {
  global tcl_path
  if { [file exists "$tcl_path/bitmaps/$bitMapName"] } {
     label .frame.label -bitmap @$tcl_path/bitmaps/$bitMapName
  }
  pack .frame.label -side left -ipadx 5m
}

proc setGreetingBitmap {} {
  global subject tcl_path
  destroy .frame.label
  set bitmapName "greeting.xbm"
  if { $subject == "\"STATISTICS\"" } {
     set bitmapName "greetingStats.xbm"
  }
  if { $subject == "\"CALCULUS\"" } {
     set bitmapName "greetingCalc.xbm"
  }
  makeGreetingCopyright  $bitmapName
}

proc menuGroupClicked {} {
  global subject
  set subject \"GROUP_THEORY\"
  setGreetingBitmap
}

proc menuStatsClicked {} {
  global subject
  set subject \"STATISTICS\"
  setGreetingBitmap
  postToMinilog "           Check in the data type. \n \
                   Your choices are one-variable data set,two- \n \
                   variable data set, multi-variable data set, \n \
                   cross-classified data set,time-series data  \n \
                   set, quality control data set and probability \n \
                   distribution."
}


proc menuCalculusClicked {} {
  global subject
  set subject \"CALCULUS\"
  setGreetingBitmap
}


# Put in menu bar
proc makeGreetingMenu {} {
# Put in menu bar
  global magnus_home
  frame .mbar -relief raised -bd 2

  menubutton .mbar.file -text File -menu .mbar.file.menu -padx 1m
  makeEditMenu .mbar.edit
  menubutton .mbar.prefs -text Preferences -menu .mbar.prefs.menu -padx 1m
  menubutton .mbar.credits -text Credits -menu .mbar.credits.menu -padx 1m
  menubutton .mbar.help -text Help -menu .mbar.help.menu -padx 1m

  pack .mbar.file .mbar.edit .mbar.prefs -side left -padx 1m
  pack .mbar.help -side right -padx 1m
  pack .mbar.credits -side right -padx 1m

  menu .mbar.file.menu 
    .mbar.file.menu add command -label "Open..." -state disabled
    .mbar.file.menu add command -label "Save..." -state disabled
    .mbar.file.menu add command -label "Save As..." -state disabled
    .mbar.file.menu add separator
    .mbar.file.menu add command -label "Save Session Log..." -command "saveLogToFile .frame3.log"
    .mbar.file.menu add separator
    .mbar.file.menu add command -label Quit -command {
      eval toSessionManager "\"[get_messageTemplate quit]\""
      exit 0
    }

  menu .mbar.prefs.menu
    .mbar.prefs.menu add checkbutton -label {Novice mode} -variable noviceMode
    .mbar.prefs.menu add cascade -label {Font size} -menu .mbar.prefs.menu.fontsz
     menu .mbar.prefs.menu.fontsz
       addFontSizeMenu Tiny 100 80
       addFontSizeMenu Small 120 100
       addFontSizeMenu Normal 140 120
       addFontSizeMenu Big 180 140
       addFontSizeMenu Huge 180 180
       addFontSizeMenu Gigantic 240 240
    .mbar.prefs.menu add cascade -label {Colors} -menu .mbar.prefs.menu.colors
    menu .mbar.prefs.menu.colors
      .mbar.prefs.menu.colors add radiobutton -label Barbados \
        -value Barbados -variable theColorSet -command switchColors
      .mbar.prefs.menu.colors add radiobutton -label Siberia \
        -value Siberia -variable theColorSet -command switchColors
      .mbar.prefs.menu.colors add radiobutton -label "Custom..." \
        -value Custom -variable theColorSet -command colorPicker \
        -state disabled

#    .mbar.prefs.menu add cascade -label {Subject} \
#       -menu .mbar.prefs.menu.subject
#     menu .mbar.prefs.menu.subject
#       .mbar.prefs.menu.subject add radiobutton -label "Group Theory" \
#	       -command menuGroupClicked \
#	       -variable subjectLabel
    
#       .mbar.prefs.menu.subject add radiobutton -label "Statistics" \
#	       -command menuStatsClicked \
#               -variable subjectLabel
#       .mbar.prefs.menu.subject add radiobutton -label "Calculus" \
#	       -command menuCalculusClicked \
#               -variable subjectLabel

  menu .mbar.credits.menu
    .mbar.credits.menu add command -label "Contributors" \
      -command "showHelp {Contributors::Contributors}"

    .mbar.credits.menu add command -label "Acknowledgments" \
      -command "showHelp {Acknowledgments::Acknowledgments}"

  menu .mbar.help.menu
    .mbar.help.menu add command -label "The Manual" -command "showHelp"
    .mbar.help.menu add separator
    .mbar.help.menu add command -label "About CAISS-STAT" \
      -command "showHelp {aboutMagnus::About CAISS-STAT}"
    .mbar.help.menu add command -label "Release Notes" \
      -command "showHelp {releaseNotes::Release Notes}"
    .mbar.help.menu add separator
    .mbar.help.menu add command -label "Show copyright" \
      -command "makeHelp $magnus_home/doc/COPYRIGHT Copyright {Copyright}"
    .mbar.help.menu add command -label "Show GNU General Public License" \
      -command "makeHelp $magnus_home/doc/COPYING GPL \
       {GNU General Public License}"
    .mbar.help.menu add separator
    .mbar.help.menu add command -label "Contacting the Authors" \
      -command "showHelp {authors::Contacting the Authors}"

  pack .mbar -side top -fill x

}

proc makeGreetingHelp {f} {
  label $f.desp1  -text "  The fundamental object in statistics is a data set. To" -relief flat
  label $f.desp2  -text "begin to use CAISS-STAT we must \"check in\" a data set." -relief flat
  button $f.btnDespCAISS -foreground blue -text "DESCRIPTION OF CAISS-STAT" -relief flat \
             -command "showHelp {aboutMagnus::About CAISS-STAT}"
  button $f.btnDespDataSet -foreground blue -text "DESCRIPTION OF DATA SETS AND DATA SET TYPES" -relief flat \
             -command "showHelp {DescriptionDataSet::Description Of Data Set}"
  button $f.btnProceed -foreground blue -text "PROCEED TO CHECK-IN" -relief flat \
             -command "raise .workspace"
  pack $f.desp1 -side top
  pack $f.desp2 -side top
  pack $f.btnDespCAISS -side top
  pack $f.btnDespDataSet -side top
  pack $f.btnProceed -side top
}

proc makeGreeting {} {
  global tcl_path magnus_home subject

  # Put the greeting stuff in the root widget.

  wm title . "CAISS-STAT"
  wm iconname . "Magnus"
  #db wm minsize . 450 400
  #@rn Test screen size
  wm geometry . +10+20


  makeGreetingMenu

  # Put in greeting
  frame .frame -relief raised -bd 2
  makeGreetingCopyright "greeting.xbm"
  pack .frame -side top -fill x
  # Make the help session
  frame .helpFrame -relief raised -bd 2
  makeGreetingHelp .helpFrame
  pack .helpFrame -side top -fill x
  # Make the session log

  frame .frame3 -relief raised -bd 1
  frame .frame3.frame -relief raised -bd 1
  label .frame3.frame.label -text "Session Log" -relief flat
  text .frame3.log -height 20 -width 50 -relief flat \
       -wrap word -yscrollcommand ".frame3.s set" -highlightthickness 0
  scrollbar .frame3.s -relief sunken -command ".frame3.log yview"
  pack .frame3.frame.label -side left -fill x
  pack .frame3.frame -side top -fill x
  pack .frame3.s -side right -fill y
  pack .frame3.log -fill both -expand 1 -padx 1m -pady 1m
  pack .frame3 -side top -fill both -expand 1


  frame .frame4 -relief raised -bd 1

  button .frame4.goTop -text "Go Top" -command ".frame3.log yview 0"
  button .frame4.goBottom -text "Go Bottom" -command ".frame3.log yview end"
  button .frame4.search -text "Search" -command editSearch

  button .frame4.view -text [logGetViewButtonText .frame3.log] -width 9 \
    -command "logChangeViewButton .frame3.log .frame4.view"

  pack .frame4.goTop .frame4.goBottom .frame4.search .frame4.view \
       -pady 2m -ipadx 5m -ipady 2 -side left -expand 1 
  pack .frame4 -side top -fill x


  wm protocol . WM_DELETE_WINDOW {
    alert "Please do not try to delete the session log!"
  }

}


proc addFontSizeMenu {label cs hs} {
  if {[fontExists -Adobe-Helvetica-Bold-R-Normal-*-$hs-*] == 1 && \
      [fontExists *-Courier-medium-r-normal--*-$cs-*-*-*-*-*-*] == 1} {
    .mbar.prefs.menu.fontsz add radiobutton -label $label \
      -value $label -variable fontSize -command "changeFontSize $cs $hs" \
      -font -Adobe-Helvetica-Bold-R-Normal-*-$hs-*
  }
}
