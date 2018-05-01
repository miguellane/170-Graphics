
// Ensure the header file is included only once in multi-file projects
#ifndef SO_SCORPION_H
#define SO_SCORPION_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "so\so_torso.h"
# include "so\so_leg.h"
# include "so\so_claw.h"
# include "so\so_tail.h"
// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoScorpion : public GlObjects
 { private :
	 GlMaterial material;
	 float shine;
	 SoClaw lClaw, rClaw;
	 SoLeg l1Leg, r1Leg, l2Leg, r2Leg, l3Leg, r3Leg, l4Leg, r4Leg;
	 SoTorso torso;
	 SoTail tail;
   public :
    SoScorpion ();
	 void init(const GlProgram& prog, GlMaterial& mat, float shine);
    void build (int nface);
	 void draw(GsMat& tr, GsMat& pr, GlLight& light, float deg1, float deg2, float deg3, float deg4, float deg5);
 };

#endif // SO_CAPSULE_H
