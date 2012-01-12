proc braces_not_matched {message} {
     set num_open 0
     set num_close 0
    set len [string length $message]
    for {set i 0} {$i<$len} {incr i} {
       set ch [string index $message $i]
	if {$ch=="\{"} {
          incr num_open
        }
        if {$ch=="\}"} {
          incr num_close
        }
    }
    puts stderr "$message with $num_open open $num_close close"
  
    if {$num_open!=$num_close} {
      return 1
    } else {
      return 0
    }
}

proc fromBackEnd {} {
#  puts stderr "enter from_session_manager"
  global back_end
  global result_type
  set fileID $back_end
  if { [gets $fileID message] < 0 } {
  #    fileevent $sessionManager readable ""
#      puts stderr "session manager stop"
  } else {
      if {[string length $message] == 0} {
         return 
      }
                                                           
      puts stderr "read from back_end: $message"
      regsub -all "gap\> " $message "" message
      regsub -all "]" $message "\}" message
      set message [substitute_left_bracket $message "\{"]
      if {$result_type=="NEW_GROUP_PERMUTATION"} {
          global new_group_name
          global new_objectID
          puts stderr "id:$new_objectID"
          set create_command "create_ objectID $new_objectID typeID GROUP_PERMUTATION iconID DataSet name $new_group_name"
          set create_command "$create_command  dependencies \{\} properties \{\}"
          set create_command "$create_command  viewStructure \{viewBuilder objectView viewParameters"
          set create_command "$create_command  \{viewID $new_objectID heritage \{User defined one permutation group\}\}\}"
          eval $create_command
          set result_type NULL
          incr new_objectID
      }

      if {$result_type=="SN_CONTINUE"} {
          global new_objectID
          regsub -all "gap\> " $message "" message
          regsub -all "]" $message ")" message
          set message [substitute_left_bracket $message "("]
          set description "The symmetric group is:"
          eval "post_to_log_ [expr $new_objectID-1] {$description: $message} 1"
          set result_type "NULL"      
      }

      if {$result_type=="AN_CONTINUE"} {
          global new_objectID
          puts stderr "I am here $new_objectID $message"
          set description "The alternating group is:"
          eval "post_to_log_ [expr $new_objectID-1] {$description: $message} 1"
          set result_type "NULL"      
      }

      if {$result_type=="SUBGROUP_AN_CONTINUE"} {
          global new_objectID group_elements next_index
          if {[string  first "-1" $message]!=-1} {
             eval "post_to_log_ [expr $new_objectID-1] {it is not a subgroup of An for some n} 1"
             set result_type NULL
          } elseif {$next_index>=[llength $group_elements]} {
             eval "post_to_log_ [expr $new_objectID-1] {it is a subgroup of An for some n} 1"
             set result_type NULL
          } else {
              set element [lindex $group_elements $next_index]
              incr next_index
#              alert "element: $element"
              set message "SignPerm($element);"
              eval [concat toBackEnd {$message}]
          }
    }
    if {[regexp "^IS" $result_type]!=0} {
          global new_objectID 
          global workspace_selection problem_description
          puts stderr "set: $workspace_selection"
          set objectID1  [get_objectID [lindex $workspace_selection 0]]
          set deps "$objectID1"
          set create_command "create_ objectID $new_objectID typeID $result_type iconID group_problem name \{\}"
          set create_command "$create_command  dependencies \{$deps\} properties \{\}"
          set create_command "$create_command  viewStructure \{viewBuilder resultView viewParameters"
          set create_command "$create_command  \{viewID $new_objectID heritage \{$problem_description\}\}\} definition \{\}"
          eval $create_command

          regsub -all "gap\> " $message "" message
#          alert "message: $message"
	  if {$result_type=="IS_SUBGROUP_SN"} {
              eval "post_to_log_ {$new_objectID} {$problem_description: } 1"   
	  } else {
              eval "post_to_log_ {$new_objectID} {$problem_description: $message} 1"         
          }
          if {$result_type=="IS_SN" && [string  first "true" $message]!=-1 } {
             global new_objectID 
             global workspace_selection problem_description
             set name1 [get_objectName [lindex $workspace_selection 0]]
             set message "$name1;"
             eval [concat toBackEnd {$message}]
             set result_type "SN_CONTINUE"      
          } elseif {$result_type=="IS_AN" && [string  first "true" $message]!=-1 } {
             global new_objectID 
             global workspace_selection problem_description
             set name1 [get_objectName [lindex $workspace_selection 0]]
             set message "$name1;"
             eval [concat toBackEnd {$message}]
             set result_type "AN_CONTINUE"       
          } elseif {$result_type=="IS_SUBGROUP_SN"} {
             global group_elements next_index
             set group_elements $message
             regsub -all "\{" $group_elements "" group_elements
             regsub -all "\}" $group_elements "" group_elements
	     # sunstitute ), with ;
	     set i 1
	     while {$i<[string length $group_elements]} {
                 set two_letters [string range $group_elements [expr $i-1] $i]
                 if {$two_letters=="),"} {
                     set first_half [string range $group_elements 0 [expr $i-1]]
                     set second_half [string range $group_elements [expr $i+1] end]
		     set group_elements "$first_half;$second_half"
                 } else {
                    incr i 
                 }
	     }
            
             set group_elements [split $group_elements ";"]
             set next_index 0
             set element [lindex $group_elements $next_index]
             incr next_index
             set message "SignPerm($element);"
             eval [concat toBackEnd {$message}]
             set result_type "SUBGROUP_AN_CONTINUE"    
          } else {
             set result_type NULL               
          }
          incr new_objectID    
      }
      if {$result_type=="ORDER_GROUP"||$result_type=="CENTER_GROUP"|| \
            $result_type=="NORMSUB_ENUM_GROUP"||
            $result_type=="MAXNORM_ENUM_GROUP"||
            $result_type=="NORMSUB_ENUM_GROUP_CONTI"
       } {
          global new_objectID
          global workspace_selection problem_description
          if {$result_type=="NORMSUB_ENUM_GROUP_CONTI"} {
            global pre_message
            set message "$pre_message$message"
          }
          if {[braces_not_matched $message]==0} {
            set objectID1  [get_objectID [lindex $workspace_selection 0]]
            set deps "$objectID1"
            set create_command "create_ objectID $new_objectID typeID $result_type iconID matrix_problem name \{\}"
            set create_command "$create_command  dependencies \{$deps\} properties \{\}"
            set create_command "$create_command  viewStructure \{viewBuilder resultView viewParameters"
            set create_command "$create_command  \{viewID $new_objectID heritage \{$problem_description:\}\}\} definition \{\}"
            eval $create_command
            regsub -all "gap\> " $message "" message
            set result_type NULL
            eval "post_to_log_ {$new_objectID} {$problem_description: $message} 1"          
            incr new_objectID
          } else {
            global pre_message
            set result_type NORMSUB_ENUM_GROUP_CONTI
            set pre_message $message
          }
         
      }
      if {$result_type=="SYLOWSUB_COMP_CONT"} {
          global new_objectID 
          eval "post_to_log_ [expr $new_objectID-1] {$message} 1"
          set result_type NULL
      }
      if {$result_type=="SYLOWSUB_COMP_GROUP"} {
          global new_objectID
          global workspace_selection problem_description
          set objectID1  [get_objectID [lindex $workspace_selection 0]]
          set deps "$objectID1"
          set create_command "create_ objectID $new_objectID typeID $result_type iconID matrix_problem name \{\}"
          set create_command "$create_command  dependencies \{$deps\} properties \{\}"
          set create_command "$create_command  viewStructure \{viewBuilder resultView viewParameters"
          set create_command "$create_command  \{viewID $new_objectID heritage \{$problem_description:\}\}\} definition \{\}"
          eval $create_command
          set result_type SYLOWSUB_COMP_CONT 
          eval "post_to_log_ {$new_objectID} {$problem_description: } 1"
          set name1 [get_objectName [lindex $workspace_selection 0]]
          set message "sylowP1:=List(primes,p->SylowSubgroup($name1,p));"
          eval [concat toBackEnd {$message}]          
          incr new_objectID
      }
  }
}

