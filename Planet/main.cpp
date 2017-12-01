#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

#define CAPTION "Hello Blank World"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

/////////////////////////////////////////////////////////////////////// CALLBACKS

//glutCloseFunc
void cleanup()
{
}

//glutDisplayFunc
void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw something
	glutSwapBuffers();
}

//glutIdleFunc
void idle()
{
	glutPostRedisplay();
}

//glutReshapeFunc
void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

//glutTimerFunc
void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}


//glutKeyboardFunc
void keyboard(unsigned char key, int x, int y)
{
	// glutGetModifiers for SHIFT, CTRL, ALT
}

//glutKeyboardUpFunc
void keyboardUp(unsigned char key, int x, int y)
{
	// glutGetModifiers for SHIFT, CTRL, ALT
}

//glutSpecialFunc
void special(int key, int x, int y)
{

}

//glutSpecialUpFunc
void specialUp(int key, int x, int y)
{

}

//glutMouseFunc
void mouse(int button, int state, int x, int y)
{
	// glutGetModifiers for SHIFT, CTRL, ALT
	// button GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
	// state for GLUT_UP, GLUT_DOWN
}

//glutMouseWheelFunc
void mouseWheel(int wheel, int direction, int x, int y)
{
	//wheel is wheel number (useless)
	//direction is +/- 1

}

//glutMotionFunc
//mouse motion WHILE pressing a mouse button
void motion(int x, int y)
{
}

//glutPassiveMotionFunc
//mouse motion WHILE NOT pressing a mouse button
void passiveMotion(int x, int y)
{
}

//glutWindowStatusFunc
void windowStatus(int state)
{
	switch (state) {
	case GLUT_FULLY_RETAINED:
	case GLUT_PARTIALLY_RETAINED:
		glutDisplayFunc(display);
		break;
	case GLUT_FULLY_COVERED:
	case GLUT_HIDDEN:
		glutDisplayFunc(nullptr);
		break;
	}
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	//glutInitContextFlags(GLUT_DEBUG);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	// You might get GL_INVALID_ENUM when loading GLEW.
}

void checkOpenGLInfo()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL()
{
	checkOpenGLInfo();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);
	glutWindowStatusFunc(windowStatus);
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////