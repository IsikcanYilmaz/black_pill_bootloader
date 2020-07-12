#!/usr/bin/python3

import pygame
from random_blink_animation import *
from random_triangle_animation import *
from color_test_animation import *
from random_shape_animation import *
from classes import *
from random import randint
from common import *

# Panels that make up the structure, i.e. the cube
panels = []

# Initialize animation classes
randomBlink = RandomBlink(panels)
randomTriangle = RandomTriangle(panels)
colorTest = ColorTest(panels)
randomShape = RandomShape(panels)

currentAnimation = randomShape

# Rotate panels according to their locations
ROTATE_ENABLED = True

# LED Manager variables
updateCount = 0

def getRotatedPanel(side):
    if (not ROTATE_ENABLED):
        return panels[side].surf

    if (side == NORTH):
        return pygame.transform.rotate(panels[side].surf, 180)
    if (side == EAST):
        return pygame.transform.rotate(panels[side].surf, 90)
    if (side == WEST):
        return pygame.transform.rotate(panels[side].surf, 270)
    else:
        return panels[side].surf

def initBoard(screen):
    for side in range(0, 5):
        p = Panel(4, 4, side)
        panels.append(p)
    currentAnimation.start()

def drawBoard(screen):
    screen.fill((0, 0, 0))
    X_OFFSET = 10
    Y_OFFSET = 10

    # Draw the panels on
    for side in range(0, 5):
        panels[side].renderPanel()

    # Place the panels on our board
    screen.blit(getRotatedPanel(NORTH), (X_OFFSET + PANEL_DISP_WIDTH, Y_OFFSET + 0))
    screen.blit(getRotatedPanel(WEST),  (X_OFFSET + 0, Y_OFFSET + PANEL_DISP_HEIGHT))
    screen.blit(getRotatedPanel(TOP),   (X_OFFSET + PANEL_DISP_WIDTH, Y_OFFSET + PANEL_DISP_HEIGHT))
    screen.blit(getRotatedPanel(EAST),  (X_OFFSET + PANEL_DISP_WIDTH * 2, Y_OFFSET + PANEL_DISP_HEIGHT))
    screen.blit(getRotatedPanel(SOUTH), (X_OFFSET + PANEL_DISP_WIDTH, Y_OFFSET + PANEL_DISP_HEIGHT * 2))

    # Render
    pygame.display.flip()

def stopUpdates():
    pygame.time.set_timer(UPDATE_EVENT, 0)

def updateBoard():
    currentAnimation.update()

def main():
    global updateCount
    pygame.init()
    screen = pygame.display.set_mode([WINDOW_WIDTH, WINDOW_HEIGHT])
    initBoard(screen)
    drawBoard(screen)
    running = True
    while running:
        for event in pygame.event.get():
            #print(event)

            if event.type == pygame.QUIT:
                running = False

            elif event.type == UPDATE_EVENT:
                updateBoard()
                drawBoard(screen)
                updateCount += 1

            else:
                print(event, event.type)


    pygame.quit()

if __name__ == "__main__":
    main()
