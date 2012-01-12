## Copyright (C) 1995 The New York Group Theory Cooperative
## See magnus/doc/COPYRIGHT for the full notice.

## Contents: Code for setting the contents of the workspace menus.
##
## Principal Author: Roger Needham
##
## Status: In progress.
##
## Revision History:
##


proc selectionSignature {} {

  # This sorts the selection so that it matches its signature in
  # sorted order, and returns the signature.

  global workspace_selection

  set new_selection [list]
  set signature [list]

  # Do insertion sort into both lists, comparing by typeID:

  set len 0
  foreach icon $workspace_selection {
    set typeID [get_objectType $icon]
    set i 0
    while {$i < $len} {
      if {$typeID > [lindex $signature $i]} {
        incr i
      } else {
        break
      }
    }
    set new_selection [linsert $new_selection $i $icon]
    set signature [linsert $signature $i $typeID]
    incr len
  }
  set workspace_selection $new_selection
  return [join [concat [list [llength $signature]] $signature] "_"]
}

proc add_new_num {} {
  #  toBackEnd "(setq a 2)"
    set f [open "menu.ini" r]
    set current_catalog ""
    while {[gets $f line] >=0} {
        if {[string first "#" $line 0]!=0 && [llength $line]==8} {
           set menu_command "menu_defn_append menu"
           set menu_item [lindex $line 0]
           set menu_command "$menu_command $menu_item"
           set menu_sign [lindex $line 4]
           set menu_command "$menu_command signature \{$menu_sign\}"       
           set menu_item_cons [lindex $line 6]
           set menu_item_caption [lindex $line 1]
           set menu_item_command [lindex $line 2]
           set menu_item_messages [lindex $line 5]
           set menu_catalog [lindex $line 7]
           if {$menu_item_command=="begin_catalog"} {
                  set cata_menu_command "$menu_command  itemGroups \{ \{  \{$menu_item_cons\}  "
                  set cata_menu_command "$cata_menu_command   \{\{c \{ $menu_item_caption \} \{"
                  set current_catalog [lindex $line 3]
           } else {
               if {$menu_item_command=="end_catalog"} {
		   set cata_menu_command "$cata_menu_command \}\}\}\}\}"
 		       puts stderr "$cata_menu_command "
                       eval $cata_menu_command           
               } else {
                   if {$menu_catalog == "NONE"} {
	               set menu_command "$menu_command  itemGroups \{ \{ \{$menu_item_cons\}  "
                       set menu_command "$menu_command   \{\{\{ $menu_item_caption \}"            
                       set menu_command "$menu_command \{$menu_item_command\} \{$menu_item_messages \}\}\}\}\}"
                       puts stderr "$menu_command"
                       eval $menu_command
                   } else {
                       set cata_menu_command "$cata_menu_command \{\{ $menu_item_caption \}"            
                       set cata_menu_command "$cata_menu_command \{$menu_item_command\} \{$menu_item_messages \}\}"
                  }
               }
           }
           
        }
    }
#    menu_defn_ menu checkin signature {} itemGroups { { 1 {{{create one interger} {create_new_num } {-2 101}} } } }
#    menu_defn_ menu tools signature {INT_NUM INT_NUM} itemGroups { { 1 {{{add two intergers} {add_two_nums } {-2 102}} } } }

}

proc updateMenus {} {

  global workspace_selection

  set signature [selectionSignature]

  set m .workspace.mbar.checkin.menu
  foreach c [winfo children $m] { destroy $c }
  set filled [fillMenu $m checkin $signature]
  set num [llength $workspace_selection]
  .workspace.mbar.checkin configure -state normal
  if {$num == 1} {
#    if {$filled == 1} {
#      $m add separator
#    }
#    $m add command -label "Edit the Data Set" \
#      -command objectViewSelection
  }

#  .workspace.mbar.checkin.menu add command -label new_num -command {
#    add_new_num
#  }
  .workspace.mbar.checkin.menu add command -label Quit -command {
    quit_caiss_stat
  }

  set m .workspace.mbar.tools.menu
  foreach c [winfo children $m] { destroy $c }
  set filled [fillMenu $m tools $signature]
  .workspace.mbar.tools configure -state normal
  if {$num > 0} {
#    if {$filled == 1} {
#      $m add separator
#    }

#    $m add command -label "Edit the Data Set" \
#      -command objectViewSelection

#    if {$filled == 1} {
#      $m add separator
#    }

    $m add command -label "Delete selected objects and dependencies" \
      -command workspace_deleteSelection
  } else {
    $m add command -label "Please select an object to analysis" 
  }
  if { $num > 0 || $filled == 1 } {
    $m add separator
  }

#add window submenu
  set m .workspace.mbar.window.menu
  set filled [fillMenu $m window $signature]
  .workspace.mbar.window configure -state normal
  $m add command -label "Sign In Window" \
      -command "showSignInWindow"
  $m add command -label "Log Window" \
      -command "showLogWindow"
#  update help menus

  set m .workspace.mbar.help.menu
  foreach c [winfo children $m] { destroy $c }
  set filled [fillMenu $m help $signature]
  .workspace.mbar.help configure -state normal

  $m add separator
  $m add command -label "About the Workspace" \
      -command "showHelp {aboutWorkspace::About the Workspace}"

}


proc fillMenu {m menu signature} {

  # Returns 1 if any items were put in, 0 otherwise.

  global workspace_selection A subject

  set theArray ${menu}MenuSpecs
  global $theArray

  set result 0
  $m delete 0 last
  if {![info exists ${theArray}($signature)]} {
    return $result
  }

  set icon_selection $workspace_selection
  permute icon_selection [lindex [set ${theArray}($signature)] 0]

  # Set up context for evaluating ${namei} and ${OIDi}.
  # Maybe a little dicey, since I have other variables visible here...

  set i 1
  set selection [list]
  foreach icon $icon_selection {
    set name$i [get_objectName $icon]
    set oid [get_objectID $icon]
    set x$i $oid
    lappend selection $oid
    incr i
  }
  #@rn What should these be now? Anything? (maybe done by database refs)

  # Fill in the menu

  set childIndex 0

  for {set i 1} {$i<[llength [set ${theArray}($signature)]]} {incr i} {

  set itemGroups [lindex [set ${theArray}($signature)] $i]
  puts stderr "all items:$itemGroups"
  foreach itemGroup $itemGroups {
    puts stderr "single item:$itemGroup"

    set cond [lindex $itemGroup 0]
    set val 1
#    if { [catch {eval expr $cond} val] == 0 && $val } 
     if { $cond!="1" } {
       global condition_result
       eval $cond
       set val $condition_result
      }
     if { $val==1 }  {

      set result 1
      foreach item [lindex $itemGroup 1] {

        # The evals below are to force tcl to substitute the
        # values of ${namei} and $xi into the item.

        set temp [lindex $item 0]
	
        if {$temp == "s"} {
          $m add separator
        } elseif {$temp == "c"} {
          set cm $m.child$childIndex
          incr childIndex
          eval $m add cascade -menu $cm -label "\"[lindex $item 1]\""
          menu $cm
          foreach realItem [lindex $item 2] {
            set temp [lindex $realItem 0]
            set action [lindex $realItem 1]
            if {$action == ""} {
              set action toSessionManager
            }
            set message [lindex $realItem 2]
            if {$message == ""} {
	        if {$temp == "s"} {
		    eval $cm add separator
		} else {
		    eval $cm add command -label "\"$temp\"" -state disabled
		}
            } else {
              set message [concat $message $selection]
              eval $cm add command -label "\"$temp\"" \
                -command "\"$action {$message}\""
            }
          }
        } else {
          set action [lindex $item 1]
          if {$action == ""} {
            set action toSessionManager
          }
          set message [lindex $item 2]
          if {$message == ""} {
            eval $m add command -label "\"$temp\"" -state disabled
          } else {
            set message [concat $message $selection]
            eval $m add command -label "\"$temp\"" \
              -command "\"$action {$message}\""
          }
        }
      }
    }
  }
  }
  return $result
}


proc checkinSynopsis {} {

  global checkinMenuSpecs

#@rn temp decl:
  global typeNames typePrefixes

  set w .checkinSynopsis
  if { [winfo exists $w] } {
    wm deiconify $w
    raise $w
    return
  }

  set synopsis \
"  This is a synopsis of the group theoretic objects which you can check\
in to the workspace.
  Since the Check in menu changes depending on the context (e.g., what\
you have selected in the workspace), some of the items listed here may\
not always be available. Also, some hypothetical object names cannot (yet)\
be sensibly resolved, so they are marked with `<object name>'."

  foreach signature [lsort [array names checkinMenuSpecs]] {

    if {$signature == "0"} {

      append synopsis \
        "\n\n---------------------------\nIf there are no selected objects,"

    } else {

      append synopsis \
        "\n\n---------------------------\nIf the selected objects are:\n\n"
      set index 1
      foreach type [lreplace [split $signature _] 0 0] {
        set name $typePrefixes($type)$index
        append synopsis "  $name, a $typeNames($type)\n"
        set name$index $name
        set OID$index -1
        incr index
      }
    }
    append synopsis "\nthen the objects which can be checked in are:\n\n"

    foreach itemGroup $checkinMenuSpecs($signature) {
  
      foreach item [lindex $itemGroup 1] {

        # The evals below are to force tcl to substitute the
        # values of ${namei} and ${OIDi} into the item.
  
        set temp [lindex $item 0]
        if {$temp == "s"} {
          append synopsis "  ----------\n"
        } elseif {$temp == "c"} {
          append synopsis "  [lindex $item 1]\n"
          foreach realItem [lindex $item 2] {
            if {[lindex $realItem 2] != ""} {
              eval append synopsis "\"    [lindex $realItem 0]\n\""
            }
          }
        } else {
          if {[lindex $item 2] != ""} {
            eval append synopsis "\"  $temp\n\""
          }
        }
      }
    }
  }

  append synopsis \
"\n\n\n---------------------------\nThe details, if you care (or, why\
is this so lame?):\nThe Check in and Tools menus of Magnus are built\
dynamically as the selected objects change. The front end builds the\
menus from an abstract description of what is available, which is\
supplied by the session manager.  But the description depends on the\
current context; for example, one cannot compute the intersection of\
two subgroups unless they have a common supergroup.  The details of\
context evaluation are (deliberately) hidden from the front end, so it\
is difficult to give the synopsis generating code satisfactory\
intelligence without violating the separation of front end and session\
manager. We're working on it, and as always, we welcome suggestions."

  makeTextWindow $w "Check in menu synopsis" $synopsis
}


proc toolsSynopsis {} {

  global toolsMenuSpecs typeNames

#@rn temp decl:
  global typeNames typePrefixes

  set w .toolsSynopsis
  if { [winfo exists $w] } {
    wm deiconify $w
    raise $w
    return
  }

  set synopsis \
"  This is a synopsis of the computations which Magnus supports.
  Since the Tools menu changes depending on the context (e.g., what\
you have selected in the workspace), some of the items listed here may\
not always be available. Also, some hypothetical object names cannot (yet)\
be sensibly resolved, so they are marked with `<object name>'."

  foreach signature [lsort [array names toolsMenuSpecs]] {

    append synopsis \
      "\n\n---------------------------\nIf the selected objects are:\n\n"
    set index 1
    foreach type [lreplace [split $signature _] 0 0] {
      set name $typePrefixes($type)$index
      append synopsis "  $name, a $typeNames($type)\n"
      set name$index $name
      set OID$index -1
      incr index
    }
    append synopsis "\nthen the tool options are:\n\n"

    foreach itemGroup $toolsMenuSpecs($signature) {
  
      foreach item [lindex $itemGroup 1] {

        # The evals below are to force tcl to substitute the
        # values of ${namei} and ${OIDi} into the item.
  
        set temp [lindex $item 0]
        if {$temp == "s"} {
          append synopsis "  ----------\n"
        } elseif {$temp == "c"} {
          append synopsis "  [lindex $item 1]\n"
          foreach realItem [lindex $item 2] {
            if {[lindex $realItem 2] != ""} {
              eval append synopsis "\"    [lindex $realItem 0]\n\""
            }
          }
        } else {
          if {[lindex $item 2] != ""} {
            eval append synopsis "\"  $temp\n\""
          }
        }
      }
    }
  }

  append synopsis \
"\n\n\n---------------------------\nThe details, if you care (or, why\
is this so lame?):\nThe Check in and Tools menus of Magnus are built\
dynamically as the selected objects change. The front end builds the\
menus from an abstract description of what is available, which is\
supplied by the session manager.  But the description depends on the\
current context; for example, one cannot compute the intersection of\
two subgroups unless they have a common supergroup.  The details of\
context evaluation are (deliberately) hidden from the front end, so it\
is difficult to give the synopsis generating code satisfactory\
intelligence without violating the separation of front end and session\
manager. We're working on it, and as always, we welcome suggestions."

  makeTextWindow $w "Tools menu synopsis" $synopsis
}
