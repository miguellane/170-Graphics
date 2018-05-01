
// Ensure the header file is included only once in multi-file projects
#ifndef SO_GROUND_H
#define SO_GROUND_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <gsim/gs_image.h>
// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoGround : public GlObjects {
private:
	GlMaterial material;
	float shine;
	gsuint imageID;
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec>   N; // normals
	std::vector<GsVec2>	T;
	int _numpoints;         // saves the number of points

public:
	SoGround();
	void init(const GlProgram& prog, GlMaterial& mat, float shine);
	void build();
	void draw(GsMat& tr, GsMat& pr, GlLight& light);
};

#endif
