
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so/so_axis.h"
# include "so/so_model.h"
# include "so/so_modelnorm.h"

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
{
private:
	// OpenGL shaders and programs:
	GlShader _vertexsh, _fragsh, _goufrag, _gouvert;
	GlProgram _prog, _prog2;
	// My scene objects:
	SoAxis _axis;				bool _viewaxis;
	SoModel _rhand;
	SoModel _rlarm;
	SoModel _ruarm;			bool _viewmodel;
	SoModelNorm _rhandn;
	SoModelNorm _rlarmn;
	SoModelNorm _ruarmn;		bool _viewmodeln;
	GsMat _rhandT;		GsMat _rhandR;
	GsMat _rlarmT;		GsMat _rlarmR;
	GsMat	_ruarmT;		GsMat _ruarmR;
	//Light
	GlLight _sun;
	//ShaderOpt
	bool  _viewsmooth, _viewwireframe;
	// App data:
	enum MenuEv { evOption0, evOption1 };
	float _rotx, _roty, _fovy;
	int _w, _h;
public:
	AppWindow(const char* label, int x, int y, int w, int h);
	void initPrograms();
	GsVec2 windowToScene(const GsVec2& v);

private: // functions derived from the base class
	virtual void glutMenu(int m);
	virtual void glutKeyboard(unsigned char key, int x, int y);
	virtual void glutSpecial(int key, int x, int y);
	virtual void glutMouse(int b, int s, int x, int y);
	virtual void glutMotion(int x, int y);
	virtual void glutDisplay();
	virtual void glutReshape(int w, int h);
};

#endif // APP_WINDOW_H
