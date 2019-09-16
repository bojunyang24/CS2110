#include "logic.h"
#include <stdlib.h>
#include <stdio.h>

void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    // appState = (AppState*) malloc(sizeof(AppState));
    appState -> gameOver = 0;
    appState -> win = 0;
    appState -> points = 0;
    appState -> currStage = 0;
    appState -> reset = 0;
    appState -> enemies = 2;

    Bullet b0 = {2, 2, 0};
    Bullet b1 = {12, 2, 0};
    Bullet b2 = {22, 2, 0};
    appState -> bullets[0] = b0;
    appState -> bullets[1] = b1;
    appState -> bullets[2] = b2;

    // Samus sam = {100, 1, 5};
    // appState -> samus = (Samus*) malloc(sizeof(Samus));
    // appState -> samus = &sam;
    appState -> samus.health = 100;
    appState -> samus.currLRunningSprite = 0;
    appState -> samus.currRRunningSprite = -1;
    appState -> samus.missiles = 5;
    appState -> samus.x = 185;
    appState -> samus.y = 58;
    appState -> samus.runCounter = 0;
    appState -> samus.top = appState -> samus.y;
    appState -> samus.bottom = appState -> samus.y + SAMUS_HEIGHT;
    appState -> samus.left = appState -> samus.x;
    appState -> samus.right = appState -> samus.x + SAMUS_WIDTH;
    appState -> samus.fallFactor = 0;
    appState -> samus.jumpFactor = 0;
    appState -> samus.jumping = 0;
    appState -> samus.shooting = 0;
    appState -> samus.nextBullet = 0;
    Crawler stage1Yellow1 = {0, 0, 0, 0, 0};
    Crawler stage1Yellow2 = {0, 0, 0, 0, 0};
    appState -> stage[0].crawlers[0] = stage1Yellow1;
    appState -> stage[0].crawlers[1] = stage1Yellow2;
    appState -> stage[0].leftDoor = -1;
    appState -> stage[0].rightDoor = 0;
    appState -> stage[0].safe = 1;
    appState -> stage[0].leftWall = 0;
    appState -> stage[0].rightWall = 210;

    Crawler stage2Yellow1 = {0, 2, 1, 58, 117};
    Crawler stage2Yellow2 = {0, 5, 1, 122, 117};
    appState -> stage[1].crawlers[0] = stage2Yellow1;
    appState -> stage[1].crawlers[1] = stage2Yellow2;
    appState -> stage[1].leftDoor = -1;
    appState -> stage[1].rightDoor = -1;
    appState -> stage[1].safe = 0;
    appState -> stage[1].leftWall = 0;
    appState -> stage[1].rightWall = 240;

    Crawler stage3Yellow1 = {0, 0, 0, 0, 0};
    Crawler stage3Red1 = {0, 0, 0, 0, 0};
    appState -> stage[2].crawlers[0] = stage3Yellow1;
    appState -> stage[2].crawlers[1] = stage3Red1;
    appState -> stage[2].leftDoor = 0;
    appState -> stage[2].rightDoor = -1;
    appState -> stage[2].safe = 1;
    appState -> stage[2].leftWall = 40;
    appState -> stage[2].rightWall = 240;

    Crawler stage4Yellow1 = {0, 0, 0, 0, 0};
    Crawler stage4Yellow2 = {0, 0, 0, 0, 0};
    appState -> stage[3].crawlers[0] = stage4Yellow1;
    appState -> stage[3].crawlers[1] = stage4Yellow2;
    appState -> stage[3].leftDoor = -1;
    appState -> stage[3].rightDoor = 1;
    appState -> stage[3].safe = 1;
    appState -> stage[3].leftWall = 50;
    appState -> stage[3].rightWall = 223;
}

void clearBullets(AppState *state) {
    state -> bullets[0].x = 0;
    state -> bullets[0].y = 0;
    state -> bullets[1].x = 0;
    state -> bullets[1].y = 0;
    state -> bullets[2].x = 0;
    state -> bullets[2].y = 0;
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */

    AppState nextAppState = *currentAppState;

    // current stage
    // nextAppState.currStage = 0;
    // samus actions

    //gravity and collision for samus for each stage
    if (nextAppState.currStage == 0) {
        if (nextAppState.samus.x + SAMUS_WIDTH >= 98 && nextAppState.samus.x <= 218 && nextAppState.samus.y + SAMUS_HEIGHT < 84) {
            nextAppState.samus.fallFactor = nextAppState.samus.fallFactor + 1;
        } else if (nextAppState.samus.x < 100 && nextAppState.samus.y + SAMUS_HEIGHT < 135) {
            nextAppState.samus.fallFactor = nextAppState.samus.fallFactor + 1;
        } else {
            nextAppState.samus.fallFactor = 0;
        }
        if (nextAppState.samus.fallFactor > 0) {
            // nextAppState.samus.y = 135 - SAMUS_HEIGHT;
            if (nextAppState.samus.x < 98 && nextAppState.samus.y + nextAppState.samus.fallFactor + SAMUS_HEIGHT >= 135) {
                nextAppState.samus.y = 135 - SAMUS_HEIGHT;
            } else if (nextAppState.samus.x >= 98 && nextAppState.samus.y + nextAppState.samus.fallFactor + SAMUS_HEIGHT >= 84) {
                nextAppState.samus.y = 84 - SAMUS_HEIGHT;
            } else {
                nextAppState.samus.y = nextAppState.samus.y + nextAppState.samus.fallFactor;
            }
        }
    } else if (nextAppState.currStage == 1) {
        if (nextAppState.samus.y + SAMUS_HEIGHT < 135) {
            nextAppState.samus.fallFactor = nextAppState.samus.fallFactor + 1;
        } else {
            nextAppState.samus.fallFactor = 0;
        }
        if (nextAppState.samus.fallFactor > 0) {
            // nextAppState.samus.y = 135 - SAMUS_HEIGHT;
            if (nextAppState.samus.y + nextAppState.samus.fallFactor + SAMUS_HEIGHT >= 135) {
                nextAppState.samus.y = 135 - SAMUS_HEIGHT;
            } else {
                nextAppState.samus.y = nextAppState.samus.y + nextAppState.samus.fallFactor;
            }
        }
    } else if (nextAppState.currStage == 2) {
        if (nextAppState.samus.x <= 142 && nextAppState.samus.x >= 40 && nextAppState.samus.y + SAMUS_HEIGHT < 84) {
            nextAppState.samus.fallFactor = nextAppState.samus.fallFactor + 1;
        } else if (nextAppState.samus.x > 142 && nextAppState.samus.y + SAMUS_HEIGHT < 135) {
            nextAppState.samus.fallFactor = nextAppState.samus.fallFactor + 1;
        } else {
            nextAppState.samus.fallFactor = 0;
        }
        if (nextAppState.samus.fallFactor > 0) {
            // nextAppState.samus.y = 135 - SAMUS_HEIGHT;
            if (nextAppState.samus.x > 142 && nextAppState.samus.y + nextAppState.samus.fallFactor + SAMUS_HEIGHT >= 135) {
                nextAppState.samus.y = 135 - SAMUS_HEIGHT;
            } else if (nextAppState.samus.x <= 142 && nextAppState.samus.y + nextAppState.samus.fallFactor + SAMUS_HEIGHT >= 84) {
                nextAppState.samus.y = 84 - SAMUS_HEIGHT;
            } else {
                nextAppState.samus.y = nextAppState.samus.y + nextAppState.samus.fallFactor;
            }
        }
    } else if (nextAppState.currStage == 3) {
        //end stage gravity and collision
        if (nextAppState.samus.x < 157 && nextAppState.samus.y + SAMUS_HEIGHT < 54) {
            nextAppState.samus.fallFactor = nextAppState.samus.fallFactor + 1;
        } else if (nextAppState.samus.x <= 223 && nextAppState.samus.x >= 157 && nextAppState.samus.y + SAMUS_HEIGHT < 84) {
            nextAppState.samus.fallFactor = nextAppState.samus.fallFactor + 1;
        } else {
            nextAppState.samus.fallFactor = 0;
        }
        if (nextAppState.samus.fallFactor > 0) {
            // nextAppState.samus.y = 135 - SAMUS_HEIGHT;
            if (nextAppState.samus.x > 157 && nextAppState.samus.y + nextAppState.samus.fallFactor + SAMUS_HEIGHT >= 84) {
                nextAppState.samus.y = 84 - SAMUS_HEIGHT;
            } else if (nextAppState.samus.x <= 157 && nextAppState.samus.y + nextAppState.samus.fallFactor + SAMUS_HEIGHT >= 54) {
                nextAppState.samus.y = 54 - SAMUS_HEIGHT;
            } else {
                nextAppState.samus.y = nextAppState.samus.y + nextAppState.samus.fallFactor;
            }
        }
    }
    // left
    if (KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {
        if (nextAppState.samus.x <= nextAppState.stage[nextAppState.currStage].leftWall) {
            if (nextAppState.currStage == 2 && nextAppState.samus.y > 84 && nextAppState.samus.x <= 95) {
                //a wall special to stage 3
            } else if (nextAppState.stage[nextAppState.currStage].leftDoor != 0) {
                nextAppState.currStage = nextAppState.currStage + 1;
                // clear bullets for next stage
                clearBullets(&nextAppState);
                nextAppState.samus.x = nextAppState.stage[nextAppState.currStage].rightWall - SAMUS_WIDTH;
            }
        } else {
            nextAppState.samus.x-=2;
        }
        nextAppState.samus.runCounter++;
        if (nextAppState.samus.runCounter % 5 == 0) {
            nextAppState.samus.currLRunningSprite = (nextAppState.samus.currLRunningSprite + 1) % 6;
        }
        nextAppState.samus.currRRunningSprite = -1;
    }
    // moving right
    if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {
        if (nextAppState.samus.x + 1 + SAMUS_WIDTH >= nextAppState.stage[nextAppState.currStage].rightWall) {
            //wall collision
            if (nextAppState.stage[nextAppState.currStage].rightDoor != 0) {
                //if can continue to next stage
                nextAppState.currStage--;
                clearBullets(&nextAppState);
                nextAppState.samus.x = nextAppState.stage[nextAppState.currStage].leftWall;
                // fillScreenDMA(GREEN);
            }
        } else {
            // no collision
            nextAppState.samus.x+=2;
        }
        nextAppState.samus.runCounter++;
        if (nextAppState.samus.runCounter % 5 == 0) {
            // change sprite every 5 frames of movement
            nextAppState.samus.currRRunningSprite = (nextAppState.samus.currRRunningSprite + 1) % 6;
        }
        nextAppState.samus.currLRunningSprite = -1;
    }
    // shoot
    if (KEY_DOWN(BUTTON_A, keysPressedNow)) {
        if (nextAppState.samus.shooting == 0) {
            //going from running or standing to shooting
            nextAppState.samus.shooting = 1;
            if (nextAppState.samus.currLRunningSprite == -1) {
                //running right, first shot
                nextAppState.samus.currRShootingSprite = nextAppState.samus.currRRunningSprite;
                nextAppState.samus.currRRunningSprite = -1;
                nextAppState.bullets[nextAppState.samus.nextBullet].x = nextAppState.samus.x + SAMUS_WIDTH;
                nextAppState.bullets[nextAppState.samus.nextBullet].y = nextAppState.samus.y + SAMUS_HEIGHT/2;
                nextAppState.bullets[nextAppState.samus.nextBullet].direction = 3;
            } else if (nextAppState.samus.currRRunningSprite == -1) {
                //running left, first shot
                nextAppState.samus.currLShootingSprite = nextAppState.samus.currLRunningSprite;
                nextAppState.samus.currLRunningSprite = -1;
                nextAppState.bullets[nextAppState.samus.nextBullet].x = nextAppState.samus.x;
                nextAppState.bullets[nextAppState.samus.nextBullet].y = nextAppState.samus.y + SAMUS_HEIGHT/2;
                nextAppState.bullets[nextAppState.samus.nextBullet].direction = -3;
            }
            nextAppState.samus.nextBullet = (nextAppState.samus.nextBullet + 1) % 3;
        } else {
            // button is held down, make it look like is aiming
            if (nextAppState.samus.shootCounter % 5 == 0) {
                if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow) && nextAppState.samus.currRShootingSprite >= 0) {
                    nextAppState.samus.currRShootingSprite = (nextAppState.samus.currRShootingSprite + 1) % 6;
                } else if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow) && nextAppState.samus.currLShootingSprite >= 0) {
                    // nextAppState.samus.currLShootingSprite = 6 - (nextAppState.samus.currLShootingSprite - 1);
                    nextAppState.samus.currLShootingSprite = (((nextAppState.samus.currLShootingSprite - 1) % 6) + 6) % 6;
                } else if (KEY_DOWN(BUTTON_LEFT, keysPressedNow) && nextAppState.samus.currRShootingSprite >= 0) {
                    // nextAppState.samus.currRShootingSprite = 6 - (nextAppState.samus.currRShootingSprite - 1);
                    nextAppState.samus.currRShootingSprite = (((nextAppState.samus.currRShootingSprite - 1) % 6) + 6) % 6;
                } else if (KEY_DOWN(BUTTON_LEFT, keysPressedNow) &&nextAppState.samus.currLShootingSprite >= 0) {
                    nextAppState.samus.currLShootingSprite = (nextAppState.samus.currLShootingSprite + 1) % 6;
                }
            }
        }
        nextAppState.samus.shootCounter++;
    } else {
        // stoped shooting. set sprites back to regular running
        if (nextAppState.samus.shooting == 1) {
            if (nextAppState.samus.currLShootingSprite >= 0) {
                nextAppState.samus.currLRunningSprite = nextAppState.samus.currLShootingSprite;
            } else if (nextAppState.samus.currRShootingSprite >= 0) {
                nextAppState.samus.currRRunningSprite = nextAppState.samus.currRShootingSprite;
            }
            nextAppState.samus.shooting = 0;
            nextAppState.samus.currLShootingSprite = -1;
            nextAppState.samus.currRShootingSprite = -1;
            nextAppState.samus.shootCounter = 0;
        }
    }
    // bullet logic and movement
    for (int i = 0; i < 3; i++) {
        nextAppState.bullets[i].x += nextAppState.bullets[i].direction;
        if (nextAppState.bullets[i].x < 0 || nextAppState.bullets[i].x > 240) {
            // if go out of window, stop it
            nextAppState.bullets[i].direction = 0;
        }
    }
    //crawler logic
    if (nextAppState.currStage == 1 && vBlankCounter % 5 == 0) {
        for (int i = 0; i < 2; i++) {
            // iterates thru the 2 crawlers
            nextAppState.stage[1].crawlers[i].currStep++;
            if (nextAppState.stage[1].crawlers[i].currStep % 20 < 10) {
                // <10 is moving right
                nextAppState.stage[1].crawlers[i].direction = 1;
            } else {
                // >= 10 is moving left
                nextAppState.stage[1].crawlers[i].direction = -1;
            }
            nextAppState.stage[1].crawlers[i].x += nextAppState.stage[1].crawlers[i].speed * nextAppState.stage[1].crawlers[i].direction;
            if ((nextAppState.stage[1].crawlers[i].x + CRAWLER_SIZE >= nextAppState.samus.x && nextAppState.stage[1].crawlers[i].x + CRAWLER_SIZE <= nextAppState.samus.x + SAMUS_WIDTH) || (nextAppState.samus.x + SAMUS_WIDTH >= nextAppState.stage[1].crawlers[i].x && nextAppState.samus.x + SAMUS_WIDTH <= nextAppState.stage[1].crawlers[i].x + CRAWLER_SIZE)) {
                //damage to samus
                nextAppState.samus.health -= 5;
            }
            for (int j = 0; j < 3; j++) {
                if (nextAppState.stage[1].crawlers[i].speed > 0 && (nextAppState.bullets[j].y >= nextAppState.stage[1].crawlers[i].y && nextAppState.bullets[j].y <= nextAppState.stage[1].crawlers[i].y + CRAWLER_SIZE) && (nextAppState.bullets[j].x >= nextAppState.stage[1].crawlers[i].x && nextAppState.bullets[j].x <= nextAppState.stage[1].crawlers[i].x + CRAWLER_SIZE)) {
                    //damage to crawler from bullets
                    nextAppState.stage[1].crawlers[i].speed = 0;
                    nextAppState.stage[1].crawlers[i].x = 240;
                    nextAppState.stage[1].crawlers[i].y = 0;
                    nextAppState.bullets[j].x = 240;
                    nextAppState.bullets[j].y = 0;
                    nextAppState.enemies--;
                }
            }
        }
    }

    //jump
    if (KEY_JUST_PRESSED(BUTTON_B, keysPressedNow, keysPressedBefore) && nextAppState.samus.jumping == 0) {
        //values for how high to jump
        nextAppState.samus.jumpFactor = 15;
        nextAppState.samus.jumping = 1;
    }
    if (nextAppState.samus.jumping && nextAppState.samus.jumpFactor > 0) {
        // if during the upwards motion of jumping
        nextAppState.samus.y = nextAppState.samus.y - nextAppState.samus.jumpFactor;
        nextAppState.samus.jumpFactor--;
        if (nextAppState.samus.jumpFactor <= 0) {
            //falling
            nextAppState.samus.jumping = 0;
        }
    }
    // handles the sprite transition when changing directions. this messes with shooting sprites when standing
    if (nextAppState.samus.currRRunningSprite == -1 && nextAppState.samus.currLRunningSprite == -1) {
        nextAppState.samus.currRRunningSprite = 0;
        nextAppState.samus.currLRunningSprite = 0;
    }
    //open door to end stage
    if (nextAppState.enemies == 0) {
        nextAppState.stage[2].leftDoor = 1;
    }
    //win
    if (nextAppState.currStage == 3 && nextAppState.samus.x <= 157) {
        nextAppState.win = 1;
        nextAppState.gameOver = 1;
    }
    //dies
    if (nextAppState.samus.health <= 0) {
        nextAppState.win = 0;
        nextAppState.gameOver = 1;
    }

    if (KEY_JUST_PRESSED(BUTTON_SELECT, keysPressedNow, keysPressedBefore)) {
        nextAppState.reset = 1;
    }

    UNUSED(keysPressedBefore);
    UNUSED(keysPressedNow);

    return nextAppState;
}
