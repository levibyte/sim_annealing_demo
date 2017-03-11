#ifndef instance_h
#define instance_h

#include <SDL.h>
#include <string>

class JInstance {
    
    public: 
	JInstance() {
	    m_color.r = rand()%255;
	    m_color.g = rand()%255;
	    m_color.b = rand()%255;
	    m_color.a = 255;
            
            m_name = "zurna-dhol";
            
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

#endif