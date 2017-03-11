#ifndef renderer_h
#define renderer_h

#include "instance.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class JRenderer {

public:
        JRenderer(std::string s) {
	  if( !init(s) ) exit(1);
	}
	
	SDL_Renderer* get() {
	   return gRenderer;
	}

public:  

TTF_Font* gTTF_font;
SDL_Window* gWindow;
SDL_Renderer* gRenderer;


bool init(const std::string& title)
{
        bool success = true;
	
	 	
     //*
     if(!TTF_WasInit() && TTF_Init()==-1) {
	  printf("TTF_Init: %s\n", TTF_GetError());
	  exit(1);
      }
      
      gTTF_font = TTF_OpenFont("FreeSans.ttf", 10); 
      if(!gTTF_font) {  
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(1);
      }

      //Initialize SDL
        if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
                printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
                success = false;
        }
        else
        {
                //Set texture filtering to linear
                if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
                {
                        printf( "Warning: Linear texture filtering not enabled!" );
                }

                //Create window
                gWindow = SDL_CreateWindow(const_cast<const char*>(title.c_str()), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
                if( gWindow == NULL )
                {
                        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
                        success = false;
                }
                else
                {
                        //Create renderer for window
                        gRenderer = SDL_CreateRenderer( gWindow, -1, 0 );
                        if( gRenderer == NULL )
                        {
                                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                                success = false;
                        }
                        else
                        {
                                //Initialize renderer color
                                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                                //Initialize PNG loading
                                int imgFlags = IMG_INIT_PNG;
                                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                                {
                                        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                                        success = false;
                                }
                        }
                }
        }

        return success;
}



void close()
{
        //Destroy window        
        SDL_DestroyRenderer( gRenderer );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        gRenderer = NULL;

        //Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
}


  void draw_permute_two_instances(JInstance* f, JInstance* s) {
               SDL_Point ns1 = s->get_center();
               SDL_Point nf1 = f->get_center();
               SDL_Point tmp2 = s->get_center();
               
               SDL_Point ns = s->get_center();
               SDL_Point nf = f->get_center();
               SDL_Point tmp = s->get_center();
               
             /*
               for(float i=s->get_center().y; i < f->get_center().y; i=i+5.0 ) {
                 ns.y = i;
                 s->set_center(ns);
                 draw();
              }
              
             for(float i=f->get_center().y; i > tmp.y; i=i-5.0 ) {
                 nf.y = i;
                 f->set_center(nf);
                 draw();
              }
              /**/
              
              s->set_center(nf1);
              f->set_center(ns1);
              //draw();
        }

	void draw_text(std::string s, const SDL_Point& p )
	{
	      SDL_Color c = {0, 0, 0};  
	      SDL_Surface* surfaceMessage = TTF_RenderText_Solid(gTTF_font, s.c_str(), c); 
	      SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage); 

	      SDL_Rect Message_rect; 
	      Message_rect.x = p.x;  
	      Message_rect.y = p.y; 
	      Message_rect.w = 25; 
	      Message_rect.h = 25; 

	      SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect); 
	}



	void draw_circle(const SDL_Point& center, int radius, SDL_Color c)//, SDL_Color color)
	{
	    //std::cout << "circle " << center.x << " " << center.y << std::endl;
	    SDL_SetRenderDrawColor(gRenderer , c.r,c.g,c.b,c.a);
	    for (int w = 0; w < radius * 2; w++)
	    {
		for (int h = 0; h < radius * 2; h++)
		{
		    int dx = radius - w; // horizontal offset
		    int dy = radius - h; // vertical offset
		    if ((dx*dx + dy*dy) <= (radius * radius))
		    {
			SDL_RenderDrawPoint(gRenderer , center.x + dx, center.y + dy);
		    }
		}
	    }
	    SDL_SetRenderDrawColor(gRenderer , 255,0,0,255 );
	}
};
	
#endif