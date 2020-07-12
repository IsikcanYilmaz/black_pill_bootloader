
from pygame.locals import *

# Main window dimensions
WINDOW_WIDTH  = 350
WINDOW_HEIGHT = 350

# Location enum
NORTH = 0
EAST  = 1
SOUTH = 2
WEST  = 3
TOP   = 4
NUM_PANELS = 5

# Adjacency enum
UP    = 0
RIGHT = 1
DOWN  = 2
LEFT  = 3

PANEL_PIXEL_WIDTH = 4
PIXEL_PER_PANEL = PANEL_PIXEL_WIDTH * PANEL_PIXEL_WIDTH

# Screen update rate in Hz
UPDATE_RATE      = 100
UPDATE_PERIOD_MS = int(1000/UPDATE_RATE)
UPDATE_EVENT     = USEREVENT+1
