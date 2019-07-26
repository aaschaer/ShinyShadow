class State():
    
    def __init__(self, start_address, starting_ram, starting_registers):

        self.end_reached = False
        self.current_address = start_address
        self.registers = starting_registers
        self.ram = starting_ram
        self.cr0 = None
        self.cr1 = None
        self.ret = [] # list of values to return