#ifndef algo_h
#define algo_h

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "logic.h"


class JSimulateAnnealingImpl {

    public:
	virtual int  revert_change() = 0;
	virtual int  make_change()   = 0;
	virtual void post_process()  = 0;
};

class JSimulateAnnealing  { 

    public:
	JSimulateAnnealing(JSimulateAnnealingImpl* i):m_impl(i) {
	    m_prev_res = 99999999;
	    /*
	    T0 = 100000.0;
	    Tmin = 0.01;
	    alpha = 0.9;
	    delta_t = 0.01;
	    k_step = 0.01;
	    m_prev_res = 999999;
	    */
	}

	int revert_change() {
	   return m_impl->revert_change();
	   //return m_prev_res;
	}
	
	int make_change_and_calculate_result() {
	      int r(m_impl->make_change());
	      m_last_res = r;
	      std::cout << " make change: " << r << std::endl;
	      return r;
	}
	
	void post_process() {
	    m_impl->post_process();
	}

	float decrease_temperature( float t, int k) {
           //return T0*pow(alpha,k);
	   //return t * 0.1 / k; 
        }

        float get_transition_probability(int E, float T ) {
          std::cout << m_prev_res << "----" << E << " = "<< m_prev_res-E << std::endl;
	  return exp(-E/T);
        }
          
        int try_make_unbeneficial_movement(int E, float T) {
            float p = get_transition_probability(E,T);
	    float v = ((double) rand() / (RAND_MAX)) ;
    
	    assert( p>0 || p<1 );
  	    assert(E>m_prev_res);
	    std::cout << " v is " << v << " p is " << p << std::endl;
		
	    if( v >= p ) {
		  std::cout << "revert" << std::endl;
		  m_last_res=revert_change();
		  return m_last_res;
	    }
	    
	    //assert(0);
	    std::cout << "accept" << std::endl;
	    return E;
	}

	void simulate() {
              float T0 = 100000.0;
              float Tmin = 0.01;
              float alpha = 0.9;
              float delta_t = 0.1;
	      float step_k = 0.01;
	      
	      float k = 1.0;
	      float current_T = T0;
              while ( current_T > Tmin ) {
                  std::cout << "temp = " << current_T << std::endl;
        
		  make_change_and_calculate_result();
          	  
		  if ( m_last_res > m_prev_res ) 
                    try_make_unbeneficial_movement(m_last_res,current_T);
             
		  current_T = T0*pow(alpha,k);

		  m_prev_res = m_last_res;
		  k = k + step_k;
		  post_process();
              }
        }
        
	/*
        void simulate() {
	      
	      float k = 1.0;
	      float T = T0;
	      int res = 0;
	     
              while ( T > Tmin ) {
		  std::cout << "Temp" << T << std::endl;
		  
		  res = make_change_and_calculate_result();
		  if ( res > m_prev_res ) 
                    res = try_make_unbeneficial_movement(res,T);
	      
		  std::cout << "PRes:" << m_prev_res << " CRes:" << res << std::endl;
		
		  m_prev_res = res;
		  T = decrease_temperature(T,k);
		  k = k + k_step;
		  post_process();
   		  std::cout << "\n";
	      }
	}
	*/

  
private:
	int m_prev_res;
	int m_last_res;
	//float T0;
	//float Tmin;
	//float alpha;
	//float delta_t;
	//float k_step;

	JSimulateAnnealingImpl* m_impl;
	
  
};	

#endif