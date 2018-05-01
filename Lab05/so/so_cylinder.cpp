
# include "so_cylinder.h"

SoCylinder::SoCylinder()
{
	_numpoints = 0;
}

void SoCylinder::init(const GlProgram& prog)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 3 buffers
	uniform_locations(9); // will send 4 variables: the 2 matrices below and the 2 lighting objects
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
	uniform_location(2, "lPos");
	uniform_location(3, "la");
	uniform_location(4, "ld");
	uniform_location(5, "ls");
	uniform_location(6, "ka");
	uniform_location(7, "kd");
	uniform_location(8, "ks");
	GsColor::darkgreen.get(material.ambient);	 GsColor::green.get(material.diffuse);	 GsColor::white.get(material.specular);

}

void SoCylinder::build(int nface, bool smooth)
{
	P.clear(); N.clear();// set size to zero, just in case

	float lx, ly, rx, ry;
	float len = 1;
	float r = .25;
	float ztop = len / 2;
	float zbot = -1 * len / 2;
	float theta = GS_TORAD(360.0f / nface);
	GsVec vert1, vert2, vert3;
	GsVec norm;

	for (int i = 0; i < nface; i++) {
		//left and right xy unit vectors
		lx = cos(i*theta); ly = sin(i*theta);
		rx = cos((i + 1)*theta); ry = sin((i + 1)*theta);

//TopRight, TopLeft, BotLeft
		vert1 = GsVec(r*ry, ztop, r*rx);		P.push_back(vert1);
		vert2 = GsVec(r*ly, ztop, r*lx);	  P.push_back(vert2);
		vert3 = GsVec(r*ly, zbot, r*lx);	  P.push_back(vert3);
		if (!smooth)
			for (int i = 0; i < 3; i++)
				N.push_back(normal(vert1, vert2, vert3));
		else {
			vert1 -= GsVec(0.0f, ztop, 0.0f);		vert1.normalize();		N.push_back(vert1);
			vert2 -= GsVec(0.0f, ztop, 0.0f);		vert2.normalize();		N.push_back(vert2);
			vert3 -= GsVec(0.0f, zbot, 0.0f);		vert3.normalize();		N.push_back(vert3);
		}
//BotLeft, BotRight, TopRight
		vert1 = GsVec(r*ly, zbot, r*lx);		P.push_back(vert1);
		vert2 = GsVec(r*ry, zbot, r*rx);		P.push_back(vert2);
		vert3 = GsVec(r*ry, ztop, r*rx);		P.push_back(vert3);
		if(!smooth)
			for (int i = 0; i < 3; i++)
				N.push_back(normal(vert1, vert2, vert3));
		else{
			vert1 -= GsVec(0.0f, zbot, 0.0f);		vert1.normalize();		N.push_back(vert1);
			vert2 -= GsVec(0.0f, zbot, 0.0f);		vert2.normalize();		N.push_back(vert2);
			vert3 -= GsVec(0.0f, ztop, 0.0f);		vert3.normalize();		N.push_back(vert3);
		}
//Top to Center
		vert1 = GsVec(r*ry, ztop, r*rx);		P.push_back(vert1);
		vert2 = GsVec(0.0f, ztop, 0.0f);		P.push_back(vert2);
		vert3 = GsVec(r*ly, ztop, r*lx);		P.push_back(vert3);
		for (int i = 0; i < 3; i++)
			N.push_back(normal(vert1, vert2, vert3));
//Bot to Center
		vert1 = GsVec(r*ly, zbot, r*lx);		P.push_back(vert1);
		vert2 = GsVec(0.0f, zbot, 0.0f);		P.push_back(vert2);
		vert3 = GsVec(r*ry, zbot, r*rx);		P.push_back(vert3);
		for (int i = 0; i < 3; i++)
			N.push_back(normal(vert1, vert2, vert3));
	}
	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	P.resize(0); N.resize(0);
}

void SoCylinder::draw(GsMat& tr, GsMat& pr, GlLight& light) {
	// Draw Lines:
	glUseProgram(prog);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);
	glUniform3fv(uniloc[2], 1, light.pos);
	glUniform4fv(uniloc[3], 1, light.ambient);
	glUniform4fv(uniloc[4], 1, light.diffuse);
	glUniform4fv(uniloc[5], 1, light.specular);
	glUniform4fv(uniloc[6], 1, material.ambient);
	glUniform4fv(uniloc[7], 1, material.diffuse);
	glUniform4fv(uniloc[8], 1, material.specular);


	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
}