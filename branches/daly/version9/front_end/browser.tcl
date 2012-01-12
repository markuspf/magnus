proc dviHelp_Close { } {
   alert "Don't close this window."
}
proc tkdviSupport { } {
#   global tkdvi auto_path
#   set prefix /usr/local
#   set exec_prefix ${prefix}

#   set tkdvi(tcllib) [expr {[info exists env(TKDVI_TCLLIB)] \
#	? $env(TKDVI_TCLLIB) : [file join ${prefix}/share tkdvi0.4]}]
#   set tkdvi(binlib) [expr {[info exists env(TKDVI_BINLIB)] \
#	? $env(TKDVI_BINLIB) : [file join ${exec_prefix}/lib tkdvi0.4]}]

#   lappend auto_path $tkdvi(tcllib) $tkdvi(binlib)
}

proc showDviHelp { w fileName} {
#   package require tkdvi
#   set f [dvi::code create -file $fileName]
#   tkdvi::browser::browser .dvi -file $f 
}