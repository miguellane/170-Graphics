#ifndef SO_CLOCK_H
#define SO_CLOCK_H

# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <cmath>

class SoClock : public GlObjects{
	private :
		std::vector<GsVec>   P; // coordinates
		std::vector<GsColor> C; // colors
		float deg;
		float r, sec;
		int _numpoints;

  public :
		SoClock ();
    void init ( const GlProgram& prog );
    void build (int milis);
    void draw ( GsMat& tr, GsMat& pr );
 };

#endif
