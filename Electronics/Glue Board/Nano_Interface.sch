EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr B 17000 11000
encoding utf-8
Sheet 5 5
Title "Robocar, Pinky, Nano Interface"
Date "2019-12-28"
Rev "A"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_02x06_Odd_Even J4
U 1 1 5E1CE768
P 8300 5100
F 0 "J4" H 8350 5517 50  0000 C CNN
F 1 "Conn_02x06_Odd_Even" H 8350 5426 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x06_P2.54mm_Vertical" H 8300 5100 50  0001 C CNN
F 3 "~" H 8300 5100 50  0001 C CNN
F 4 "67997-112HLF" H 8300 5100 50  0001 C CNN "MPN"
	1    8300 5100
	1    0    0    -1  
$EndComp
Text GLabel 8850 4900 2    50   Input ~ 0
+5V
Text GLabel 8850 5000 2    50   Input ~ 0
+5V
Wire Wire Line
	8600 4900 8850 4900
Wire Wire Line
	8600 5000 8850 5000
Text GLabel 8850 5100 2    50   Input ~ 0
GND
Text GLabel 7800 5300 0    50   Input ~ 0
GND
Wire Wire Line
	8600 5100 8850 5100
Wire Wire Line
	7800 5300 8100 5300
Text GLabel 7800 5000 0    50   BiDi ~ 0
SDA
Text GLabel 7800 5100 0    50   BiDi ~ 0
SCL
Text GLabel 7800 5200 0    50   BiDi ~ 0
GPIO216
Text GLabel 7800 5400 0    50   BiDi ~ 0
GPIO50
Text GLabel 8850 5400 2    50   BiDi ~ 0
GPIO79
Wire Wire Line
	7800 5000 8100 5000
Wire Wire Line
	7800 5100 8100 5100
Wire Wire Line
	7800 5200 8100 5200
Wire Wire Line
	8100 5400 7800 5400
Wire Wire Line
	8600 5400 8850 5400
Text GLabel 8850 5200 2    50   Output ~ 0
UART2_TX
Text GLabel 8850 5300 2    50   Input ~ 0
UART2_RX
Wire Wire Line
	8600 5200 8850 5200
Wire Wire Line
	8850 5300 8600 5300
Text GLabel 8850 3500 2    50   BiDi ~ 0
SDA
Text GLabel 7800 3500 0    50   BiDi ~ 0
SCL
Text GLabel 7800 3400 0    50   Input ~ 0
GND
Text GLabel 7800 4900 0    50   Output ~ 0
+3.3V
Wire Wire Line
	8100 4900 7800 4900
$Comp
L Connector:Conn_01x04_Male J7
U 1 1 5E13F760
P 8200 4250
F 0 "J7" H 8308 4531 50  0000 C CNN
F 1 "Conn_01x04_Male" H 8308 4440 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 8200 4250 50  0001 C CNN
F 3 "~" H 8200 4250 50  0001 C CNN
	1    8200 4250
	1    0    0    -1  
$EndComp
Text GLabel 8850 4150 2    50   Input ~ 0
+5V
Text GLabel 8850 4250 2    50   Input ~ 0
GND
Text GLabel 8850 3600 2    50   BiDi ~ 0
GPIO50
Text GLabel 7800 3600 0    50   BiDi ~ 0
GPIO79
Text GLabel 8850 3400 2    50   Input ~ 0
+3.3V
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J8
U 1 1 5E141539
P 8400 3500
F 0 "J8" H 8450 3175 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 8450 3266 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x03_P2.54mm_Vertical" H 8400 3500 50  0001 C CNN
F 3 "~" H 8400 3500 50  0001 C CNN
F 4 "SFH11-NBPC-D03-ST-BK" H 8400 3500 50  0001 C CNN "MPN"
	1    8400 3500
	-1   0    0    1   
$EndComp
Wire Wire Line
	7800 3400 8100 3400
Wire Wire Line
	8100 3500 7800 3500
Wire Wire Line
	7800 3600 8100 3600
Wire Wire Line
	8600 3400 8850 3400
Wire Wire Line
	8850 3500 8600 3500
Wire Wire Line
	8600 3600 8850 3600
Wire Wire Line
	8400 4150 8850 4150
Wire Wire Line
	8850 4250 8400 4250
Text GLabel 8850 4350 2    50   BiDi ~ 0
SDA
Text GLabel 8850 4450 2    50   BiDi ~ 0
SCL
Wire Wire Line
	8850 4350 8400 4350
Wire Wire Line
	8400 4450 8850 4450
Text Notes 9350 3600 0    50   ~ 0
SHITTY ADD-ON\nV. 1.69BIS\nCOMPLIANT
$EndSCHEMATC
