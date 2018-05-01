#include "so_clock.h"

SoClock::SoClock(){
   _numpoints = 0;
}

void SoClock::init ( const GlProgram& prog )
 {
   // Define buffers needed:
   set_program ( prog );
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   uniform_locations ( 2 ); // will send 2 variables: the 2 matrices below
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );
 }

void SoClock::build (int milis)
 {
  P.clear(); C.clear();
	r = .75;
	deg = GS_TORAD(1.0f);
	sec = (float)milis / 1000;
	for (int i = 0; i < 360; i++) {
		//Clock-Ring
		P.push_back(GsVec(r*sin(i*deg),								r*cos(i*deg),								0.0f));
		P.push_back(GsVec(r*sin((i + 1)*deg),					r*cos((i + 1)*deg),					0.0f));
		P.push_back(GsVec((r*1.1f)*sin((i + 1)*deg), (r*1.4f)*cos((i + 1)*deg),		0.0f));
		if(i % 2 == 0)
			for (int j = 0; j < 3; j++)
				C.push_back(GsColor::blue);
		else
			for (int j = 0; j < 3; j++)
				C.push_back(GsColor::darkblue);
		//Ticks 1/12
		if(i % 30 == 0){
			P.push_back(GsVec((r*.85f)*sin(i*deg),			(r*.85f)*cos(i*deg),			0.0f));
			P.push_back(GsVec((r*.95f)*sin(i*deg),			(r*.95f)*cos(i*deg),			0.0f));
			P.push_back(GsVec((r*.95f)*sin((i+3)*deg),	(r*.95f)*cos((i+3)*deg),	0.0f));
			for (int j = 0; j < 3; j++)
				C.push_back(GsColor::yellow);
		}
		//Ticks 1/60
		if (i % 6 == 0) {
			P.push_back(GsVec((r*.9f)*sin(i*deg),		(r*.9f)*cos(i*deg),		0.0f));
			P.push_back(GsVec((r*.95f)*sin(i*deg),	(r*.95f)*cos(i*deg), 0.0f));
			P.push_back(GsVec((r*.95f)*sin(i*deg),	(r*.95f)*cos(i*deg), 0.0f));
			for (int j = 0; j < 3; j++)
				C.push_back(GsColor::yellow);
		}
	}

	//Min
	P.push_back(GsVec(0.0f, 0.0f, 0.0f));
	P.push_back(GsVec(r*sin(sec * 6 * deg),		r*cos(sec * 6 * deg),		0.0f));
	P.push_back(GsVec(r*sin(sec * 6 * deg),		r*cos(sec * 6 * deg),		0.0f));
	for (int j = 0; j < 3; j++)
		C.push_back(GsColor::white);

	//Sec
	P.push_back(GsVec(0.0f, 0.0f, 0.0f));
	P.push_back(GsVec(r*sin(sec * 6 * 60 * deg),	r*cos(sec * 6 * 60 * deg),		0.0f));
	P.push_back(GsVec(r*sin(sec * 6 * 60 * deg),	r*cos(sec * 6 * 60 * deg),		0.0f));
	for (int j = 0; j < 3; j++)
		C.push_back(GsColor::red);

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

void SoClock::draw ( GsMat& tr, GsMat& pr ){
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

