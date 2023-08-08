#include <Custom/common.h>

void prepareScene(App*app){
    SDL_SetRenderDrawColor(app->renderer,100,100,255,255);
    SDL_RenderClear(app->renderer);
}

void presentScene(App*app){
    SDL_RenderPresent(app->renderer);
}

SDL_Texture* loadTexture(App*app, char*fname){
    SDL_Texture* texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO, "Loading %s", fname);
    texture = IMG_LoadTexture(app->renderer,fname);
    return texture;
}

void blit(App*app,SDL_Texture*texture,int x,int y){
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, NULL, NULL,&dest.w, &dest.h);
    SDL_RenderCopy(app->renderer,texture,NULL,&dest);
}