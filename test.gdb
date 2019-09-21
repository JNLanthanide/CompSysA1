start
# TODO: Add debugging commands here.
p /x 192
p /t 192
p /t 0x50
p /d 0x50
p /x 0b1101110
p /d 0b1101110
p "Some basic tests.."
p UTF8_CONT(128) != 0
p UTF8_2B(192) != 0
p UTF8_3B(224) != 0
p UTF8_4B(240) != 0
#
p "Testing UTF8_CONT.."
p UTF8_CONT(128 + 1) != 0
p UTF8_CONT(128 | 1) != 0
p UTF8_CONT(128 | 63) != 0
p UTF8_CONT(128 | 63) > 0
p UTF8_CONT(128 + 64) == 0
p UTF8_CONT(128 | 64) == 0
#
p "Testing UTF8_2B.."
p UTF8_2B(128 + 64) != 0
p UTF8_2B(128 | 64) != 0
p UTF8_2B(128 | 64 | 31) != 0
p UTF8_2B(128 | 64 | 31) > 0
p UTF8_2B(128 + 64 + 32) == 0
p UTF8_2B(128 | 64 | 32) == 0
#
p "Testing UTF8_3B.."
p UTF8_3B(128 + 64 + 32) != 0
p UTF8_3B(128 | 64 | 32) != 0
p UTF8_3B(128 | 64 | 32 | 15) != 0
p UTF8_3B(128 | 64 | 32 | 15) > 0
p UTF8_3B(128 + 64 + 32 + 16) == 0
p UTF8_3B(128 | 64 | 32 | 16) == 0
#
p "Testing UTF8_4B.."
p UTF8_4B(128 + 64 + 32 + 16) != 0
p UTF8_4B(128 | 64 | 32 | 16) != 0
p UTF8_4B(128 | 64 | 32 | 16 | 7) != 0
p UTF8_4B(128 | 64 | 32 | 16 | 7) > 0
p UTF8_4B(128 + 64 + 32 + 16 + 8) == 0
p UTF8_4B(128 | 64 | 32 | 16 | 8) == 0
#
p "More student tests.."
p UTF8_CONT(128 - 32) == 0
p UTF8_CONT(170 - 32) != 0
p UTF8_CONT(0b10000000) > 0
p UTF8_CONT(0b100000000) == 0
p UTF8_CONT(192^64) != 0

p UTF8_2B(224 - 32) > 0
p UTF8_2B(0b11000000) > 0
p UTF8_2B(224 ^ 32) != 0

p UTF8_3B(240 - 16) != 0
p UTF8_3B(0b11100000) > 0
p UTF8_3B(0b111000000) == 0

p UTF8_4B(248 - 8) != 0
p UTF8_4B(0b11110000) > 0
p UTF8_4B(0b111100000) == 0
q
