EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr B 17000 11000
encoding utf-8
Sheet 4 5
Title "Robocar, Pinky, Sensors"
Date "2019-12-28"
Rev "A"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Analog_ADC:ADS1115IDGS U?
U 1 1 5E067AB7
P 12000 2750
AR Path="/5E067AB7" Ref="U?"  Part="1" 
AR Path="/5E058596/5E067AB7" Ref="U4"  Part="1" 
F 0 "U4" H 12250 2350 50  0000 C CNN
F 1 "ADS1115IDGS" H 12450 2250 50  0000 C CNN
F 2 "Package_SO:TSSOP-10_3x3mm_P0.5mm" H 12000 2250 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ads1113.pdf" H 11950 1850 50  0001 C CNN
F 4 "ADS1115IDGSR" H 12000 2750 50  0001 C CNN "MPN"
	1    12000 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0147
U 1 1 5E314BBD
P 12000 3350
F 0 "#PWR0147" H 12000 3100 50  0001 C CNN
F 1 "GND" H 12005 3177 50  0000 C CNN
F 2 "" H 12000 3350 50  0001 C CNN
F 3 "" H 12000 3350 50  0001 C CNN
	1    12000 3350
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0148
U 1 1 5E3152F6
P 12000 1400
F 0 "#PWR0148" H 12000 1250 50  0001 C CNN
F 1 "+5V" H 12015 1573 50  0000 C CNN
F 2 "" H 12000 1400 50  0001 C CNN
F 3 "" H 12000 1400 50  0001 C CNN
	1    12000 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0149
U 1 1 5E3159BD
P 12950 3350
F 0 "#PWR0149" H 12950 3100 50  0001 C CNN
F 1 "GND" H 12955 3177 50  0000 C CNN
F 2 "" H 12950 3350 50  0001 C CNN
F 3 "" H 12950 3350 50  0001 C CNN
	1    12950 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	12400 2950 12950 2950
Wire Wire Line
	12950 2950 12950 3350
Wire Wire Line
	12000 3150 12000 3350
$Comp
L Device:R R31
U 1 1 5E3160E6
P 12750 2250
F 0 "R31" H 12820 2296 50  0000 L CNN
F 1 "10k" H 12820 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 12680 2250 50  0001 C CNN
F 3 "~" H 12750 2250 50  0001 C CNN
F 4 "RC0603FR-0710KL" H 12750 2250 50  0001 C CNN "MPN"
	1    12750 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R32
U 1 1 5E31690D
P 13250 2250
F 0 "R32" H 13320 2296 50  0000 L CNN
F 1 "10k" H 13320 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 13180 2250 50  0001 C CNN
F 3 "~" H 13250 2250 50  0001 C CNN
F 4 "RC0603FR-0710KL" H 13250 2250 50  0001 C CNN "MPN"
	1    13250 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R33
U 1 1 5E316F25
P 13600 2250
F 0 "R33" H 13670 2296 50  0000 L CNN
F 1 "10k" H 13670 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 13530 2250 50  0001 C CNN
F 3 "~" H 13600 2250 50  0001 C CNN
F 4 "RC0603FR-0710KL" H 13600 2250 50  0001 C CNN "MPN"
	1    13600 2250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0150
U 1 1 5E317505
P 12750 1850
F 0 "#PWR0150" H 12750 1700 50  0001 C CNN
F 1 "+5V" H 12765 2023 50  0000 C CNN
F 2 "" H 12750 1850 50  0001 C CNN
F 3 "" H 12750 1850 50  0001 C CNN
	1    12750 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	12400 2550 12750 2550
Wire Wire Line
	12750 2550 12750 2400
Wire Wire Line
	12400 2750 13250 2750
Wire Wire Line
	13250 2750 13250 2400
Wire Wire Line
	12400 2850 13600 2850
Wire Wire Line
	13600 2850 13600 2400
Wire Wire Line
	13250 2100 13250 1950
Wire Wire Line
	13250 1950 12750 1950
Wire Wire Line
	12750 1950 12750 2100
Wire Wire Line
	13250 1950 13600 1950
Wire Wire Line
	13600 1950 13600 2100
Connection ~ 13250 1950
Wire Wire Line
	12750 1850 12750 1950
Connection ~ 12750 1950
Text GLabel 13950 2750 2    50   BiDi ~ 0
SCL
Text GLabel 13950 2850 2    50   BiDi ~ 0
SDA
Wire Wire Line
	13250 2750 13950 2750
Connection ~ 13250 2750
Wire Wire Line
	13950 2850 13600 2850
Connection ~ 13600 2850
Text GLabel 11250 2750 0    50   Input ~ 0
DRIVE_CS_ADC
$Comp
L Device:C C17
U 1 1 5E31A1E4
P 12250 1750
F 0 "C17" H 12365 1796 50  0000 L CNN
F 1 "1uF" H 12365 1705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 12288 1600 50  0001 C CNN
F 3 "~" H 12250 1750 50  0001 C CNN
F 4 "C0805C105K4RACTU" H 12250 1750 50  0001 C CNN "MPN"
	1    12250 1750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0151
U 1 1 5E31B40D
P 12250 2000
F 0 "#PWR0151" H 12250 1750 50  0001 C CNN
F 1 "GND" H 12255 1827 50  0000 C CNN
F 2 "" H 12250 2000 50  0001 C CNN
F 3 "" H 12250 2000 50  0001 C CNN
	1    12250 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	12250 1900 12250 2000
Wire Wire Line
	12250 1600 12250 1500
Wire Wire Line
	12250 1500 12000 1500
Wire Wire Line
	12000 1500 12000 2250
Wire Wire Line
	12000 1400 12000 1500
Connection ~ 12000 1500
Text GLabel 11250 2650 0    50   Input ~ 0
STEERING_POS_ADC
Text Notes 13200 3350 0    50   ~ 0
ADC - ADS1115IDGS\n\nI2C ADDRESS: 1001_000X (0x90)\nSHIFTED ADY: 0100_1000  (0x48)
Text GLabel 3000 4550 0    50   Input ~ 0
OUTA
Text GLabel 3000 5850 0    50   Input ~ 0
OUTB
Wire Wire Line
	3000 5850 3350 5850
Wire Wire Line
	3350 5050 3350 5000
Wire Wire Line
	3000 4550 3350 4550
Wire Wire Line
	3350 4550 3350 4650
Wire Wire Line
	3350 5750 3350 5850
Wire Wire Line
	3350 5450 3350 5400
Connection ~ 3350 5400
Wire Wire Line
	3350 5400 3350 5350
Connection ~ 3350 5000
Wire Wire Line
	3350 5000 3350 4950
$Comp
L Device:C C18
U 1 1 5E0D4836
P 3800 5200
F 0 "C18" H 3915 5246 50  0000 L CNN
F 1 "33nF" H 3915 5155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3838 5050 50  0001 C CNN
F 3 "~" H 3800 5200 50  0001 C CNN
F 4 "C0805C333K5RACTU" H 3800 5200 50  0001 C CNN "MPN"
	1    3800 5200
	1    0    0    -1  
$EndComp
$Comp
L AMC1301DWVR:AMC1301DWVR U7
U 1 1 5E0DF078
P 5000 5250
F 0 "U7" H 5000 5917 50  0000 C CNN
F 1 "AMC1301DWVR" H 5000 5826 50  0000 C CNN
F 2 "libs:SOIC127P1150X280-8N" H 5000 5250 50  0001 L BNN
F 3 "TEXAS INSTRUMENTS" H 5000 5250 50  0001 L BNN
F 4 "AMC1301DWVR" H 5000 5250 50  0001 L BNN "MPN"
	1    5000 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 5000 4150 5150
Wire Wire Line
	4150 5150 4400 5150
Wire Wire Line
	4150 5400 4150 5250
Wire Wire Line
	4150 5250 4400 5250
$Comp
L Device:C C21
U 1 1 5E0E2906
P 6700 5200
F 0 "C21" H 6815 5246 50  0000 L CNN
F 1 "33nF" H 6815 5155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6738 5050 50  0001 C CNN
F 3 "~" H 6700 5200 50  0001 C CNN
F 4 "C0805C333K5RACTU" H 6700 5200 50  0001 C CNN "MPN"
	1    6700 5200
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:NCP1117-5.0_SOT223 U5
U 1 1 5E0F108D
P 5000 2500
F 0 "U5" H 5000 2742 50  0000 C CNN
F 1 "NCP1117-5.0_SOT223" H 5000 2651 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 5000 2700 50  0001 C CNN
F 3 "http://www.onsemi.com/pub_link/Collateral/NCP1117-D.PDF" H 5100 2250 50  0001 C CNN
F 4 "NCP1117LPST50T3G" H 5000 2500 50  0001 C CNN "MPN"
	1    5000 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C20
U 1 1 5E0F7A8F
P 6000 2750
F 0 "C20" H 6115 2796 50  0000 L CNN
F 1 "10uF" H 6115 2705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6038 2600 50  0001 C CNN
F 3 "~" H 6000 2750 50  0001 C CNN
F 4 "C0805C106K4PACTU" H 6000 2750 50  0001 C CNN "MPN"
	1    6000 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C19
U 1 1 5E0F8204
P 4200 2750
F 0 "C19" H 4315 2796 50  0000 L CNN
F 1 "10uF" H 4315 2705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4238 2600 50  0001 C CNN
F 3 "~" H 4200 2750 50  0001 C CNN
F 4 "C0805C106K4PACTU" H 4200 2750 50  0001 C CNN "MPN"
	1    4200 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0152
U 1 1 5E0F9835
P 5000 3000
F 0 "#PWR0152" H 5000 2750 50  0001 C CNN
F 1 "GND" H 5005 2827 50  0000 C CNN
F 2 "" H 5000 3000 50  0001 C CNN
F 3 "" H 5000 3000 50  0001 C CNN
	1    5000 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0153
U 1 1 5E0F9E57
P 5750 5850
F 0 "#PWR0153" H 5750 5600 50  0001 C CNN
F 1 "GND" H 5755 5677 50  0000 C CNN
F 2 "" H 5750 5850 50  0001 C CNN
F 3 "" H 5750 5850 50  0001 C CNN
	1    5750 5850
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0154
U 1 1 5E0FA37A
P 6100 4500
F 0 "#PWR0154" H 6100 4350 50  0001 C CNN
F 1 "+5V" H 6115 4673 50  0000 C CNN
F 2 "" H 6100 4500 50  0001 C CNN
F 3 "" H 6100 4500 50  0001 C CNN
	1    6100 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R34
U 1 1 5E0FE7DE
P 3350 4800
F 0 "R34" H 3420 4846 50  0000 L CNN
F 1 "10k" H 3420 4755 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3280 4800 50  0001 C CNN
F 3 "~" H 3350 4800 50  0001 C CNN
F 4 "RC0603FR-0710KL" H 3350 4800 50  0001 C CNN "MPN"
	1    3350 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R36
U 1 1 5E0FF0BF
P 3350 5600
F 0 "R36" H 3420 5646 50  0000 L CNN
F 1 "10k" H 3420 5555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3280 5600 50  0001 C CNN
F 3 "~" H 3350 5600 50  0001 C CNN
F 4 "RC0603FR-0710KL" H 3350 5600 50  0001 C CNN "MPN"
	1    3350 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R35
U 1 1 5E0FF790
P 3350 5200
F 0 "R35" H 3420 5246 50  0000 L CNN
F 1 "470R" H 3420 5155 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3280 5200 50  0001 C CNN
F 3 "~" H 3350 5200 50  0001 C CNN
F 4 "ESR03EZPJ471" H 3350 5200 50  0001 C CNN "MPN"
	1    3350 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 5650 5750 5650
Wire Wire Line
	5750 5650 5750 5750
Wire Wire Line
	5600 5750 5750 5750
Connection ~ 5750 5750
Wire Wire Line
	5750 5750 5750 5850
Wire Wire Line
	6700 5050 6700 5000
Wire Wire Line
	6700 5350 6700 5400
$Comp
L power:+5VA #PWR0155
U 1 1 5E10AA62
P 6000 2400
F 0 "#PWR0155" H 6000 2250 50  0001 C CNN
F 1 "+5VA" H 6015 2573 50  0000 C CNN
F 2 "" H 6000 2400 50  0001 C CNN
F 3 "" H 6000 2400 50  0001 C CNN
	1    6000 2400
	1    0    0    -1  
$EndComp
$Comp
L power:+5VA #PWR0156
U 1 1 5E10AFB4
P 5750 4500
F 0 "#PWR0156" H 5750 4350 50  0001 C CNN
F 1 "+5VA" H 5765 4673 50  0000 C CNN
F 2 "" H 5750 4500 50  0001 C CNN
F 3 "" H 5750 4500 50  0001 C CNN
	1    5750 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 5150 6450 5000
Wire Wire Line
	6450 5000 6700 5000
Wire Wire Line
	5600 5150 6450 5150
Wire Wire Line
	6700 5400 6450 5400
Wire Wire Line
	6450 5400 6450 5250
Wire Wire Line
	5600 5250 6450 5250
Wire Wire Line
	5600 4950 6100 4950
Wire Wire Line
	5600 4850 5750 4850
$Comp
L power:GND #PWR0157
U 1 1 5E119BAA
P 6000 3000
F 0 "#PWR0157" H 6000 2750 50  0001 C CNN
F 1 "GND" H 6005 2827 50  0000 C CNN
F 2 "" H 6000 3000 50  0001 C CNN
F 3 "" H 6000 3000 50  0001 C CNN
	1    6000 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0158
U 1 1 5E119F02
P 4200 3000
F 0 "#PWR0158" H 4200 2750 50  0001 C CNN
F 1 "GND" H 4205 2827 50  0000 C CNN
F 2 "" H 4200 3000 50  0001 C CNN
F 3 "" H 4200 3000 50  0001 C CNN
	1    4200 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 2400 4200 2500
Wire Wire Line
	4200 2500 4700 2500
Connection ~ 4200 2500
Wire Wire Line
	4200 2500 4200 2600
Wire Wire Line
	5300 2500 6000 2500
Wire Wire Line
	6000 2500 6000 2600
Wire Wire Line
	6000 2400 6000 2500
Connection ~ 6000 2500
Wire Wire Line
	5000 2800 5000 3000
Wire Wire Line
	4200 2900 4200 3000
Wire Wire Line
	6000 2900 6000 3000
Wire Wire Line
	3350 5000 3800 5000
Wire Wire Line
	3350 5400 3800 5400
Wire Wire Line
	3800 5050 3800 5000
Connection ~ 3800 5000
Wire Wire Line
	3800 5000 4150 5000
Wire Wire Line
	3800 5350 3800 5400
Connection ~ 3800 5400
Wire Wire Line
	3800 5400 4150 5400
Wire Wire Line
	7200 5400 6700 5400
Connection ~ 6700 5400
Wire Wire Line
	7200 5000 6700 5000
Connection ~ 6700 5000
Text GLabel 7200 5000 2    50   Input ~ 0
VMOTOR_P_ADC
Text GLabel 7200 5400 2    50   Input ~ 0
VMOTOR_N_ADC
Text GLabel 11250 2850 0    50   Input ~ 0
VMOTOR_P_ADC
Text GLabel 11250 2950 0    50   Input ~ 0
VMOTOR_N_ADC
Wire Wire Line
	11250 2650 11600 2650
Wire Wire Line
	11600 2750 11250 2750
Wire Wire Line
	11250 2850 11600 2850
Wire Wire Line
	11600 2950 11250 2950
$Comp
L power:VBUS #PWR0159
U 1 1 5E14FBDC
P 4200 2400
F 0 "#PWR0159" H 4200 2250 50  0001 C CNN
F 1 "VBUS" H 4215 2573 50  0000 C CNN
F 2 "" H 4200 2400 50  0001 C CNN
F 3 "" H 4200 2400 50  0001 C CNN
	1    4200 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4500 5750 4850
Wire Wire Line
	6100 4500 6100 4950
$Comp
L Sensor:INA260 U8
U 1 1 5E17C722
P 12000 5300
F 0 "U8" H 12250 5800 50  0000 C CNN
F 1 "INA260" H 12350 5700 50  0000 C CNN
F 2 "Package_SO:TSSOP-16_4.4x5mm_P0.65mm" H 12000 4700 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ina260.pdf" H 12000 5200 50  0001 C CNN
F 4 "INA260AIPW" H 12000 5300 50  0001 C CNN "MPN"
	1    12000 5300
	1    0    0    -1  
$EndComp
NoConn ~ 12400 5100
NoConn ~ 11600 5100
NoConn ~ 11600 5500
Text GLabel 11400 5200 0    50   Input ~ 0
VMOTOR_BAT
Wire Wire Line
	11400 5200 11600 5200
$Comp
L power:GND #PWR0160
U 1 1 5E183F5A
P 12000 5850
F 0 "#PWR0160" H 12000 5600 50  0001 C CNN
F 1 "GND" H 12005 5677 50  0000 C CNN
F 2 "" H 12000 5850 50  0001 C CNN
F 3 "" H 12000 5850 50  0001 C CNN
	1    12000 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	12000 5700 12000 5850
Text GLabel 13500 5200 2    50   BiDi ~ 0
SCL
Text GLabel 13500 5300 2    50   BiDi ~ 0
SDA
$Comp
L Device:R R39
U 1 1 5E188E95
P 13150 4950
F 0 "R39" H 13220 4996 50  0000 L CNN
F 1 "10k" H 13220 4905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 13080 4950 50  0001 C CNN
F 3 "~" H 13150 4950 50  0001 C CNN
F 4 "RC0603FR-0710KL" H 13150 4950 50  0001 C CNN "MPN"
	1    13150 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R37
U 1 1 5E1896C6
P 12850 4950
F 0 "R37" H 12920 4996 50  0000 L CNN
F 1 "10k" H 12920 4905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 12780 4950 50  0001 C CNN
F 3 "~" H 12850 4950 50  0001 C CNN
F 4 "RC0603FR-0710KL" H 12850 4950 50  0001 C CNN "MPN"
	1    12850 4950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0161
U 1 1 5E189CE6
P 12850 4600
F 0 "#PWR0161" H 12850 4450 50  0001 C CNN
F 1 "+5V" H 12865 4773 50  0000 C CNN
F 2 "" H 12850 4600 50  0001 C CNN
F 3 "" H 12850 4600 50  0001 C CNN
	1    12850 4600
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0162
U 1 1 5E18A217
P 12000 4600
F 0 "#PWR0162" H 12000 4450 50  0001 C CNN
F 1 "+5V" H 12015 4773 50  0000 C CNN
F 2 "" H 12000 4600 50  0001 C CNN
F 3 "" H 12000 4600 50  0001 C CNN
	1    12000 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	12850 4700 13150 4700
Wire Wire Line
	13150 4700 13150 4800
Connection ~ 12850 4700
Wire Wire Line
	12850 4700 12850 4800
Wire Wire Line
	12400 5200 12500 5200
Wire Wire Line
	12850 5100 12850 5200
Connection ~ 12850 5200
Wire Wire Line
	12850 5200 13500 5200
Wire Wire Line
	13150 5100 13150 5300
Connection ~ 13150 5300
Wire Wire Line
	13150 5300 13500 5300
Wire Wire Line
	12850 4600 12850 4700
Wire Wire Line
	12000 4600 12000 4900
$Comp
L Sensor:INA260 U9
U 1 1 5E1AA0FE
P 12000 7250
F 0 "U9" H 12250 7750 50  0000 C CNN
F 1 "INA260" H 12350 7650 50  0000 C CNN
F 2 "Package_SO:TSSOP-16_4.4x5mm_P0.65mm" H 12000 6650 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ina260.pdf" H 12000 7150 50  0001 C CNN
F 4 "INA260AIPW" H 12000 7250 50  0001 C CNN "MPN"
	1    12000 7250
	1    0    0    -1  
$EndComp
NoConn ~ 12400 7050
NoConn ~ 11600 7050
NoConn ~ 11600 7450
Text GLabel 11400 7150 0    50   Input ~ 0
VLOGIC_BAT
Wire Wire Line
	11400 7150 11600 7150
$Comp
L power:GND #PWR0163
U 1 1 5E1AA109
P 12000 7800
F 0 "#PWR0163" H 12000 7550 50  0001 C CNN
F 1 "GND" H 12005 7627 50  0000 C CNN
F 2 "" H 12000 7800 50  0001 C CNN
F 3 "" H 12000 7800 50  0001 C CNN
	1    12000 7800
	1    0    0    -1  
$EndComp
Wire Wire Line
	12000 7650 12000 7800
Text GLabel 13500 7150 2    50   BiDi ~ 0
SCL
Text GLabel 13500 7250 2    50   BiDi ~ 0
SDA
$Comp
L Device:R R40
U 1 1 5E1AA113
P 13150 6900
F 0 "R40" H 13220 6946 50  0000 L CNN
F 1 "10k" H 13220 6855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 13080 6900 50  0001 C CNN
F 3 "~" H 13150 6900 50  0001 C CNN
F 4 "RC0603FR-0710KL" H 13150 6900 50  0001 C CNN "MPN"
	1    13150 6900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R38
U 1 1 5E1AA11A
P 12850 6900
F 0 "R38" H 12920 6946 50  0000 L CNN
F 1 "10k" H 12920 6855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 12780 6900 50  0001 C CNN
F 3 "~" H 12850 6900 50  0001 C CNN
F 4 "RC0603FR-0710KL" H 12850 6900 50  0001 C CNN "MPN"
	1    12850 6900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0164
U 1 1 5E1AA120
P 12850 6550
F 0 "#PWR0164" H 12850 6400 50  0001 C CNN
F 1 "+5V" H 12865 6723 50  0000 C CNN
F 2 "" H 12850 6550 50  0001 C CNN
F 3 "" H 12850 6550 50  0001 C CNN
	1    12850 6550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0165
U 1 1 5E1AA126
P 12000 6550
F 0 "#PWR0165" H 12000 6400 50  0001 C CNN
F 1 "+5V" H 12015 6723 50  0000 C CNN
F 2 "" H 12000 6550 50  0001 C CNN
F 3 "" H 12000 6550 50  0001 C CNN
	1    12000 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	12850 6650 13150 6650
Wire Wire Line
	13150 6650 13150 6750
Connection ~ 12850 6650
Wire Wire Line
	12850 6650 12850 6750
Wire Wire Line
	12400 7150 12850 7150
Wire Wire Line
	12400 7250 12550 7250
Wire Wire Line
	12850 7050 12850 7150
Connection ~ 12850 7150
Wire Wire Line
	12850 7150 13500 7150
Wire Wire Line
	13150 7050 13150 7250
Connection ~ 13150 7250
Wire Wire Line
	13150 7250 13500 7250
Wire Wire Line
	12850 6550 12850 6650
Wire Wire Line
	12000 6550 12000 6850
Wire Wire Line
	12400 7450 12550 7450
Wire Wire Line
	12400 5400 12500 5400
Wire Wire Line
	12500 5400 12500 5200
Connection ~ 12500 5200
Wire Wire Line
	12500 5200 12850 5200
Text Notes 13200 5800 0    50   ~ 0
VMOTOR BATTERY SENSE - INA260\n\nI2C ADDRESS: 1001_111X (0x9E)\nSHIFTED ADY: 0100_1111  (0x4F)
Wire Wire Line
	12400 5300 13150 5300
Wire Wire Line
	12500 5400 12500 5500
Wire Wire Line
	12500 5500 12400 5500
Connection ~ 12500 5400
Wire Wire Line
	12400 7350 12550 7350
Wire Wire Line
	12550 7350 12550 7250
Connection ~ 12550 7250
Wire Wire Line
	12550 7250 13150 7250
Wire Wire Line
	12550 7450 12550 7350
Connection ~ 12550 7350
Text Notes 13200 7800 0    50   ~ 0
VLOGIC BATTERY SENSE - INA260\n\nI2C ADDRESS: 1001_010X (0x94)\nSHIFTED ADY: 0100_1010  (0x4A)
$Comp
L Device:C C23
U 1 1 5E1FBBA4
P 14500 6900
F 0 "C23" H 14615 6946 50  0000 L CNN
F 1 "1uF" H 14615 6855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 14538 6750 50  0001 C CNN
F 3 "~" H 14500 6900 50  0001 C CNN
F 4 "C0805C105K4RACTU" H 14500 6900 50  0001 C CNN "MPN"
	1    14500 6900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C22
U 1 1 5E200723
P 14500 4950
F 0 "C22" H 14615 4996 50  0000 L CNN
F 1 "1uF" H 14615 4905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 14538 4800 50  0001 C CNN
F 3 "~" H 14500 4950 50  0001 C CNN
F 4 "C0805C105K4RACTU" H 14500 4950 50  0001 C CNN "MPN"
	1    14500 4950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0166
U 1 1 5E201431
P 14500 6550
F 0 "#PWR0166" H 14500 6400 50  0001 C CNN
F 1 "+5V" H 14515 6723 50  0000 C CNN
F 2 "" H 14500 6550 50  0001 C CNN
F 3 "" H 14500 6550 50  0001 C CNN
	1    14500 6550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0167
U 1 1 5E201CD4
P 14500 4600
F 0 "#PWR0167" H 14500 4450 50  0001 C CNN
F 1 "+5V" H 14515 4773 50  0000 C CNN
F 2 "" H 14500 4600 50  0001 C CNN
F 3 "" H 14500 4600 50  0001 C CNN
	1    14500 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0168
U 1 1 5E202022
P 14500 5200
F 0 "#PWR0168" H 14500 4950 50  0001 C CNN
F 1 "GND" H 14505 5027 50  0000 C CNN
F 2 "" H 14500 5200 50  0001 C CNN
F 3 "" H 14500 5200 50  0001 C CNN
	1    14500 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0169
U 1 1 5E202545
P 14500 7150
F 0 "#PWR0169" H 14500 6900 50  0001 C CNN
F 1 "GND" H 14505 6977 50  0000 C CNN
F 2 "" H 14500 7150 50  0001 C CNN
F 3 "" H 14500 7150 50  0001 C CNN
	1    14500 7150
	1    0    0    -1  
$EndComp
Wire Wire Line
	14500 4600 14500 4800
Wire Wire Line
	14500 5100 14500 5200
Wire Wire Line
	14500 6550 14500 6750
Wire Wire Line
	14500 7050 14500 7150
Text Notes 1300 6250 0    50   ~ 0
FOR REV B:  Replace R35 with a 2K ohm resistor and add two 1K ohm resistors from each end of R35 to ground.
$EndSCHEMATC
