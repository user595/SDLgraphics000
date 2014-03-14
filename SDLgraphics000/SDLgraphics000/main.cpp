/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//Flip flags
const int FLIP_VERTICAL = 1;
const int FLIP_HORIZONTAL = 2;

//The surfaces
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *dots = NULL;
SDL_Surface *message = NULL;
SDL_Surface *topLeft = NULL;
SDL_Surface *topRight = NULL;
SDL_Surface *bottomLeft = NULL;
SDL_Surface *bottomRight = NULL;
SDL_Surface *global_surface = NULL;

//The event structure that will be used
SDL_Event event;

//The font that's going to be used
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 2, 2, 2 };//255,255,255 is white?

//The portions of the sprite map to be blitted
SDL_Rect clip[ 4 ];

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image using SDL_image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
    }

    //Return the optimized image
    return optimizedImage;
}

bool load_files()
{
    //Load the sprite map
    dots = load_image( "dots.png" );

    //If there was an problem loading the sprite map
    if( dots == NULL )
    {
        return false;
    }

    //Open the font
    font = TTF_OpenFont( "lazy.ttf", 28 );

    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }

    //If eveything loaded fine
    return true;
}


void apply_surface_four_arg( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Rectangle to hold the offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "PNG test", NULL );

    //If everything initialized fine
    return true;
}

Uint32 get_pixel32( SDL_Surface *surface, int x, int y )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

SDL_Surface *flip_surface( SDL_Surface *surface, int flags )
{
    //Pointer to the soon to be flipped surface
    SDL_Surface *flipped = NULL;

	flipped = surface;
	return flipped;


    //If the image is color keyed
    if( surface->flags & SDL_SRCCOLORKEY )
    {
        flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, 0 );
    }
    //Otherwise
    else
    {
        flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
    }

    //If the surface must be locked
    if( SDL_MUSTLOCK( surface ) )
    {
        //Lock the surface
        SDL_LockSurface( surface );
    }

    //Go through columns
    for( int x = 0, rx = flipped->w - 1; x < flipped->w; x++, rx-- )
    {
        //Go through rows
        for( int y = 0, ry = flipped->h - 1; y < flipped->h; y++, ry-- )
        {
            //Get pixel
            Uint32 pixel = get_pixel32( surface, x, y );

            //Copy pixel
            if( ( flags & FLIP_VERTICAL ) && ( flags & FLIP_HORIZONTAL ) )
            {
                put_pixel32( flipped, rx, ry, pixel );
            }
            else if( flags & FLIP_HORIZONTAL )
            {
                put_pixel32( flipped, rx, y, pixel );
            }
            else if( flags & FLIP_VERTICAL )
            {
                put_pixel32( flipped, x, ry, pixel );
            }
        }
    }

    //Unlock surface
    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }

    //Copy color key 
    if( surface->flags & SDL_SRCCOLORKEY )
    {
        SDL_SetColorKey( flipped, SDL_RLEACCEL | SDL_SRCCOLORKEY, surface->format->colorkey );
    }

    //Return flipped surface
    return flipped;
}

SDL_Surface *change_surface( SDL_Surface *surface )
{
    //Pointer to the soon to be flipped surface
    SDL_Surface *flipped = surface;

    //If the image is color keyed
    if( surface->flags & SDL_SRCCOLORKEY )
    {
        flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, 0 );
    }
    //Otherwise
    else
    {
        //flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
    }

    //If the surface must be locked
    if( SDL_MUSTLOCK( surface ) )
    {
        //Lock the surface
        SDL_LockSurface( surface );
    }

    //Go through columns
    for( int x = 0, rx = flipped->w - 1; x < flipped->w; x++, rx-- )
    {
		//break;
        //Go through rows
        for( int y = 0, ry = flipped->h - 1; y < flipped->h; y++, ry-- )
        {
            //Get pixel
            Uint32 pixel = get_pixel32( surface, x, y );

			put_pixel32( flipped, rx, ry, pixel );
            
        }
    }

    //Unlock surface
    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }

    //Copy color key
    if( surface->flags & SDL_SRCCOLORKEY )
    {
        SDL_SetColorKey( flipped, SDL_RLEACCEL | SDL_SRCCOLORKEY, surface->format->colorkey );
    }

    //Return flipped surface
    return flipped;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( image );

    //Close the font that was used
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    SDL_FreeSurface( topLeft );
    SDL_FreeSurface( topRight );
    SDL_FreeSurface( bottomLeft );
    SDL_FreeSurface( bottomRight );

    //Quit SDL
    SDL_Quit();
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}


int main( int argc, char* args[] )
{

    //Make sure the program waits for a quit
    bool quit = false;


    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the image
    image = load_image( "look.png" );

    //If there was a problem in loading the image
    if( image == NULL )
    {
        return 1;
    } 

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Apply the surface to the screen
    apply_surface( 0, 0, image, screen );

    //Clip range for the top left
    clip[ 0 ].x = 0;
    clip[ 0 ].y = 0;
    clip[ 0 ].w = 100;
    clip[ 0 ].h = 100;

    //Clip range for the top right
    clip[ 1 ].x = 100;
    clip[ 1 ].y = 0;
    clip[ 1 ].w = 100;
    clip[ 1 ].h = 100;

    //Clip range for the bottom left
    clip[ 2 ].x = 0;
    clip[ 2 ].y = 100;
    clip[ 2 ].w = 100;
    clip[ 2 ].h = 100;

    //Clip range for the bottom right
    clip[ 3 ].x = 100;
    clip[ 3 ].y = 100;
    clip[ 3 ].w = 100;
    clip[ 3 ].h = 100;

    //Apply the sprites to the screen
    apply_surface( 0, 0, dots, screen, &clip[ 0 ] );
    apply_surface( 540, 0, dots, screen, &clip[ 1 ] );
    apply_surface( 0, 380, dots, screen, &clip[ 2 ] );
    apply_surface( 540, 380, dots, screen, &clip[ 3 ] );

    //Render the text
    message = TTF_RenderText_Solid( font, "The quick brown fox jumps over the lazy frog", textColor );
	apply_surface( 0, 150, message, screen );

    //If there was an error in rendering the text 
    if( message == NULL )
    {
        return 1;
    }

    //Flip surfaces
	//topLeft = screen;
    topRight = change_surface( screen );
    //bottomLeft = flip_surface( topLeft, FLIP_VERTICAL );
    //bottomRight = flip_surface( screen, FLIP_HORIZONTAL | FLIP_VERTICAL );

	//topLeft = flip_surface( screen, FLIP_HORIZONTAL | FLIP_VERTICAL );
	//if (topLeft != screen)
		//screen = topLeft;
	
	screen = topRight;

    //Update the screen
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
    }
	

    //While the user hasn't quit
    while( quit == false )
    {
        //While there's an event to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
			if( event.type == SDL_KEYDOWN )
			{
			/*
				screen = bottomRight;
				SDL_Flip( screen );
				break;
				*/
				switch( event.key.keysym.sym )
                {
                    case SDLK_UP: screen = topLeft; break;
                    case SDLK_DOWN: screen = bottomLeft; break;
                    case SDLK_LEFT: screen = topRight; break;
                    case SDLK_RIGHT: screen = bottomRight; break;
                }
				//SDL_Flip( screen );

			}
        }
    }

    //Free the surface and quit SDL
    clean_up();

    return 0;
}
