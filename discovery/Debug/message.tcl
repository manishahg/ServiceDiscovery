package require Tk

wm title . "Message!!"
grid [ttk::frame .c -padding "3 3 12 12"] -column 0 -row 0 -sticky nwes
grid columnconfigure . 0 -weight 1; grid rowconfigure . 0 -weight 1

foreach arg $::argv {}
grid [ttk::label .c.meters -text $arg] -column 2 -row 2 -sticky we
grid [ttk::button .c.calc -text "OK" -command "exit"] -column 3 -row 3 -sticky w


foreach w [winfo children .c] {grid configure $w -padx 5 -pady 5}



