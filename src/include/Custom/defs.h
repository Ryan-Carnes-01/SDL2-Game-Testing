#include <SDL2/SDL.h>
#define SCREEN_WIDTH 1240
#define SCREEN_HEIGHT 720
#define MAX_KEYBOARD_KEYS 350
#define PLAYER_SPEED 15
#define BULLET_SPEED 80
#define FIRE_DELAY 15
#define FRAME_DELAY 16

/*struct App{
    struct Entity;
    struct Stage;
    void initSDL();
    void destroySDL();
    void prepareScene();
    void presentScene();
    SDL_Texture* loadTexture(char*);
    void blit(SDL_Texture*, float, float);
    void doInput();
    void initPlayer();
    void initStage();
    void drawPlayer();
    void drawBullets();
    void doDraw();
    void doKeyUp(SDL_KeyboardEvent*);
    void doKeyDown(SDL_KeyboardEvent*);
    void doLogic();
    void doPlayer();
    void doBullets();
};*/