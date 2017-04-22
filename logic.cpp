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
	    
	    /*SDL_RenderClear( m_renderer->get() );
	    SDL_SetRenderDrawColor( m_renderer->get(), 0, 0, 0, 255);
	    //SDL_RenderClear( m_renderer->get() );
	    SDL_RenderPresent( m_renderer->get() );
	    */
	    
	    //*
	    JSimulateAnnealingImpl* impl = new JSimulateAnnealingMyImpl(this);
                                        //T0,  Tmin, time_step
	    JSimulateAnnealing j(impl,10000.0 , 0.1 , 0.01);
	    j.simulate();
            delete impl;
            /**/
	    
            /*
	    add_change();
            */
            
	    calc_intersections();
	    draw();
	    std::cout << "BEGIN: " << m_start_res << " CURRENT: " << m_last_res << std::endl;
        }


	void JManager::revert() {
	      undo_permute();
	      //return calc_intersections(); 
	}

         int JManager::change() {
	      add_change();
	      return calc_intersections(); 
	}
       
        
        std::multiset<int> JManager::get_real_vect(const std::vector<JInstance*>& iv, bool dbg=false) {
          std::multiset<int> ov;
          for(int i=0;i<iv.size();i++) {
	    //if(dbg)  std::cout << "               name:" << iv[i]->get_name() << " pos:" << iv[i]->get_rownum() << std::endl;
	    ov.insert(iv[i]->get_rownum());
	  }
	  //if (dbg) std::cout << std::endl;
          
          return ov;
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
           
           std::multiset<int> real_v = get_real_vect(m_layers[i]);

	    //std::cout << "\n\n\n\n****intersections for " << i << "->" << i + 1 << "\n" ;

           for (unsigned int j=0; j<real_v.size(); j++ ) {
            // std::cout << "  ." << i << " "  << j <<" looking by the name of" << m_layers[i][j]->get_name() << std::endl;
	     count = count + count_intersections(seen,get_insts(m_layers[i][j]));
	   }
	    //std::cout << "intersections for " << i << "->" << i + 1 << " ***"<< count  << "*** \n \n" ;
	    return count;
        }
        
 
        int JManager::count_intersections(std::multiset<int>& seen, const std::vector<JInstance*>& v1) {
	      std::multiset<int> v = get_real_vect(v1,true);
	    
	      int res = 0;
	      for(std::multiset<int>::const_iterator i=v.begin();i!=v.end();++i) {
		
		res = res + std::count_if(seen.begin(), seen.end(), ay_qez_ban(*i));
		seen.insert(*i);
	      }

	   return res;
        }
 
 
        //FIXME!
        std::vector<JInstance*> JManager::get_insts(JInstance* i) {
              std::vector<JInstance*> v;//, v2;
               //std::cout << "      " << i->get_name() << " get_inst:" << std::endl;
               std::multimap<JInstance*,JInstance*>::iterator itlow = m_connections.lower_bound(i);  
               std::multimap<JInstance*,JInstance*>::iterator itup = m_connections.upper_bound(i);   
               std::multimap<JInstance*,JInstance*>::iterator it;

              for (it=itlow; it!=itup; ++it) { 
                v.push_back((*it).second);
 	        //std::cout << "       " << (*it).second->get_name() << std::endl;
	      
		
	      }
	      
	      //std::cout << "   " << i->get_namae() << " get_inst:" << std::endl;
               
             return v;
        }
        
       
        void JManager::do_and_draw() {
        
	  
	}
        
        void JManager::add_change() {
              while ( ! permute_two_rand_instances_in_layer(rand()%m_layers_cnt) );
	}
        
        //FIXME this part need to be rewrited
        bool JManager::permute_two_rand_instances_in_layer(int ln) {
	      //assert(0);
	      int f = rand()%m_layers[ln].size();
	      int s = rand()%m_layers[ln].size();
	      if ( f == s ) return false;
	      
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
              int tmp = m_layers[ln][f]->get_rownum();
              m_layers[ln][f]->set_rownum(m_layers[ln][s]->get_rownum());
	      m_layers[ln][s]->set_rownum(tmp);
	      
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
	      
              int tmp = m_layers[m_fixme_permuted[q].second][m_fixme_permuted[q].first.second]->get_rownum();
              m_layers[m_fixme_permuted[q].second][m_fixme_permuted[q].first.second]->set_rownum(m_layers[m_fixme_permuted[q].second][m_fixme_permuted[q].first.first]->get_rownum());
              m_layers[m_fixme_permuted[q].second][m_fixme_permuted[q].first.first]->set_rownum(tmp);
              
              
              std::swap(m_layers[m_fixme_permuted[q].second][m_fixme_permuted[q].first.second],m_layers[m_fixme_permuted[q].second][m_fixme_permuted[q].first.first]);
	      q++;
	    }
	    
	    if ( !m_permuted.empty() ) m_permuted.erase(m_permuted.begin(),m_permuted.begin()+m_permuted.size());
	    if ( !m_fixme_permuted.empty() ) m_fixme_permuted.erase(m_fixme_permuted.begin(),m_fixme_permuted.begin()+m_fixme_permuted.size());
        }
	


	void create_column() {
	  
	  
	}
	
	void JManager::init_data() {
	    for (unsigned int i=0; i<m_layers_cnt; i++ ) {
	      //int perclm = 1+rand()%m_max_per_clm;
	      int perclm = m_max_per_clm;
              m_layers[i].resize(perclm);
	      for (unsigned int j=0; j<perclm ; j++ ) {
		SDL_Point p;
		p.x = 20*i+10; p.y = 20*j+10;
		JInstance* ii = new JInstance;
		ii->set_center(p);
		std::stringstream z;
                z << "i" << i << "-" << j;
                ii->set_name(z.str());
                z.str("");
		m_layers[i][j] = ii;
		ii->set_rownum(j);
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

        
        
        // FIXME should be moved.
	void JManager::draw() {
	    SDL_SetRenderDrawColor( m_renderer->get(), 0xFF, 0xFF, 0xFF, 0xFF );
	    SDL_RenderClear( m_renderer->get() );
	    //* 
	    for(unsigned i=0; i<m_layers_cnt; i++)
	      for (unsigned int j=0; j<m_layers[i].size(); j++ ) {
		m_renderer->draw_circle(m_layers[i][j]->get_center(),3,m_layers[i][j]->get_color());  
		//std::cout << "ayqezban" << std::endl;
		//m_renderer->draw_text(m_layers[i][j]->get_name(),m_layers[i][j]->get_center());
	      }
	      
	      std::multimap<JInstance*,JInstance*>::iterator i;
	      for(i=m_connections.begin();i!=m_connections.end();++i)
		SDL_RenderDrawLine(m_renderer->get(),(*i).first->get_center().x,(*i).first->get_center().y,
					      (*i).second->get_center().x,(*i).second->get_center().y);
	      /**/
	      SDL_RenderPresent( m_renderer->get() );
	}


