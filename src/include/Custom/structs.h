#include <Custom/common.h>

typedef struct{
    void (*logic)(void);
    void (*draw)(void);
}Delegate;

typedef struct{
    SDL_Renderer *renderer;
    SDL_Window *window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS] = {0};

    void initSDL(){
        int rendererflags, windowflags;
        rendererflags = SDL_RENDERER_ACCELERATED;
        windowflags = 0;

        if(SDL_Init(SDL_INIT_VIDEO) < 0){
            printf("Couldn't initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }

        window = SDL_CreateWindow("SDL2 Game Testing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,SCREEN_HEIGHT,windowflags);
        if(!window){
            printf("failed to %d by %d window: %s\n",SCREEN_WIDTH,SCREEN_HEIGHT,SDL_GetError());
            exit(1);
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        renderer = SDL_CreateRenderer(window,-1,rendererflags);
        if(!renderer){
            printf("failed to create renderer %s\n", SDL_GetError());
            exit(1);
        }
    }
    void destroySDL(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
}App;

struct Entity{
    float x;
    float y;
    float dx;
    float dy;
    int health;
    int reload;
    int h;
    int w;
    SDL_Texture* texture;
    Entity *next;
    void init(int init_x, int init_y, int init_health, SDL_Texture* init_texture){
        x = init_x;
        y = init_y;
        dx = 0;
        dy = 0;
        health = init_health;
        texture = init_texture;
        
    }
};

typedef struct {
    struct Entity fighterHead, *fighterTail;
    struct Entity bulletHead, *bulletTail;
}Stage;
