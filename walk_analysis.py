import math
from conversion import *


def pyth_distance(c1, c2):
    x1 = single_to_double(hex_str_to_single(c1[0]))
    y1 = single_to_double(hex_str_to_single(c1[1]))
    x2 = single_to_double(hex_str_to_single(c2[0]))
    y2 = single_to_double(hex_str_to_single(c2[1]))
    c_sqr = (x2-x1)**2 + (x2-x1)**2
    return math.sqrt(c_sqr)

def x_distance(c1, c2):
    x1 = single_to_double(hex_str_to_single(c1[0]))
    x2 = single_to_double(hex_str_to_single(c2[0]))
    return x2 - x1

def y_distance(c1, c2):
    y1 = single_to_double(hex_str_to_single(c1[1]))
    y2 = single_to_double(hex_str_to_single(c2[1]))
    return y2 - y1

start = ("40800000", "41C00000")
target = ("40327EF7", "421BCDDC")

paths = [
    [
        ("40800000", "41C00000"), # very first step is eactly half
        ("407E7FFB", "41C250A4"),
        ("407B7FF1", "41C6F1EC"),
        ("40787FE7", "41CB9334"),
        ("40757FDD", "41D0347C"),
        ("40727FD3", "41D4D5C4"),
        ("406F7FC9", "41D9770C"),
        ("406C7FBF", "41DE1854"),
        ("40697FB5", "41E2B99C"),
        ("40667FAB", "41E75AE4"),
        ("40637FA1", "41EBFC2C"),
        ("40607F97", "41F09D74"),
        ("405D7F8D", "41F53EBC"),
        ("405A7F83", "41F9E004"),
        ("40577F79", "41FE814C"),
        ("40547F6F", "4201914A"),
        ("40517F65", "4203E1EE"),
        ("404E7F5B", "42063292"),
        ("404B7F51", "42088336"),
        ("40487F47", "420AD3DA"),
        ("40457F3D", "420D247E"),
        ("40427F33", "420F7522"),
        ("403F7F29", "4211C5C6"),
        ("403C7F1F", "4214166A"),
        ("40397F15", "4216670E"),
        ("40367F0B", "4218B7B2"),
        ("40337F01", "421B0856"),
        ("40327EF7", "421BCDDC"), # last step is an adjustment to meet dest
    ],
    [
        ("40327EF7", "421BCDDC"),
        ("403F495B", "42199F9C"),
        ("404C13BF", "4217715C"),
        ("4058DE23", "4215431C"),
        ("4065A887", "421314DC"),
        ("407272EB", "4210E69C"),
        ("407F3D4F", "420EB85C"),
        ("408603DA", "420C8A1C"),
        ("408C690C", "420A5BDC"),
        ("4092CE3E", "42082D9C"),
        ("40993370", "4205FF5C"),
        ("409F98A2", "4203D11C"),
        ("40A5FDD4", "4201A2DC"),
        ("40AC6306", "41FEE937"),
        ("40B2C838", "41FA8CB6"),
        ("40B92D6A", "41F63035"),
        ("40BF929C", "41F1D3B4"),
        ("40C5F7CE", "41ED7733"),
        ("40CC5D00", "41E91AB2"),
        ("40D2C232", "41E4BE31"),
        ("40D92764", "41E061B0"),
        ("40DF8C96", "41DC052F"),
        ("40E5F1C8", "41D7A8AE"),
        ("40EC56FA", "41D34C2D"),
        ("40F2BC2C", "41CEEFAC"),
        ("40F9215E", "41CA932B"),
        ("40FF8690", "41C636AA"),
        ("4102F5E1", "41C1DA29"),
        ("4106287A", "41BD7DA8"),
        ("41095B13", "41B92127"),
        ("410C8DAC", "41B4C4A6"),
        ("410FC045", "41B06825"),
        ("4112F2DE", "41AC0BA4"),
        ("41162577", "41A7AF23"),
        ("41195810", "41A352A2"),
        ("411C8AA9", "419EF621"),
        ("411FBD42", "419A99A0"),
        ("4122EFDB", "41963D1F"),
        ("41262274", "4191E09E"),
        ("4129550D", "418D841D"),
        ("412C87A6", "4189279C"),
        ("412FBA3F", "4184CB1B"),
        ("4132ECD8", "41806E9A"),
        ("41361F71", "41782433"),
        ("4139520A", "416F6B32"),
        ("413C84A3", "4166B231"),
        ("413FB73C", "415DF930"),
        ("4142E9D5", "4155402F"),
        ("41461C6E", "414C872E"),
        ("41494F07", "4143CE2D"),
        ("414C81A0", "413B152C"),
        ("414BD369", "413CF06C"),
    ],
]

# print(distance(start, target))

for path in paths:
    for i in range(1, len(path)):
        print(pyth_distance(path[i], path[i-1]))
        print(x_distance(path[i], path[i-1]))
        print(y_distance(path[i], path[i-1]))
        print()
    print()


c = 0.06629463248411971