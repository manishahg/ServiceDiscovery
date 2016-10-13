package require Tk

wm title . "Message!!"
grid [ttk::frame .c -padding "5 5 12 12"] -column 0 -row 0 -sticky nwes
grid columnconfigure . 0 -weight 1; grid rowconfigure . 0 -weight 1

foreach arg $::argv {}
grid [ttk::label .c.msg -text $arg] -column 3 -row 3 -sticky we
grid [ttk::button .c.ok -text "OK" -command "exit"] -column 3 -row 5 -sticky we


foreach w [winfo children .c] {grid configure $w -padx 10 -pady 10}







