#include <iostream>
#include <sstream>
#include <string>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "../Engine/Engine.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace cgj;

#define CAPTION "Hello Munkey"

//Shader input attributes
#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

//STUFF

Camera camera;
OrbitControl orbit;


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
	Scene* scene = Storage<Scene>::instance().get("example");
	scene->draw();

	glutSwapBuffers();
}

//glutIdleFunc
void idle()
{
	//glutPostRedisplay();
}

//glutReshapeFunc
void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
	
	float aspect = float(w) / float(h);
	mat4 projection = glm::ortho(-aspect*2.0, aspect*2.0, -2.0, 2.0, -2.0, 4.0);
	camera.projection(projection);

}

void timer(int value);

void timerRefreshWindow()
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void timerFPS()
{
	glutPostRedisplay();
	glutTimerFunc(16, timer, 1);
}

//glutTimerFunc
void timer(int value)
{
	switch (value) {
	case 0:
		timerRefreshWindow();
		break;
	case 1:
		timerFPS();
		break;
	}
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
	//glutIdleFunc(idle);
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

/////////////////////////////////////////////////////////////////////// ENGINE SETUP

#define VERT_SHADER_FILE "assets/basic_shader.vert"
#define FRAG_SHADER_FILE "assets/basic_shader.frag"
//#define VERT_SHADER_FILE "assets/basic_color.vert"
//#define FRAG_SHADER_FILE "assets/basic_color.frag"
//#define VERT_SHADER_FILE "assets/blinn_phong.vert"
//#define FRAG_SHADER_FILE "assets/blinn_phong.frag"
#define MUNKEY_FILE "assets/munkey.obj"	
//#define MUNKEY_FILE "assets/icosphere.obj"	

ShaderProgram shaderProgram;
VertexShader vertexShader;
FragmentShader fragmentShader;

void createShaderProgram()
{
	vertexShader.load(VERT_SHADER_FILE);
	fragmentShader.load(FRAG_SHADER_FILE);

	shaderProgram.create()
		.attach(&vertexShader) //if it's not compiled, it compiles the shader
		.attach(&fragmentShader)
		.bindAttribute(VERTICES, "in_Position")
		.bindAttribute(NORMALS, "in_Normal")
		.link();
	
	//Add to the storage so it can be accessed by the rest of the engine
	//It's not necessary, but it's being used to test this feature
	Storage<ShaderProgram>::instance().add("basic", &shaderProgram);
}

Mesh munkey_mesh;

void createMeshes()
{
	munkey_mesh.load(MUNKEY_FILE);

	Storage<Mesh>::instance().add("munkey", &munkey_mesh);
}



void setupCamera()
{
	float aspect = float(WinX) / float(WinY);
	mat4 projection = glm::ortho(-aspect*2.0, aspect*2.0, -2.0, 2.0, -2.0, 4.0);
	orbit = OrbitControl(2.0f, 0.0f, 0.0f);
	camera = Camera(&orbit, projection);
}

Scene scene;
Node munkey;

void createScene()
{
	scene = Scene(camera);
	munkey.mesh(*Storage<Mesh>::instance().get("munkey"));
	munkey.shader(*Storage<ShaderProgram>::instance().get("basic"));
	scene.root()->addChild(&munkey);
	
	Storage<Scene>::instance().add("example", &scene);
}


void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createShaderProgram();
	createMeshes();
	setupCamera();
	createScene();	
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////