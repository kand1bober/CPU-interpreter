require_relative("asm.rb") #adding file with module
init_cmd("common/cmd_bin.txt")

#-------- Code ----------
add 1, 2, 3

#load to mem
st 8, 12, 1
st 9, 16, 1
    
#cond
beq 10, 1, 0

#read mem
ldp 4, 5, 12, 1
ld 12, 16, 1


#------------------------

close_cmd
