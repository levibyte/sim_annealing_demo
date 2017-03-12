#ifndef logic_h
#define logic_h


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

#include "init.h"
#include "instance.h"
#include "algo.h"
#include "renderer.h"
	
class JManager {
  
  public:  
   
	 class ay_qez_ban {
	      public:
		  ay_qez_ban(int v):m_v(v) {}
	      public:
		    bool operator()(int a) {
			return (a > m_v);
		    }
		    
		    int m_v;
	   };
    
	JManager(JRenderer* r):m_renderer(r),m_layers_cnt(5),m_max_per_clm(5),m_conn_density(1),m_last_res(0) {
	  
	  m_layers.resize(m_layers_cnt);
	  srand(time(0));
	  init_data();
          
          m_start_res = calc_intersections();
	  
	  //m_last_fitness;
          //print_dbg();
	  draw();
          std::cout << "BEGIN: " <<  m_start_res << std::endl;
	}

	    
        void action();
	void revert();
        int change();
	
        std::pair<int,int> get_id_by_inst(JInstance* inst);
        std::vector<int> get_real_vect(const std::vector<JInstance*>& iv);
        void print_dbg();
        int calc_intersections();
        int calc_intersection(int i);
        int count_intersections(std::multiset<int>& seen, const std::vector<JInstance*>& v1);
        std::vector<JInstance*> get_insts(JInstance* i);
        void do_and_draw();
        void add_change();
        bool permute_two_rand_instances_in_layer(int ln);
        void undo_permute();
	void init_data();
	void draw();

  private:
	std::vector< std::pair<JInstance*,JInstance*> > m_permuted;
        std::vector< std::pair< std::pair<int,int>, int > > m_fixme_permuted;
        
        std::vector< std::vector<JInstance*> > m_layers;
	std::multimap<JInstance*,JInstance*> m_connections;
	
	int m_max_per_clm;
	int m_layers_cnt;
	int m_conn_density;
        
	int m_start_res;
	int m_last_res;

	JRenderer* m_renderer;
};


class JMySimulateAnnealingImpl : public JSimulateAnnealingImpl
{
    public:
	  JMySimulateAnnealingImpl(JManager* j):m_logic(j) { assert(m_logic); }
	  virtual void revert_change() { m_logic->revert(); }
	  virtual int  make_change() { return m_logic->change(); }
	  virtual void post_process() { m_logic->draw(); }

    private:  
	  JManager* m_logic;
};


#endif