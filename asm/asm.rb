class Cmd
    attr_accessor :cmd, :arg1, :arg2, :arg3, :arg4    
    
    PREFIX = "parse_"
    ALLOWED_OPS = ["add", "sub"].freeze

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
            method_name = "#{PREFIX}#{@cmd}"
            send(method_name) if respond_to?(method_name)
        else
            puts "Invalid instruction"
        end
    end

private

    ARG_1_SHIFT = 21
    ARG_2_SHIFT = 16
    ARG_3_SHIFT = 11

    def parse_add()
        @instr |= (@arg1 << ARG_1_SHIFT) 
        @instr |= (@arg2 << ARG_2_SHIFT) 
        @instr |= (@arg3 << ARG_3_SHIFT) 
        @instr |= (0b00011000)

        @instr &= 0xFFFFFFFF #leaving only 32 bits
        
        return @instr 
    end

    def parse_sub()
        
    end

    def parse_or()
        
    end

    def parse_bext()
        
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

code = File.open("../common/cmd_bin.txt", "wb")

File.open(filename, "r") do |file|
    file.each_line do |line|
        cmd = Cmd.new
        cmd.init(line.downcase.chomp)
        cmd.parse()
        cmd = nil
    end
end    

code.close 
#------------------------------------------------

 



# file.close


# File.open(filename, 'r') do |file|
#     file.each_line do |line|
#       # Здесь вы можете работать с каждой строкой
#       # Например, убрать символ новой строки в конце
#       line.chomp!
  
#       # Ваша логика обработки строки
#       puts "Обрабатываю строку: #{line}"
#       # ... ваш код ...
#     end
#   end