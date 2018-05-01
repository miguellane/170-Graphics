
# include "so_modelnorm.h"

SoModelNorm::SoModelNorm()
{
	_numpoints = 0;
}

void SoModelNorm::init(const GlProgram& prog, char* file)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 3 buffers
	uniform_locations(2); // will send 4 variables: the 2 matrices below and the 2 lighting objects
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
	model.load(file);
}

void SoModelNorm::build(bool smooth) {
	P.clear(); C.clear();// set size to zero, just in case

	float scale = 0.02f;
	GsVec vec1, vec2, vec3, norm1, norm2, norm3;
	if (smooth) {
		for (int i = 0; i < model.fsize; i++) {
			vec1 = model.V[model.F[i].va];		P.push_back((vec1) * scale);
			norm1 = model.N[model.F[i].na];		P.push_back((vec1 + norm1) * scale);
			vec2 = model.V[model.F[i].vb];		P.push_back((vec2) * scale);
			norm2 = model.N[model.F[i].nb]; 		P.push_back((vec2 + norm2) * scale);
			vec3 = model.V[model.F[i].vc];		P.push_back((vec3) * scale);
			norm3 = model.N[model.F[i].nc];		P.push_back((vec3 + norm3) * scale);
		}
	}
	else {
		for (int i = 0; i < model.fsize; i++) {
			vec1 = model.V[model.F[i].va];
			vec2 = model.V[model.F[i].vb];
			vec3 = model.V[model.F[i].vc];		P.push_back(((vec1 + vec2 + vec3) / 3) * scale);
			norm1 = model.N[model.F[i].na];
			norm2 = model.N[model.F[i].nb];
			norm3 = model.N[model.F[i].nc];		P.push_back(((vec1 + vec2 + vec3) / 3 + (norm1 + norm2 + norm3) / 3) * scale);
		}
	}
	for (int i = 0; i < P.size(); i++)
		C.push_back(GsColor::white);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(GLbyte), &C[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	P.resize(0); C.resize(0);
}

void SoModelNorm::draw(GsMat& tr, GsMat& pr) {
	// Draw Lines:
	glUseProgram(prog);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // normals
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_LINES, 0, _numpoints);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}