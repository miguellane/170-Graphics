
# include "so_grid.h"

SoGrid::SoGrid()
 {
   _numpoints = 0;
 }

void SoGrid::init ( const GlProgram& prog )
 {
   // Define buffers needed:
   set_program ( prog );
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   uniform_locations ( 2 ); // will send 2 variables: the 2 matrices below
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );
 }

void SoGrid::build (int size)
 {
	P.clear(); C.clear();
	float div = 1 * pow(0.5f, size);
	int bound = (int)(1 / div);

	for (int i = -bound; i <= bound; i++) {
		for (int j = -bound; j <= bound; j++) {
			P.push_back(GsVec(i * div, j * div, 1.0f));		P.push_back(GsVec(i * div, j * div, -1.0f));
			P.push_back(GsVec(i * div, 1.0f, j * div));		P.push_back(GsVec(i * div, -1.0f, j * div));
			P.push_back(GsVec(1.0f, i * div, j * div));		P.push_back(GsVec(-1.0f, i * div, j * div));
			for (int k = 0; k < 6; k++) C.push_back(GsColor::white);
	}	}

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

void SoGrid::draw ( GsMat& tr, GsMat& pr )
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

   glDrawArrays ( GL_LINES, 0, _numpoints );
 }

