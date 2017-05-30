
# include <iostream>
# include "app_window.h"

//==========================================================================
// Main routine
//==========================================================================
int main(int argc, char** argv ) {
	// Init freeglut library:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitContextProfile(GLUT_CORE_PROFILE );

	// Now create the window of your application:
	AppWindow* w = new AppWindow("CSE 170 Project 2", 300, 300, 800, 600);

	// Loops forever
	w->run();
}
