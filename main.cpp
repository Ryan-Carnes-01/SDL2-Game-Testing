#include <Custom/common.h>
#include <Custom/structs.h>
#include <Custom/input.h>
#include <Custom/draw.h>


int main(int argc, char*argv[]){
    App app;
    struct Entity player;
    struct Entity bullet;

    app.initSDL();
    player.init(PLAYER_START_X,PLAYER_START_Y,PLAYER_HEALTH,loadTexture(&app,(char*)"imgs/gunguy.png"));
    bullet.init(0,0,0,loadTexture(&app, (char*)"imgs/bullet2.png"));

    while(1){
        prepareScene(&app);
        doInput(&app);

        player.x += player.dx;
        player.y += player.dy;

        if(app.keyboard[SDL_SCANCODE_UP]){
            player.y -= PLAYER_SPEED;
        }
        if(app.keyboard[SDL_SCANCODE_DOWN]){    
            player.y += PLAYER_SPEED;
        }
        if(app.keyboard[SDL_SCANCODE_LEFT]){   
            player.x -= PLAYER_SPEED;
        }
        if(app.keyboard[SDL_SCANCODE_RIGHT]){     
            player.x += PLAYER_SPEED;
        }

        if(app.keyboard[SDL_SCANCODE_SPACE] && bullet.health == 0){
            bullet.x = player.x+210;
            bullet.y = player.y+135;
            bullet.dx = BULLET_SPEED;
            bullet.dy = 0;
            bullet.health = 1;
        }

        bullet.x += bullet.dx;
        bullet.y += bullet.dy;

        if(bullet.x > SCREEN_WIDTH){
            bullet.health = 0;
        }

        blit(&app, player.texture, player.x, player.y);
        if(bullet.health > 0){
            blit(&app, bullet.texture, bullet.x, bullet.y);
        }

        presentScene(&app);
        SDL_Delay(FRAME_DELAY);
    }
    return 0;
}