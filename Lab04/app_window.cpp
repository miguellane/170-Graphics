
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h ){
	initPrograms ();
  addMenuEntry ( "Option 0", evOption0 );
  addMenuEntry ( "Option 1", evOption1 );
  _viewaxis = false;
  _viewcylinder = true;
	_viewnorm = false;
	_viewsmooth = false;
  _fovy = GS_TORAD(60.0f);
  _rotx = _roty = 0;
  _nfaceoff = 0;
  _w = w;
  _h = h;
 _sun.pos = GsVec(0.0f, 1.0f, 1.0f);
 GsColor::gray.get(_sun.ambient);	 GsColor::white.get(_sun.diffuse);	 GsColor::white.get(_sun.specular);
  //Hardcode Val
  _nface = 10;
}

void AppWindow::initPrograms ()
 {
   // Load your shaders and link your programs here:
   _vertexsh.load_and_compile ( GL_VERTEX_SHADER, "../3dcolored.vert" );
   _fragsh.load_and_compile ( GL_FRAGMENT_SHADER, "../flat.frag" );
	 _gouvert.load_and_compile(GL_VERTEX_SHADER, "../gouraud.vert");
	 _goufrag.load_and_compile(GL_FRAGMENT_SHADER, "../gouraud.frag");
   _prog.init_and_link ( _vertexsh, _fragsh);
	 _prog2.init_and_link (_gouvert, _goufrag);

   // Init my scene objects:
   _axis.init ( _prog );
	 _norm.init ( _prog );
   _cylinder.init( _prog2 );
 }

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene ( const GsVec2& v )
 {
   // GsVec2 is a lighteweight class suitable to return by value:
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
 }

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
 {
	bool rd = true;
	const float off = (float).01;
   switch ( key ) {
		case ' ': _viewaxis = !_viewaxis;  break;
	  case 'q': _cylinder.changed = true; _nfaceoff++; break;
	  case 'a': _cylinder.changed = true; _nfaceoff--;  break;
	  case 'w':  _sun.pos.y += off;  break;
	  case 's':  _sun.pos.y -= off;  break;

	  case 'z': _cylinder.changed = true; _viewsmooth = false;  break;
	  case 'x': _cylinder.changed = true; _viewsmooth = true;  break;
		case 'c': _viewnorm = true;  break;
		case 'v': _viewnorm = false;  break;
	  case 27 : exit(1); // Esc was pressed
	}
	if(rd) redraw();
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incr=GS_TORAD(2.5f);
   const float incf=0.05f;
   switch ( key )
    { case GLUT_KEY_LEFT:      _roty-=incr; break;
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

void AppWindow::glutMotion ( int x, int y )
 {
 }

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay () {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   if ( _axis.changed )				_axis.build(1.0f);
   if (_cylinder.changed)			_cylinder.build(_nface + _nfaceoff, _viewsmooth);
	 if (_cylinder.changed)			_norm.build(_nface + _nfaceoff, _viewsmooth);
   // Define our scene transformation:
   GsMat rx, ry, stransf;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix

   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, persp, sproj;

   GsVec eye(0,0,2), center(0,0,0), up(0,1,0);
   camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix

   float aspect=1.0f, znear=0.1f, zfar=50.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
   sproj = persp * camview; // set final scene projection

   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause the values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.

   // Draw:
   if ( _viewaxis ) _axis.draw ( stransf, sproj );
   if (_viewcylinder) _cylinder.draw(stransf, sproj, _sun);
	 if (_viewnorm) _norm.draw(stransf, sproj);

   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

