# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

AppWindow::AppWindow(const char* label, int x, int y, int w, int h)
	:GlutWindow(label, x, y, w, h) {
	initPrograms();
	addMenuEntry("Option 0", evOption0);
	addMenuEntry("Option 1", evOption1);

	_viewaxis = false;

	_viewtube = true;
	_tubepic = 0;
	_nfaces = 5;		_rt = 0.25f;		_rb = 0.25f;
	_shade = 0.75f;

	_sun.pos = GsVec(0.00f, 1.00f, 1.00f);
	GsColor::white.get(_sun.ambient);	 GsColor::white.get(_sun.diffuse);	 GsColor::white.get(_sun.specular);
	_fovy = GS_TORAD(60.0f);
	_rotx = _roty = 0;
	_w = w;
	_h = h;
}

void AppWindow::initPrograms()
{
	// Load your shaders and link your programs here:
	_vertexsh.load_and_compile(GL_VERTEX_SHADER, "../shaders/3dcolored.vert");
	_fragsh.load_and_compile(GL_FRAGMENT_SHADER, "../shaders/flat.frag");
	_gouvert.load_and_compile(GL_VERTEX_SHADER, "../shaders/gouraud2.vert");
	_goufrag.load_and_compile(GL_FRAGMENT_SHADER, "../shaders/gouraud2.frag");
	_prog.init_and_link(_vertexsh, _fragsh);
	_prog2.init_and_link(_gouvert, _goufrag);

	GlMaterial mattube;
	GsColor::darkgreen.get(mattube.ambient);	GsColor::green.get(mattube.diffuse);		GsColor::white.get(mattube.specular);
	// Init my scene objects:
	_axis.init(_prog);
	_tube.init(_prog2, mattube);
}

GsVec2 AppWindow::windowToScene(const GsVec2& v)
{
	return GsVec2((2.0f*(v.x / float(_w))) - 1.0f,
		1.0f - (2.0f*(v.y / float(_h))));
}

void AppWindow::glutKeyboard(unsigned char key, int x, int y)
{
	bool rd = true;
	const float off = (float).05;
	GsMat math;
	switch (key) {
	case ' ': _viewaxis = !_viewaxis;  break;
	case 'q': _tube.changed = true; _nfaces++; break;
	case 'a': _tube.changed = true; _nfaces--; break;
	case 'w': _tube.changed = true; _rt += off; break;
	case 's': _tube.changed = true; _rt -= off; break;
	case 'e': _tube.changed = true; _rb += off; break;
	case 'd': _tube.changed = true; _rb -= off; break;
	case 'z': _tube.changed = true; _tubepic = !_tubepic; break;
	case 'x': _shade += off; break;
	case 'c': _shade -= off; break;
	case 27: exit(1);
	}
	if (rd) redraw();
}

void AppWindow::glutSpecial(int key, int x, int y)
{
	bool rd = true;
	const float incr = GS_TORAD(2.5f);
	const float incf = 0.05f;
	switch (key)
	{
	case GLUT_KEY_LEFT:      _roty -= incr; break;
	case GLUT_KEY_RIGHT:     _roty += incr; break;
	case GLUT_KEY_UP:        _rotx -= incr; break;
	case GLUT_KEY_DOWN:      _rotx += incr; break;
	case GLUT_KEY_PAGE_UP:   _fovy -= incf; break;
	case GLUT_KEY_PAGE_DOWN: _fovy += incf; break;
	case GLUT_KEY_HOME:      _fovy = GS_TORAD(60.0f); _rotx = _roty = 0; break;
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
	std::cout << "Menu Event: " << m << std::endl;
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
	if (_tube.changed)		_tube.build(_nfaces, _rt, _rb);
	// Define our scene transformation:
	GsMat rx, ry, stransf;
	rx.rotx(_rotx);
	ry.roty(_roty);
	stransf = rx*ry;
	GsMat camview, persp, sproj;
	GsVec eye(0, 0, 2), center(0, 0, 0), up(0, 1, 0);
	camview.lookat(eye, center, up);
	float aspect = 1.0f, znear = 0.1f, zfar = 50.0f;
	persp.perspective(_fovy, aspect, znear, zfar);
	sproj = persp * camview;
	//Draw

	if (_viewaxis)	_axis.draw(stransf, sproj);
	if (_viewtube)	_tube.draw(stransf, sproj, _sun, _shade, _tubepic);


	glFlush();
	glutSwapBuffers();
}

