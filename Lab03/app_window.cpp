
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
{
  initPrograms ();
  addMenuEntry ( "Toggle Axis", evOption0 );
  _viewaxis = false;
  _viewclock = _viewclocksh = true, _pauseclock = false;
  _fovy = GS_TORAD(60.0f);
  _rotx = _roty = 0;
	_lightx = _lighty = _lightz = 0;
  _w = w;
  _h = h;
	_milis = 0, _resetmilis = 0;
}

void AppWindow::initPrograms (){
  _vertexsh.load_and_compile ( GL_VERTEX_SHADER, "../3dcolored.vert" );
  _fragsh.load_and_compile ( GL_FRAGMENT_SHADER, "../flat.frag" );
  _prog.init_and_link ( _vertexsh, _fragsh );

  _axis.init ( _prog );
  _clock.init( _prog );
}

GsVec2 AppWindow::windowToScene ( const GsVec2& v ){
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
}

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y ){
  bool rd = true;
	const float incr = 0.01f;
  switch ( key ){
		case ' ': _clock.changed = true; _pauseclock = !_pauseclock; break;
		case '/': _viewclocksh = !_viewclocksh; break;
    case 'q': _lightx += incr; break;
		case 'a': _lightx -= incr; break;
		case 'w': _lighty += incr; break;
		case 's': _lighty -= incr; break;
		case 'e': _lightz += incr; break;
		case 'd': _lightz -= incr; break;
		case 13: _clock.changed = true; _resetmilis = glutGet(GLUT_ELAPSED_TIME); break;
    case 27 : exit(1); // Esc was pressed

  }
  if (rd) redraw();
}

void AppWindow::glutSpecial ( int key, int x, int y ){
  bool rd=true;
  const float incr=GS_TORAD(2.5f);
  const float incf=0.05f;
	switch ( key ){
    case GLUT_KEY_LEFT:      _roty-=incr; break;
    case GLUT_KEY_RIGHT:     _roty+=incr; break;
    case GLUT_KEY_UP:        _rotx-=incr; break;
    case GLUT_KEY_DOWN:      _rotx+=incr; break;
    case GLUT_KEY_PAGE_UP:   _fovy-=incf; break;
    case GLUT_KEY_PAGE_DOWN: _fovy+=incf; break;
    case GLUT_KEY_HOME:      _fovy=GS_TORAD(60.0f); _rotx=_roty=0; break;
    default: return; // return without rendering
	}
  if (rd) redraw(); // ask the window to be rendered when possible
}

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   // The mouse is not used in this example.
   // Recall that a mouse click in the screen corresponds
   // to a whole line traversing the 3D scene.
 }

void AppWindow::glutMotion ( int x, int y ){
}

void AppWindow::glutMenu ( int m ){
	bool rd = true;
	switch(m){
		case 0: _viewaxis = !_viewaxis; break;
	}
	if (rd) redraw();
}

void AppWindow::glutReshape ( int w, int h ){
  glViewport( 0, 0, w, h );
  _w=w; _h=h;
}

void AppWindow::glutIdle(){
	if(_pauseclock)
		_resetmilis = glutGet(GLUT_ELAPSED_TIME) - _milis;
	_milis = glutGet(GLUT_ELAPSED_TIME) - _resetmilis;  
	_clock.changed = true;
	redraw();
}

void AppWindow::glutDisplay (){
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  if ( _axis.changed ){
		_axis.build(1.0f);
	}
	if (_clock.changed){
		_clock.build(_milis);
  }

   // Define our scene transformation:
   GsMat rx, ry, stransf, tran, shadowtransf;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix

//	 rx.rotx(GS_TORAD(90.0f));
//	 tran.translation(0.0f, -.75f*1.4, -.75f*1.4);
//	 shadowtransf = tran*rx;
   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, persp, sproj, shadowproj;
   GsVec eye(0,0,2), center(0.0,-0.5,-0.5), up(0,1,0);
   camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix
   float aspect=1.0f, znear=0.1f, zfar=50.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
   sproj = persp * camview; // set final scene projection

//	 eye = GsVec(0 + _lightx, -1 + _lighty, -1 + _lightz);
//	 center = GsVec(0.0, -0.75*1.4, -0.75*1.4);
//	 camview.lookat(eye, center, up);
//	 shadowtransf = camview;
//	 persp.perspective(GS_TORAD(-45.0f), aspect, znear, zfar);

	GsVec v, q, n;
	float dotvn, dotqn;
	v.set(0.0f + _lightx, .75f*1.4f + _lighty, .75f*1.4f + _lightz);
	v = v / sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	q.set(0.0f, -.75f*1.4f, -.75f*1.4f);
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
//	shadowproj.zero();
//	shadowproj.setl1(GsVec(dotqn, 0.0f, 0.0f));
//	shadowproj.setl2(GsVec(0.0f, dotqn, 0.0f));
//	shadowproj.setl3(GsVec(0.0f, 0.0f, dotqn));
//	shadowproj.setl4(n);
   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause the values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.

   // Draw:
   if ( _viewaxis ) _axis.draw ( stransf, sproj );
   if (_viewclock) _clock.draw(stransf, sproj);
//	 if (_viewclocksh) _clock.draw(stransf*shadowtransf, sproj);
//	 if (_viewclocksh) _clock.draw(shadowproj*stransf, sproj);
	 if (_viewclocksh) _clock.draw(stransf*shadowproj, sproj);
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

