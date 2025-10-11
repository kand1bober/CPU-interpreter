require_relative("asm.rb") #adding file with module
init_cmd("common/cmd_bin.txt")

#-------- Code ----------
l_am!
add x1 , x2, x3

j l_am
j l_bm

l_bm!   
st x1, -8.(x2)

#------------------------

close_cmd
