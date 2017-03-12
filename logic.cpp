#include "logic.h"
#include "instance.h"
#include "algo.h"
#include "renderer.h"


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

	    
        void JManager::action() {
	    JSimulateAnnealingImpl* impl = new JMySimulateAnnealingImpl(this);
	    JSimulateAnnealing j(impl);
	    j.simulate();
	    
	    calc_intersections();
	    std::cout << "BEGIN: " << m_start_res << " CURRENT: " << m_last_res << std::endl;
	    //<< " BEST: " << m_last_fitness << std::endl;
        }


	void JManager::revert() {
	      undo_permute();
	      //return calc_intersections(); 
	}

         int JManager::change() {
	      add_change();
	      return calc_intersections(); 
	}
       

	//FIXME
        std::pair<int,int> JManager::get_id_by_inst(JInstance* inst) {
            
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
        
        std::vector<int> JManager::get_real_vect(const std::vector<JInstance*>& iv) {
          std::vector<int> ov;
          for(int i=0;i<iv.size();i++) ov.push_back(get_id_by_inst(iv[i]).second);
          
          return ov;
        }
        
        
        void JManager::print_dbg() {
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
        
        int JManager::calc_intersections() {
          int r = 0;
          for(unsigned int i=0; i<m_layers_cnt-1; i++ ) r = r + calc_intersection(i);
          
          //std::cout << "FI: " << r << std::endl;
          //std::cout << "************************\n\n" << std::endl;
          m_last_res = r;
          return r;
        }
        
        int JManager::calc_intersection(int i) {
           std::multiset<int> seen;
           int count = 0;
           
           //FIXME veryugly
           std::vector<int> real_v = get_real_vect(m_layers[i]);
           
           for (unsigned int j=0; j<real_v.size(); j++ ) {
             count = count + count_intersections(seen,get_insts(m_layers[i][j]));
           }
 
	   return count;
        }
        
        
        int JManager::count_intersections(std::multiset<int>& seen, const std::vector<JInstance*>& v1) {
	    std::vector<int>::const_iterator i;
	    std::vector<int> v = get_real_vect(v1);
	    std::sort(v.begin(),v.end());
	      int res = 0;
	      for(i=v.begin();i!=v.end();++i) {
		res = res + std::count_if(seen.begin(), seen.end(), ay_qez_ban(*i));
		seen.insert(*i);
	      }

	   return res;
        }
 
 
        //FIXME!
        std::vector<JInstance*> JManager::get_insts(JInstance* i) {
             
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
        
        
        void JManager::do_and_draw() {
        
	  
	}
        
        void JManager::add_change() {
	      //int count = rand()%m_layers_cnt;
	      //for(unsigned int i=0; i<count; i++ ) permute_two_rand_instances_in_layer(rand()%m_layers_cnt);
              
              while ( ! permute_two_rand_instances_in_layer(rand()%m_layers_cnt) );
	}
        
        bool JManager::permute_two_rand_instances_in_layer(int ln) {
	      //assert(0);
	      int f = rand()%m_layers[ln].size();
	      int s = rand()%m_layers[ln].size();
	      if ( f == s ) return false;
	      
	      //std::cout << "permuting m_layers["<< ln << "]["<< f << "] <=> m_layers["<< ln << "]["<< s << "]" << std::endl;
	      //std::swap(m_layers[ln][f],m_layers[ln][s]);
	      //m_layers[ln][f]
	      // m_layers[ln][s] = m_layers[ln][f];
              //m_layers[ln][s] = tmp;
	      
	      if ( !m_permuted.empty() ) m_permuted.erase(m_permuted.begin(),m_permuted.begin()+m_permuted.size());
               if ( !m_fixme_permuted.empty() ) m_fixme_permuted.erase(m_fixme_permuted.begin(),m_fixme_permuted.begin()+m_fixme_permuted.size());
	      
              std::pair<JInstance*,JInstance*> k;
              if ( m_layers[ln][f]->get_center().y > m_layers[ln][s]->get_center().y )
                k = std::make_pair<JInstance*,JInstance*>(m_layers[ln][f],m_layers[ln][s]);
              else
                k = std::make_pair<JInstance*,JInstance*>(m_layers[ln][s],m_layers[ln][f]);
                
              std::pair<int,int> q1(s,f);
              std::pair<std::pair<int,int>,int> q2(q1,ln);
              
              m_permuted.push_back(k);
              m_fixme_permuted.push_back(q2);
              
              //FIXME mess and add delete
              m_renderer->draw_permute_two_instances(k.first,k.second);
              std::swap(m_layers[ln][f],m_layers[ln][s]);
	      
	      return true;
	}
	
        
        void JManager::undo_permute() {
	    if (m_permuted.empty()) return;
	  
	    std::vector<std::pair<JInstance*,JInstance*> >::iterator i;
	    
	    int q = 0;
	    for (i=m_permuted.begin();i!=m_permuted.end();i++) 
	    { 
	      m_renderer->draw_permute_two_instances((*i).second,(*i).first);
	      std::swap(m_layers[m_fixme_permuted[q].second][m_fixme_permuted[q].first.second],m_layers[m_fixme_permuted[q].second][m_fixme_permuted[q].first.first]);
	      q++;
	    }
	    
	    if ( !m_permuted.empty() ) m_permuted.erase(m_permuted.begin(),m_permuted.begin()+m_permuted.size());
	    if ( !m_fixme_permuted.empty() ) m_fixme_permuted.erase(m_fixme_permuted.begin(),m_fixme_permuted.begin()+m_fixme_permuted.size());
        }
	

	void JManager::init_data() {
	    for (unsigned int i=0; i<m_layers_cnt; i++ ) {
	      //int perclm = 1+rand()%m_max_per_clm;
	      int perclm = m_max_per_clm;
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
		   //add_preds();
		   //int last_col_size = m_layers[i-1].size();
		   //JInstance* ri = m_layers[i-1][]
		   //*
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
                    /**/
		  }
		}
	      }
	    }
	//std::cout << "----------------" << std::endl;
        }
         
	void JManager::draw() {
	    SDL_SetRenderDrawColor( m_renderer->get(), 0xFF, 0xFF, 0xFF, 0xFF );
	    SDL_RenderClear( m_renderer->get() );
	    //* 
	    for(unsigned i=0; i<m_layers_cnt; i++)
	      for (unsigned int j=0; j<m_layers[i].size(); j++ ) {
		m_renderer->draw_circle(m_layers[i][j]->get_center(),5,m_layers[i][j]->get_color());  
		//std::cout << "ayqezban" << std::endl;
		//draw_text(m_layers[i][j]->get_name(),m_layers[i][j]->get_center());
	      }
	      
	      std::multimap<JInstance*,JInstance*>::iterator i;
	      for(i=m_connections.begin();i!=m_connections.end();++i)
		SDL_RenderDrawLine(m_renderer->get(),(*i).first->get_center().x,(*i).first->get_center().y,
					      (*i).second->get_center().x,(*i).second->get_center().y);
	      /**/
	      SDL_RenderPresent( m_renderer->get() );
	}


