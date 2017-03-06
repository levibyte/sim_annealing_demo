#include "init.h"

#include <set>
#include <map>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <functional>

//#include <SDL_ttf.h>

float distance(const SDL_Point& a, const SDL_Point& b){
  return sqrt(pow(abs(a.x - b.x), 2) + pow(abs(a.y - b.y), 2));
}

bool operator<(const SDL_Point &lhs, const SDL_Point &rhs) {
    return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
}

/*
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
*/

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

        void set_name(std::string s) { m_name = s;}
        std::string get_name() { return m_name; }
        
        void set_value(int v) { m_value = v; }
	int get_value() { return m_value; }
      
    private:
	int m_value;
	SDL_Point m_center_point;
	std::string m_name;
	
	SDL_Color m_color;  

  
};

class JManager {
  
  public:  
    
	JManager():m_layers_cnt(4),m_max_per_clm(5),m_conn_density(1),m_last_fitness(0) {
	  m_layers.resize(m_layers_cnt);
	  srand(time(0));
	  init_data();
          
          m_last_fitness = calc_intersections();
           std::cout << "BEGIN:" <<  m_last_fitness << std::endl;
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

        void update() {
          place();
          //calc_intersections();
          //draw();
          
        }
        
        void place() {
            int i = 0; 
            while ( i < 100000 ) { 
              add_change();
              int c = calc_intersections(); 
              if ( c < m_last_fitness ) {
                m_last_fitness = c; 
                break;
              }
              else 
                undo_permute();
              i++;  
            }
              //draw();
             std::cout << "LAST:" <<  m_last_fitness << std::endl;
        }
        
        int calc_intersections() {
          m_cvalue = 0;
          int r = 0;
          for(unsigned int i=0; i<m_layers_cnt-1; i++ ) r = r + calc_intersection(i);
          
          //std::cout << "FI: " << r << std::endl;
          return r;
        }
        
        int calc_intersection(int i) {
           std::multiset<int> seen;
           int count = 0;
           for (unsigned int j=0; j<m_layers[i].size(); j++ ) {
             //std::cout << "apr:" << i << "--" << j << std::endl;
             count = count + count_intersections(seen,get_insts(m_layers[i][j]));
              //std::cout << "---" << std::endl;
           }
           //std::cout << " intersections for " << i << "-" << i+1 << "  " << count << std::endl;
        return count;
        }
        
        bool mypred(int a)
        {
            //std::cout <<"     mypred" << a << " " << m_cvalue << std::endl;
            //std::cout <<"mypred" << a << std::endl;
            //return true;
            return ( m_cvalue < a );
        }
        
        int count_intersections(std::multiset<int>& seen, const std::vector<JInstance*>& v) {
            std::vector<JInstance*>::const_iterator i;
           // std::set<int>::iterator si = seen.begin();
            int res = 0;
            for(i=v.begin();i!=v.end();++i) {
              /*for(int ssi=0;ssi!=seen.size();ssi++) {
                std::advance(si, ssi);
                std::cout << *(si)  << " <?> " << (*i)->get_center().y << std::endl;
                //if ( seen[si] < (*i)->get_center().y  ) res = res + 1;
                ///seen.insert((*i)->get_center().y);
                si = seen.begin();
              }*/
              //std::count_if(seen.begin(), seen.end(), f )
             //std::cout << "set size" << seen.size() << std::endl;
             //std::cout << "processing " << (*i)->get_name() << std::endl;
             m_cvalue = (*i)->get_center().y;
              //std::count_if(seen.begin(), seen.end(),f );
              res = res + std::count_if(seen.begin(), seen.end(), std::bind1st(std::mem_fun(&JManager::mypred),1));
              //std :: cout << " res = " << res << std::endl;
              seen.insert((*i)->get_center().y);
              m_cvalue = 0;
              //std::cout << " set inserted -!-" << (*i)->get_center().y << std::endl;
            }
            //std::cout << "-!-" << res << std::endl;
         return res;
        }
 
 
 
 
 
 
        std::vector<JInstance*> get_insts(JInstance* i) {
             
              std::vector<JInstance*> v;
               std::multimap<JInstance*,JInstance*>::iterator itlow = m_connections.lower_bound(i);  
               std::multimap<JInstance*,JInstance*>::iterator itup = m_connections.upper_bound(i);   
                std::multimap<JInstance*,JInstance*>::iterator it;

              for (it=itlow; it!=itup; ++it) v.push_back((*it).second);
              
             //std::cout << " SIZE=" << v.size() << std::endl;
              return v;
        }
        
        /*
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
	*/
        
        void sirun_draw() {
          
          
        }
        
        void do_and_draw() {
            //place();
            //propogate();
            //calc_intersections();
            draw();
        }
        
        void add_change() {
	      //permute_two_rand_instances_in_layer(3);
	      int count = rand()%m_layers_cnt;
	      for(unsigned int i=0; i<count; i++ ) permute_two_rand_instances_in_layer(rand()%m_layers_cnt);
	}
        
        void permute_two_rand_instances_in_layer(int ln) {
	      //assert(0);
	      int f = rand()%m_layers[ln].size();
	      int s = rand()%m_layers[ln].size();
	      if ( f == s ) return;
	      
	      if ( m_permuted.size() > 0 ) m_permuted.erase(m_permuted.begin(),m_permuted.begin()+m_permuted.size());
	      
	      std::pair<JInstance*,JInstance*> k(m_layers[ln][f],m_layers[ln][s]);
              m_permuted.push_back(k);
	      permute_two_instances(m_layers[ln][f],m_layers[ln][s]);
              
	      
	      //f=1;
	      //s=2;
	      
	      //std::cout << "ptri" << ln << ": " << f << "-->" << s << std::endl;

	      /**/
	      //int count = rand()%m_layers[ln].size();
	      //for(unsigned init i=0; i<count; i++ ) permute_rand_instances_in_layer(rand()%m_layers_cnt);
	}
	
	void permute_two_instances(JInstance* f, JInstance* s) {
              SDL_Point tmp = s->get_center();
              s->set_center(f->get_center());
              f->set_center(tmp);
              
              JInstance* tmp1 = s;
              s = f;
              f = tmp1;
        }
        
        void undo_permute() {
          std::vector<std::pair<JInstance*,JInstance*>>::iterator i;
          for (i=m_permuted.begin();i!=m_permuted.end();i++) permute_two_instances((*i).second,(*i).first);
          
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
		std::stringstream z;
                z << "i" << i << "-" << j;
                ii->set_name(z.str());
                z.str("");
		m_layers[i][j] = ii;
		//std::cout << i << " " << j << std::endl;
		 if (i) {
		  //int k = 1 + rand()%m_layers[i-1].size();
		  int k = 1+rand()%m_conn_density; 
		  //std::vector<int> indexes(k);
		  //for(unsigned q=0;q<k;q++ ) indexes[q] = rand()%m_layers[i-1].size();
		  std::set<JInstance*> seen;
                  //std::cout << " -----processing " << ii << std::endl;
                  for(unsigned q=0;q<k;q++ ) {
                    //std::set<JInstance*> seen;
		    //rand()%(m_layers[i-1].size())
		    JInstance* ri = m_layers[i-1][rand()%(m_layers[i-1].size())];
                    if ( seen.find(ri) == seen.end() ) {
		    //m_connections[ri] = ii;
                    //std::make_pair<JInstance*,JInstance*>(ri,ii)
                      //std::cout << "creating " << ri << " ( " << ri->get_name() << " ) --> " <<  ii << " ( " << ii->get_name() << " ) " << std::endl;
                      std::pair<JInstance*,JInstance*> p(ri,ii);
                      m_connections.insert(p);
                      seen.insert(ri);
                    }
		  }
		}
	      }
	    }
	//std::cout << "----------------" << std::endl;
        }
         
	
  private:
	std::vector< std::pair<JInstance*,JInstance*> > m_permuted;
        
        std::vector< std::vector<JInstance*> > m_layers;
	std::multimap<JInstance*,JInstance*> m_connections;
	
	//std::multimap<JInstance*,JInstance*>
	//int m_per_layer_
	int m_max_per_clm;
	int m_layers_cnt;
	int m_conn_density;
        
        int m_last_fitness;
        
        static int m_cvalue;
        ;
};

int JManager::m_cvalue = 0;