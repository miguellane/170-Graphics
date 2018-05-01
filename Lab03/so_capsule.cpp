
# include "so_capsule.h"

SoCapsule::SoCapsule()
 {
   _numpoints = 0;
 }

void SoCapsule::init ( const GlProgram& prog )
 {
   // Define buffers needed:
   set_program ( prog );
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   uniform_locations ( 2 ); // will send 2 variables: the 2 matrices below
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );
 }

void SoCapsule::build ( float len, float rt, float rb, int nface )
 {
   P.clear(); C.clear(); // set size to zero, just in case
   //P.reserve(18); C.reserve((nface * 6)+(nface * nface * 6)); // reserve space to avoid re-allocations from the calls below
   
   ztop = len / 2;
   zbot = -1 * len / 2;
   pi = 3.1415926535897;
   theta = 2 * pi / nface;

   for (int i = 0; i < nface; i++) {
	  //left and right xy unit vectors
	  ly = cos(i*theta); lx = sin(i*theta);
	  ry = cos((i+1)*theta); rx = sin((i+1)*theta);
	  //Column
	  //TopLeft, TopRight, BotLeft
	  P.push_back(GsVec(rt*lx, ztop, rt*ly));
	  P.push_back(GsVec(rt*rx, ztop, rt*ry));
	  P.push_back(GsVec(rb*lx, zbot, rb*ly));
	  //TopRight, BotLeft, BotRight
	  P.push_back(GsVec(rt*rx, ztop, rt*ry));
	  P.push_back(GsVec(rb*lx, zbot, rb*ly));
	  P.push_back(GsVec(rb*rx, zbot, rb*ry));
	  for (int j = 0; j < 6; j++)
		  C.push_back(GsColor::white);
//		  C.push_back(GsColor::random());
      for (int j = 0; j < nface/4; j++){
		  //TopDome
		  //TopLeft, TopRight, BotLeft
		  P.push_back(GsVec(rt*lx*cos((j + 1)*theta), ztop+rt*sin((j + 1)*theta), rt*ly*cos((j + 1)*theta)));
		  P.push_back(GsVec(rt*rx*cos((j + 1)*theta), ztop+rt*sin((j + 1)*theta), rt*ry*cos((j + 1)*theta)));
		  P.push_back(GsVec(rt*lx*cos(j*theta), ztop+rt*sin(j*theta), rt*ly*cos(j*theta)));
		  //TopRight, BotLeft, BotRight
		  P.push_back(GsVec(rt*rx*cos((j + 1)*theta), ztop + rt*sin((j + 1)*theta), rt*ry*cos((j + 1)*theta)));
		  P.push_back(GsVec(rt*lx*cos(j*theta), ztop + rt*sin(j*theta), rt*ly*cos(j*theta)));
		  P.push_back(GsVec(rt*rx*cos(j*theta), ztop + rt*sin(j*theta), rt*ry*cos(j*theta)));
		  for (int j = 0; j < 6; j++)
			  C.push_back(GsColor::red);
//              C.push_back(GsColor::random());
          //BotDome
		  //TopLeft, TopRight, BotLeft
		  P.push_back(GsVec(rb*lx*cos(-j*theta), zbot + rb*sin(-j*theta), rb*ly*cos(-j*theta)));
		  P.push_back(GsVec(rb*rx*cos(-j*theta), zbot + rb*sin(-j*theta), rb*ry*cos(-j*theta)));
		  P.push_back(GsVec(rb*lx*cos(-(j+1)*theta), zbot + rb*sin(-(j+1)*theta), rb*ly*cos(-(j+1)*theta)));
		  //TopRight, BotLeft, BotRight
		  P.push_back(GsVec(rb*rx*cos(-j*theta), zbot + rb*sin(-j*theta), rb*ry*cos(-j*theta)));
		  P.push_back(GsVec(rb*lx*cos(-(j + 1)*theta), zbot + rb*sin(-(j + 1)*theta), rb*ly*cos(-(j + 1)*theta)));
		  P.push_back(GsVec(rb*rx*cos(-(j + 1)*theta), zbot + rb*sin(-(j + 1)*theta), rb*ry*cos(-(j + 1)*theta)));
		  for (int j = 0; j < 6; j++)
			  C.push_back(GsColor::blue);
//		      C.push_back(GsColor::random());
      }
   }
   // send data to OpenGL buffers:
   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] );
   glBufferData ( GL_ARRAY_BUFFER, P.size()*3*sizeof(float), &P[0], GL_STATIC_DRAW );
   glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
   glBufferData ( GL_ARRAY_BUFFER, C.size()*4*sizeof(gsbyte), &C[0], GL_STATIC_DRAW );

   // save size so that we can free our buffers and later just draw the OpenGL arrays:
   _numpoints = P.size();

   // free non-needed memory:
   P.resize(0); C.resize(0);
 }

void SoCapsule::draw ( GsMat& tr, GsMat& pr )
 {
   // Draw Lines:
   glUseProgram ( prog );
   glBindVertexArray ( va[0] );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] ); // positions
   glEnableVertexAttribArray ( 0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[1] ); // colors
   glEnableVertexAttribArray ( 1 );
   glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

   glUniformMatrix4fv ( uniloc[0], 1, GL_FALSE, tr.e );
   glUniformMatrix4fv ( uniloc[1], 1, GL_FALSE, pr.e );

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glDrawArrays(GL_TRIANGLES, 0, _numpoints);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 }

