require_relative("asm.rb") #adding file with module
init_cmd("common/cmd_bin.txt")

#-------- Code ----------

slti x2, x0, 1 #init first value of fibonachi

l_cycle!
add x3, x1, x2
st x2, 0.(x0)
st x3, 4.(x0)
ldp x1, x2, 0.(x0)

j l_cycle

#------------------------

close_cmd
