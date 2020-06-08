
from classes import *
from common import *

ANIMATION_UPDATE_FREQUENCY = 100
ANIMATION_UPDATE_PERIOD_MS = int(100 / ANIMATION_UPDATE_FREQUENCY)

class RandomBlinkMetaData:
    def __init__(self):
        self.offTime = 0
        self.decrementRate = 1

class RandomBlink:
    def __init__(self, panelsPtr):
        self.count = 0
        self.panelsPtr = panelsPtr
        self.panelMetadata = []
        for p in range(0, NUM_PANELS):
            panelArr = []
            for y in range(0, PANEL_PIXEL_WIDTH):
                row = []
                for x in range(0, PANEL_PIXEL_WIDTH):
                    m = RandomBlinkMetaData()
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
                    pixel = panel.getPixelByCoords(x, y)
                    metadata = self.panelMetadata[panelIdx][y][x]
                    if (pixel.r == pixel.g == pixel.b == 0 and
                        metadata.offTime == 0):
                        if (self.count % 200 < 100):
                            pixel.r = 0
                            pixel.g = randint(200, 255)
                            pixel.b = randint(200, 255)
                        else:
                            pixel.r = randint(200, 255)
                            pixel.g = 0
                            pixel.b = randint(200, 255)
                        metadata.offTime = randint(0, 1000)
                    else:
                        pixel.r -= metadata.decrementRate if pixel.r > 0 else 0
                        pixel.g -= metadata.decrementRate if pixel.g > 0 else 0
                        pixel.b -= metadata.decrementRate if pixel.b > 0 else 0
                        metadata.offTime -= 1 if metadata.offTime > 0 else 0
        self.count += 1

