#!/usr/bin/python3

import serial
import argparse



def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("file", help="Firmware binary file to transfer")
    args = parser.parse_args()

if __name__ == "__main__":
    main()
