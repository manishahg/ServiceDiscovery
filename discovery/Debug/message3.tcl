package require Tk

wm title . "Message!!"
grid [ttk::frame .c -padding "5 5 20 20"] -column 0 -row 0 -sticky nwes
grid columnconfigure . 0 -weight 1; grid rowconfigure . 0 -weight 1

foreach arg $::argv {}
grid [ttk::label .c.msg -text $arg] -column 2 -row 2 -sticky we
grid [ttk::button .c.ok -text "OK" -command invoc] -column 1 -row 3 -sticky w
grid [ttk::button .c.cancel -text "Cancel" -command "exit"] -column 3 -row 3 -sticky e
grid [ttk::label .c.ok_msg -text "Press 'OK' to invoke the service!!"] -column 2 -row 4 -sticky we
grid [ttk::label .c.cancel_msg -text "Press 'Cancel' to discontinue!!"] -column 2 -row 5 -sticky we


foreach w [winfo children .c] {grid configure $w -padx 7 -pady 7}

proc invoc {} {  
	set filename "invoc.txt"
	set fp [open $filename w+]
       
	puts $fp "1"
	exit
}



