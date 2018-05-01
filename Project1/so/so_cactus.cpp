
# include "so_cactus.h"

SoCactus::SoCactus()
{
	_numpoints = 0;
}

void SoCactus::init(const GlProgram& prog, GlMaterial& mat, float shine)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1);
	gen_buffers(3);
	uniform_locations(12);
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
	uniform_location(10, "Tex1");
	uniform_location(11, "tShade");
	this->material = mat;
	this->shine = shine;

	GsImage I;
	if (!I.load("../images/cactus.png")) {
		std::cout << "COULD NOT LOAD IMAGE!\n";
		exit(1);
	}
	glGenTextures(1, &imageID);
	glBindTexture(GL_TEXTURE_2D, imageID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, I.w(), I.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, I.data());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	I.init(0, 0);
}

void SoCactus::build()
{
	P.clear(); N.clear(); T.clear();
	int nface = 16;
	float r = 0.25f;
	float ztop = 2.0f;
	float lx, ly, rx, ry;
	float theta = GS_TORAD(360.0f / nface);
	GsVec vert1, vert2, vert3;
	GsVec norm;

	for (int i = 0; i < nface; i++) {
		float n = (float) nface;
		//left and right xy unit vectors
		lx = cos(i*theta); ly = sin(i*theta);
		rx = cos((i + 1)*theta); ry = sin((i + 1)*theta);

//TopRight, TopLeft, BotLeft
		vert1 = GsVec(r*ry, ztop, r*rx);		P.push_back(vert1);		//T.push_back(GsVec2((i + 1) / n, 0));
		vert2 = GsVec(r*ly, ztop, r*lx);		P.push_back(vert2);		//T.push_back(GsVec2((i) / n, 0));
		vert3 = GsVec(r*ly, 0, r*lx);			P.push_back(vert3);		//T.push_back(GsVec2((i) / n, 1));

		vert1 -= GsVec(0.0f, ztop, 0.0f);		vert1.normalize();		N.push_back(vert1);
		vert2 -= GsVec(0.0f, ztop, 0.0f);		vert2.normalize();		N.push_back(vert2);
		vert3 -= GsVec(0.0f, 0, 0.0f);			vert3.normalize();		N.push_back(vert3);
//BotLeft, BotRight, TopRight
		vert1 = GsVec(r*ly, 0, r*lx);			P.push_back(vert1);		//T.push_back(GsVec2((i) / n, 1));
		vert2 = GsVec(r*ry, 0, r*rx);			P.push_back(vert2);		//T.push_back(GsVec2((i + 1) / n, 1));
		vert3 = GsVec(r*ry, ztop, r*rx);		P.push_back(vert3);		//T.push_back(GsVec2((i + 1) / n, 0));

		vert1 -= GsVec(0.0f, 0, 0.0f);			vert1.normalize();		N.push_back(vert1);
		vert2 -= GsVec(0.0f, 0, 0.0f);			vert2.normalize();		N.push_back(vert2);
		vert3 -= GsVec(0.0f, ztop, 0.0f);		vert3.normalize();		N.push_back(vert3);
	}
	for(int i = 0; i < P.size(); i += 6){
		T.push_back(GsVec2(1.0f, 1.0f));
		T.push_back(GsVec2(0.0f, 1.0f));
		T.push_back(GsVec2(0.0f, 0.0f));
		T.push_back(GsVec2(0.0f, 0.0f));
		T.push_back(GsVec2(1.0f, 0.0f));
		T.push_back(GsVec2(1.0f, 1.0f));
	}
	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, T.size() * 2 * sizeof(float), &T[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	P.resize(0); N.resize(0); T.resize(0);
}

void SoCactus::draw(GsMat& tr, GsMat& pr, GlLight& light) {
	float impact = 0.5f;
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
	//Texture
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindTexture(GL_TEXTURE_2D, imageID);

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
	glUniform1fv(uniloc[11], 1, &impact);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
}