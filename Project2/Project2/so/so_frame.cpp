
# include "so_frame.h"
void SoFrame::func(int type, GsVec& vec) {
	double x = vec.x;
	double y = vec.z;
	double z = 0;
	switch (type) {
	case 0: z = 0;	break;
	case 1: z = x * pow(y, 3) - y * pow(x, 3); break;
	case 2: z = (pow(x, 2) + 3 * pow(y, 2)) * exp(pow(-x, 2) - pow(y, 2)); break;
	case 3: z = -x * y * exp(pow(-x, 2) - pow(y, 2)); break;
	case 4: z = 1 + -1 / (pow(x, 2) + pow(y, 2)); break;
	case 5: z = cos(abs(x) + abs(y)); break;
	case 6: z = cos(abs(x) + abs(y)) * (abs(x) + abs(y)); break;
	case 7: z = pow(pow(0.4, 2) - pow(0.6 - pow(pow(x, 2) + pow(y, 2), 0.5), 2), 0.5); break;
	case 8: z = 0.7 / log(pow(x, 2) + pow(y, 2)) + 0.6; break;
	}
	vec.y = z;
}

void SoFrame::eval_lagrange(const std::vector<GsVec>& ctrlpnts, std::vector<GsVec>& curvepnts, int segments) {
	GsVec add;
	float term, t;
	//For each line A-----B save A
	curvepnts.push_back(ctrlpnts[0]);
	for (int s = 1; s < segments; s++) {
		t = (float)(ctrlpnts.size() - 1) * s / segments;
		//Start Lagrange
		add = GsVec(0, 0, 0);
		for (int i = 0; i < ctrlpnts.size(); i++) {
			term = 1;
			for (int j = 0; j < ctrlpnts.size(); j++) {
				if (j != i)
					term *= (t - j) / (i - j);
			}
			add += ctrlpnts[i] * term;
		}
		curvepnts.push_back(add);
		//End Lagrange
	}
	//Save B
	curvepnts.push_back(ctrlpnts[ctrlpnts.size() - 1]);
}
SoFrame::SoFrame()
 {
   _numpoints = 0;
 }

void SoFrame::init ( const GlProgram& prog )
 {
   // Define buffers needed:
   set_program ( prog );
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   uniform_locations ( 2 ); // will send 2 variables: the 2 matrices below
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );
 }

void SoFrame::build (int type, int size, int segments)
 {

	P.clear(); C.clear();
	std::vector<GsVec> Cntrl;

	float div = 1 * pow(0.5f, size);
	int bound = (int)(1 / div);
	GsVec t;
	//P.push_back(GsVec(0.0f, 0.0f, 0.0f));
	for (int i = -bound; i <= bound; i++) {
		t.x = i * div;
		for (int j = -bound; j <= bound; j++) {
			t.z = j * div;
			func(type, t);
			Cntrl.push_back(t);
		}
		eval_lagrange(Cntrl, P, segments);
		Cntrl.clear();
	}
	

	for (int i = 0; i < P.size(); i++) C.push_back(GsColor::red);

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

void SoFrame::draw ( GsMat& tr, GsMat& pr )
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

   glDrawArrays ( GL_POINTS, 0, _numpoints );
 }

