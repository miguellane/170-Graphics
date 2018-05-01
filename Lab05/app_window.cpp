
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

AppWindow::AppWindow(const char* label, int x, int y, int w, int h)
	:GlutWindow(label, x, y, w, h) {
	initPrograms();
	addMenuEntry("Option 0", evOption0);
	addMenuEntry("Option 1", evOption1);

	_viewaxis = false;
	_viewmodel = true;
	_viewmodeln = false;

	_viewsmooth = false;
	_viewwireframe = false;

	_rhandT.identity();		_rhandR.identity();
	_rlarmT.identity();		_rlarmR.identity();
	_ruarmT.identity();		_ruarmR.identity();
	_rhandT.translation(0.01f, 0.01f, 0.50f);
	_rlarmT.translation(0.00f, 0.00f, 0.55f);
	_ruarmT.translation(0.00f, 0.00f, 0.00f);

	_sun.pos = GsVec(0.00f, 1.00f, 1.00f);
	GsColor::gray.get(_sun.ambient);	 GsColor::white.get(_sun.diffuse);	 GsColor::white.get(_sun.specular);

	_fovy = GS_TORAD(60.0f);
	_rotx = _roty = 0;
	_w = w;
	_h = h;
}

void AppWindow::initPrograms()
{
	// Load your shaders and link your programs here:
	_vertexsh.load_and_compile(GL_VERTEX_SHADER, "../3dcolored.vert");
	_fragsh.load_and_compile(GL_FRAGMENT_SHADER, "../flat.frag");
	_gouvert.load_and_compile(GL_VERTEX_SHADER, "../gouraud.vert");
	_goufrag.load_and_compile(GL_FRAGMENT_SHADER, "../gouraud.frag");
	_prog.init_and_link(_vertexsh, _fragsh);
	_prog2.init_and_link(_gouvert, _goufrag);

	GlMaterial mathand, matsleeve;
	GsColor::yellow.get(mathand.ambient);			GsColor::yellow.get(mathand.diffuse);		GsColor::white.get(mathand.specular);
	GsColor::darkgreen.get(matsleeve.ambient);	GsColor::green.get(matsleeve.diffuse);		GsColor::white.get(matsleeve.specular);
	// Init my scene objects:
	_axis.init(_prog);
	_rhand.init(_prog2, "models/rhand.m", mathand);		_rlarm.init(_prog2, "models/rlowerarm.m", matsleeve);		_ruarm.init(_prog2, "models/rupperarm.m", matsleeve);
	_rhandn.init(_prog, "models/rhand.m");					_rlarmn.init(_prog, "models/rlowerarm.m");						_ruarmn.init(_prog, "models/rupperarm.m");
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
	case 'q': math.rotx(-off);		_ruarmR  *= math;		break;
	case 'a': math.rotx(off);		_ruarmR  *= math;		break;	
	case 'w': math.rotx(-off);		_rlarmR  *= math;		break;
	case 's': math.rotx(off);		_rlarmR  *= math;		break;
	case 'e': math.rotx(-off);		_rhandR  *= math;		break;
	case 'd': math.rotx(off);		_rhandR  *= math;		break;
	case 'z': _rhand.changed = true; _viewwireframe = !_viewwireframe;  break;
	case 'x': _rhand.changed = true; _viewsmooth = false;  break;
	case 'c': _rhand.changed = true; _viewsmooth = true;  break;
	case 'v': _viewmodeln = !_viewmodeln;  break;
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
	if (_rhand.changed){
		_rhand.build(_viewsmooth);
		_rlarm.build(_viewsmooth);
		_ruarm.build(_viewsmooth);
	}
	if (_rhandn.changed){
		_rhandn.build(_viewsmooth);
		_rlarmn.build(_viewsmooth);
		_ruarmn.build(_viewsmooth);
	}
	//Entire Transform for Obj
	GsMat _rhandM, _rlarmM, _ruarmM;
	_ruarmM = _ruarmT * _ruarmR;
	_rlarmM = _ruarmM * _rlarmT * _rlarmR;
	_rhandM = _rlarmM * _rhandT * _rhandR;
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
	if(_viewwireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (_viewaxis) _axis.draw(stransf, sproj);
	if (_viewmodel){
		_rhand.draw(stransf * _rhandM, sproj, _sun);
		_rlarm.draw(stransf * _rlarmM, sproj, _sun);
		_ruarm.draw(stransf * _ruarmM, sproj, _sun);
	}
	if (_viewmodeln){
		_rhandn.draw(stransf * _rhandM, sproj);
		_rlarmn.draw(stransf * _rlarmM, sproj);
		_ruarmn.draw(stransf * _ruarmM, sproj);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glFlush();
	glutSwapBuffers();
}

