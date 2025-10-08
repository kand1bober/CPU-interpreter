class Cmd
    attr_accessor :cmd, :arg1, :arg2, :arg3, :arg4    
    
    PREFIX = "parse_"
    ALLOWED_OPS = %w[add sub or bext slti st ssat ldp beq ld syscall j usat clz].freeze

    def init(cmd = nil, arg1 = 0, arg2 = 0, arg3 = 0, arg4 = 0)
        @cmd = cmd
        @arg1 = arg1
        @arg2 = arg2
        @arg3 = arg3
        @arg4 = arg4
        @instr = 0

        @jump_table = []
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
    ARG_SHIFTS = [nil, 21, 16, 11].freeze
    ARG_MASK = 2**5 - 1
    
    FUNC_SHIFT = 26
    FUNC_MASK = 2**6 - 1 

    INSTR_MASK = 0xFFFFFFFF
    LAST_5_MASK = 2**5 - 1
    LAST_10_MASK = 2**10 - 1 
    LAST_15_MASK = 2**15 - 1
    LAST_25_MASK = 2**25 - 1
    
    def set_arg(num, arg)
        @instr |= (arg & ARG_MASK) << ARG_SHIFTS[num]
    end

    def set_last(arg, mask)
        @instr |= ((arg & ARG_MASK) & mask)
    end

    def set_code(code)
        @instr |= ((code & FUNC_MASK) << FUNC_SHIFT)
        @instr &= INSTR_MASK #leaving only 32 bits
    end

    def parse_add()
        set_arg(1, @arg2)
        set_arg(2, @arg3)
        set_arg(3, @arg1)
        set_last(0b00011000, LAST_5_MASK)
        set_code(0)
    end

    def parse_sub()
        set_arg(1, @arg2)
        set_arg(2, @arg3)
        set_arg(3, @arg1)
        set_last(0b00111001, LAST_5_MASK)
        set_code(0)
    end

    def parse_or()
        set_arg(1, @arg2)
        set_arg(2, @arg3)
        set_arg(3, @arg1)
        set_last(0b00010000, LAST_5_MASK)
        set_code(0)
    end

    def parse_bext()
        set_arg(1, @arg1)
        set_arg(2, @arg2)
        set_arg(3, @arg3)
        set_last(0b00001111, LAST_5_MASK)
        set_code(0)
    end

    def parse_slti()
        set_arg(1, @arg1)
        set_arg(2, @arg2)
        set_last(@arg3, LAST_15_MASK)
        set_code(0b00111101)       
    end 

    def parse_st()
        set_arg(1, @arg3)
        set_arg(2, @arg1)
        set_last(@arg2, LAST_15_MASK)
        set_code(0b00111000)       

        #TODO: add allign check of offset
    end 
    
    def parse_ssat()
        set_arg(1, @arg1)
        set_arg(2, @arg2)
        set_arg(3, @arg3)
        set_code(0b00111111)       
    end 

    def parse_ldp()
        set_arg(1, @arg4)
        set_arg(2, @arg1)
        set_arg(3, @arg2)
        set_last(@arg3, LAST_10_MASK)
        set_code(0b00110101)       
    end

    def parse_beq()
        set_arg(1, @arg1)
        set_arg(2, @arg2)
        set_last(@arg3, LAST_15_MASK)
        set_code(0b00010110)           
    end

    def parse_ld()
        set_arg(1, @arg3)
        set_arg(2, @arg1)
        set_last(@arg2, LAST_15_MASK)
        set_code(0b00111110)           
    end

    def parse_syscall()
        set_last(0b010101, LAST_5_MASK)
        set_code(0)
    end

    def parse_j()
        #TODO: arg1 -- кол-во инструкций между текущей и меткой 
        index = @arg1 * 4
        index &= (2**31 - 1 - 3) #crop last 2 bits
        index &= (2**28 - 1) #crop first 4 bits
        set_last((index >> 2), LAST_25_MASK)
        set_code(0b00110000)                   
    end

    def parse_usat()
        set_arg(1, @arg1)
        set_arg(2, @arg2)
        set_arg(3, @arg3)
        set_code(0b00100010)           
    end

    def parse_clz()
        set_arg(1, @arg1)
        set_arg(2, @arg2)
        set_last(0b011100, LAST_5_MASK)
        set_code(0)
    end 
end 
    
#------------------------------------------------

puts("Type filename to compile:\n")

filename = String.new
filename = gets.chomp

File.open("common/cmd_bin.txt", "wb") do |code|
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
