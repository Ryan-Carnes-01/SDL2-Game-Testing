#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <iostream>
#include <Custom/defs.h>

struct App{
    struct Entity{
        float x,y,dx,dy;
        int w,h;
        int health;
        int fireDelay;
        SDL_Texture*texture;
        struct Entity*next;
    };
    struct Stage{
        struct Entity playerHead, *playerTail;
        struct Entity bulletHead, *bulletTail;
        SDL_Texture* bulletTexture;
    }; 
    
    SDL_Renderer* renderer; 
    SDL_Window* window;
    int keyboard[MAX_KEYBOARD_KEYS] = {0};
    struct Stage stage;

    void initSDL(){
        int rendererflags = SDL_RENDERER_ACCELERATED;
        int windowflags = 0;

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
    void initStage(){
        stage.playerTail = &stage.playerHead;
        stage.bulletTail = &stage.bulletHead;

        stage.bulletTexture = loadTexture((char*)"imgs/bullet2.png");
        initPlayer();
    }    
    void initPlayer(){
        struct Entity* player = new struct Entity;
        stage.playerTail->next = player;
        stage.playerTail = player;

        player->x = 100;
        player->y = 100;
        player->health = 100;
        player->fireDelay = 0;
        player->texture = loadTexture((char*)"imgs/gunguy.png");
        SDL_QueryTexture(player->texture,NULL,NULL,&player->w,&player->h);
    }

    void destroySDL(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
    
    void prepareScene(){
        SDL_SetRenderDrawColor(renderer, 100,100,255,255);
        SDL_RenderClear(renderer);
    }
    void presentScene(){
        SDL_RenderPresent(renderer);
    }
    
    SDL_Texture* loadTexture(char*fname){
        SDL_Texture* texture;
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO, "Loading %s", fname);
        texture = IMG_LoadTexture(renderer,fname);
        return texture;
    }
    void blit(SDL_Texture*texture, float x, float y){
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;

        SDL_QueryTexture(texture, NULL, NULL,&dest.w, &dest.h);
        SDL_RenderCopy(renderer,texture,NULL,&dest);
    }
    void doDraw(){
        drawPlayer();
        drawBullets();
    }
    void drawPlayer(){
        blit(stage.playerTail->texture,stage.playerTail->x,stage.playerTail->y);
    }
    void drawBullets(){
	    Entity *b;
	    for (b = stage.bulletHead.next ; b != NULL ; b = b->next)
	    {
		    blit(b->texture, b->x, b->y);
	    }
    }
    
    void doInput(){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT: 
                    destroySDL(); 
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    doKeyDown(&event.key);
                    break;
                case SDL_KEYUP:
                    doKeyUp(&event.key);
                    break;
                default:
                    break;
            }   
        }        
    }   
    void doKeyUp(SDL_KeyboardEvent *event){
	    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS){
            printf("KEYUP");
		    keyboard[event->keysym.scancode] = 0;
	    }
    }
    void doKeyDown(SDL_KeyboardEvent *event){
	    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS){
            printf("KEYDOWN");
		    keyboard[event->keysym.scancode] = 1;
	    }
    }

    void doLogic(){
        doPlayer();
        doBullets();
    }
    void doPlayer(){
        struct Entity*player = stage.playerTail;
        player->dx = player->dy = 0;

        if(player->fireDelay > 0){
            player->fireDelay--;
        }

        if(keyboard[SDL_SCANCODE_UP]){
            if(player->y >= 0){
                player->dy = -PLAYER_SPEED;
            }
        }
        if(keyboard[SDL_SCANCODE_DOWN]){
            if(player->y + player->h <= SCREEN_HEIGHT){
                player->dy = PLAYER_SPEED;
            }
        }
        if(keyboard[SDL_SCANCODE_LEFT]){
            if(player->x >= 0){
                player->dx = -PLAYER_SPEED;
            }
        }
        if(keyboard[SDL_SCANCODE_RIGHT]){
            if(player->x + player->w <= SCREEN_WIDTH){
                player->dx = PLAYER_SPEED;
            }
        }
        if(keyboard[SDL_SCANCODE_SPACE] && player->fireDelay == 0){
            fireBullet();
            player->fireDelay = FIRE_DELAY;
        }

        player->x += player->dx;
        player->y += player->dy;
    }
    void fireBullet(){
        struct Entity*bullet = new struct Entity;
        struct Entity*player = stage.playerTail;
        stage.bulletTail->next = bullet;
        stage.bulletTail = bullet;

        bullet->x = player->x + 210;
        bullet->y = player->y + 135;
        bullet->dx = BULLET_SPEED;
        bullet->dy = 0;
        bullet->health = 1;
        bullet->texture = stage.bulletTexture;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
    }
    void doBullets(){
        struct Entity*b,*prev;
        prev = &stage.bulletHead;
        for(b = stage.bulletHead.next ; b != NULL ; b = b->next){
            b->x += b->dx;
            b->y += b->dy;

            if(b->x > SCREEN_WIDTH){
                if (b == stage.bulletTail){
				    stage.bulletTail = prev;
			    }
			    prev->next = b->next;
			    free(b);
			    b = prev;
            }
            prev = b;
        }
    }
};

int main(int argc, char*argv[]){
    struct App app;
    app.initSDL();
    app.initStage();
    while(1){
        app.prepareScene();
        app.doInput();
        app.doLogic();
        app.doDraw();
        app.presentScene();
    }
    SDL_Delay(16);
    return 0;
}