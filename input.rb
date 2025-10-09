require_relative("asm.rb") #adding file with module
init_cmd("common/cmd_bin.txt")

#-------- Code ----------
st 1, 4, 2
add 1, 2, 3
#------------------------

close_cmd
