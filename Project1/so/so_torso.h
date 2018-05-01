
// Ensure the header file is included only once in multi-file projects
#ifndef SO_TORSO_H
#define SO_TORSO_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "so\so_capsule.h"
// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoTorso : public GlObjects
 { private :
	 GlMaterial material;
	 float shine;
	 SoCapsule p1, p2, p3, p4, p5, p6, p7, p8, p9;
   public :
    SoTorso ();
	 void init(const GlProgram& prog, GlMaterial& mat, float shine);
    void build (int nface);
	 void draw(GsMat& tr, GsMat& pr, GlLight& light);
 };

#endif // SO_CAPSULE_H
