require_relative("asm.rb") #adding file with module
init_cmd("executables/test")

#-------- Code ----------
# l_beg!
# add x1, x2, x3
# j l_beg

j l_end
add x1, x2, x3

l_end!
add x1, x2, x3
syscall
#------------------------

close_cmd
