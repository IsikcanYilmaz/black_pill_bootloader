
from classes import *
from common import *

ANIMATION_UPDATE_FREQUENCY = 1
ANIMATION_UPDATE_PERIOD_MS = int(100 / ANIMATION_UPDATE_FREQUENCY)

class RandomTriangleMetaData:
    def __init__(self):
        self.offTime = 0
        self.decrementRate = 1

class RandomTriangle:
    def __init__(self, panelsPtr):
        self.testx = self.testy = 0
        self.count = 0
        self.cycleCount = 0
        self.randr = randint(0, 255)
        self.randg = randint(0, 255)
        self.randb = randint(0, 255)
        self.panelsPtr = panelsPtr
        self.panelMetadata = []
        for p in range(0, NUM_PANELS):
            panelArr = []
            for y in range(0, PANEL_PIXEL_WIDTH):
                row = []
                for x in range(0, PANEL_PIXEL_WIDTH):
                    m = RandomTriangleMetaData()
                    m.offTime = randint(0, 1000)
                    row.append(m)
                panelArr.append(row)
            self.panelMetadata.append(panelArr)

    def start(self):
        pygame.time.set_timer(UPDATE_EVENT, ANIMATION_UPDATE_PERIOD_MS)

    def update(self):
        for panelIdx, panel in enumerate(self.panelsPtr):
            for x in range(0, PANEL_PIXEL_WIDTH):
                for y in range(0, PANEL_PIXEL_WIDTH):
                    if (self.testx >= x):
                        if (panelIdx == TOP and self.cycleCount % 2):
                            pix = panel.getPixelByCoords(PANEL_PIXEL_WIDTH - 1 - x, y, True)
                        else:
                            pix = panel.getPixelByCoords(x, y, True)
                        pix.r = self.randr
                        pix.g = self.randg
                        pix.b = self.randb
        self.testx += 1
        if (self.testx > 3):
            self.testx = 0
            self.cycleCount += 1
            self.randr = randint(0, 255)
            self.randg = randint(0, 255)
            self.randb = randint(0, 255)
