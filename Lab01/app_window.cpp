
# include <iostream>
# include <stdlib.h>
# include "app_window.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   //addMenuEntry ( "Option 0", evOption0 );
   //addMenuEntry ( "Option 1", evOption1 );
   _markc = GsColor::yellow;
   _w=w; _h=h;
 }

void AppWindow::initPrograms ()
 {
   // We are following the OpenGL API version 4, 
   // see docs at: http://www.opengl.org/sdk/docs/man/

   // Load your shaders and link your programs here:
   _vertexsh.load_and_compile ( GL_VERTEX_SHADER, "2dcolored.vert" );
   _fragsh.load_and_compile ( GL_FRAGMENT_SHADER, "flat.frag" );
   _prog.init_and_link ( _vertexsh, _fragsh );

   // Define buffers needed for each of your OpenGL objects:
   // (here they all use the same definitions because we are using the same shaders)
   // Program for rendering triangles:
   { GlObjects& o = _tris;
     o.set_program ( _prog );
     o.gen_vertex_arrays ( 1 );
     o.gen_buffers ( 2 );
     o.uniform_locations ( 2 );
     o.uniform_location ( 0, "vTransf" );
     o.uniform_location ( 1, "vLocf" );
   }

   // Define buffers needed for each of your OpenGL objects:
   { GlObjects& o = _pts;
     o.set_program ( _prog );
     o.gen_vertex_arrays ( 1 );
     o.gen_buffers ( 2 );
     o.uniform_locations ( 2 );
     o.uniform_location ( 0, "vTransf" );
     o.uniform_location ( 1, "vLocf" );
   }

   // Define buffers needed for each of your OpenGL objects:
   { GlObjects& o = _lines;
     o.set_program ( _prog );
     o.gen_vertex_arrays ( 1 );
     o.gen_buffers ( 2 );
     o.uniform_locations ( 2 );
     o.uniform_location ( 0, "vTransf" );
     o.uniform_location ( 1, "vLocf" );
   }
 }


// mouse events are in window coordinates, but your scene is in [0,1]x[0,1],
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
   switch ( key )
    { case ' ': // space bar
	   std::cout << "Space pressed.\n";
       _mark.set ( 1.5, 1.5 );
       redraw();
	   break;

	  case 27: // Esc was pressed
	   exit(1);
	}
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incx=0.02f;
   const float incy=0.02f;
   switch ( key )
    { case GLUT_KEY_LEFT:  linesTopOffset-=incx; break;
      case GLUT_KEY_RIGHT: linesTopOffset+=incx; break;
      case GLUT_KEY_UP:    linesBotOffset+=incy; break;
      case GLUT_KEY_DOWN:  linesBotOffset-=incy; break;
      default: rd=false; // no redraw
	}

   if (rd) 
    { _lines.changed = true; // mark that new lines have to be generated
   _pts.changed = true;
      redraw(); // ask the window to be rendered when possible
    }
 }

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   _markc = GsColor::yellow;
   _mark = windowToScene ( GsVec2(x,y) );
   _lines.changed = true; // mark that new lines have to be generated
   // note: a better design would be to only change the geometry when really needed so
   // that we minimize overhead with buffer definition and transfer, for example here
   // we could just send a transformation to our shader and keep the geometry the same.
   redraw();
 }

void AppWindow::glutMotion ( int x, int y )
 {
   _markc = GsColor::red;
   _mark = windowToScene ( GsVec2(x,y) );
   _lines.changed = true; // mark that new lines have to be generated
   redraw();
 }

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutIdle()
 {
   _pts.changed = true;
   redraw();
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

// here we will redraw the scene according to the current state of the application.
void AppWindow::buildObjects()
{
	// Define a cross with some lines:
	if (_lines.changed) // needs update
	{
		_linecoords.clear(); _linecolors.clear();
		// Encode our lines in buffers according to _mark position, _markc color and size s:
		const float s = 0.05f;
		_linecoords.push_back(GsVec(_mark.x - s, _mark.y, 0)); _linecolors.push_back(_markc);
		_linecoords.push_back(GsVec(_mark.x + s, _mark.y, 0)); _linecolors.push_back(_markc);
		_linecoords.push_back(GsVec(_mark.x, _mark.y - s, 0)); _linecolors.push_back(_markc);
		_linecoords.push_back(GsVec(_mark.x, _mark.y + s, 0)); _linecolors.push_back(_markc);

		_linecoords.push_back(GsVec(-1.0, -0.5 + linesBotOffset , -1.0)); _linecolors.push_back(GsColor::white);
		_linecoords.push_back(GsVec(1.0, -0.5 + linesBotOffset, -1.0)); _linecolors.push_back(GsColor::white);
		_linecoords.push_back(GsVec(-1.0, 0.5 + linesTopOffset, -1.0)); _linecolors.push_back(GsColor::white);
		_linecoords.push_back(GsVec(1.0, 0.5 + linesTopOffset, -1.0)); _linecolors.push_back(GsColor::white);
		// send data to OpenGL buffers:
		glBindBuffer(GL_ARRAY_BUFFER, _lines.buf[0]);
		glBufferData(GL_ARRAY_BUFFER, _linecoords.size() * 3 * sizeof(float), &_linecoords[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, _lines.buf[1]);
		glBufferData(GL_ARRAY_BUFFER, _linecolors.size() * 4 * sizeof(gsbyte), &_linecolors[0], GL_STATIC_DRAW);
		// mark that data does not need more changes:
		_lines.changed = false;
	}

	// Define some white points:
	if (_pts.changed) // needs update
	{
		_ptcoords.clear(); _ptcolors.clear();
		// Encode some white points in buffers:
		
		double ranX;
		double ranY;
		int randInt;
		for (int i = 0; i < 5000; i++) {
			randInt = rand() % 200 - 100;
			ranX = (double)randInt / 100;

			if (i < 2500) {
				randInt = rand() % (int)((.5 + linesBotOffset) * 100) - 100;
				ranY = (double)randInt / 100;
			}
			else {
				randInt = rand() % (int)((.5 - linesTopOffset) * 100) + ((.5 + linesTopOffset)* 100);
				ranY = (double)randInt / 100;
			}
			_ptcoords.push_back(GsVec(ranX, ranY, -1.0)); _ptcolors.push_back(GsColor::random());
		}
		// send data to OpenGL buffers:
		glBindBuffer(GL_ARRAY_BUFFER, _pts.buf[0]);
		glBufferData(GL_ARRAY_BUFFER, _ptcoords.size() * 3 * sizeof(float), &_ptcoords[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, _pts.buf[1]);
		glBufferData(GL_ARRAY_BUFFER, _ptcolors.size() * 4 * sizeof(gsbyte), &_ptcolors[0], GL_STATIC_DRAW);
		// mark that data does not need more changes:
		_pts.changed = false;
	}

	// Define some triangles:
	if (_tris.changed) // needs update
	{
		_tricoords.clear(); _tricolors.clear();
		//M [-.8 to -.4]X [-.5 to .5]Y
		_tricoords.push_back(GsVec(-0.7, -0.5, 0.0)); _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-0.8, -0.5, 0.0));  _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-0.7, 0.5, 0.0));  _tricolors.push_back(GsColor::red);

		_tricoords.push_back(GsVec(-0.6, -0.5, 0.0)); _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-0.5, 0.5, 0.0));  _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-0.7, 0.5, 0.0));  _tricolors.push_back(GsColor::red);
		
		_tricoords.push_back(GsVec(-0.5, 0.5, 0.0)); _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-0.4, -0.5, 0.0));  _tricolors.push_back(GsColor::red);
		_tricoords.push_back(GsVec(-0.5, -0.5, 0.0));  _tricolors.push_back(GsColor::red);

		_tricoords.push_back(GsVec(-0.6, -0.4, -0.1)); _tricolors.push_back(GsColor::black);
		_tricoords.push_back(GsVec(-0.55, 0.5, -0.1));  _tricolors.push_back(GsColor::black);
		_tricoords.push_back(GsVec(-0.65, 0.5, -0.1));  _tricolors.push_back(GsColor::black);
		//I [-.2 to .0]X [-.5 to .5]Y
		_tricoords.push_back(GsVec(-0.1, -0.3, 0.0)); _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(-0.2, -0.5, 0.0));  _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(0.0, -0.5, 0.0));  _tricolors.push_back(GsColor::green);

		_tricoords.push_back(GsVec(-0.06, 0.4, 0.0)); _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(-0.14, 0.4, 0.0));  _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(-0.06, -0.4, 0.0));  _tricolors.push_back(GsColor::green);

		_tricoords.push_back(GsVec(-0.06, -0.4, 0.0)); _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(-0.14, 0.4, 0.0));  _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(-0.14, -0.4, 0.0));  _tricolors.push_back(GsColor::green);
		
		_tricoords.push_back(GsVec(-0.1, 0.3, 0.0)); _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(-0.2, 0.5, 0.0));  _tricolors.push_back(GsColor::green);
		_tricoords.push_back(GsVec(0.0, 0.5, 0.0));  _tricolors.push_back(GsColor::green);
		//G [.2 to .4]X [-.5 to .5]Y
		_tricoords.push_back(GsVec(0.4, 0.4, 0.0)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.4, 0.5, 0.0));  _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.2, 0.5, 0.0));  _tricolors.push_back(GsColor::blue);

		_tricoords.push_back(GsVec(0.2, 0.5, 0.0)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.3, 0.5, 0.0));  _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.2, -0.5, 0.0));  _tricolors.push_back(GsColor::blue);

		_tricoords.push_back(GsVec(0.2, -0.5, 0.0)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.2, -0.4, 0.0));  _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.4, -0.5, 0.0));  _tricolors.push_back(GsColor::blue);

		_tricoords.push_back(GsVec(0.4, -0.5, 0.0)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.4, 0.0, 0.0));  _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.3, -0.5, 0.0));  _tricolors.push_back(GsColor::blue);

		_tricoords.push_back(GsVec(0.4, 0.0, 0.0)); _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.4, -0.1, 0.0));  _tricolors.push_back(GsColor::blue);
		_tricoords.push_back(GsVec(0.3, 0.0, 0.0));  _tricolors.push_back(GsColor::blue);
		//U [.6 to .8]X [-.5 to .5]Y
		_tricoords.push_back(GsVec(0.6, 0.5, 0.0)); _tricolors.push_back(GsColor::yellow);
		_tricoords.push_back(GsVec(0.6, -0.5, 0.0));  _tricolors.push_back(GsColor::yellow);
		_tricoords.push_back(GsVec(0.65, -0.5, 0.0));  _tricolors.push_back(GsColor::yellow);

		_tricoords.push_back(GsVec(0.6, -0.5, 0.0)); _tricolors.push_back(GsColor::yellow);
		_tricoords.push_back(GsVec(0.6, -0.4, 0.0));  _tricolors.push_back(GsColor::yellow);
		_tricoords.push_back(GsVec(0.8, -0.5, 0.0));  _tricolors.push_back(GsColor::yellow);

		_tricoords.push_back(GsVec(0.6, -0.5, 0.0)); _tricolors.push_back(GsColor::yellow);
		_tricoords.push_back(GsVec(0.8, -0.5, 0.0));  _tricolors.push_back(GsColor::yellow);
		_tricoords.push_back(GsVec(0.8, -0.4, 0.0));  _tricolors.push_back(GsColor::yellow);

		_tricoords.push_back(GsVec(0.75, -0.5, 0.0)); _tricolors.push_back(GsColor::yellow);
		_tricoords.push_back(GsVec(0.8, -0.5, 0.0));  _tricolors.push_back(GsColor::yellow);
		_tricoords.push_back(GsVec(0.8, 0.5, 0.0));  _tricolors.push_back(GsColor::yellow);

		// send data to OpenGL buffers:
		glBindBuffer(GL_ARRAY_BUFFER, _tris.buf[0]);
		glBufferData(GL_ARRAY_BUFFER, _tricoords.size() * 3 * sizeof(float), &_tricoords[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, _tris.buf[1]);
		glBufferData(GL_ARRAY_BUFFER, _tricolors.size() * 4 * sizeof(gsbyte), &_tricolors[0], GL_STATIC_DRAW);
		// mark that data does not need more changes:
		_tris.changed = false;
	}
 }

void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Update objects if needed:
   buildObjects();

   // Define some identity transformations - our shaders require them but in this
   // example code we do not need to use them, so just let them be GsMat::id:
   GsMat vtransf(GsMat::id), vproj(GsMat::id); // transformations

   // Draw Lines:
   glUseProgram ( _lines.prog );
   glBindVertexArray ( _lines.va[0] );

   glBindBuffer ( GL_ARRAY_BUFFER, _lines.buf[0] ); // positions
   glEnableVertexAttribArray ( 0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, _lines.buf[1] ); // colors
   glEnableVertexAttribArray ( 1 );
   glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

   glUniformMatrix4fv ( _lines.uniloc[0], 1, GL_FALSE, vtransf.e );
   glUniformMatrix4fv ( _lines.uniloc[1], 1, GL_FALSE, vproj.e );

   glDrawArrays ( GL_LINES, 0, _linecoords.size() );
   // (see documentation at: https://www.opengl.org/sdk/docs/man/html/glDrawArrays.xhtml)

   // Draw Points:
   glUseProgram ( _pts.prog );
   glBindVertexArray ( _pts.va[0] );

   glBindBuffer ( GL_ARRAY_BUFFER, _pts.buf[0] ); // positions
   glEnableVertexAttribArray ( 0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, _pts.buf[1] ); // colors
   glEnableVertexAttribArray ( 1 );
   glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

   glUniformMatrix4fv ( _pts.uniloc[0], 1, GL_FALSE, vtransf.e );
   glUniformMatrix4fv ( _pts.uniloc[1], 1, GL_FALSE, vproj.e );

   glDrawArrays ( GL_POINTS, 0, _ptcoords.size() );

   // Draw Triangles:
   glUseProgram ( _tris.prog );
   glBindVertexArray ( _tris.va[0] );

   glBindBuffer ( GL_ARRAY_BUFFER, _tris.buf[0] ); // positions
   glEnableVertexAttribArray ( 0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, _tris.buf[1] ); // colors
   glEnableVertexAttribArray ( 1 );
   glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

   glUniformMatrix4fv ( _tris.uniloc[0], 1, GL_FALSE, vtransf.e );
   glUniformMatrix4fv ( _tris.uniloc[1], 1, GL_FALSE, vproj.e );

   glDrawArrays ( GL_TRIANGLES, 0, _tricoords.size() );

   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

