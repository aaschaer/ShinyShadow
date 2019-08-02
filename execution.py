from source import SOURCE
import functions
from conversion import *


class PPC_executor():
    
    def __init__(self, start_address, starting_ram={}, starting_registers={}):

        self.end_reached = False
        self.current_address = start_address
        self.registers = starting_registers
        self.ram = {}
        for key, val in starting_ram.items():
            self.write_hex_str_to_ram(key, val)
        self.cr0 = None
        self.cr1 = None

    def print_line(self):
        print(hex(self.current_address), SOURCE[self.current_address])
        print("")

    def print_ram(self):
        start_address = 0x0
        last_address = 0x0
        hex_str = ""

        for key in sorted(self.ram.keys()):
            if last_address + 1 == key:
                hex_str += self.ram[key]
                last_address = key
            else:
                if hex_str:
                    print("{}: {}".format(hex(start_address), hex_str))
                start_address = key
                last_address = key
                hex_str = self.ram[key]

        if hex_str:
            print("{}: {}\n".format(hex(start_address), hex_str))

    def write_hex_str_to_ram(self, address, hex_str):
        """
        store given hex_str byte by byte into ram starting at given address
        """
        assert len(hex_str) % 2 == 0
        for i in range(len(hex_str) // 2):
            self.ram[address + i] = hex_str[(i * 2):(i * 2 + 2)]

    def read_hex_str_from_ram(self, address, byte_len):
        """
        read specified number of bytes from ram starting at given address
        """
        if address in (0x809e5474, 0x809e5475, 0x809e5476, 0x809e5478,
                       0x809e547c, 0x809e547d, 0x809e547e, 0x809e547f):
            print("reading from current position")
        
        ret = ""
        for i in range(byte_len):
            try:
                ret += self.ram[address + i]
            except KeyError:
                raise KeyError("no value at address {}"
                               .format(hex(address + i)))
        return ret

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

    def execute(self):
        DEBUG = False
        breakpoints = []

        while not self.end_reached:

            if self.current_address in breakpoints:
                DEBUG = True

            try:
                args = SOURCE[self.current_address].replace(",", " ").split()
            except KeyError:
                raise Exception("no source code for address", hex(
                    self.current_address))

            if DEBUG:
                self.print_line()
                input("press Enter to run line")

            f = method_to_call = getattr(functions, args[0].strip("-+."))

            try:
                f(self, *args[1:])
            except Exception as e:
                self.print_line()
                self.print_registers()
                self.print_ram()
                raise

            if DEBUG:
                self.print_registers()
                self.print_ram()
