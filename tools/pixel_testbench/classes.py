#!/usr/bin/python3
import time
import pygame
from common import *
from random import randint

PANEL_DISP_WIDTH = 105
PANEL_DISP_HEIGHT = 105

PIXEL_DISP_WIDTH = 20
PIXEL_DISP_HEIGHT = 20

PANEL_DISP_SIZE = (PANEL_DISP_WIDTH, PANEL_DISP_HEIGHT)
PIXEL_DISP_SIZE = (PIXEL_DISP_WIDTH, PIXEL_DISP_HEIGHT)

class Pixel:
    def __init__(self, r = 0, g = 0, b = 0):
        self.surf = pygame.Surface(PIXEL_DISP_SIZE)
        self.r = r
        self.g = g
        self.b = b
        self.surf.fill((self.r, self.g, self.b))

    def renderPixel(self):
        self.surf.fill((self.r, self.g, self.b))

    def __str__(self):
        return "%03d %03d %03d" % (self.r, self.g, self.b)

class Panel:
    def __init__(self, sizex, sizey, location):
        self.surf = pygame.Surface(PANEL_DISP_SIZE)
        self.pixels = []
        self.location = location
        self.sizex = sizex
        self.sizey = sizey
        for y in range(0, sizey):
            row = []
            for x in range(0, sizex):
                row.append(Pixel(0,0,0))
            self.pixels.append(row)

    def renderPanel(self):
        self.surf.fill((40, 40, 0))
        for y in range(0, self.sizey):
            ycoord = y * PIXEL_DISP_HEIGHT + (5 + y * 5)
            for x in range(0, self.sizex):
                xcoord = x * PIXEL_DISP_WIDTH + (5 + x * 5)
                pix = self.getPixelByCoords(x, y)
                pix.renderPixel()
                self.surf.blit(pix.surf, (xcoord, ycoord))

    def getPixelByCoords(self, x, y, debug=False):
        # TODO # Theres some wied bug here that had me do this weird indexing.
        # this works now, somehow but need to fix it
        # i.e. the need of xIdx and yIdx. its weird. for this hack to work
        # one needs to pass True for debug
        if (debug):
            xIdx = self.sizex - 1 - x
            yIdx = y
            return self.pixels[yIdx][xIdx]
        else:
            yIdx = self.sizey - 1 - y
            xIdx = self.sizex - 1 - x
        return self.pixels[yIdx][xIdx]

    def printPanel(self):
        for y in range(0, self.sizey):
            str = ""
            for x in range(0, self.sizex):
                str += ("[%s] " % (self.pixels[y][x]))
            print(str)



if __name__ == "__main__":
    p = Panel(4, 4, None)
    p.printPanel()
