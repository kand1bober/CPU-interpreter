module Cmd
    ARG_SHIFTS = [nil, 21, 16, 11].freeze
    ARG_MASK = 2**5 - 1
    FUNC_SHIFT = 26
    FUNC_MASK = 2**6 - 1
    INSTR_MASK = 0xFFFFFFFF
    LAST_5_MASK = 2**5 - 1
    LAST_10_MASK = 2**10 - 1
    LAST_15_MASK = 2**15 - 1
    LAST_25_MASK = 2**25 - 1
  
    class << self #open singltone class for this module
      attr_accessor :instr, :file
    end
  
    def self.reset_instr
      @instr = 0
    end
  
    def self.set_arg(num, arg)
      @instr |= ((arg & ARG_MASK) << ARG_SHIFTS[num])
    end
  
    def self.set_last(arg, mask)
      @instr |= (arg & mask)
    end
  
    def self.set_code(code)
      @instr |= ((code & FUNC_MASK) << FUNC_SHIFT)
      @instr &= INSTR_MASK
    end
  
    def self.check_align(addr)
      abort("misaligned address(=#{addr})") if (addr & 3) != 0
    end
  
    def self.emit
      @file.write([@instr].pack("L<"))
      reset_instr
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
    Cmd.set_last(imm, Cmd::LAST_15_MASK)
    Cmd.set_code(0b00111101)
    Cmd.emit
  end
  
  def st(rt, offset, base)
    Cmd.set_arg(1, base)
    Cmd.set_arg(2, rt)
    Cmd.check_align(offset)
    Cmd.set_last(offset, Cmd::LAST_15_MASK)
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
  
  def ldp(rt1, rt2, offset, base)
    Cmd.set_arg(1, base)
    Cmd.set_arg(2, rt1)
    Cmd.set_arg(3, rt2)
    Cmd.check_align(offset)
    Cmd.set_last(offset, Cmd::LAST_10_MASK)
    Cmd.set_code(0b00110101)
    Cmd.emit
  end
  
  def beq(rs, rt, offset)
    Cmd.set_arg(1, rs)
    Cmd.set_arg(2, rt)
    Cmd.set_last(offset, Cmd::LAST_15_MASK)
    Cmd.set_code(0b00010110)
    Cmd.emit
  end
  
  def ld(rt, offset, base)
    Cmd.set_arg(1, base)
    Cmd.set_arg(2, rt)
    Cmd.check_align(offset)
    Cmd.set_last(offset, Cmd::LAST_15_MASK)
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
    Cmd.set_last((index >> 2), Cmd::LAST_25_MASK)
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

