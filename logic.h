#include "init.h"

#include <set>
#include <map>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iostream>

#include <SDL_ttf.h>

float distance(const SDL_Point& a, const SDL_Point& b){
  return sqrt(pow(abs(a.x - b.x), 2) + pow(abs(a.y - b.y), 2));
}

bool operator<(const SDL_Point &lhs, const SDL_Point &rhs) {
    return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
}

void draw_text(std::string s, const SDL_Point& p)
{
      
      TTF_Font* Sans = TTF_OpenFont("./FreeSans.ttf", 42); 
      if(!Sans) {  
	printf("TTF_OpenFont: %s\n", TTF_GetError());
	return;
      }

      SDL_Color c = {0, 0, 0};  
      SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, s.c_str(), c); 
      SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage); 

      SDL_Rect Message_rect; //create a rect
      Message_rect.x = p.x;  //controls the rect's x coordinate 
      Message_rect.y = p.y; // controls the rect's y coordinte
      Message_rect.w = 10; // controls the width of the rect
      Message_rect.h = 10; // controls the height of the rect

      SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect); //you put the renderer's name 
}

class JInstance {
    
    public: 
	JInstance() {
	    m_color.r = rand()%255;
	    m_color.g = rand()%255;
	    m_color.b = rand()%255;
	    m_color.a = 255;
	}
      
    public:
	const SDL_Point& get_center() { return m_center_point ; }
	void set_center(const SDL_Point& p) { m_center_point = p; }
	const SDL_Color& get_color() { return m_color; }
	//void set_name()	{
	void set_value(int v) { m_value = v; }
	int get_value() { return m_value; }
      
    private:
	int m_value;
	SDL_Point m_center_point;
	//std::string m_name;
	
	SDL_Color m_color;  

  
};

class JManager {
  
  public:  
    
	JManager():m_layers_cnt(3),m_max_per_clm(3),m_conn_density(1) {
	  m_layers.resize(m_layers_cnt);
	  srand(time(0));
	  init_data();
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
	  for(unsigned i=0; i<m_layers_cnt; i++)
	    for (unsigned int j=0; j<m_layers[i].size(); j++ ) {
	      draw_circle(m_layers[i][j]->get_center(),5,m_layers[i][j]->get_color());  
	      //draw_text(m_layers[i][j]->m_name,m_layers[i][j]->get_center());
	    }
	    
	    std::multimap<JInstance*,JInstance*>::iterator i;
	    for(i=m_connections.begin();i!=m_connections.end();++i)
	      SDL_RenderDrawLine(gRenderer,(*i).first->get_center().x,(*i).first->get_center().y,
					   (*i).second->get_center().x,(*i).second->get_center().y);
	    
	}

        void place() {
            //set_classes();
            add_change();
        }
        
        void propogate() {
	  for(unsigned int i=0; i<m_layers_cnt; i++ ) {
	    for (unsigned int j=0; j<m_layers[i].size(); j++ ) {
	      if (!i) m_layers[i][j]->set_value(100); else {
		int v = 0;
		std::multimap<JInstance*,JInstance*>::iterator c_im = m_connections.find(m_layers[i][j]);
		if ( c_im != m_connections.end() ) {
		  v = v + (*c_im).second->get_value();
		  //v = v/k;
		  m_layers[i][j]->set_value(v);
		}
	      }
	    }
	  }
	}
	
        void do_and_draw() {
            //place();
            propogate();
            draw();
        }
        
        void add_change() {
	      //permute_two_rand_instances_in_layer(3);
	      int count = rand()%m_layers_cnt;
	      for(unsigned int i=0; i<count; i++ ) permute_two_rand_instances_in_layer(rand()%m_layers_cnt);
	}
        
        void permute_two_rand_instances_in_layer(int ln) {
	      
	      int f = rand()%m_layers[ln].size();
	      int s = rand()%m_layers[ln].size();
	      if ( f == s ) return;
	      
	      //f=1;
	      //s=2;
	      
	      //std::cout << "ptri" << ln << ": " << f << "-->" << s << std::endl;

	      SDL_Point tmp = m_layers[ln][s]->get_center();
	      m_layers[ln][s]->set_center(m_layers[ln][f]->get_center());
	      m_layers[ln][f]->set_center(tmp);
	      
	      JInstance* tmp1 = m_layers[ln][s];
	      m_layers[ln][s] = m_layers[ln][f];
	      m_layers[ln][f] = tmp1;
	      /**/
	      //int count = rand()%m_layers[ln].size();
	      //for(unsigned init i=0; i<count; i++ ) permute_rand_instances_in_layer(rand()%m_layers_cnt);
	}
	
        void init_data() {
	    for (unsigned int i=0; i<m_layers_cnt; i++ ) {
	      int perclm = 1+rand()%m_max_per_clm;
	      m_layers[i].resize(perclm);
	      for (unsigned int j=0; j<perclm ; j++ ) {
		SDL_Point p;
		p.x = 50*i+10; p.y = 50*j+10;
		JInstance* ii = new JInstance;
		ii->set_center(p);
		//ii->m_name = "asdasdasdasdadsasdasd";
		m_layers[i][j] = ii;
		//std::cout << i << " " << j << std::endl;
		 if (i) {
		  //int k = 1 + rand()%m_layers[i-1].size();
		  int k = 1+rand()%m_conn_density; 
		  //std::vector<int> indexes(k);
		  //for(unsigned q=0;q<k;q++ ) indexes[q] = rand()%m_layers[i-1].size();
		  for(unsigned q=0;q<k;q++ ) {
		    //rand()%(m_layers[i-1].size())
		    JInstance* ri = m_layers[i-1][rand()%(m_layers[i-1].size())];
		    //m_connections[ri] = ii;
		    m_connections.insert(std::make_pair<JInstance*,JInstance*>(ri,ii));
		  }
		}
	      }
	    }
	}
         
	
  private:
	std::vector< std::vector<JInstance*> > m_layers;
	std::multimap<JInstance*,JInstance*> m_connections;
	
	//std::multimap<JInstance*,JInstance*>
	//int m_per_layer_
	int m_max_per_clm;
	int m_layers_cnt;
	int m_conn_density;
};