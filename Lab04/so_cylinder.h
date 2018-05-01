
// Ensure the header file is included only once in multi-file projects
#ifndef SO_CYLINDER_H
#define SO_CYLINDER_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <cmath>
// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoCylinder : public GlObjects{
	private :
		GlMaterial material;
		std::vector<GsVec>   P; // coordinates
		std::vector<GsVec>   N; // normals
		int _numpoints;         // saves the number of points

	public :
		SoCylinder ();
    void init ( const GlProgram& prog);
    void build ( int nface, bool smooth );
    void draw ( GsMat& tr, GsMat& pr , GlLight& light);
 };

#endif
