module Cmd
    ARG_SHIFTS = [nil, 21, 16, 11].freeze
    ARG_MASK = 2**5 - 1
    FUNC_SHIFT = 26
    FUNC_MASK = 2**6 - 1
    INSTR_MASK = 0xFFFFFFFF
    LAST_5_MASK = 2**5 - 1
    LAST_11_MASK = 2**11 - 1
    LAST_16_MASK = 2**16 - 1
    LAST_26_MASK = 2**26 - 1
  
    class << self #open singltone class for this module
        attr_accessor :instr, :file
        
        def reset_instr
            @instr = 0
        end
    
        def set_arg(num, arg)
            @instr |= ((arg & ARG_MASK) << ARG_SHIFTS[num])
        end
      
        def set_last(arg, mask)
            @instr |= (arg & mask)
        end
      
        def set_code(code)
            @instr |= ((code & FUNC_MASK) << FUNC_SHIFT)
            @instr &= INSTR_MASK
        end
      
        def check_align(addr)
            abort("misaligned address(=#{addr})") if (addr & 3) != 0
        end
      
        def emit
            @file.write([@instr].pack("L<"))
            reset_instr
        end    
    end
  
end

#adding meеhod to class Integer
class Integer
    def call(base)
        [self, base]
    end
end

#redefinition of method_missing
#for it to return array of strings 
def method_missing(method, *args)
    if args.empty?
        if method.to_s[0] == 'x'
            num = method.to_s.delete_prefix('x').to_i
            if num > 31 || num < 0
                abort("wrong register num (= #{num})")
            else
                return num
            end
        else
            if method.to_s == 'to_int'
                # puts("method: #{method.to_s}")
            else 
                abort("wrong register name")
            end
        end 
    else 
        abort("wrong syntax of register")
    end
end

#------ Callable methods (top-level) ------
  
def init_cmd(filename)
    Cmd.file = File.open(filename, "wb")
    Cmd.reset_instr
end
  
def close_cmd
    Cmd.file&.close
end
  
def add(rd, rs, rt)
    Cmd.set_arg(1, rs)
    Cmd.set_arg(2, rt)
    Cmd.set_arg(3, rd)
    Cmd.set_last(0b00011000, Cmd::LAST_5_MASK)
    Cmd.set_code(0)
    Cmd.emit
end

def sub(rd, rs ,rt)
    Cmd.set_arg(1, rs)
    Cmd.set_arg(2, rt)
    Cmd.set_arg(3, rd)
    Cmd.set_last(0b00111001, Cmd::LAST_5_MASK)
    Cmd.set_code(0)
    Cmd.emit
end

def or(rd, rs, rt)
    Cmd.set_arg(1, rs)
    Cmd.set_arg(2, rt)
    Cmd.set_arg(3, rd)
    Cmd.set_last(0b00010000, Cmd::LAST_5_MASK)
    Cmd.set_code(0)
    Cmd.emit
end
  
def bext(rd, rs1, rs2)
    Cmd.set_arg(1, rd)
    Cmd.set_arg(2, rs1)
    Cmd.set_arg(3, rs2)
    Cmd.set_last(0b00001111, Cmd::LAST_5_MASK)
    Cmd.set_code(0)
    Cmd.emit
end
  
def slti(rt, rs, imm)
    Cmd.set_arg(1, rs)
    Cmd.set_arg(2, rt)
    Cmd.set_last(imm, Cmd::LAST_16_MASK)
    Cmd.set_code(0b00111101)
    Cmd.emit
end
  
# mem is array of 2
def st(rt, mem)
    offset, base = mem  

    Cmd.set_arg(1, base)
    Cmd.set_arg(2, rt)
    Cmd.check_align(offset)
    Cmd.set_last(offset, Cmd::LAST_16_MASK)
    Cmd.set_code(0b00111000)
    Cmd.emit
end
  
def ssat(rd, rs, imm5)
    Cmd.set_arg(1, rd)
    Cmd.set_arg(2, rs)
    Cmd.set_arg(3, imm5)
    Cmd.set_code(0b00111111)
    Cmd.emit
end

# mem is array of 2
def ldp(rt1, rt2, mem)
    offset, base = mem
    Cmd.set_arg(1, base)
    Cmd.check_align(offset)
    Cmd.set_last(offset, Cmd::LAST_11_MASK)
    Cmd.set_arg(2, rt1)
    Cmd.set_arg(3, rt2)
    Cmd.set_code(0b00110101)
    Cmd.emit
end

def beq(rs, rt, offset)
    Cmd.set_arg(1, rs)
    Cmd.set_arg(2, rt)
    Cmd.set_last(offset, Cmd::LAST_16_MASK)
    Cmd.set_code(0b00010110)
    Cmd.emit
end

# mem is array of 2
def ld(rt, mem)
    offset, base = mem
    Cmd.set_arg(1, base)
    Cmd.set_arg(2, rt)
    Cmd.check_align(offset)
    Cmd.set_last(offset, Cmd::LAST_16_MASK)
    Cmd.set_code(0b00111110)
    Cmd.emit
end
  
def syscall
    Cmd.set_last(0b010101, Cmd::LAST_5_MASK)
    Cmd.set_code(0)
    Cmd.emit
end
  
def j(target)
    index = target * 4 #TODO: jumps
    index &= (2**31 - 1 - 3)
    index &= (2**28 - 1)
    Cmd.set_last((index >> 2), Cmd::LAST_26_MASK)
    Cmd.set_code(0b00110000)
    Cmd.emit
end
  
def usat(rd, rs, imm5)
    Cmd.set_arg(1, rd)
    Cmd.set_arg(2, rs)
    Cmd.set_arg(3, imm5)
    Cmd.set_code(0b00100010)
    Cmd.emit
end
  
def clz(rd, rs)
    Cmd.set_arg(1, rd)
    Cmd.set_arg(2, rs)
    Cmd.set_last(0b011100, Cmd::LAST_5_MASK)
    Cmd.set_code(0)
    Cmd.emit
end

