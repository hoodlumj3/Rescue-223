/* keycodes.h     Keycodes for the IBM XT & AT keyboards
                  for use with getkey()                     */

#define FLUSHKBD() { while (kbhit()) getkey(); }

#define   SHIFT_TAB  0x0f00
#define   ALT_Q      0x1000
#define   ALT_W      0x1100
#define   ALT_E      0x1200
#define   ALT_R      0x1300
#define   ALT_T      0x1400
#define   ALT_Y      0x1500
#define   ALT_U      0x1600
#define   ALT_I      0x1700
#define   ALT_O      0x1800
#define   ALT_P      0x1900

#define   ALT_A      0x1e00
#define   ALT_S      0x1f00
#define   ALT_D      0x2000
#define   ALT_F      0x2100
#define   ALT_G      0x2200
#define   ALT_H      0x2300
#define   ALT_J      0x2400
#define   ALT_K      0x2500
#define   ALT_L      0x2600

#define   ALT_Z      0x2c00
#define   ALT_X      0x2d00
#define   ALT_C      0x2e00
#define   ALT_V      0x2f00
#define   ALT_B      0x3000
#define   ALT_N      0x3100
#define   ALT_M      0x3200
#define   F1         0x3b00
#define   F2         0x3c00
#define   F3         0x3d00
#define   F4         0x3e00
#define   F5         0x3f00
#define   F6         0x4000
#define   F7         0x4100
#define   F8         0x4200
#define   F9         0x4300
#define   F10        0x4400

#define   HOME       0x4700
#define   UP         0x4800
#define   PGUP       0x4900
#define   LEFT       0x4b00
#define   RIGHT      0x4d00
#define   END        0x4f00
#define   DOWN       0x5000
#define   PGDN       0x5100
#define   INSERT     0x5200
#define   DELETE     0x5300

#define   SHIFT_F1   0x5400
#define   SHIFT_F2   0x5500
#define   SHIFT_F3   0x5600
#define   SHIFT_F4   0x5700
#define   SHIFT_F5   0x5800
#define   SHIFT_F6   0x5900
#define   SHIFT_F7   0x5a00
#define   SHIFT_F8   0x5b00
#define   SHIFT_F9   0x5c00
#define   SHIFT_F10  0x5d00

#define   CNTL_F1   0x5e00
#define   CNTL_F2   0x5f00
#define   CNTL_F3   0x6000
#define   CNTL_F4   0x6100
#define   CNTL_F5   0x6200
#define   CNTL_F6   0x6300
#define   CNTL_F7   0x6400
#define   CNTL_F8   0x6500
#define   CNTL_F9   0x6600
#define   CNTL_F10  0x6700

#define   ALT_F1    0x6800
#define   ALT_F2    0x6900
#define   ALT_F3    0x6a00
#define   ALT_F4    0x6b00
#define   ALT_F5    0x6c00
#define   ALT_F6    0x6d00
#define   ALT_F7    0x6e00
#define   ALT_F8    0x6f00
#define   ALT_F9    0x7000
#define   ALT_F10   0x7100

#define   CNTL_PRTSC 0x7200
#define   CNTL_LEFT  0x7300
#define   CNTL_RIGHT 0x7400
#define   CNTL_END   0x7500
#define   CNTL_PGDN  0x7600
#define   CNTL_HOME  0x7700

#define   ALT_1      0x7800
#define   ALT_2      0x7900
#define   ALT_3      0x7a00
#define   ALT_4      0x7b00
#define   ALT_5      0x7c00
#define   ALT_6      0x7d00
#define   ALT_7      0x7e00
#define   ALT_8      0x7f00
#define   ALT_9      0x8000
#define   ALT_0      0x8100

#define   CNTL_PGUP  0x8400

/* keys with ASCII value (Scan code not shown) */

#define   CNTL_A     1
#define   CNTL_B     2
#define   CNTL_C     3
#define   CNTL_D     4
#define   CNTL_E     5
#define   CNTL_F     6
#define   CNTL_G     7
#define   CNTL_H     8
#define   CNTL_I     9
#define   CNTL_J     10
#define   CNTL_K     11
#define   CNTL_L     12
#define   CNTL_M     13
#define   ENTER      13
#define   CNTL_N     14
#define   CNTL_O     15
#define   CNTL_P     16
#define   CNTL_Q     17
#define   CNTL_R     18
#define   CNTL_S     19
#define   CNTL_T     20
#define   CNTL_U     21
#define   CNTL_V     22
#define   CNTL_W     23
#define   CNTL_X     24
#define   CNTL_Y     25
#define   CNTL_Z     26

#define   ESCAPE     27
