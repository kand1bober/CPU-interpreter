require_relative("asm.rb") #adding file with module
init_cmd("executables/fib")

#-------- Code ----------
#x4 plays a role of constant zero
#x5 plays a role of constant 1
#x6 plays a role of counter

syscall #input N
slti x2, x4, 1 #init first value of fibonachi
slti x5, x4, 1 #init const 1
slti x6, x4, 1 #init counter to 1

l_cycle!
add x3, x1, x2
st x2, 0.(x4)
st x3, 4.(x4)
ldp x1, x2, 0.(x4)
add x6, x6, x5 #incr counter

beq x6, x0, 1 #skip next jump, if (counter == N)
j l_cycle

slti x8, x4, 1 #set x0 to 1, to make write syscall
ld x0, 4.(x4)
syscall
#------------------------

close_cmd
