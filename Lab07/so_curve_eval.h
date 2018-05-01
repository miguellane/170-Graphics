
// Ensure the header file is included only once in multi-file projects
#ifndef SO_CURVE_EVAL_H
#define SO_CURVE_EVAL_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <vector>

void eval_bezier(const GsArray<GsVec>& ctrlpnts, GsArray<GsVec>& curvepnts, int segments);
void eval_lagrange(const GsArray<GsVec>& ctrlpnts, GsArray<GsVec>& curvepnts, int segments);
#endif
