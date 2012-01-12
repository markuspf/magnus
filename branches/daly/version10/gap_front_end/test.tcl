
proc create_permutation_group_ok {w message} {
  global result_type g_type
  global new_group_name
  set name [$w.frame1.entry get]
  set new_group_name $name
  if {$name==""} {
     alert "please give a name for your group"
     return
  }
  if {$g_type==1} {
    set size [$w.frame2.fsize.size get]
    set def "symmetric group with size $size"
    set message "$new_group_name:=SymmetricGroup($size);"
  }
  if {$g_type==2} {
    set size [$w.frame2.fsize.size get]
    set def "alternating group with size $size"    
    set message "$new_group_name:=AlternatingGroup($size);"
  }
  if {$g_type==3} {
    set gen [$w.frame2.frame.text get 1.0 end-1c]
    set def "define group with $gen"    
    regsub gp $gen "" gen 
    set message "$new_group_name:=Group$gen;"
  }

  set result_type NEW_GROUP_PERMUTATION
  save_pendingCheckin $name create_permutation_group "$def" "check in new permutation group" $message
  eval [concat toBackEnd {$message}]
  destroy $w
}
proc disable_gen_def {f} {
  $f.frame.text configure -state disable -background grey
  $f.fsize.size configure -state normal -background white
}

proc enable_gen_def {f} {
  $f.frame.text configure -state normal -background white
  $f.fsize.size configure -state disable -background grey
}

proc create_permutation_group {message} {
  # $message is the SM message template
  # $dialog is the name of the calling proc
  # $title is the dialog window title
  # $name is the default name of the object
  # $help is the help string to display
  # $defn is the default/erroneous definition
  # $errorMsg is the syntax error message (empty if none)
  global tcl_path
  set title "Define a permutation group"
  set cathegory 1
  set name "P1"
  set defn ""
  set cursorPos "1.2"
  set dialog "create_permutation_group"
  global g_type
  set g_type 1 

  set w [uniqueDialogName]
  toplevel $w
  wm title $w $title
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200

  # Make menu bar

  frame $w.mbar -relief raised -bd 2

# @am change the look of File menu

  button $w.mbar.filemenu1  -relief flat -text "Read from file"\
      -command "getDefnFromFile_Button $w.mbar.filemenu1 $w.frame2.frame.text"
  button $w.mbar.filemenu2  -relief flat -text "Read from database"\
      -command "getDefnFromDatabase_Button $w.mbar.filemenu2 $cathegory $w.frame1.entry $w.frame2.frame.text"

  button $w.mbar.filemenu3  -relief flat -text "Magnus' choice" \
      -command "getRandomDefn_Button $cathegory \"$message\" $w.frame2.frame.text"

#  menubutton $w.mbar.file -text File -menu $w.mbar.file.menu
  makeEditMenu $w.mbar.edit
  menubutton $w.mbar.help -text Help -menu $w.mbar.help.menu

  pack  $w.mbar.filemenu1   $w.mbar.filemenu2  $w.mbar.filemenu3 $w.mbar.edit -side left
  #	pack  $w.mbar.filemenu1   $w.mbar.filemenu2   $w.mbar.edit -side left

#  pack $w.mbar.file $w.mbar.edit -side left
  pack $w.mbar.help -side right

#  menu $w.mbar.file.menu 
#    $w.mbar.file.menu add command -label "Read definition from file" \
#      -command "getDefnFromFile $w.frame2.frame.text"
#    $w.mbar.file.menu add command -label "Read object from database" \
#      -command "getDefnFromDatabase $cathegory $w.frame1.entry $w.frame2.frame.text"

  menu $w.mbar.help.menu
  set helps     {{{Permutation group format} pmgpFormatDialog} \
		     {{Cycle format} cycleFormatDialog} } 

  foreach pair $helps {
    $w.mbar.help.menu add command -label [lindex $pair 0] \
      -command "makeDefnHelp $w [lindex $pair 1] 1"
  }

  pack $w.mbar -side top -fill x

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

  # Make definition field

  frame $w.frame2 -relief raised -bd 2
  frame $w.frame2.frame -relief flat
  frame $w.frame2.ftype -relief flat
  frame $w.frame2.fsize -relief flat
  ##

  radiobutton $w.frame2.ftype.blt_symm -variable g_type -value 1 -text "Symmetric Group"  -bitmap @$tcl_path/bitmaps/Sn.xbm -command "disable_gen_def $w.frame2" 
        
  radiobutton $w.frame2.ftype.blt_alt -variable g_type -value 2 -text "Alternating Group" -bitmap @$tcl_path/bitmaps/An.xbm -command "disable_gen_def $w.frame2" 

  
  label $w.frame2.fsize.label -fg blue -text "n=" -width 4
 
  entry  $w.frame2.fsize.size  -width 5

  ##
  pack $w.frame2.ftype -side top -expand 1 -fill x
  pack $w.frame2.fsize -side top -expand 1 -fill x

  pack $w.frame2.ftype.blt_symm -side left
  pack $w.frame2.ftype.blt_alt -side left
#  pack $w.frame2.ftype.blt_gen -side right

  pack $w.frame2.fsize.label -side left
  pack $w.frame2.fsize.size -side left

  ###
#  label $w.frame2.label -text "Definition of Generators"
 
  radiobutton $w.frame2.blt_gen -variable g_type -value 3 -text "Define Group by Cycles" -command "enable_gen_def $w.frame2" 


  text $w.frame2.frame.text -relief sunken -bd 2 -height 5 -width 50 \
    -wrap word -yscrollcommand "$w.frame2.frame.s set" 

  scrollbar $w.frame2.frame.s -relief sunken \
    -command "$w.frame2.frame.text yview"
  pack $w.frame2.blt_gen -side top -anchor nw
  pack $w.frame2.frame.text -side left -fill both -expand 1
  pack $w.frame2.frame.s -side right -fill y
  pack $w.frame2.frame -side top -fill both -expand 1 -padx 2m -pady 1m
  pack $w.frame2 -fill both -expand 1
  bind $w.frame2.frame.text <Tab> "focus $w.frame1.entry"
  bind $w.frame2.frame.text <Meta-o> "focus $w.frame1.entry"

  if { $defn=="" } {
     set defn "gp((1,2,3) )"
  }

  $w.frame2.frame.text insert 1.0 $defn

  focus $w.frame2.frame.text
  $w.frame2.frame.text mark set insert $cursorPos

  # Make OK and Cancel buttons

  frame $w.frame3 -relief raised -bd 2
  frame $w.frame3.frame -relief flat
  button $w.frame3.frame.cancel -relief raised -bd 2 -text "Cancel" -width 8\
         -command "destroy $w"
  button $w.frame3.frame.ok -relief raised -bd 2 -text "Ok" -width 8 \
	  -command "create_permutation_group_ok $w {$message}"

  #@db 02/27 changed the order of Ok and Cancel buttons.

  pack $w.frame3.frame.ok $w.frame3.frame.cancel -side left\
    -expand 1 -padx 3m -pady 3m -ipady 1m
  pack $w.frame3.frame -fill x
  pack $w.frame3 -fill x

  # Make the help field?

  if { [inNoviceMode] } {
    makeDefnHelp $w [lindex [lindex $helps 0] 1]
  }
  disable_gen_def $w.frame2
}

proc create_permutation_group_old {message} {
  set w [uniqueDialogName]
  toplevel $w
  wm title $w "check in new permutationn group"
  wm iconname $w "Dialog"
  #@db wm minsize $w 200 200
  # Make menus
  global g_type
  set g_type 1                               
  frame $w.frame -relief raised -bd 2

  frame $w.frame.fname 
  frame $w.frame.ftype 
  frame $w.frame.fsize 
  frame $w.frame.fdefinition 
  frame $w.frame.fbtn

  label $w.frame.fname.label -fg blue  -text "The name of varable:"
  entry $w.frame.fname.name

  radiobutton $w.frame.ftype.blt_symm -variable g_type -value 1 -text "Symmetric Group" -command "disable_gen_def $w.frame"
        
  radiobutton $w.frame.ftype.blt_alt -variable g_type -value 2 -text "Alternating Group" -command "disable_gen_def $w.frame"
  radiobutton $w.frame.ftype.blt_gen -variable g_type -value 3 -text "Define Group by Generators" -command "enable_gen_def $w.frame"
  
  label $w.frame.fsize.label -fg blue -text "the size of the set"
 
  entry $w.frame.fsize.size  

  label $w.frame.fdefinition.label -fg blue -text "The definition of generators:"
  text $w.frame.fdefinition.def -height 6 -width 20

  button $w.frame.fbtn.ok -text "OK" -command "create_permutation_group_ok $w {$message}"
  button $w.frame.fbtn.cancel -text "CANCEL" -command "destroy $w "                                                                            
  pack $w.frame -fill x
  pack $w.frame.fname  
  pack $w.frame.ftype 
  pack $w.frame.fsize 
  pack $w.frame.fdefinition                                                                              
  pack $w.frame.fbtn

  pack $w.frame.fname.label -side left
  pack $w.frame.fname.name -side right

  pack $w.frame.ftype.blt_symm -side left
  pack $w.frame.ftype.blt_alt -side left
  pack $w.frame.ftype.blt_gen -side right

  pack $w.frame.fsize.label -side left
  pack $w.frame.fsize.size -side right

  pack $w.frame.fdefinition.label -side top
  pack $w.frame.fdefinition.def -side top                                                                           

  pack $w.frame.fbtn.ok -side left
  pack $w.frame.fbtn.cancel -side right
  disable_gen_def $w.frame
}

proc is_perfect {message} {
  global workspace_selection
  global result_type
  global problem_description
  set problem_description "Is Perfect test:"
  puts stderr  $workspace_selection
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "IsPerfectGroup($name1);"
  eval [concat toBackEnd {$message}]
  set result_type IS_PERFECT
}

proc is_simple {message} {
  global workspace_selection
  global result_type
  global problem_description
  set problem_description "Is Simple test:"
   puts stderr  $workspace_selection
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "IsSimple($name1);"
  eval [concat toBackEnd {$message}]
  set result_type IS_SIMPLE
}

proc is_p_group {message} {
  global workspace_selection
  global result_type
  global problem_description
  set problem_description "Is P Group test:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "IsPGroup($name1);"
  eval [concat toBackEnd {$message}]
  set result_type IS_P_GROUP
}

proc  is_abelian {message} {
  global workspace_selection
  global result_type
  global problem_description
  set problem_description "Is Ablean test:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "IsAbelian($name1);"
  eval [concat toBackEnd {$message}]
  set result_type IS_ABELIAN
}

proc is_cyclic {message} {
  global workspace_selection
  global result_type
  global problem_description
  set problem_description "Is Cyclic Test:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "IsCyclic($name1);"
  eval [concat toBackEnd {$message}]
  set result_type IS_CYCLIC
}

proc is_nilpotent {message} {
  global workspace_selection
  global result_type
  global problem_description
  set problem_description "Is Nilpotent test:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "IsNilpotent($name1);"
  eval [concat toBackEnd {$message}]
  set result_type IS_NILPOTENT
}

proc is_solvable {message} {
  global workspace_selection
  global result_type
  global problem_description
  set problem_description "Is An Solvable:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "IsSolvableGroup($name1);"
  eval [concat toBackEnd {$message}]
  set result_type IS_SOLVABLE
}

proc is_Sn {message} {
  global workspace_selection
  global result_type
  global problem_description
  set problem_description "Is Sn test:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "IsNaturalSymmetricGroup($name1);"
  eval [concat toBackEnd {$message}]
  set result_type IS_SN
}

proc is_An  {message} {
  global workspace_selection
  global result_type
  global problem_description
  set problem_description "Is An test:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "IsNaturalAlternatingGroup($name1);"
  eval [concat toBackEnd {$message}]
  set result_type IS_AN
}

proc is_subgroup_Sn {message} {
  global workspace_selection problem_description
  global result_type
  set problem_description "Subgroup if Sm test:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "List($name1);"
  eval [concat toBackEnd {$message}]
  set result_type IS_SUBGROUP_SN
}

proc order_permute_group {message} {
  global workspace_selection problem_description
  global result_type
  set problem_description "The order of the group is:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "Order($name1);"
  eval [concat toBackEnd {$message}]
  set result_type ORDER_GROUP
}

proc  center_permute_group {message} {
  global workspace_selection problem_description
  global result_type
  set problem_description "The center of the group is:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "Center($name1);"
  eval [concat toBackEnd {$message}]
  set result_type CENTER_GROUP
}

proc candecom_permute_group {message} {
   alert "has not been implemented yet"
}

proc sylowsub_permute_group {message} {
  global workspace_selection problem_description
  global result_type
  set problem_description "The p-Sylow subgroups are:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "primes:=Set(Factors(Order($name1)));"
  eval [concat toBackEnd {$message}]
  set result_type SYLOWSUB_COMP_GROUP
}
proc endomor_permute_group {message} {
   alert "has not been implemented yet"
}
proc automor_permute_group {message} {
   alert "has not been implemented yet"
			}
proc normsub_permute_group {message} {
   alert "has not been implemented yet"
			}
proc indexsub_permute_group {message} {
   alert "has not been implemented yet"
			 }
proc words_enum_group {message} {
   alert "has not been implemented yet"
		   }
proc normsub_enum_group {message} {
  global workspace_selection problem_description
  global result_type
  set problem_description "The normal subgroups of the group are:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "NormalSubgroups($name1);"
  eval [concat toBackEnd {$message}]
  set result_type NORMSUB_ENUM_GROUP
}
proc maxnorm_enum_group {message} {
  global workspace_selection problem_description
  global result_type
  set problem_description "The maximal normal subgroups of the group are:"
#  puts stderr [lindex $workspace_selection 1]
  set name1 [get_objectName [lindex $workspace_selection 0]]
  set message "MaximalNormalSubgroups($name1);"
  eval [concat toBackEnd {$message}]
  set result_type MAXNORM_ENUM_GROUP
}
proc relators_enum_group {message} {
   alert "has not been implemented yet"
		      }
proc endo_enum_group {message} {
   alert "has not been implemented yet"
		  }
proc auto_enum_group {message} {
   alert "has not been implemented yet"
		  }
