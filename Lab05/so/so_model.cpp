
# include "so_model.h"

SoModel::SoModel()
{
	_numpoints = 0;
}

void SoModel::init(const GlProgram& prog, char* file, GlMaterial mat)
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
	model.load(file);
	this->material = mat;
}

void SoModel::build(bool smooth)
{
	P.clear(); N.clear();// set size to zero, just in case
	float scale = 0.02f;
	GsVec vec1, vec2, vec3, norm1, norm2, norm3;
	if(smooth){
		for(int i = 0; i < model.fsize; i++){
			vec1 = model.V[model.F[i].va] * scale;	P.push_back(vec1);	
			vec2 = model.V[model.F[i].vb] * scale;	P.push_back(vec2);
			vec3 = model.V[model.F[i].vc] * scale;	P.push_back(vec3);
			norm1 = model.N[model.F[i].na];			N.push_back(norm1);
			norm2 = model.N[model.F[i].nb]; 			N.push_back(norm2);
			norm3 = model.N[model.F[i].nc];			N.push_back(norm3);
		}
	}else{
		for (int i = 0; i < model.fsize; i++) {
			vec1 = model.V[model.F[i].va] * scale;	P.push_back(vec1);
			vec2 = model.V[model.F[i].vb] * scale;	P.push_back(vec2);
			vec3 = model.V[model.F[i].vc] * scale;	P.push_back(vec3);
			norm1 = model.N[model.F[i].na];
			norm2 = model.N[model.F[i].nb];	
			norm3 = model.N[model.F[i].nc];
			for(int j = 0; j < 3; j++)
				N.push_back((norm1, norm2, norm3) / 3);
		}
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

void SoModel::draw(GsMat& tr, GsMat& pr, GlLight& light) {
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