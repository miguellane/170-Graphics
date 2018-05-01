
# include "so_surface.h"


bool SoSurface::func(int type, const GsVec& vec) {
	double x = vec.x;
	double y = vec.z;
	double z = 0;
	switch(type){
	case 0: z = 0;	break;
	case 1: z = x * pow(y,3) - y * pow(x,3); break;
	case 2: z = (pow(x,2) + 3 * pow(y,2)) * exp(pow(-x,2) - pow(y,2)); break;
	case 3: z = -x * y * exp(pow(-x, 2) - pow(y, 2)); break;
	case 4: z = 1 + -1 / (pow(x, 2) + pow(y, 2)); break;
	case 5: z = cos(abs(x) + abs(y)); break;
	case 6: z = cos(abs(x) + abs(y)) * (abs(x) + abs(y)); break;
	case 7: z = pow(pow(0.4, 2) - pow(0.6 - pow(pow(x, 2) + pow(y, 2), 0.5), 2), 0.5); break;
	case 8: z = 0.7 / log(pow(x,2) + pow(y,2)) + 0.6; break;
	}
	
	return (vec.y < z) ? true : false;
}


void SoSurface::calcTet(bool aIn, bool bIn, bool cIn, bool dIn, const GsVec& a, const GsVec& b, const GsVec& c, const GsVec& d) {
	//Clockwise
	std::vector<GsVec> New;
	GsVec norm, v1, v2;

	if (aIn != bIn)	New.push_back((a + b) / 2);
	if (aIn != cIn)	New.push_back((a + c) / 2);
	if (aIn != dIn)	New.push_back((a + d) / 2);
	if (bIn != cIn)	New.push_back((b + c) / 2);
	if (bIn != dIn)	New.push_back((b + d) / 2);
	if (cIn != dIn)	New.push_back((c + d) / 2);


	for (int i = 2; i < New.size(); i++) {
		v1 = New[i - 1] - New[i - 2];
		v2 = New[i] - New[i - 2];
		norm.cross(v1, v2);
		if(norm.y < 0){

			norm.cross(v2, v1);
			P.push_back(New[i]);
			P.push_back(New[i - 1]);
			P.push_back(New[i - 2]);
			T.push_back(GsVec2((New[i].x + 1) , New[i].z + 1) / 2);
			T.push_back(GsVec2((New[i - 1].x + 1), New[i - 1].z + 1) / 2);
			T.push_back(GsVec2((New[i - 2].x + 1), New[i - 2].z + 1) / 2);
		} else {
			P.push_back(New[i - 2]);
			P.push_back(New[i - 1]);
			P.push_back(New[i]);
			T.push_back(GsVec2((New[i - 2].x + 1), New[i - 2].z + 1) / 2);
			T.push_back(GsVec2((New[i - 1].x + 1), New[i - 1].z + 1) / 2);
			T.push_back(GsVec2((New[i].x + 1), New[i].z + 1) / 2);
		}
		N.push_back(norm);
		N.push_back(norm);
		N.push_back(norm);
	}
	/*
	P.push_back(a);	N.push_back(a);	P.push_back(b);	N.push_back(b);	P.push_back(c);	N.push_back(c);
	P.push_back(c);	N.push_back(c);	P.push_back(d);	N.push_back(d);	P.push_back(a);	N.push_back(a);
	P.push_back(a);	N.push_back(a);	P.push_back(b);	N.push_back(b);	P.push_back(d);	N.push_back(d);
	P.push_back(b);	N.push_back(b);	P.push_back(c);	N.push_back(c);	P.push_back(d);	N.push_back(d);
	*/	
}
void SoSurface::calcSqr(bool aIn, bool bIn, bool cIn, bool dIn, bool eIn, bool fIn, bool gIn, bool hIn, const GsVec& a, const GsVec& b, const GsVec& c, const GsVec& d, const GsVec& e, const GsVec& f, const GsVec& g, const GsVec& h) {
	//Clockwise top, clockwise bottom 
	calcTet(cIn, bIn, eIn, aIn, c, b, e, a);
	calcTet(cIn, aIn, eIn, dIn, c, a, e, d);
	calcTet(cIn, gIn, eIn, fIn, c, g, e, f);
	calcTet(cIn, hIn, eIn, gIn, c, h, e, g);
	calcTet(cIn, fIn, eIn, bIn, c, f, e, b);
	calcTet(cIn, dIn, eIn, hIn, c, d, e, h);
}
SoSurface::SoSurface()
{
	_numpoints = 0;
}

void SoSurface::init(const GlProgram& prog, GlMaterial mat)
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

	GsImage I;
	if (!I.load("../images/NepNep.png")) {
		std::cout << "COULD NOT LOAD IMAGE!\n";
		exit(1);
	}
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
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

void SoSurface::build(int type, int size)
{
	P.clear(); N.clear(); T.clear();
	float div = 1 * pow(0.5f, size);
	int bound = (int)(1 / div);
	bool aIn, bIn, cIn, dIn, eIn, fIn, gIn, hIn;
	GsVec a, b, c, d, e, f, g, h;
	float x , x1, y, y1, z, z1;

	for(int i = -bound; i < bound; i++){
		x = i * div;
		x1 = (i + 1) * div;
		for(int j = -bound; j < bound; j++){
			y = j * div;
			y1 = (j + 1) * div;
			for(int k = -bound; k < bound; k++){
				z = k * div;
				z1 = (k + 1) * div;
//Cube Corners
				a = GsVec(x, y1, z);		b = GsVec(x1, y1, z);	
				d = GsVec(x, y1, z1);	c = GsVec(x1, y1, z1);

				e = GsVec(x, y, z);		f = GsVec(x1, y, z);
				h = GsVec(x, y, z1);		g = GsVec(x1, y, z1);
//Cube Cotners				
				aIn = func(type, a);	bIn = func(type, b);	cIn = func(type, c);	dIn = func(type, d);
				eIn = func(type, e);	fIn = func(type, f);	gIn = func(type, g);	hIn = func(type, h);
				calcSqr(aIn, bIn, cIn, dIn, eIn, fIn, gIn, hIn, a, b, c, d, e, f, g, h);
			}
		}
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
	changed = false;
	P.resize(0); N.resize(0); T.resize(0);
}

void SoSurface::draw(GsMat& tr, GsMat& pr, GlLight& light, float shade, bool texture) {
	sh = 1.0f;
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

	if(texture)
		glBindTexture(GL_TEXTURE_2D, ID);
	else
		glBindTexture(GL_TEXTURE_2D, GLuint(0));

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);
	glUniform3fv(uniloc[2], 1, light.pos);
	glUniform4fv(uniloc[3], 1, light.ambient);
	glUniform4fv(uniloc[4], 1, light.diffuse);
	glUniform4fv(uniloc[5], 1, light.specular);
	glUniform4fv(uniloc[6], 1, material.ambient);
	glUniform4fv(uniloc[7], 1, material.diffuse);
	glUniform4fv(uniloc[8], 1, material.specular);
	glUniform1fv(uniloc[9], 1, &sh);
	glUniform1fv(uniloc[11], 1, &shade);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
}