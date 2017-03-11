#include <SDL2/SDL_ttf.h>

	void render_permute_two_instances(JInstance* f, JInstance* s) {
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

        void draw() {
	    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	    SDL_RenderClear( gRenderer );
	    //* 
	    for(unsigned i=0; i<m_layers_cnt; i++)
	      for (unsigned int j=0; j<m_layers[i].size(); j++ ) {
		draw_circle(m_layers[i][j]->get_center(),5,m_layers[i][j]->get_color());  
		//draw_text(m_layers[i][j]->get_name(),m_layers[i][j]->get_center());
	      }
	      
	      std::multimap<JInstance*,JInstance*>::iterator i;
	      for(i=m_connections.begin();i!=m_connections.end();++i)
		SDL_RenderDrawLine(gRenderer,(*i).first->get_center().x,(*i).first->get_center().y,
					      (*i).second->get_center().x,(*i).second->get_center().y);
	      /**/
	      SDL_RenderPresent( gRenderer );
	}
