EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr B 17000 11000
encoding utf-8
Sheet 2 5
Title "Robocar, Pinky, Power System"
Date "2019-12-28"
Rev "A"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L R-745.0D:R-745.0D PS1
U 1 1 5E0742C0
P 5600 5200
F 0 "PS1" H 5600 5667 50  0000 C CNN
F 1 "R-745.0D" H 5600 5576 50  0000 C CNN
F 2 "libs:CONV_R-745.0D" H 5600 5200 50  0001 L BNN
F 3 "Manufacturer Recommendations" H 5600 5200 50  0001 L BNN
F 4 "R-745.0D" H 5600 5200 50  0001 C CNN "MPN"
	1    5600 5200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Barrel_Jack J2
U 1 1 5E07962E
P 3400 2050
F 0 "J2" H 3457 2375 50  0000 C CNN
F 1 "Barrel_Jack" H 3457 2284 50  0000 C CNN
F 2 "Connector_BarrelJack:BarrelJack_Horizontal" H 3450 2010 50  0001 C CNN
F 3 "~" H 3450 2010 50  0001 C CNN
F 4 "PJ-102B" H 3400 2050 50  0001 C CNN "MPN"
	1    3400 2050
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 5E07D5B3
P 3350 5100
F 0 "J1" H 3350 4800 50  0000 C CNN
F 1 "Conn_01x02_Male" H 3650 4900 50  0000 C CNN
F 2 "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal" H 3350 5100 50  0001 C CNN
F 3 "~" H 3350 5100 50  0001 C CNN
F 4 "XT30PW-M" H 3350 5100 50  0001 C CNN "MPN"
	1    3350 5100
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5E08083B
P 4200 2400
F 0 "#PWR0101" H 4200 2150 50  0001 C CNN
F 1 "GND" H 4205 2227 50  0000 C CNN
F 2 "" H 4200 2400 50  0001 C CNN
F 3 "" H 4200 2400 50  0001 C CNN
	1    4200 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 5000 4350 5000
Wire Wire Line
	3700 2150 3900 2150
Wire Wire Line
	3700 1950 4200 1950
$Comp
L power:VBUS #PWR0102
U 1 1 5E0884D7
P 13450 6900
F 0 "#PWR0102" H 13450 6750 50  0001 C CNN
F 1 "VBUS" H 13465 7073 50  0000 C CNN
F 2 "" H 13450 6900 50  0001 C CNN
F 3 "" H 13450 6900 50  0001 C CNN
	1    13450 6900
	1    0    0    -1  
$EndComp
Connection ~ 4350 5000
Wire Wire Line
	4350 5000 4750 5000
Wire Wire Line
	6300 5000 6700 5000
Wire Wire Line
	6700 5000 6700 4500
Wire Wire Line
	6700 4500 7100 4500
$Comp
L power:GND #PWR0103
U 1 1 5E080241
P 4350 5500
F 0 "#PWR0103" H 4350 5250 50  0001 C CNN
F 1 "GND" H 4355 5327 50  0000 C CNN
F 2 "" H 4350 5500 50  0001 C CNN
F 3 "" H 4350 5500 50  0001 C CNN
	1    4350 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5E08964D
P 6700 5500
F 0 "#PWR0104" H 6700 5250 50  0001 C CNN
F 1 "GND" H 6705 5327 50  0000 C CNN
F 2 "" H 6700 5500 50  0001 C CNN
F 3 "" H 6700 5500 50  0001 C CNN
	1    6700 5500
	1    0    0    -1  
$EndComp
NoConn ~ 4900 5200
$Comp
L Device:CP C5
U 1 1 5E08AB46
P 11750 3550
F 0 "C5" H 11868 3596 50  0000 L CNN
F 1 "100uF" H 11868 3505 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric" H 11788 3400 50  0001 C CNN
F 3 "~" H 11750 3550 50  0001 C CNN
F 4 "C1210C107M4PAC7800" H 11750 3550 50  0001 C CNN "MPN"
	1    11750 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C3
U 1 1 5E08D8A2
P 7100 5200
F 0 "C3" H 7218 5246 50  0000 L CNN
F 1 "100uF" H 7218 5155 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric" H 7138 5050 50  0001 C CNN
F 3 "~" H 7100 5200 50  0001 C CNN
F 4 "C1210C107M4PAC7800" H 7100 5200 50  0001 C CNN "MPN"
	1    7100 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 5400 6700 5500
Wire Wire Line
	6300 5400 6700 5400
Wire Wire Line
	3850 5100 3850 5450
Wire Wire Line
	3550 5100 3850 5100
Wire Wire Line
	4350 5450 4350 5400
Wire Wire Line
	4350 5100 4350 5000
Wire Wire Line
	3850 5450 4350 5450
Wire Wire Line
	4350 5450 4350 5500
Connection ~ 4350 5450
Wire Wire Line
	4200 2000 4200 1950
$Comp
L Device:D D1
U 1 1 5E095242
P 7550 4500
F 0 "D1" H 7550 4284 50  0000 C CNN
F 1 "STPS5L60S" H 7550 4375 50  0000 C CNN
F 2 "agg:DO-214AB-SMC" H 7550 4500 50  0001 C CNN
F 3 "~" H 7550 4500 50  0001 C CNN
F 4 "STPS5L60S" H 7550 4500 50  0001 C CNN "MPN"
	1    7550 4500
	-1   0    0    1   
$EndComp
$Comp
L Device:R R3
U 1 1 5E0981F7
P 6550 5200
F 0 "R3" V 6650 5200 50  0000 C CNN
F 1 "2.6k" V 6450 5200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6480 5200 50  0001 C CNN
F 3 "~" H 6550 5200 50  0001 C CNN
F 4 "RC0603FR-072K61L" V 6550 5200 50  0001 C CNN "MPN"
	1    6550 5200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6300 5200 6400 5200
Wire Wire Line
	6700 5200 6700 5400
Connection ~ 6700 5400
Wire Wire Line
	6700 5400 7100 5400
Wire Wire Line
	7100 5400 7100 5350
Wire Wire Line
	7100 5050 7100 4500
Connection ~ 7100 4500
$Comp
L power:GND #PWR0105
U 1 1 5E09C302
P 11750 4650
F 0 "#PWR0105" H 11750 4400 50  0001 C CNN
F 1 "GND" H 11755 4477 50  0000 C CNN
F 2 "" H 11750 4650 50  0001 C CNN
F 3 "" H 11750 4650 50  0001 C CNN
	1    11750 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0106
U 1 1 5E09D38F
P 13450 3200
F 0 "#PWR0106" H 13450 3050 50  0001 C CNN
F 1 "+5V" H 13465 3373 50  0000 C CNN
F 2 "" H 13450 3200 50  0001 C CNN
F 3 "" H 13450 3200 50  0001 C CNN
	1    13450 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 2150 3900 2350
Wire Wire Line
	3900 2350 4200 2350
Wire Wire Line
	4200 2350 4200 2300
Wire Wire Line
	4200 2400 4200 2350
Connection ~ 4200 2350
Wire Wire Line
	4200 1950 6550 1950
Connection ~ 4200 1950
$Comp
L Device:R R1
U 1 1 5E078ECF
P 6550 2200
F 0 "R1" V 6650 2200 50  0000 C CNN
F 1 "23.7k" V 6450 2200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6480 2200 50  0001 C CNN
F 3 "~" H 6550 2200 50  0001 C CNN
F 4 "RC0603FR-0723K7L" V 6550 2200 50  0001 C CNN "MPN"
	1    6550 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5E07A0F6
P 6550 2600
F 0 "R2" V 6650 2600 50  0000 C CNN
F 1 "5k" V 6450 2600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6480 2600 50  0001 C CNN
F 3 "~" H 6550 2600 50  0001 C CNN
F 4 "CR0603-FX-4991ELF" V 6550 2600 50  0001 C CNN "MPN"
	1    6550 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5E07A6BA
P 6550 2850
F 0 "#PWR0107" H 6550 2600 50  0001 C CNN
F 1 "GND" H 6555 2677 50  0000 C CNN
F 2 "" H 6550 2850 50  0001 C CNN
F 3 "" H 6550 2850 50  0001 C CNN
	1    6550 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 2050 6550 1950
Connection ~ 6550 1950
Wire Wire Line
	6550 1950 7550 1950
Wire Wire Line
	6550 2350 6550 2400
Wire Wire Line
	6550 2750 6550 2850
$Comp
L Device:R R6
U 1 1 5E08CAC8
P 8350 4800
F 0 "R6" V 8450 4800 50  0000 C CNN
F 1 "23.7k" V 8250 4800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8280 4800 50  0001 C CNN
F 3 "~" H 8350 4800 50  0001 C CNN
F 4 "RC0603FR-0723K7L" V 8350 4800 50  0001 C CNN "MPN"
	1    8350 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5E08D58D
P 8350 5200
F 0 "R7" V 8450 5200 50  0000 C CNN
F 1 "5k" V 8250 5200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8280 5200 50  0001 C CNN
F 3 "~" H 8350 5200 50  0001 C CNN
F 4 "CR0603-FX-4991ELF" V 8350 5200 50  0001 C CNN "MPN"
	1    8350 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5E08DB27
P 8350 5500
F 0 "#PWR0108" H 8350 5250 50  0001 C CNN
F 1 "GND" H 8355 5327 50  0000 C CNN
F 2 "" H 8350 5500 50  0001 C CNN
F 3 "" H 8350 5500 50  0001 C CNN
	1    8350 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 4500 8350 4650
Wire Wire Line
	8350 4950 8350 5000
Wire Wire Line
	8350 5350 8350 5500
Wire Wire Line
	8350 4500 8850 4500
$Comp
L power:GND #PWR0109
U 1 1 5E0908A8
P 9250 4750
F 0 "#PWR0109" H 9250 4500 50  0001 C CNN
F 1 "GND" H 9255 4577 50  0000 C CNN
F 2 "" H 9250 4750 50  0001 C CNN
F 3 "" H 9250 4750 50  0001 C CNN
	1    9250 4750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5E09266F
P 7550 2200
F 0 "R4" V 7650 2200 50  0000 C CNN
F 1 "10.2k" V 7450 2200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7480 2200 50  0001 C CNN
F 3 "~" H 7550 2200 50  0001 C CNN
F 4 "RC0603FR-0710K2L" V 7550 2200 50  0001 C CNN "MPN"
	1    7550 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5E092EA9
P 7550 2600
F 0 "R5" V 7650 2600 50  0000 C CNN
F 1 "5k" V 7450 2600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7480 2600 50  0001 C CNN
F 3 "~" H 7550 2600 50  0001 C CNN
F 4 "CR0603-FX-4991ELF" V 7550 2600 50  0001 C CNN "MPN"
	1    7550 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5E093482
P 7550 2850
F 0 "#PWR0110" H 7550 2600 50  0001 C CNN
F 1 "GND" H 7555 2677 50  0000 C CNN
F 2 "" H 7550 2850 50  0001 C CNN
F 3 "" H 7550 2850 50  0001 C CNN
	1    7550 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 2050 7550 1950
Wire Wire Line
	7550 2350 7550 2400
Wire Wire Line
	7550 2750 7550 2850
Wire Wire Line
	7550 1950 8850 1950
Wire Wire Line
	8850 1950 8850 3250
Wire Wire Line
	8850 3250 10100 3250
Connection ~ 7550 1950
Wire Wire Line
	10100 3350 8350 3350
Wire Wire Line
	8350 3350 8350 2400
Wire Wire Line
	8350 2400 7550 2400
Connection ~ 7550 2400
Wire Wire Line
	7550 2400 7550 2450
Wire Wire Line
	10100 3450 7050 3450
Wire Wire Line
	7050 3450 7050 2400
Wire Wire Line
	7050 2400 6550 2400
Connection ~ 6550 2400
Wire Wire Line
	6550 2400 6550 2450
Wire Wire Line
	8850 4500 8850 3650
Wire Wire Line
	8850 3650 10100 3650
Wire Wire Line
	8350 5000 9000 5000
Wire Wire Line
	9000 5000 9000 3750
Wire Wire Line
	9000 3750 10100 3750
Connection ~ 8350 5000
Wire Wire Line
	8350 5000 8350 5050
$Comp
L Device:R R8
U 1 1 5E09A6FB
P 10000 4500
F 0 "R8" H 10100 4550 50  0000 C CNN
F 1 "18.7k" H 10150 4450 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9930 4500 50  0001 C CNN
F 3 "~" H 10000 4500 50  0001 C CNN
F 4 "RC0603FR-0718K7L" H 10000 4500 50  0001 C CNN "MPN"
	1    10000 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 5E09B1AB
P 9550 4500
F 0 "C4" H 9665 4546 50  0000 L CNN
F 1 "1uF" H 9665 4455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 9588 4350 50  0001 C CNN
F 3 "~" H 9550 4500 50  0001 C CNN
F 4 "C0805C105K4RACTU" H 9550 4500 50  0001 C CNN "MPN"
	1    9550 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 3950 10100 3950
Wire Wire Line
	9550 4350 9550 4150
Wire Wire Line
	9550 4150 10100 4150
Wire Wire Line
	10100 4250 10000 4250
Wire Wire Line
	10000 4250 10000 4350
$Comp
L power:GND #PWR0111
U 1 1 5E0A06CA
P 9550 4750
F 0 "#PWR0111" H 9550 4500 50  0001 C CNN
F 1 "GND" H 9555 4577 50  0000 C CNN
F 2 "" H 9550 4750 50  0001 C CNN
F 3 "" H 9550 4750 50  0001 C CNN
	1    9550 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 5E0A0945
P 10000 4750
F 0 "#PWR0112" H 10000 4500 50  0001 C CNN
F 1 "GND" H 10005 4577 50  0000 C CNN
F 2 "" H 10000 4750 50  0001 C CNN
F 3 "" H 10000 4750 50  0001 C CNN
	1    10000 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 4650 10000 4750
Wire Wire Line
	9550 4650 9550 4750
Wire Wire Line
	9250 3950 9250 4750
$Comp
L TPS2121RUXR:TPS2121RUXR U6
U 1 1 5E0759B5
P 10600 3850
F 0 "U6" H 10600 4817 50  0000 C CNN
F 1 "TPS2121RUXR" H 10600 4726 50  0000 C CNN
F 2 "libs:IC_TPS2121RUXR" H 10600 3850 50  0001 L BNN
F 3 "TEXAS INSTRUMENTS" H 10600 3850 50  0001 L BNN
F 4 "TPS2121RUXR" H 10600 3850 50  0001 L BNN "MPN"
	1    10600 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 4500 7400 4500
Wire Wire Line
	7700 4500 8350 4500
Connection ~ 8350 4500
$Comp
L power:GND #PWR0113
U 1 1 5E0AA954
P 11200 3450
F 0 "#PWR0113" H 11200 3200 50  0001 C CNN
F 1 "GND" H 11205 3277 50  0000 C CNN
F 2 "" H 11200 3450 50  0001 C CNN
F 3 "" H 11200 3450 50  0001 C CNN
	1    11200 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	11100 3350 11200 3350
Wire Wire Line
	11200 3350 11200 3450
Text Notes 2450 2250 0    100  ~ 0
LOGIC\nGSE\nINPUT
Text Notes 2450 5200 0    100  ~ 0
LOGIC\nBATTERY\nINPUT
Text Notes 13800 7000 0    100  ~ 0
MOTOR\nBUS
Text Notes 13800 3300 0    100  ~ 0
LOGIC\nBUS
$Comp
L Device:C C1
U 1 1 5E0B1109
P 4200 2150
F 0 "C1" H 4315 2196 50  0000 L CNN
F 1 "1uF" H 4315 2105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4238 2000 50  0001 C CNN
F 3 "~" H 4200 2150 50  0001 C CNN
F 4 "C0805C105K4RACTU" H 4200 2150 50  0001 C CNN "MPN"
	1    4200 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C9
U 1 1 5E0F6241
P 11150 7300
F 0 "C9" H 11268 7346 50  0000 L CNN
F 1 "100uF" H 11268 7255 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric" H 11188 7150 50  0001 C CNN
F 3 "~" H 11150 7300 50  0001 C CNN
F 4 "C1210C107M4PAC7800" H 11150 7300 50  0001 C CNN "MPN"
	1    11150 7300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5E08A39D
P 4350 5250
F 0 "C2" H 4465 5296 50  0000 L CNN
F 1 "1uF" H 4465 5205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4388 5100 50  0001 C CNN
F 3 "~" H 4350 5250 50  0001 C CNN
F 4 "C0805C105K4RACTU" H 4350 5250 50  0001 C CNN "MPN"
	1    4350 5250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 5E0F77BF
P 10150 7300
F 0 "C7" H 10265 7346 50  0000 L CNN
F 1 "1uF" H 10265 7255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 10188 7150 50  0001 C CNN
F 3 "~" H 10150 7300 50  0001 C CNN
F 4 "C0805C105K4RACTU" H 10150 7300 50  0001 C CNN "MPN"
	1    10150 7300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 5E0FA8A8
P 10650 7300
F 0 "C8" H 10765 7346 50  0000 L CNN
F 1 "10uF" H 10765 7255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 10688 7150 50  0001 C CNN
F 3 "~" H 10650 7300 50  0001 C CNN
F 4 "C0805C106K4PACTU" H 10650 7300 50  0001 C CNN "MPN"
	1    10650 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	11150 7150 11150 7050
Wire Wire Line
	10650 7150 10650 7050
Connection ~ 10650 7050
Wire Wire Line
	10650 7050 11150 7050
Wire Wire Line
	10150 7150 10150 7050
Wire Wire Line
	10150 7050 10650 7050
$Comp
L power:GND #PWR0114
U 1 1 5E1015AC
P 10150 7550
F 0 "#PWR0114" H 10150 7300 50  0001 C CNN
F 1 "GND" H 10155 7377 50  0000 C CNN
F 2 "" H 10150 7550 50  0001 C CNN
F 3 "" H 10150 7550 50  0001 C CNN
	1    10150 7550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 5E101EA6
P 10650 7550
F 0 "#PWR0115" H 10650 7300 50  0001 C CNN
F 1 "GND" H 10655 7377 50  0000 C CNN
F 2 "" H 10650 7550 50  0001 C CNN
F 3 "" H 10650 7550 50  0001 C CNN
	1    10650 7550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 5E1022CA
P 11150 7550
F 0 "#PWR0116" H 11150 7300 50  0001 C CNN
F 1 "GND" H 11155 7377 50  0000 C CNN
F 2 "" H 11150 7550 50  0001 C CNN
F 3 "" H 11150 7550 50  0001 C CNN
	1    11150 7550
	1    0    0    -1  
$EndComp
Wire Wire Line
	11150 7450 11150 7550
Wire Wire Line
	10650 7450 10650 7550
Wire Wire Line
	10150 7450 10150 7550
Wire Wire Line
	4750 5000 4750 6650
Wire Wire Line
	4750 6650 9650 6650
Connection ~ 4750 5000
Wire Wire Line
	4750 5000 4900 5000
$Comp
L Connector:Conn_01x02_Male J3
U 1 1 5E109960
P 3350 7600
F 0 "J3" H 3350 7300 50  0000 C CNN
F 1 "Conn_01x02_Male" H 3650 7400 50  0000 C CNN
F 2 "Connector_AMASS:AMASS_XT30PW-M_1x02_P2.50mm_Horizontal" H 3350 7600 50  0001 C CNN
F 3 "~" H 3350 7600 50  0001 C CNN
F 4 "XT30PW-M" H 3350 7600 50  0001 C CNN "MPN"
	1    3350 7600
	1    0    0    1   
$EndComp
Text Notes 2450 7700 0    100  ~ 0
MOTOR\nBATTERY\nINPUT
$Comp
L Device:C C6
U 1 1 5E10B395
P 4300 7750
F 0 "C6" H 4415 7796 50  0000 L CNN
F 1 "1uF" H 4415 7705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4338 7600 50  0001 C CNN
F 3 "~" H 4300 7750 50  0001 C CNN
F 4 "C0805C105K4RACTU" H 4300 7750 50  0001 C CNN "MPN"
	1    4300 7750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 7500 4300 7500
Wire Wire Line
	4300 7600 4300 7500
Connection ~ 4300 7500
Wire Wire Line
	4300 7500 9650 7500
$Comp
L power:GND #PWR0117
U 1 1 5E10FDC9
P 4300 8000
F 0 "#PWR0117" H 4300 7750 50  0001 C CNN
F 1 "GND" H 4305 7827 50  0000 C CNN
F 2 "" H 4300 8000 50  0001 C CNN
F 3 "" H 4300 8000 50  0001 C CNN
	1    4300 8000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 7600 3850 7600
Wire Wire Line
	3850 7600 3850 7950
Wire Wire Line
	3850 7950 4300 7950
Wire Wire Line
	4300 7950 4300 7900
Wire Wire Line
	4300 7950 4300 8000
Connection ~ 4300 7950
$Comp
L Connector:TestPoint JLBAT1
U 1 1 5E1187F8
P 9650 6800
F 0 "JLBAT1" H 9950 6900 50  0000 R CNN
F 1 "TestPoint" H 9592 6917 50  0001 R CNN
F 2 "TestPoint:TestPoint_Pad_D4.0mm" H 9850 6800 50  0001 C CNN
F 3 "~" H 9850 6800 50  0001 C CNN
	1    9650 6800
	-1   0    0    1   
$EndComp
$Comp
L Connector:TestPoint JVBUS1
U 1 1 5E119F00
P 9800 7050
F 0 "JVBUS1" V 9850 7000 50  0000 C CNN
F 1 "TestPoint" V 9904 7122 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_D4.0mm" H 10000 7050 50  0001 C CNN
F 3 "~" H 10000 7050 50  0001 C CNN
	1    9800 7050
	0    -1   -1   0   
$EndComp
$Comp
L Connector:TestPoint JMBAT1
U 1 1 5E11AC9D
P 9650 7300
F 0 "JMBAT1" H 9350 7400 50  0000 L CNN
F 1 "TestPoint" V 9754 7372 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_D4.0mm" H 9850 7300 50  0001 C CNN
F 3 "~" H 9850 7300 50  0001 C CNN
	1    9650 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 7300 9650 7500
Connection ~ 10150 7050
Wire Wire Line
	9650 6650 9650 6800
Wire Wire Line
	9800 7050 10150 7050
Wire Wire Line
	11100 3250 11750 3250
Wire Wire Line
	11750 3250 11750 3400
Wire Wire Line
	11100 4450 11750 4450
Wire Wire Line
	11750 4450 11750 3700
Wire Wire Line
	11750 4450 11750 4650
Connection ~ 11750 4450
$Comp
L Switch:SW_SPST SW1
U 1 1 5E16187A
P 12700 3250
F 0 "SW1" H 12700 3400 50  0000 C CNN
F 1 "SW_SPST" H 12700 3100 50  0000 C CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 12700 3250 50  0001 C CNN
F 3 "~" H 12700 3250 50  0001 C CNN
F 4 "M2011LL1W01-G" H 12700 3250 50  0001 C CNN "MPN"
	1    12700 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	11750 3250 12500 3250
Connection ~ 11750 3250
Wire Wire Line
	12900 3250 13450 3250
Wire Wire Line
	13450 3250 13450 3200
$Comp
L Switch:SW_SPST SW2
U 1 1 5E16792C
P 12700 7050
F 0 "SW2" H 12700 7200 50  0000 C CNN
F 1 "SW_SPST" H 12700 6900 50  0000 C CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 12700 7050 50  0001 C CNN
F 3 "~" H 12700 7050 50  0001 C CNN
F 4 "M2011LL1W01-C" H 12700 7050 50  0001 C CNN "MPN"
	1    12700 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	11150 7050 12500 7050
Connection ~ 11150 7050
Wire Wire Line
	12900 7050 13450 7050
Wire Wire Line
	13450 7050 13450 6900
$Comp
L Device:LED D2
U 1 1 5E17B2C5
P 13450 4100
F 0 "D2" V 13489 3983 50  0000 R CNN
F 1 "LED" V 13398 3983 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 13450 4100 50  0001 C CNN
F 3 "~" H 13450 4100 50  0001 C CNN
F 4 "MPR3BWD" V 13450 4100 50  0001 C CNN "MPN"
	1    13450 4100
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D3
U 1 1 5E17C7EB
P 13450 7700
F 0 "D3" V 13489 7583 50  0000 R CNN
F 1 "LED" V 13398 7583 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 13450 7700 50  0001 C CNN
F 3 "~" H 13450 7700 50  0001 C CNN
F 4 "MPR3BWD" V 13450 7700 50  0001 C CNN "MPN"
	1    13450 7700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 5E17D36D
P 13450 8000
F 0 "#PWR0118" H 13450 7750 50  0001 C CNN
F 1 "GND" H 13455 7827 50  0000 C CNN
F 2 "" H 13450 8000 50  0001 C CNN
F 3 "" H 13450 8000 50  0001 C CNN
	1    13450 8000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 5E1810CE
P 13450 4650
F 0 "#PWR0119" H 13450 4400 50  0001 C CNN
F 1 "GND" H 13455 4477 50  0000 C CNN
F 2 "" H 13450 4650 50  0001 C CNN
F 3 "" H 13450 4650 50  0001 C CNN
	1    13450 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 5E188924
P 13450 7300
F 0 "R10" H 13550 7350 50  0000 C CNN
F 1 "470" H 13600 7250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 13380 7300 50  0001 C CNN
F 3 "~" H 13450 7300 50  0001 C CNN
F 4 "ESR03EZPJ471" H 13450 7300 50  0001 C CNN "MPN"
	1    13450 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	13450 3250 13450 3450
Connection ~ 13450 3250
Wire Wire Line
	13450 3750 13450 3950
Wire Wire Line
	13450 4250 13450 4650
$Comp
L Device:R R9
U 1 1 5E1821EC
P 13450 3600
F 0 "R9" H 13550 3650 50  0000 C CNN
F 1 "47R" H 13600 3550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 13380 3600 50  0001 C CNN
F 3 "~" H 13450 3600 50  0001 C CNN
F 4 "ESR03EZPJ470" H 13450 3600 50  0001 C CNN "MPN"
	1    13450 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	13450 7050 13450 7150
Connection ~ 13450 7050
Wire Wire Line
	13450 7450 13450 7550
Wire Wire Line
	13450 7850 13450 8000
$EndSCHEMATC
