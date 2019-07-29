from source import SOURCE
import functions
from conversion import *


class PPC_executor():
    
    def __init__(self, start_address, starting_ram, starting_registers):

        self.end_reached = False
        self.current_address = start_address
        self.registers = starting_registers
        self.ram = starting_ram
        self.cr0 = None
        self.cr1 = None
        self.ret = [] # list of values to return

    def print_line(self):
        print(hex(self.current_address), SOURCE[self.current_address])
        print("")

    def print_registers(self):
        for key in sorted(self.registers.keys()):
            val = self.registers[key]
            if isinstance(val, int):
                print("{}: {}".format(key, int_to_hex_str(val)))
            elif isinstance(val, float):
                print("{}: {}".format(key,  double_to_hex_str(val)))
            else:
                print("{}: {}".format(key,  val))
                print("UNEXPECTED TYPE: {}".format(type(val)))
        print("")

    def print_ram(self):
        for key in sorted(self.ram.keys()):
            print("{}: {}".format(hex(key), self.ram[key]))
        print("")
            

    def execute(self):
        DEBUG = False
        breakpoints = []

        while not self.end_reached:

            if self.current_address in breakpoints:
                DEBUG = True

            try:
                args = SOURCE[self.current_address].replace(",", " ").split()
            except KeyError:
                raise Exception("no source code for address", hex(self.current_address))

            if DEBUG:
                self.print_line()
                input("press Enter to run line")

            f = method_to_call = getattr(functions, args[0].strip("-+"))

            try:
                f(self, *args[1:])
            except Exception as e:
                self.print_line()
                self.print_registers()
                # self.print_ram()
                raise

            if DEBUG:
                self.print_registers()

        return self.ret
