
// Ensure the header file is included only once in multi-file projects
#ifndef SO_CAPSULE_H
#define SO_CAPSULE_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoCapsule : public GlObjects
{
private:
	GlMaterial material;
	float shine;
	std::vector<GsVec> P;
	std::vector<GsVec> N;
	int _numpoints;         // saves the number of points

public:
	SoCapsule();
	void init(const GlProgram& prog, GlMaterial& mat, float shine);
	void build(int nface, float len, float r4x, float r4y, float r3x, float r3y, float r2x, float r2y, float r1x, float r1y);
	void draw(GsMat& tr, GsMat& pr, GlLight& light);
};

#endif
