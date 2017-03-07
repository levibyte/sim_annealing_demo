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

#include <SDL2/SDL_ttf.h>

//#include "core.h"


float distance(const SDL_Point& a, const SDL_Point& b){
  return sqrt(pow(abs(a.x - b.x), 2) + pow(abs(a.y - b.y), 2));
}

bool operator<(const SDL_Point &lhs, const SDL_Point &rhs) {
    return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
}


void draw_text(std::string s, const SDL_Point& p )
{
      
      

      SDL_Color c = {0, 0, 0};  
      SDL_Surface* surfaceMessage = TTF_RenderText_Solid(gTTF_font, s.c_str(), c); 
      SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage); 

      SDL_Rect Message_rect; //create a rect
      Message_rect.x = p.x;  //controls the rect's x coordinate 
      Message_rect.y = p.y; // controls the rect's y coordinte
      Message_rect.w = 25; // controls the width of the rect
      Message_rect.h = 25; // controls the height of the rect

      SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect); //you put the renderer's name 
}


class JInstance {
    
    public: 
	JInstance() {
	    m_color.r = rand()%255;
	    m_color.g = rand()%255;
	    m_color.b = rand()%255;
	    m_color.a = 255;
            
            m_name = "zyrrrnaaa";
            
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
        
        //std::count_if(seen_pcer.begin(), seen_pcer.end(), [,](int x, int y) { (x>0 && JManager::mypred() > 0)});
  
};

class JManager {
  
  public:  
    
	JManager():m_layers_cnt(9),m_max_per_clm(7),m_conn_density(1),m_last_fitness(0) {
	  m_layers.resize(m_layers_cnt);
	  srand(time(0));
	  init_data();
          
          m_last_fitness = calc_intersections();
          //print_dbg();
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
          
          
         SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
         SDL_RenderClear( gRenderer );
           
	  for(unsigned i=0; i<m_layers_cnt; i++)
	    for (unsigned int j=0; j<m_layers[i].size(); j++ ) {
	      draw_circle(m_layers[i][j]->get_center(),5,m_layers[i][j]->get_color());  
	      //draw_text(m_layers[i][j]->get_name(),m_layers[i][j]->get_center());
	    }
	    
	    std::multimap<JInstance*,JInstance*>::iterator i;
	    for(i=m_connections.begin();i!=m_connections.end();++i)
	      SDL_RenderDrawLine(gRenderer,(*i).first->get_center().x,(*i).first->get_center().y,
					   (*i).second->get_center().x,(*i).second->get_center().y);
	    SDL_RenderPresent( gRenderer );
             
	}

        void update() {
          simulate_annealing();
          //print_dbg();
          //calc_intersections();
          //draw();
          //undo_permute();
          //calc_intersections();
        }



        float decrease_temperature( float t, int k) {
           //return t * 0.1 / k; 
           return t * 0.1 / k; 

        }

        float get_transition_probability(int E, float T ) {
          return exp(-E/T);
        }
          
        void try_make_movement(int E, float T) {
            float p = get_transition_probability(E,T);
            float v = rand();

            //std::cout << "p is " << p << " , v is" << v << std::endl;
            if( v >= p ) {
                std::cout << "Nooooo" << std::endl;
                undo_permute();
            } else {
              assert(0);
                std::cout << "Accepted" << std::endl;
            }
        }
          

        void simulate_annealing() {
              float T = 100000.0;
              float current_t = T;
              float cold_t = 0.5;
              int k = 1;
              
              while ( current_t > cold_t ) {
                  //std::cout << current_t << std::endl;
                  add_change();
                  int c = calc_intersections(); 
                 
                  if ( c <= m_last_fitness ) 
                    m_last_fitness = c;
                  else
                    try_make_movement(c,current_t);
             
                   
                   //current_t = T/log(k);
                   //k = k + 0.1;
                   current_t = decrease_temperature(T,k);
                  
                   k = k + 1;
              }
              
              std::cout << "SCORE:" << m_last_fitness << std::endl;
        }
        
        /*
        void place() {
          //add_change();
          
          //*            
            int i = 0; 
            while ( true ) { 
              add_change();
              int c = calc_intersections(); 
              if ( c <= m_last_fitness ) {
                m_last_fitness = c; 
                //break;
              }
              else 
                undo_permute();
              i++;  
            }
              //draw();
            std::cout << "LAST:" <<  m_last_fitness << std::endl;
        }
        */

//FIXME
        std::pair<int,int> get_id_by_inst(JInstance* inst) {
            //unsigned int i=0;
           // unsigned int j=0;
            
            unsigned int a=0;
            unsigned int b=0;
            
            bool found = false;
            
            for(unsigned int i=0; i<m_layers_cnt; ++i ) {
                for (unsigned int j=0; j<m_layers[i].size(); ++j )  {
                  //std::cout << i << " " << j << "::: " << inst << " ?"  << m_layers[i][j] << std::endl;               
                  if ( m_layers[i][j] == inst ) { a=i; b=j; found=true; break; }
                }    
               if (found) break;
            }    
              //std::cout << "               " << a << "::" << b << std::endl;    
              
           return std::make_pair<int,int>(a,b);
        }
        
        std::vector<int> get_real_vect(const std::vector<JInstance*>& iv) {
          std::vector<int> ov;
          for(int i=0;i<iv.size();i++) ov.push_back(get_id_by_inst(iv[i]).second);
          
          return ov;
        }
        
        
        void print_dbg() {
         std::cout << "DBG begin: " << std::endl;
          for(unsigned int i=0; i<m_layers_cnt; i++ ) {
          for (unsigned int j=0; j<m_layers[i].size(); j++ ) {
           std::cout << "m_layers["<< i << "]["<< j << "] is " << m_layers[i][j] << " aka " << m_layers[i][j]->get_name() << std::endl;
           get_insts(m_layers[i][j]);
          }
           std::cout << std::endl;
          }
         std::cout << "DBG end: \n\n" << std::endl;
        }
        
        int calc_intersections() {
          m_cvalue = 0;
          int r = 0;
          for(unsigned int i=0; i<m_layers_cnt-1; i++ ) r = r + calc_intersection(i);
          
          //std::cout << "FI: " << r << std::endl;
          //std::cout << "************************\n\n" << std::endl;
          
          return r;
        }
        
        int calc_intersection(int i) {
           std::multiset<int> seen;
           int count = 0;
           
           //FIXME veryugly
           std::vector<int> real_v = get_real_vect(m_layers[i]);
           
           for (unsigned int j=0; j<real_v.size(); j++ ) {
             //std::cout << "apr:" << i << "--" << j << " known as " << m_layers[i][j]->get_name() << std::endl;
             count = count + count_intersections(seen,get_insts(m_layers[i][j]));
              //std::cout << "---" << std::endl;
           }
           //std::cout << " intersections for " << i << "-" << i+1 << "(" << count <<  ") \n\n\n" << std::endl;
        return count;
        }
        
        bool mypred(int a)
        {
            //std::cout <<"     mypred" << a << " " << m_cvalue << std::endl;
            //std::cout <<"mypred" << a << std::endl;
            //return true;
            return ( m_cvalue < a );
        }
        
        int count_intersections(std::multiset<int>& seen, const std::vector<JInstance*>& v1) {
            std::vector<int>::const_iterator i;
            
           // std::set<int>::iterator si = seen.begin();
           std::vector<int> v = get_real_vect(v1);
           m_cvalue = 0; 
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
            // std::cout << "  set size--" << seen.size() << std::endl;
             //std::cout << "  processing " <<  m_layers[get_id_by_inst(*i).first][get_id_by_inst(*i).second]->get_name() << std::endl;
            // m_cvalue = (*i)->get_center().y;
              m_cvalue = (*i);
              //std::count_if(seen.begin(), seen.end(),f );
              res = res + std::count_if(seen.begin(), seen.end(), std::bind1st(std::mem_fun(&JManager::mypred),1));
              //std :: cout << "   res = " << res << std::endl;
              seen.insert(m_cvalue);
              
             // std::cout << "   set inserted -!-" << m_cvalue << std::endl;
              m_cvalue = 0;
            }
           // std::cout << "  -!-" << res << std::endl;
         return res;
        }
 
 
 
 
 
        //FIXME!
        std::vector<JInstance*> get_insts(JInstance* i) {
             
              std::vector<JInstance*> v;//, v2;
              //std::vector<int> v1;
              
               std::multimap<JInstance*,JInstance*>::iterator itlow = m_connections.lower_bound(i);  
               std::multimap<JInstance*,JInstance*>::iterator itup = m_connections.upper_bound(i);   
                std::multimap<JInstance*,JInstance*>::iterator it;

              for (it=itlow; it!=itup; ++it) {
                v.push_back((*it).second);
                //std::cout << "  -> " << (*it).second << " (" << (*it).second->get_name() << ") " << std::endl;
                //get_id_by_inst((*it).second);
              }
              
             //std::cout << " SIZE=" << v.size() << std::endl;
            // v = get_real_vect(v1);
             //for (int=0; i!=v.size(); ++i) {
               
             //}
             
             
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
	      //permute_two_rand_instances_in_layer(1);
	      //int count = rand()%m_layers_cnt;
	      //for(unsigned int i=0; i<count; i++ ) permute_two_rand_instances_in_layer(rand()%m_layers_cnt);
              
              permute_two_rand_instances_in_layer(rand()%m_layers_cnt);
	}
        
        void permute_two_rand_instances_in_layer(int ln) {
	      //assert(0);
	      int f = rand()%m_layers[ln].size();
	      int s = rand()%m_layers[ln].size();
	      if ( f == s ) return;
	      
	      //std::cout << "permuting m_layers["<< ln << "]["<< f << "] <=> m_layers["<< ln << "]["<< s << "]" << std::endl;
	      //std::swap(m_layers[ln][f],m_layers[ln][s]);
	      //m_layers[ln][f]
	      // m_layers[ln][s] = m_layers[ln][f];
              //m_layers[ln][s] = tmp;
	      
	      if ( m_permuted.size() > 0 ) m_permuted.erase(m_permuted.begin(),m_permuted.begin()+m_permuted.size());
               if ( m_fixme_permuted.size() > 0 ) m_fixme_permuted.erase(m_fixme_permuted.begin(),m_fixme_permuted.begin()+m_fixme_permuted.size());
	      
              std::pair<JInstance*,JInstance*> k;
              if ( m_layers[ln][f]->get_center().y > m_layers[ln][s]->get_center().y )
                k = std::make_pair<JInstance*,JInstance*>(m_layers[ln][f],m_layers[ln][s]);
              else
                k = std::make_pair<JInstance*,JInstance*>(m_layers[ln][s],m_layers[ln][f]);
                
              std::pair<int,int> q1(s,f);
              std::pair<std::pair<int,int>,int> q2(q1,ln);
              
              m_permuted.push_back(k);
              m_fixme_permuted.push_back(q2);
	      //m_permuted.push_back()
              
              //FIXME mess and add delete
              
              permute_two_instances(k.first,k.second);
              std::swap(m_layers[ln][f],m_layers[ln][s]);
              //JInstance* tmp =  m_layers[ln][f];
              //m_layers[ln][f] = new JInstance(*m_layers[ln][s]);
              //m_layers[ln][s] = new JInstance(*tmp);
	     
               
              
              //m_connections.insert(k);
              
	      //f=1;
	      //s=2;
	      
	      //std::cout << "ptri" << ln << ": " << f << "-->" << s << std::endl;

	      /**/
	      //int count = rand()%m_layers[ln].size();
	      //for(unsigned init i=0; i<count; i++ ) permute_rand_instances_in_layer(rand()%m_layers_cnt);
	}
	
	//void permute_two_instances_wrap(int c, int s, int m)
	
	//FIXME!!!
	void permute_two_instances(JInstance* f, JInstance* s) {
              
              //JInstance* tmp1 = s;
              //s = f;
              //f = tmp1;

              SDL_Point ns1 = s->get_center();
               SDL_Point nf1 = f->get_center();
               SDL_Point tmp2 = s->get_center();
               
              SDL_Point ns = s->get_center();
               SDL_Point nf = f->get_center();
               SDL_Point tmp = s->get_center();
               
             /*
               for(float i=s->get_center().y; i < f->get_center().y; i=i+50.0 ) {
                 ns.y = i;
                 s->set_center(ns);
                 draw();
              }
              
             for(float i=f->get_center().y; i > tmp.y; i=i-50.0 ) {
                 nf.y = i;
                 f->set_center(nf);
                 draw();
              }
              /**/
              
              //SDL_Point tmp = s->get_center();
              s->set_center(nf1);
              f->set_center(ns1);
              draw();
                            
            
              
              draw();
        }
        
        void undo_permute() {
          std::vector<std::pair<JInstance*,JInstance*> >::iterator i;
          int q = 0;
          for (i=m_permuted.begin();i!=m_permuted.end();i++) 
          { 
            permute_two_instances((*i).second,(*i).first);
            std::swap(m_layers[m_fixme_permuted[q].second][m_fixme_permuted[q].first.second],m_layers[m_fixme_permuted[q].second][m_fixme_permuted[q].first.first]);
            q++;
          }
        }
	
        void init_data() {
	    for (unsigned int i=0; i<m_layers_cnt; i++ ) {
	      int perclm = 1+rand()%m_max_per_clm;
	      //int perclm = m_max_per_clm;
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
        std::vector< std::pair< std::pair<int,int>, int > > m_fixme_permuted;
        
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

//FIXME
int JManager::m_cvalue = 0;