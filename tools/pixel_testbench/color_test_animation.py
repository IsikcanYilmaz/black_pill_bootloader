
from classes import *
from common import *

ANIMATION_UPDATE_FREQUENCY = 10
ANIMATION_UPDATE_PERIOD_MS = int(100 / ANIMATION_UPDATE_FREQUENCY)

class ColorTestMetaData:
    def __init__(self):
        self.offTime = 0
        self.decrementRate = 1

class ColorTest:
    def __init__(self, panelsPtr):
        self.r = self.g = self.b = 0

        self.h = 0
        self.s = 50
        self.v = 100

        self.addv = True
        self.addh = True
        self.adds = True

        self.ratev = 0
        self.rateh = 10
        self.rates = 0

        self.panelsPtr = panelsPtr
        self.panelMetadata = []
        for p in range(0, NUM_PANELS):
            panelArr = []
            for y in range(0, PANEL_PIXEL_WIDTH):
                row = []
                for x in range(0, PANEL_PIXEL_WIDTH):
                    m = ColorTestMetaData()
                    m.offTime = randint(0, 1000)
                    row.append(m)
                panelArr.append(row)
            self.panelMetadata.append(panelArr)

    def start(self):
        pygame.time.set_timer(UPDATE_EVENT, ANIMATION_UPDATE_PERIOD_MS)

    def stop(self):
        pygame.time.set_timer(UPDATE_EVENT, 0)

    def update(self):
        for panelIdx, panel in enumerate(self.panelsPtr):
            for x in range(0, PANEL_PIXEL_WIDTH):
                for y in range(0, PANEL_PIXEL_WIDTH):
                    pix = panel.getPixelByCoords(x, y)
                    (rr, gg, bb) = hls_to_rgb(self.h / 359, self.s / 100,  self.v / 100)
                    print(rr, gg, bb)
                    (pix.r, pix.g, pix.b) = (int(rr * 255), int(gg * 255), int(bb * 255))
        print(pix, " | " , self.h, self.s, self.v)

        if (self.addv):
            self.v += self.ratev
        else:
            self.v -= self.ratev
        if (self.v >= 100 or self.v <= 0):
            self.addv = not self.addv

        if (self.addh):
            self.h += self.rateh
        else:
            self.h -= self.rateh
        if (self.h >= 359 or self.h <= 0):
            self.addh = not self.addh

        if (self.adds):
            self.s += self.rates
        else:
            self.s -= self.rates
        if (self.s >= 100 or self.s <= 0):
            self.adds = not self.adds
