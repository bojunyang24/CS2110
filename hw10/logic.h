#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"

// useful ints
#define SAMUS_HEIGHT 25
#define SAMUS_WIDTH 20
#define BULLET_LENGTH 8
#define BULLET_HEIGHT 4
#define CRAWLER_SIZE 18

//Samus
typedef struct {
    int health; // if 0 die, max is 100
    int runCounter;
    int currLRunningSprite; // -1: not running. 0,1,2,3,4,5
    int currRRunningSprite; // -1: not running
    int currLShootingSprite; // 6
    int currRShootingSprite; // 6
    int missiles; // how many missles left
    int x;
    int y;
    int top, bottom, left, right;
    int fallFactor; //gravity
    int jumpFactor; //jumping
    int jumping; //0:false, 1:true
    int shootCounter; //which bullet to shoot.2 yellow, 1 magenta
    int shooting; //is samus in shooting form
    int nextBullet; //nextBullet to shoot
} Samus;
//crawler
typedef struct {
    int currStep; // 10 steps each way. 0-9
    int speed; // positive int
    int direction; // -1 or 1
    int x;
    int y;
} Crawler;
//relevant info for each stage
typedef struct {
    int leftDoor; // -1: no door, 0: closed, 1: opened
    int rightDoor; // -1: no door, 0: closed, 1: opened
    int safe; // 0: not safe, 1: safe. if safe no enemies spawn
    Crawler crawlers[2]; // 2 crawlers per "stage"
    int leftWall; // for collision
    int rightWall;
} Stage;
//bullet
typedef struct {
    int x, y, direction;
} Bullet;

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver; // 0: not over, 1: over
    int win; // 0: lose, 1: win
    int points; // get points from killing enemies
    Samus samus;
    Stage stage[4];
    int currStage;
    int reset;
    Bullet bullets[3]; // bullets
    int enemies; //keep track of enemies, able to proceed when all enemies defeated
} AppState;

/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/



// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
