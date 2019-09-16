#include "graphics.h"
#include "gba.h"
#include <stdio.h>

#include "images/stage1.h"
#include "images/stage2.h"
#include "images/stage3.h"
#include "images/stage3pass.h"
#include "images/stage4.h"

#include "images/samusL1.h"
#include "images/samusL2.h"
#include "images/samusL3.h"
#include "images/samusL4.h"
#include "images/samusL5.h"
#include "images/samusL6.h"
#include "images/samusR1.h"
#include "images/samusR2.h"
#include "images/samusR3.h"
#include "images/samusR4.h"
#include "images/samusR5.h"
#include "images/samusR6.h"

#include "images/samusSL1.h"
#include "images/samusSL2.h"
#include "images/samusSL3.h"
#include "images/samusSL4.h"
#include "images/samusSL5.h"
#include "images/samusSL6.h"
#include "images/samusSR1.h"
#include "images/samusSR2.h"
#include "images/samusSR3.h"
#include "images/samusSR4.h"
#include "images/samusSR5.h"
#include "images/samusSR6.h"

#include "images/orangecrawler.h"
// TA-TODO: Include any header files for images generated by nin10kit.
// Example for the provided garbage image:
//#include "images/garbage.h"

// TA-TODO: Add any draw/undraw functions for sub-elements of your app here.
// For example, for a snake game, you could have a drawSnake function
// or a drawFood function
//
// e.g.:
// static void drawSnake(Snake* snake);
// static void drawFood(Food* food);

// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    drawFullScreenImageDMA(stage1);
    // drawImageDMA(185, 58, 20, 25, samusL1);
    drawImageDMA(state -> samus.x, state -> samus.y, 20, 25, samusL1);
    // drawImageDMA(60, 114, 20, 20, orangecrawler);
    // drawImageDMA(100, 63, 20, 20, orangecrawler);
    // TODO: draw current score, health, and missiles
    // char buffer[20];
    // sprintf(buffer, "x: %d y: %d", (state -> samus.x), (state -> samus.y));
    // drawString(80, 140, buffer, MAGENTA);
    // UNUSED(state);
    // UNUSED(buffer);
}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame. E.g. in a Snake game, erase the Snake, the food & the score.
void undrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    // redraw the stage
    if (state -> currStage == 0) {
        drawFullScreenImageDMA(stage1);
    } else if (state -> currStage == 1) {
        drawFullScreenImageDMA(stage2);
    } else if (state -> currStage == 2) {
        //for the stage before the final stage, show that door is green if all enemies defeated
        if (state -> enemies == 0) {
            drawFullScreenImageDMA(stage3pass);
        } else {
            drawFullScreenImageDMA(stage3);
        }
    } else if (state -> currStage == 3) {
        drawFullScreenImageDMA(stage4);
    }

    // UNUSED(state);
}

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.

    // lines
    // drawRectDMA(98, 84, 120, 5, GREEN);
    // drawRectDMA(210, 0, 1, 100, GREEN);
    // drawRectDMA(0, 135 - SAMUS_HEIGHT, 240, 5, GREEN);
    // drawRectDMA(200, 0, 5, 160, MAGENTA);

    // draw crawlers
    if (state -> currStage == 1 && state -> stage[1].safe == 0) {
        drawImageDMA(state -> stage[1].crawlers[0].x, state -> stage[1].crawlers[0].y, CRAWLER_SIZE, CRAWLER_SIZE, orangecrawler);
        drawImageDMA(state -> stage[1].crawlers[1].x, state -> stage[1].crawlers[1].y, CRAWLER_SIZE, CRAWLER_SIZE, orangecrawler);
    }
    // draw bullets
    drawRectDMA(state -> bullets[0].x, state -> bullets[0].y, BULLET_LENGTH, BULLET_HEIGHT, YELLOW);
    drawRectDMA(state -> bullets[1].x, state -> bullets[1].y, BULLET_LENGTH, BULLET_HEIGHT, YELLOW);
    drawRectDMA(state -> bullets[2].x, state -> bullets[2].y, BULLET_LENGTH, BULLET_HEIGHT, MAGENTA);
    // draw new samus position and sprite
    if (state -> samus.shooting == 0) {
        switch(state -> samus.currLRunningSprite) {
            case -1:
            break;
            case 0:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusL1);
            break;
            case 1:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusL2);
            break;
            case 2:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusL3);
            break;
            case 3:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusL4);
            break;
            case 4:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusL5);
            break;
            case 5:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusL6);
            break;
        }
        switch(state -> samus.currRRunningSprite) {
            case -1:
            break;
            case 0:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusR1);
            break;
            case 1:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusR2);
            break;
            case 2:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusR3);
            break;
            case 3:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusR4);
            break;
            case 4:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusR5);
            break;
            case 5:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusR6);
            break;
        }
    } else {
        switch(state -> samus.currLShootingSprite) {
            case -1:
            break;
            case 0:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSL6);
            break;
            case 1:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSL5);
            break;
            case 2:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSL4);
            break;
            case 3:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSL3);
            break;
            case 4:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSL2);
            break;
            case 5:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSL1);
            break;
        }
        switch(state -> samus.currRShootingSprite) {
            case -1:
            break;
            case 0:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSR1);
            break;
            case 1:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSR2);
            break;
            case 2:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSR3);
            break;
            case 3:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSR4);
            break;
            case 4:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSR5);
            break;
            case 5:
            drawImageDMA(state -> samus.x, state -> samus.y, SAMUS_WIDTH, SAMUS_HEIGHT, samusSR6);
            break;
        }
    }

    // draw text
    char buffer[30];
    sprintf(buffer, "Health: %d Foes: %d", (state -> samus.health), (state -> enemies));
    drawString(70, 140, buffer, WHITE);
    UNUSED(state);
}
