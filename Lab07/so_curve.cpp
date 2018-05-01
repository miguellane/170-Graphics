
# include "so_curve.h"

SoCurve::SoCurve()
 {
 _on3D = false;
 }

void SoCurve::init ( const GlProgram& prog, const GlMaterial& mat, float shine)
 {
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

void SoCurve::build (SoPoly& ctrlPoly, int segments, bool mode, bool& _build3D)
{
	V.remove(0, V.size());
	ctrlPoly.curveEval(V, segments, mode);

	GsVec norm;
	for (int i = 0; i < V.size(); i++){
		norm = V[i];
		norm.normalize();
		N.push(norm);
	}
	
	if(_build3D){
		if(_on3D){
			_build3D = false;
			changed = false;
		} else{
			_on3D = true;
		}
	}
	if(_on3D){
		GsArray<GsVec> tV, tN;
		GsVec left, right, Z;
		Z = GsVec(0.0f, 0.0f, -0.5f);
		for(int i = 0; i < V.size() - 1; i++){
			left = V[i];
			right = V[i + 1];
			tV.push( right + Z );	tV.push( left + Z );		tV.push( left );
			tV.push( left );			tV.push( right );			tV.push( right + Z);
			tN.push( N[i] );			tN.push( N[i] );			tN.push( N[i] );
			tN.push( N[i] );			tN.push( N[i + 1] );		tN.push( N[i + 1] );
		}
		V = tV;
		N = tN;

	}



	_numpoints = V.size();
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, V.size() * 3 * sizeof(float), &V[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);

}

void SoCurve::draw ( GsMat& tr, GsMat& pr , const GlLight& light)
 {
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


	if(_on3D){
		glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	}else{
		glDrawArrays(GL_LINE_STRIP, 0, _numpoints);
	}

	glBindVertexArray(0);
 }

