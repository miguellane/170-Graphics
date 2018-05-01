
// Ensure the header file is included only once in multi-file projects
#ifndef SO_SURFACE_H
#define SO_SURFACE_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <gsim/gs_image.h>
# include <math.h>
// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoSurface : public GlObjects {
private:
	GlMaterial material;
	float sh;
	gsuint ID;
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec>   N; // normals
	std::vector<GsVec2>	T;
	int _numpoints;         // saves the number of points

	bool func(int type, const GsVec& vec);
	void calcTet(bool aIn, bool bIn, bool cIn, bool dIn,
		const GsVec& a, const GsVec& b, const GsVec& c, const GsVec& d
	);
	void calcSqr(bool aIn, bool bIn, bool cIn, bool dIn, bool eIn, bool fIn, bool gIn, bool hIn,
		const GsVec& a, const GsVec& b, const GsVec& c, const GsVec& d, const GsVec& e, const GsVec& f, const GsVec& g, const GsVec& h
	);


public:
	SoSurface();
	void init(const GlProgram& prog, GlMaterial mat);
	void build(int type, int size);
	void draw(GsMat& tr, GsMat& pr, GlLight& light, float shade, bool texture);
};

#endif
