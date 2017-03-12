#ifndef algo_h
#define algo_h

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "logic.h"


class JSimulateAnnealingImpl {

    public:
	virtual void  revert_change() = 0;
	virtual int  make_change()   = 0;
	virtual void post_process()  = 0;
};

class JSimulateAnnealing  { 

    public:
	JSimulateAnnealing(JSimulateAnnealingImpl* i):m_impl(i) {
	    
	    //*
	    T0 = 1000.0;
	    Tmin = 0.1;
	    alpha = 0.9;
	    delta_t = 0.01;
	    k_step = 0.1;
	    m_prev_res = 999999;
	    /**/
	}

	int revert_change() {
	   m_impl->revert_change();
	   return m_prev_res;
	}
	
	int make_change_and_calculate_result() {
	    return m_impl->make_change();
	}
	
	void post_process() {
	    m_impl->post_process();
	}

	float decrease_temperature( float t, int k) {
           return T0*pow(alpha,k);
	   //return t * 0.1 / k; 
        }

        float get_transition_probability(int E, float T ) {
          //std::cout << m_prev_res << "----" << E << " = "<< m_prev_res-E << std::endl;
	  return exp(-E/T);
        }
          
        int try_make_unbeneficial_change(int E, float T) {
            float p = get_transition_probability(E,T);
	    float v = ((double) rand() / (RAND_MAX)) ;
    
	    assert( p>0 || p<1 );
  	    assert(E>m_prev_res);
	    //std::cout << " v is " << v << " p is " << p << std::endl;
		
	    if( v >= p ) {
		  //std::cout << "revert" << std::endl;
		  //m_last_res=revert_change();
		  return revert_change();
		  //return m_last_res;
	    }
	    
	    //assert(0);
	    //std::cout << "accept" << std::endl;
	    return E;
	}

	
	//*
        void simulate() {
	      
	      float k = 1.0;
	      float T = T0;
	      int res = 0;
	     
              while ( T > Tmin ) {
		  //std::cout << "Temp" << T << std::endl;
		  
		  res = make_change_and_calculate_result();
		  if ( res > m_prev_res ) 
                    res = try_make_unbeneficial_change(res,T);
	      
		  //std::cout << "PRes:" << m_prev_res << " CRes:" << res << std::endl;
		
		  m_prev_res = res;
		  T = decrease_temperature(T,k);
		  k = k + k_step;
		  post_process();
   		  
		  //std::cout << "\n";
	      }
	}
	/**/

  
private:
	int m_prev_res;
	int m_last_res;
	
	float T0;
	float Tmin;
	float alpha;
	float delta_t;
	float k_step;

	JSimulateAnnealingImpl* m_impl;
	
  
};	

#endif