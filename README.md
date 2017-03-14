simulated annealing ( and simple placement demo)
===
This is just-for-fun project which will try to provide "generic" code for simulated annealing.
Actual SA is in algo.h file, so you just need this single header if you like to use SA in your task.

Inorder to use, you need to have your own implementation dervied from default implementation.
Morelikely you will pass pointer to your logic to bind your logic with SA interfaces. 

```
include "algo.h"
class JMySimulateAnnealingImpl : public JSimulateAnnealingImpl
{
    public:
          JMySimulateAnnealingImpl(Logic* j):m_logic(j) { assert(m_logic); }
          virtual void revert_change() { m_logic->revert(); }
          virtual int  make_change() { return m_logic->change(); }
          virtual void post_process() { m_logic->draw(); }

    private:  
          Logic* m_logic;
};
```

Then you can use JSimulateAnnealing class wich will perform actual SA. 

```
        JSimulateAnnealingImpl* impl = new JMySimulateAnnealingImpl(this);
        JSimulateAnnealing j(impl,100000.0,10.0,0.01); //T0,Tmin,timestep;
        j.simulate();
```


More visual example goes within package (SDL needed).
Target is to reduce number of intercrossings by performing more optimal placement.

Starting with random placement ( each module can have only connections to modules in next adjcent column)
---
<img src="https://image.ibb.co/n428La/bsmall.png">

The optimized placement after applying SA.
---
<img src="https://image.ibb.co/cHreRF/asmall.png">

