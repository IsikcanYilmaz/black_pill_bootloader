EESchema Schematic File Version 4
LIBS:light_thing_hw-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L power:GND #PWR013
U 1 1 5EC8FB9C
P 1950 2100
F 0 "#PWR013" H 1950 1850 50  0001 C CNN
F 1 "GND" H 1800 2050 50  0000 C CNN
F 2 "" H 1950 2100 50  0001 C CNN
F 3 "" H 1950 2100 50  0001 C CNN
	1    1950 2100
	1    0    0    -1  
$EndComp
Text GLabel 2050 2500 0    50   Input ~ 0
BT_SWCLK
Text GLabel 2050 2350 0    50   Input ~ 0
BT_SWDIO
Text GLabel 4500 2950 2    50   Input ~ 0
BT_RST
Text Notes 7050 6900 0    185  ~ 0
FANSTEL BT832 BLE MODULE
$Comp
L Fanstel-kicad-library-cache:BC832-V1 U3
U 1 1 5EC29001
P 3300 2800
F 0 "U3" H 3275 4004 45  0000 C CNN
F 1 "BC832-V1" H 3300 3600 45  0000 C CNN
F 2 "Fanstel_modules:BC832-BC832" H 3330 2950 20  0001 C CNN
F 3 "" H 3300 2800 60  0001 C CNN
	1    3300 2800
	1    0    0    -1  
$EndComp
Text GLabel 2050 2950 0    50   Input ~ 0
BT_TX
Text GLabel 2050 3100 0    50   Input ~ 0
BT_RX
Wire Wire Line
	2050 2050 1950 2050
Wire Wire Line
	1950 2050 1950 2100
$EndSCHEMATC
