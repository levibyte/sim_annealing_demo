#include "init.h"

#include <set>
#include <map>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iostream>

float distance(const SDL_Point& a, const SDL_Point& b){
  return sqrt(pow(abs(a.x - b.x), 2) + pow(abs(a.y - b.y), 2));
}

bool operator<(const SDL_Point &lhs, const SDL_Point &rhs) {
    return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
}

class JInstance {
    public:
	const SDL_Point& get_center() { return m_center_point ; }
	void set_center(const SDL_Point& p) { m_center_point = p; }
      
    private:
	SDL_Point m_center_point;
};

class JManager {
  
  public:  
    
	JManager():m_layers_cnt(13) {
	  m_layers.resize(m_layers_cnt);
	  srand(time(0));
	  init_data();
	}

	void draw_circle(const SDL_Point& center, int radius)//, SDL_Color color)
	{
	    //std::cout << "circle " << center.x << " " << center.y << std::endl;
	    SDL_SetRenderDrawColor(gRenderer , 255,0,0,255);
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
	}

        void draw() {
	  for(unsigned i=0; i<m_layers_cnt; i++)
	    for (unsigned int j=0; j<m_layers[i].size(); j++ ) 
	      draw_circle(m_layers[i][j]->get_center(),5);  
	  
	    std::map<JInstance*,JInstance*>::iterator i;
	    for(i=m_connections.begin();i!=m_connections.end();++i)
	      SDL_RenderDrawLine(gRenderer,(*i).first->get_center().x,(*i).first->get_center().y,
					   (*i).second->get_center().x,(*i).second->get_center().y);
	    
	}

        void place() {
            //set_classes();
            add_change();
        }
        
        void do_and_draw() {
            place();
            draw();
        }
        
        void add_change() {
	      int count = rand()%m_layers_cnt;
	      for(unsigned int i=0; i<count; i++ ) permute_two_rand_instances_in_layer(rand()%m_layers_cnt);
	}
        
        void permute_two_rand_instances_in_layer(int ln) {
	      
	      int f = rand()%m_layers[ln].size();
	      int s = rand()%m_layers[ln].size();
	      if ( f == s ) return;
	      
	      std::cout << "ptri" << ln << ": " << f << "-->" << s << std::endl;

	      SDL_Point tmp = m_layers[ln][f]->get_center();
	      m_layers[ln][s]->set_center(m_layers[ln][f]->get_center());
	      m_layers[ln][f]->set_center(tmp);
	      
	      /*JInstance* tmp1 = m_layers[ln][f];
	      m_layers[ln][s] = m_layers[ln][f];
	      m_layers[ln][f] = tmp1;
	      */
	      //int count = rand()%m_layers[ln].size();
	      //for(unsigned init i=0; i<count; i++ ) permute_rand_instances_in_layer(rand()%m_layers_cnt);
	}
	
        void init_data() {
	    for (unsigned int i=0; i<m_layers_cnt; i++ ) {
	      int perclm = 1+rand()%10;
	      m_layers[i].resize(perclm);
	      for (unsigned int j=0; j<perclm ; j++ ) {
		SDL_Point p;
		p.x = 50*i; p.y = 50*j;
		JInstance* ii = new JInstance;
		ii->set_center(p);
		m_layers[i][j] = ii;
		//std::cout << i << " " << j << std::endl;
		if (i) {
		  int k = m_layers[i-1].size()+rand()%m_layers[i-1].size();
		  for(unsigned q=0;q<k;q++ ) {
		    JInstance* ri = m_layers[i-1][rand()%(m_layers[i-1].size())];
		    m_connections[ri] = ii;
		  }
		}
	      }
	    }
	}
         
	
  private:
	std::vector< std::vector<JInstance*> > m_layers;
	std::map<JInstance*,JInstance*> m_connections;
	
	//std::multimap<JInstance*,JInstance*>
	//int m_per_layer_
	int m_layers_cnt;
};