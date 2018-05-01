#ifndef SO_MODEL_H
#define SO_MODEL_H

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
class SoModel : public GlObjects {
private:
	GlMaterial material;
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec>   N; // normals
	Model model;
	int _numpoints;         // saves the number of points

public:
	SoModel();
	void init(const GlProgram& prog, char* file, GlMaterial mat);
	void build(bool smooth);
	void draw(GsMat& tr, GsMat& pr, GlLight& light);
};

#endif
