class Cmd
    attr_accessor :cmd, :arg1, :arg2, :arg3, :arg4    
    
    PREFIX = "parse_"
    ALLOWED_OPS = ["add", "sub", "or", "bext"].freeze

    def init(cmd = nil, arg1 = 0, arg2 = 0, arg3 = 0, arg4 = 0)
        @cmd = cmd
        @arg1 = arg1
        @arg2 = arg2
        @arg3 = arg3
        @arg4 = arg4
        @instr = 0
    end
    
    def parse()
        if ALLOWED_OPS.include?(@cmd)

            puts("#{cmd} #{arg1} #{arg2} #{arg3} #{arg4}")
            
            method_name = "#{PREFIX}#{@cmd}"
            send(method_name) if respond_to?(method_name, true)
        else
            puts "Invalid instruction"
        end
    end

private
    ARG_1_SHIFT = 21
    ARG_2_SHIFT = 16
    ARG_3_SHIFT = 11

    def parse_add()
        @instr |= (@arg2 << ARG_1_SHIFT) 
        @instr |= (@arg3 << ARG_2_SHIFT) 
        @instr |= (@arg1 << ARG_3_SHIFT) 
        @instr |= (0b00011000)

        @instr &= 0xFFFFFFFF #leaving only 32 bits
        
        return @instr
    end

    def parse_sub()
        @instr |= (@arg2 << ARG_1_SHIFT) 
        @instr |= (@arg3 << ARG_2_SHIFT) 
        @instr |= (@arg1 << ARG_3_SHIFT) 
        @instr |= (0b00111001)

        @instr &= 0xFFFFFFFF #leaving only 32 bits
        
        return @instr
    end

    def parse_or()
        @instr |= (@arg2 << ARG_1_SHIFT) 
        @instr |= (@arg3 << ARG_2_SHIFT) 
        @instr |= (@arg1 << ARG_3_SHIFT) 
        @instr |= (0b00010000)

        @instr &= 0xFFFFFFFF #leaving only 32 bits
        
        return @instr
    end

    def parse_bext()
        @instr |= (@arg1 << ARG_1_SHIFT) 
        @instr |= (@arg2 << ARG_2_SHIFT) 
        @instr |= (@arg3 << ARG_3_SHIFT) 
        @instr |= (0b00001111)

        @instr &= 0xFFFFFFFF #leaving only 32 bits
        
        return @instr
    end

    def parse_syscall()

    end

    def parse_clz()

    end 

    def parse_slti()

    end 

    def parse_st()

    end 

    def parse_ssat()

    end 
end 
    
#------------------------------------------------

puts("Type filename to compile:\n")

filename = String.new
filename = "../" + gets.chomp

File.open("../common/cmd_bin.txt", "wb") do |code|
    File.open(filename, "r") do |file|
        file.each_line do |line|
            line.chomp

            arg_arr = line.split(' ')
            arg_arr.each_with_index do |arg, i|
                if i == 0
                    arg_arr[i] = arg.downcase    
                else 
                    arg_arr[i] = arg.to_i
                end                    
            end

            cmd = Cmd.new
            cmd.init(arg_arr[0], arg_arr[1], arg_arr[2], arg_arr[3], arg_arr[4])
            instr = cmd.parse()

            code.write([instr].pack("L<"))
        end
    end    
end
