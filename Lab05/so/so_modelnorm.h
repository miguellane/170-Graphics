
// Ensure the header file is included only once in multi-file projects
#ifndef SO_MODELNORM_H
#define SO_MODELNORM_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "model.h"
# include "ogl_tools.h"
# include <cmath>
// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoModelNorm : public GlObjects {
private:
	std::vector<GsVec>   P; // normal vector points
	std::vector<GsColor> C; // colors
	Model model;
	int _numpoints;         // saves the number of points

public:
	SoModelNorm();
	void init(const GlProgram& prog, char* file);
	void build(bool smooth);
	void draw(GsMat& tr, GsMat& pr);
};

#endif
