
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so/so_axis.h"
# include "so/so_ground.h"
# include "so/so_cactus.h"
# include "so/so_scorpion.h"
// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
{
private:
	// OpenGL shaders and programs:
	GlShader _vertexsh, _fragsh, _goufrag, _gouvert, _goufragtext, _gouverttext;
	GlProgram _prog, _prog2, _prog3;
	// My scene objects:
	SoAxis _axis;				bool _viewaxis;
	SoGround _ground;			bool _viewground;
	SoCactus _cactus;				SoCactus	_cactussh;
	SoCactus _cactus2;			SoCactus	_cactus2sh;			bool _viewcactus;
	SoScorpion _scorpion;		SoScorpion _scorpionsh;		bool _viewscorpion;
	//Camera
	bool _animated;
	// App data:
	enum MenuEv { evOption0, evOption1, evOption2, evOption3};
	float _scorpRY;
	GsVec _scorpF;
	float _scorpD1, _scorpD2, _scorpD3, _scorpD4, _scorpD5;
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
