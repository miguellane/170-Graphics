
# include "so_norm.h"

SoNorm::SoNorm()
 {
   _numpoints = 0;
 }

void SoNorm::init ( const GlProgram& prog)
 {
   // Define buffers needed:
   set_program ( prog );
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 3 buffers
   uniform_locations ( 2 ); // will send 4 variables: the 2 matrices below and the 2 lighting objects
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );
 }

void SoNorm::build ( int nface, bool smooth ){
	P.clear(); C.clear();// set size to zero, just in case

	float lx, ly, rx, ry;
	float len = 1;
	float r = .25;
	float ztop = len / 2;
	float zbot = -1 * len / 2;
	float theta = GS_TORAD(360.0f / nface);
	GsVec vert1, vert2, vert3;
	std::vector<GsVec> iN, N;
	GsVec norm;

	for (int i = 0; i < nface; i++) {
		//left and right xy unit vectors
		lx = cos(i*theta); ly = sin(i*theta);
		rx = cos((i + 1)*theta); ry = sin((i + 1)*theta);

		//TopRight, TopLeft, BotLeft
		vert1 = GsVec(r*ry, ztop, r*rx);
		vert2 = GsVec(r*ly, ztop, r*lx); 
		vert3 = GsVec(r*ly, zbot, r*lx); 
		if (!smooth){
			iN.push_back((vert1 + vert2 + vert3) / 3);
			N.push_back(normal(vert1, vert2, vert3));
		}else{
			iN.push_back(vert1);		vert1 -= GsVec(0.0f, ztop, 0.0f);		vert1.normalize();		N.push_back(vert1);
			iN.push_back(vert2);		vert2 -= GsVec(0.0f, ztop, 0.0f);		vert2.normalize();		N.push_back(vert2);
			iN.push_back(vert3);		vert3 -= GsVec(0.0f, zbot, 0.0f);		vert3.normalize();		N.push_back(vert3);
		}
		//BotLeft, BotRight, TopRight
		vert1 = GsVec(r*ly, zbot, r*lx);
		vert2 = GsVec(r*ry, zbot, r*rx);
		vert3 = GsVec(r*ry, ztop, r*rx);
		if (!smooth) {
			iN.push_back((vert1 + vert2 + vert3) / 3);
			N.push_back(normal(vert1, vert2, vert3));
		}
		else {
			iN.push_back(vert1);		vert1 -= GsVec(0.0f, zbot, 0.0f);		vert1.normalize();		N.push_back(vert1);
			iN.push_back(vert2);		vert2 -= GsVec(0.0f, zbot, 0.0f);		vert2.normalize();		N.push_back(vert2);
			iN.push_back(vert3);		vert3 -= GsVec(0.0f, ztop, 0.0f);		vert3.normalize();		N.push_back(vert3);
		}
		//Top to Center
		vert1 = GsVec(r*ry, ztop, r*rx);
		vert2 = GsVec(0.0f, ztop, 0.0f);
		vert3 = GsVec(r*ly, ztop, r*lx);
		if (!smooth) {
			iN.push_back((vert1 + vert2 + vert3) / 3);
			N.push_back(normal(vert1, vert2, vert3));
		}
		else {
			iN.push_back(vert1);		iN.push_back(vert2);		iN.push_back(vert3);
			for (int i = 0; i < 3; i++)
				N.push_back(normal(vert1, vert2, vert3));
		}
		//Bot to Center
		vert1 = GsVec(r*ly, zbot, r*lx);
		vert2 = GsVec(0.0f, zbot, 0.0f);
		vert3 = GsVec(r*ry, zbot, r*rx);
		if (!smooth) {
			iN.push_back((vert1 + vert2 + vert3) / 3);
			N.push_back(normal(vert1, vert2, vert3));
		}
		else {
			iN.push_back(vert1);		iN.push_back(vert2);		iN.push_back(vert3);
			for (int i = 0; i < 3; i++)
				N.push_back(normal(vert1, vert2, vert3));
		}
	}

   // send data to OpenGL buffers:
	for(int i = 0; i < N.size(); i++){
		P.push_back(iN[i]);
		P.push_back(iN[i] + N[i] * .1);
	}
	for(int i = 0; i < P.size(); i++)
		C.push_back(GsColor::white);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0] );
	glBufferData(GL_ARRAY_BUFFER, P.size()*3*sizeof(float), &P[0], GL_STATIC_DRAW );
  glBindBuffer(GL_ARRAY_BUFFER, buf[1] );
  glBufferData(GL_ARRAY_BUFFER, C.size()*4*sizeof(GLbyte), &C[0], GL_STATIC_DRAW );

  // save size so that we can free our buffers and later just draw the OpenGL arrays:
  _numpoints = P.size();

	P.resize(0); C.resize(0);
}

void SoNorm::draw ( GsMat& tr, GsMat& pr){
   // Draw Lines:
   glUseProgram ( prog );
   glBindVertexArray ( va[0] );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] ); // normals
   glEnableVertexAttribArray ( 0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	 glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	 glEnableVertexAttribArray(1);
	 glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

  glUniformMatrix4fv ( uniloc[0], 1, GL_FALSE, tr.e );
  glUniformMatrix4fv ( uniloc[1], 1, GL_FALSE, pr.e );

	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_LINES, 0, _numpoints);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}