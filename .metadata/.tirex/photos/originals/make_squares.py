#!/usr/bin/env python

import subprocess

FILES = ["msp-exp430fr2355.png", "msp-exp430g2et.png"]


def main():

    for filename in FILES:
        picture = filename.split('.')[0]
        subprocess.call(["convert", filename, "-thumbnail", "200x200^",
                         "-gravity", "center", "-extent", "200x200",
                         picture + "_square.png"])


if __name__ == '__main__':
    main()
