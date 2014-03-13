#include "SDL.h"
int main( int argc, char* args[] )
{
SDL_Surface* hello = NULL;
    SDL_Surface* screen = NULL;SDL_Init( SDL_INIT_EVERYTHING );

    //Set up screen
    screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );

    //Load image
    hello = SDL_LoadBMP( "hello_world.bmp" ); //Apply image to screen
    SDL_BlitSurface( hello, NULL, screen, NULL );

    //Update Screen
    SDL_Flip( screen );

    //Pause
    SDL_Delay( 10000 );  /*2000 milliseconds is two seconds*/
	//Free the loaded image
    SDL_FreeSurface( hello );

    //Quit SDL
    SDL_Quit();

    return 0;
}