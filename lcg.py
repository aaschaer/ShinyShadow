"""
Implements a Linear Congruential Generator as used by
Pokemon Colloseum for pRNG
"""
from conversion import hex_str_to_double


class LCG():

    def __init__(self, seed):

        self.state = seed

    def generate(self, generation_constant=65536.0):
        """
        simulates subroutine 801adc7c
        generation constant is from rtoc
        """
        assert isinstance(generation_constant, float)

        self.state = (self.state * 214013 + 2531011) & 0xffffffff

        n = 0x4330000000000000 | (self.state >> 16)
        n = hex_str_to_double(hex(n)[2:])
        n -= 4503599627370496.0
        n /= generation_constant

        assert isinstance(n, float)

        return n
