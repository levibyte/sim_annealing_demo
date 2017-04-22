#ifndef algo_h
#define algo_h

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>


class JSimulateAnnealingImpl {
    public:
	virtual void  revert_change() = 0;
	virtual int  make_change()   = 0;
    public:  
	virtual void post_process_time_step()  {}
	virtual float decrease_temperature(float T0, float T, float time) {
	    float alpha = 0.9;
	    //float beta = 10;
	    //float delta_t = 1.0;
	    //return T0 - beta*time;
	    //return T0/(1+alpha*log(1+time));	
	    //return T0/(1+alpha*time);	
	    return T0*pow(alpha,time);
	  }

	virtual  float get_transition_probability(int E, float T ) {
            //std::cout << m_prev_res << "----" << E << " = "<< m_prev_res-E << std::endl;
	    return exp(-E/T);
	  }

};

class JSimulateAnnealing  { 

    public:
	JSimulateAnnealing(JSimulateAnnealingImpl* i, float T0, float Tmin, float time_step):
	T0(T0),Tmin(Tmin),time_step(time_step),m_impl(i),m_prev_res(99999) {
	}

	int revert_change() {
	   m_impl->revert_change();
	   return m_prev_res;
	}
	
	int make_change_and_calculate_result() {
	    return m_impl->make_change();
	}
	
	void post_process_time_step() {
	    m_impl->post_process_time_step();
	}

        int try_make_unbeneficial_change(int E, float T) {
            float p = m_impl->get_transition_probability(E,T);
	    float v = ((double) rand() / (RAND_MAX)) ;
    
	    assert( p>0 || p<1 );
  	    assert(E>m_prev_res);
	    //std::cout << " v is " << v << " p is " << p << std::endl;
		
	    if( v >= p ) {
		  return revert_change();
	    }
	    
	    //assert(0);
	    //std::cout << "accept" << std::endl;
	    return E;
	}

	
	//*
        void simulate() {
	      
	      float time = 1.0;
	      float T = T0;
	      int res = 0;
	     
              while ( T > Tmin ) {
		  //std::cout << "Temp" << T << std::endl;
		  
		  res = make_change_and_calculate_result();
		  if ( res > m_prev_res ) 
                    res = try_make_unbeneficial_change(res,T);
	      
		  //std::cout << "PRes:" << m_prev_res << " CRes:" << res << std::endl;
		
		  m_prev_res = res;
		  T = m_impl->decrease_temperature(T0,T,time);
		  assert(T0>T);
		  time = time + time_step;
		  post_process_time_step();
   		  
		  //std::cout << "\n";
	      }
	}
	/**/

  
private:
	int m_prev_res;
	int m_last_res;
	
	float T0;
	float Tmin;
	float time_step;

	JSimulateAnnealingImpl* m_impl;
};	

#endif