# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

AppWindow::AppWindow(const char* label, int x, int y, int w, int h)
	:GlutWindow(label, x, y, w, h) {
	initPrograms();
	addMenuEntry("View Axis", evOption0);
	addMenuEntry("View Ground", evOption1);
	addMenuEntry("View Cactus", evOption2);
	addMenuEntry("View Scorpion", evOption3);
	_viewaxis = false;
	_viewground = true;
	_viewcactus = true;
	_viewscorpion = true;
	_animated = false;
	_scorpF = GsVec(0.0f, 0.0f, 0.0f);
	_scorpRY = _scorpD1 = _scorpD2 = _scorpD3 = _scorpD4 = _scorpD5 = 0;
	_w = w;
	_h = h;
}

void AppWindow::initPrograms()
{
	// Load your shaders and link your programs here:
	_vertexsh.load_and_compile(GL_VERTEX_SHADER, "../shaders/3dcolored.vert");
	_fragsh.load_and_compile(GL_FRAGMENT_SHADER, "../shaders/flat.frag");
	_gouvert.load_and_compile(GL_VERTEX_SHADER, "../shaders/gouraud.vert");
	_goufrag.load_and_compile(GL_FRAGMENT_SHADER, "../shaders/gouraud.frag");
	_gouverttext.load_and_compile(GL_VERTEX_SHADER, "../shaders/gouraud2.vert");
	_goufragtext.load_and_compile(GL_FRAGMENT_SHADER, "../shaders/gouraud2.frag");
	_prog.init_and_link(_vertexsh, _fragsh);
	_prog2.init_and_link(_gouvert, _goufrag);
	_prog3.init_and_link(_gouverttext, _goufragtext);
	GlMaterial material;

	_axis.init(_prog);
	GsColor::black.get(material.ambient);	GsColor::black.get(material.diffuse);		GsColor::black.get(material.specular);
	_ground.init(_prog3, material, 0.1f);
//Cacti
	GsColor::green.get(material.ambient);	GsColor::green.get(material.diffuse);		GsColor::white.get(material.specular);
	_cactus.init(_prog3, material, 0.1f);
	GsColor::black.get(material.ambient);	GsColor::black.get(material.diffuse);		GsColor::black.get(material.specular);
	_cactussh.init(_prog2, material, 0.1f);
	GsColor::darkgreen.get(material.ambient);	GsColor::darkgreen.get(material.diffuse);		GsColor::darkgreen.get(material.specular);
	_cactus2.init(_prog3, material, 0.1f);
	GsColor::black.get(material.ambient);	GsColor::black.get(material.diffuse);		GsColor::black.get(material.specular);
	_cactus2sh.init(_prog2, material, 0.1f);
//Scorpion
	GsColor::brown.get(material.ambient);	GsColor::yellow.get(material.diffuse);		GsColor::black.get(material.specular);
	_scorpion.init(_prog2, material, 0.1f);
	GsColor::black.get(material.ambient);	GsColor::black.get(material.diffuse);		GsColor::black.get(material.specular);
	_scorpionsh.init(_prog2, material, 0.1f);
}

GsVec2 AppWindow::windowToScene(const GsVec2& v)
{
	return GsVec2((2.0f*(v.x / float(_w))) - 1.0f,
		1.0f - (2.0f*(v.y / float(_h))));
}

void AppWindow::glutKeyboard(unsigned char key, int x, int y)
{
	bool rd = true;
	const float incf = 5.0f;
	switch (key) {
	case ' ':	_animated = !_animated; break;
	case 'q':	_scorpD1 += incf; break;
	case 'a':	_scorpD1 -= incf; break;
	case 'w':	_scorpD2 += incf; break;
	case 's':	_scorpD2 -= incf; break;
	case 'e':	_scorpD3 += incf; break;
	case 'd':	_scorpD3 -= incf; break;
	case 'r':	_scorpD4 += incf; break;
	case 'f':	_scorpD4 -= incf; break;
	case 't':	_scorpD5 += incf; break;
	case 'g':	_scorpD5 -= incf; break;
	case 27: exit(1);
	}
	if (rd) redraw();
}

void AppWindow::glutSpecial(int key, int x, int y)
{
	bool rd = true;
	const float incr = GS_TORAD(5.0f);
	const float incf = 0.05f;
	switch (key)
	{
	case GLUT_KEY_LEFT:	_scorpRY += incr;		break;
	case GLUT_KEY_RIGHT:	_scorpRY -= incr;		break;
	case GLUT_KEY_UP:		_scorpF += GsVec(incf * sin(_scorpRY), 0.0f, incf * cos(_scorpRY));		break;
	case GLUT_KEY_DOWN:	_scorpF -= GsVec(incf * sin(_scorpRY), 0.0f, incf * cos(_scorpRY));		break;
	default: return;
	}
	if (rd) redraw();
}

void AppWindow::glutMouse(int b, int s, int x, int y)
{
}

void AppWindow::glutMotion(int x, int y)
{
}

void AppWindow::glutMenu(int m)
{
	bool rd = true;
	switch (m)
	{
	case 0:	_viewaxis = !_viewaxis;				break;
	case 1:	_viewground = !_viewground;		break;
	case 2:	_viewcactus = !_viewcactus;		break;
	case 3:	_viewscorpion = !_viewscorpion;	break;
	}
	if (rd) redraw();
}

void AppWindow::glutReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	_w = w; _h = h;
}

void AppWindow::glutDisplay() {
	// Clear the rendering window 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (_axis.changed)		_axis.build(1.0f);
	if (_ground.changed)		_ground.build();
	if (_cactus.changed){	_cactus.build();						_cactussh.build();}
	if (_cactus2.changed){	_cactus2.build();						_cactus2sh.build();}
	if (_scorpion.changed){	_scorpion.build(16);					_scorpionsh.build(16);}

//Scorpion
	GsMat scorptransf;
	{
	GsMat movement;
	movement.translation(_scorpF);
	scorptransf.roty(_scorpRY);
	scorptransf = movement * scorptransf;
	}
//Scene
	GsMat stransf;

	GsMat sproj;
	{
	GsMat camview, persp;
	GsVec eye(0, 2, 2), center(0, 0, 0), up(0, 1, 0);
	if (_animated){
		eye = GsVec(0, 2, -1);
		center = GsVec(0, 0, 0);
		eye = scorptransf * eye;
		center = scorptransf * center;
	}
	camview.lookat(eye, center, up);
	float fovy = GS_TORAD(60.0f);	float aspect = 1.0f, znear = 0.1f, zfar = 50.0f;
	persp.perspective(fovy, aspect, znear, zfar );
	sproj = persp * camview;
	}
//Lighting
	GlLight sun;
	{
	sun.pos = GsVec(0.00f, 100.00f, 100.00f);
	GsColor::white.get(sun.ambient);	 GsColor::white.get(sun.diffuse);	 GsColor::white.get(sun.specular);
	}
	GsMat shadowproj;
	{
	GsVec v, q, n;
	float dotvn, dotqn;
	v.set(sun.pos);
	v = v / sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	q.set(0.0f, 0.001f, 0.0f);
	n.set(0.0f, 1.0f, 0.0f);
	dotvn = v.x*n.x + v.y*n.y + v.z*n.z;
	dotqn = q.x*n.x + q.y*n.y + q.z*n.z;
	shadowproj.zero();
	shadowproj.setl1(GsVec(dotvn, 0.0f, 0.0f) - GsVec(0.0f, v.x * n.y, 0.0f));
	shadowproj.setl2(GsVec(0.0f, dotvn, 0.0f) - GsVec(0.0f, v.y * n.y, 0.0f));
	shadowproj.setl3(GsVec(0.0f, 0.0f, dotvn) - GsVec(0.0f, v.z * n.y, 0.0f));
	shadowproj[3] = (v.x * dotqn);
	shadowproj[7] = (v.y * dotqn);
	shadowproj[11] = (v.z * dotqn);
	shadowproj[15] = dotvn;
	}
	sun.pos = stransf * sun.pos;
//Scene
	GsMat scale, cactMat1, cactMat2;
	{
	scale.scale(3.0f);
	cactMat1.translation(1.0f, 0.0f, 1.0f);
	cactMat2.translation(-2.0f, 0.0f, -2.0f);
	cactMat2 = cactMat2 * scale;
	}
//Draw
	if (_viewaxis)			_axis.draw(stransf, sproj);
	if (_viewground)		_ground.draw(stransf * scale, sproj, sun);
	if (_viewcactus){		_cactus.draw(stransf * cactMat1, sproj, sun);						_cactussh.draw(stransf * cactMat1 * shadowproj, sproj, sun);
								_cactus2.draw(stransf * cactMat2, sproj, sun);						_cactus2sh.draw(stransf * cactMat2 * shadowproj, sproj, sun);}
	if (_viewscorpion){	_scorpion.draw(scorptransf * stransf, sproj, sun, _scorpD1, _scorpD2, _scorpD3, _scorpD4, _scorpD5);
								_scorpionsh.draw(stransf * shadowproj * scorptransf, sproj, sun, _scorpD1, _scorpD2, _scorpD3, _scorpD4, _scorpD5);}

	glFlush();
	glutSwapBuffers();
}

