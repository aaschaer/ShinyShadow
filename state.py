class State():
    
    def __init__(self, start_address, f1=None, sp=None):

        self.end_reached = False
        self.current_address = start_address
        self.registers = {
            "f1": f1,
            "sp": sp,
        }
        self.ram = {}
        self.cr0 = None
        self.cr1 = None