EESchema Schematic File Version 4
LIBS:light_thing_hw-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Sensor_Motion:LSM6DS3 U?
U 1 1 5F1C10A5
P 6500 2300
F 0 "U?" H 7141 2346 50  0000 L CNN
F 1 "LSM6DS3" H 6400 2300 50  0000 L CNN
F 2 "Package_LGA:LGA-14_3x2.5mm_P0.5mm_LayoutBorder3x4y" H 6100 1600 50  0001 L CNN
F 3 "www.st.com/resource/en/datasheet/lsm6ds3.pdf" H 6600 1650 50  0001 C CNN
	1    6500 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 2900 6500 3000
Wire Wire Line
	6500 3000 6600 3000
Wire Wire Line
	6600 2900 6600 3000
Connection ~ 6600 3000
Wire Wire Line
	6600 3000 6600 3050
$Comp
L power:GND #PWR?
U 1 1 5F1C10B1
P 6600 3050
F 0 "#PWR?" H 6600 2800 50  0001 C CNN
F 1 "GND" H 6605 2877 50  0000 C CNN
F 2 "" H 6600 3050 50  0001 C CNN
F 3 "" H 6600 3050 50  0001 C CNN
	1    6600 3050
	1    0    0    -1  
$EndComp
Text GLabel 5900 2400 0    60   Input ~ 0
I2C1_SDA
Text GLabel 5900 2500 0    60   Input ~ 0
I2C1_SCL
$Comp
L power:GND #PWR?
U 1 1 5F1C10B9
P 1100 1500
AR Path="/5EC768AC/5F1C10B9" Ref="#PWR?"  Part="1" 
AR Path="/5F1C10B9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1100 1250 50  0001 C CNN
F 1 "GND" H 950 1450 50  0000 C CNN
F 2 "" H 1100 1500 50  0001 C CNN
F 3 "" H 1100 1500 50  0001 C CNN
	1    1100 1500
	1    0    0    -1  
$EndComp
Text GLabel 1200 1900 0    50   Input ~ 0
BT_SWCLK
Text GLabel 1200 1750 0    50   Input ~ 0
BT_SWDIO
Text GLabel 3650 2350 2    50   Input ~ 0
BT_RST
$Comp
L Fanstel-kicad-library-cache:BC832-V1 U?
U 1 1 5F1C10C2
P 2450 2200
AR Path="/5EC768AC/5F1C10C2" Ref="U?"  Part="1" 
AR Path="/5F1C10C2" Ref="U?"  Part="1" 
F 0 "U?" H 2425 3404 45  0000 C CNN
F 1 "BC832-V1" H 2450 3000 45  0000 C CNN
F 2 "Fanstel_modules:BC832-BC832_3d1" H 2480 2350 20  0001 C CNN
F 3 "" H 2450 2200 60  0001 C CNN
	1    2450 2200
	1    0    0    -1  
$EndComp
Text GLabel 1200 2350 0    50   Input ~ 0
BT_TX
Text GLabel 1200 2500 0    50   Input ~ 0
BT_RX
Wire Wire Line
	1200 1450 1100 1450
Wire Wire Line
	1100 1450 1100 1500
Text GLabel 3650 1750 2    50   Input ~ 0
I2C2_SCL
Text GLabel 3650 1600 2    50   Input ~ 0
I2C2_SDA
$Comp
L Device:C_Small C?
U 1 1 5F1C10CF
P 6750 1500
F 0 "C?" H 6842 1546 50  0000 L CNN
F 1 "100nF" H 6842 1455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6750 1500 50  0001 C CNN
F 3 "~" H 6750 1500 50  0001 C CNN
	1    6750 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 1300 6600 1400
$Comp
L power:GND #PWR?
U 1 1 5F1C10D7
P 6750 1600
F 0 "#PWR?" H 6750 1350 50  0001 C CNN
F 1 "GND" H 6755 1427 50  0000 C CNN
F 2 "" H 6750 1600 50  0001 C CNN
F 3 "" H 6750 1600 50  0001 C CNN
	1    6750 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 1400 6600 1400
Connection ~ 6600 1400
Wire Wire Line
	6600 1400 6600 1700
Text GLabel 7100 2000 2    50   Input ~ 0
ACCEL_EXTI_1
Text GLabel 7100 2100 2    50   Input ~ 0
ACCEL_EXTI_2
Wire Wire Line
	6500 1700 6500 1400
Wire Wire Line
	6500 1400 6600 1400
$Comp
L power:GND #PWR?
U 1 1 5F1C10E4
P 5550 2100
F 0 "#PWR?" H 5550 1850 50  0001 C CNN
F 1 "GND" H 5400 2050 50  0000 C CNN
F 2 "" H 5550 2100 50  0001 C CNN
F 3 "" H 5550 2100 50  0001 C CNN
	1    5550 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 2200 5700 2200
Wire Wire Line
	5700 2200 5700 2100
Wire Wire Line
	5700 2000 5550 2000
Wire Wire Line
	5550 2000 5550 2100
Wire Wire Line
	5900 2000 5700 2000
Connection ~ 5700 2000
Connection ~ 5700 2100
Wire Wire Line
	5700 2100 5700 2000
Wire Wire Line
	5700 2100 5900 2100
Text Notes 9300 950  0    129  ~ 0
BLE AND ACCEL
Text GLabel 1200 2200 0    50   Input ~ 0
BT_RED_LED
NoConn ~ 1200 1600
NoConn ~ 1200 2050
NoConn ~ 3400 3300
NoConn ~ 3300 3300
NoConn ~ 3200 3300
NoConn ~ 3100 3300
NoConn ~ 3000 3300
NoConn ~ 2900 3300
NoConn ~ 2800 3300
NoConn ~ 2700 3300
NoConn ~ 2600 3300
NoConn ~ 2500 3300
NoConn ~ 2400 3300
NoConn ~ 2300 3300
NoConn ~ 2200 3300
NoConn ~ 2100 3300
NoConn ~ 2000 3300
NoConn ~ 1900 3300
NoConn ~ 1800 3300
NoConn ~ 1700 3300
NoConn ~ 1600 3300
NoConn ~ 1500 3300
NoConn ~ 5900 2600
Text GLabel 3650 1900 2    50   Input ~ 0
BT_GPIO_0
Text GLabel 3650 2050 2    50   Input ~ 0
BT_GPIO_1
Text GLabel 3650 2200 2    50   Input ~ 0
BT_GPIO_2
Text GLabel 3650 2500 2    50   Input ~ 0
BT_GPIO_3
Text GLabel 3650 1450 2    50   Input ~ 0
BLE_3V3_IN
Text GLabel 6600 1300 2    50   Input ~ 0
ACCEL_3V3_IN
$EndSCHEMATC
