
# include "so_capsule.h"

SoCapsule::SoCapsule()
{
	_numpoints = 0;
}

void SoCapsule::init(const GlProgram& prog, GlMaterial& mat, float shine)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1);
	gen_buffers(2);
	uniform_locations(10);
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
	uniform_location(2, "lPos");
	uniform_location(3, "la");
	uniform_location(4, "ld");
	uniform_location(5, "ls");
	uniform_location(6, "ka");
	uniform_location(7, "kd");
	uniform_location(8, "ks");
	uniform_location(9, "sh");
	this->material = mat;
	this->shine = shine;
}

void SoCapsule::build(int nface, float len, float r4x, float r4y, float r3x, float r3y, float r2x, float r2y, float r1x, float r1y)
{
//		z1		z2					z3		z4
//		 /------------------------\
//		|		|					|		|
//		r1		r2					r3		r4
//		|		|					|		|
//		 \------------------------/
	P.clear(); N.clear(); // set size to zero, just in case
	float leftx, lefty, rightx, righty;
	float theta = GS_TORAD(360.0f / nface);
	float z1 = 0.0f;
	float z2 = len * 1 / 5;
	float z3 = len * 5 / 6;
	float z4 = len;
	GsVec vert1, vert2, vert3;

	for (int i = 0; i < nface; i++) {
		float n = (float)nface;
		//left and right xy unit vectors
		leftx = cos(i * theta);			lefty = sin(i * theta);
		rightx = cos((i + 1) * theta);	righty = sin((i + 1) * theta);

		//C - 4
		{
		//TopRight, TopLeft, BotLeft
		vert1 = GsVec(r4y * righty, z4, r4x * rightx);		P.push_back(vert1);
		vert2 = GsVec(0.0f, z4, 0.0f);							P.push_back(vert2);
		vert3 = GsVec(r4y * lefty, z4, r4x * leftx);			P.push_back(vert3);

		vert1 -= GsVec(0.0f, z4, 0.0f);		vert1.normalize();		N.push_back(vert1);
		vert2 -= GsVec(0.0f, z4, 0.0f);		vert2.normalize();		N.push_back(vert2);
		vert3 -= GsVec(0.0f, z4, 0.0f);		vert3.normalize();		N.push_back(vert3);
		}
		//4 - 3
		{
		//TopRight, TopLeft, BotLeft
		vert1 = GsVec(r4y * righty, z4, r4x * rightx);		P.push_back(vert1);
		vert2 = GsVec(r4y * lefty, z4, r4x * leftx);			P.push_back(vert2);
		vert3 = GsVec(r3y * lefty, z3, r3x * leftx);			P.push_back(vert3);

		vert1 -= GsVec(0.0f, z4, 0.0f);		vert1.normalize();		N.push_back(vert1);
		vert2 -= GsVec(0.0f, z4, 0.0f);		vert2.normalize();		N.push_back(vert2);
		vert3 -= GsVec(0.0f, z3, 0.0f);		vert3.normalize();		N.push_back(vert3);
		//BotLeft, BotRight, TopRight
		vert1 = GsVec(r3y * lefty, z3, r3x * leftx);			P.push_back(vert1);
		vert2 = GsVec(r3y * righty, z3, r3x * rightx);		P.push_back(vert2);
		vert3 = GsVec(r4y * righty, z4, r4x * rightx);		P.push_back(vert3);

		vert1 -= GsVec(0.0f, z3, 0.0f);		vert1.normalize();		N.push_back(vert1);
		vert2 -= GsVec(0.0f, z3, 0.0f);		vert2.normalize();		N.push_back(vert2);
		vert3 -= GsVec(0.0f, z4, 0.0f);		vert3.normalize();		N.push_back(vert3);
		}
		//3 - 2
		{
		//TopRight, TopLeft, BotLeft
		vert1 = GsVec(r3y * righty,	z3,	r3x * rightx);			P.push_back(vert1);
		vert2 = GsVec(r3y * lefty,		z3,	r3x * leftx);			P.push_back(vert2);
		vert3 = GsVec(r2y * lefty,		z2,	r2x * leftx);			P.push_back(vert3);

		vert1 -= GsVec(0.0f, z3, 0.0f);		vert1.normalize();		N.push_back(vert1);
		vert2 -= GsVec(0.0f, z3, 0.0f);		vert2.normalize();		N.push_back(vert2);
		vert3 -= GsVec(0.0f, z2, 0.0f);		vert3.normalize();		N.push_back(vert3);
		//BotLeft, BotRight, TopRight
		vert1 = GsVec(r2y * lefty,		z2,	r2x * leftx);			P.push_back(vert1);
		vert2 = GsVec(r2y * righty,	z2,	r2x * rightx);			P.push_back(vert2);
		vert3 = GsVec(r3y * righty,	z3,	r3x * rightx);			P.push_back(vert3);

		vert1 -= GsVec(0.0f, z2, 0.0f);		vert1.normalize();		N.push_back(vert1);
		vert2 -= GsVec(0.0f, z2, 0.0f);		vert2.normalize();		N.push_back(vert2);
		vert3 -= GsVec(0.0f, z3, 0.0f);		vert3.normalize();		N.push_back(vert3);
		}
		//2 - 1
		{
		//TopRight, TopLeft, BotLeft
		vert1 = GsVec(r2y * righty, z2, r2x * rightx);			P.push_back(vert1);
		vert2 = GsVec(r2y * lefty, z2, r2x * leftx);			P.push_back(vert2);
		vert3 = GsVec(r1y * lefty, z1, r1x * leftx);			P.push_back(vert3);

		vert1 -= GsVec(0.0f, z2, 0.0f);		vert1.normalize();		N.push_back(vert1);
		vert2 -= GsVec(0.0f, z2, 0.0f);		vert2.normalize();		N.push_back(vert2);
		vert3 -= GsVec(0.0f, z1, 0.0f);		vert3.normalize();		N.push_back(vert3);
		//BotLeft, BotRight, TopRight
		vert1 = GsVec(r1y * lefty, z1, r1x * leftx);			P.push_back(vert1);
		vert2 = GsVec(r1y * righty, z1, r1x * rightx);		P.push_back(vert2);
		vert3 = GsVec(r2y * righty, z2, r2x * rightx);		P.push_back(vert3);

		vert1 -= GsVec(0.0f, z1, 0.0f);		vert1.normalize();		N.push_back(vert1);
		vert2 -= GsVec(0.0f, z1, 0.0f);		vert2.normalize();		N.push_back(vert2);
		vert3 -= GsVec(0.0f, z2, 0.0f);		vert3.normalize();		N.push_back(vert3);
		}
		//1 - C
		{
		//TopRight, TopLeft, BotLeft
		vert1 = GsVec(r1y * righty, z1, r1x * rightx);		P.push_back(vert1);
		vert2 = GsVec(0.0f, z1, 0.0f);							P.push_back(vert2);
		vert3 = GsVec(r1y * lefty, z1, r1x * leftx);			P.push_back(vert3);

		vert1 -= GsVec(0.0f, z1, 0.0f);		vert1.normalize();		N.push_back(vert1);
		vert2 -= GsVec(0.0f, z1, 0.0f);		vert2.normalize();		N.push_back(vert2);
		vert3 -= GsVec(0.0f, z1, 0.0f);		vert3.normalize();		N.push_back(vert3);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);

	_numpoints = P.size();

	P.resize(0); N.resize(0);
}

void SoCapsule::draw(GsMat& tr, GsMat& pr, GlLight& light)
{
	// Draw Lines:
	glUseProgram(prog);
	glBindVertexArray(va[0]);
	//Verticies
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Normal
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);
	glUniform3fv(uniloc[2], 1, light.pos);
	glUniform4fv(uniloc[3], 1, light.ambient);
	glUniform4fv(uniloc[4], 1, light.diffuse);
	glUniform4fv(uniloc[5], 1, light.specular);
	glUniform4fv(uniloc[6], 1, material.ambient);
	glUniform4fv(uniloc[7], 1, material.diffuse);
	glUniform4fv(uniloc[8], 1, material.specular);
	glUniform1fv(uniloc[9], 1, &shine);


	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
}

