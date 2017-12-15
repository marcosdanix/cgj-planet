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

int WinX = 800, WinY = 600;
int WindowHandle = 0;
unsigned int FrameCount = 0;
float zoom = 1.5;
float aspect;

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

void recalculateProjection()
{	
	mat4 projection = glm::ortho(-aspect*zoom, aspect*zoom, -zoom, zoom, -4.0f, 4.0f);
	//A bit of a mouthful
	Storage<Scene>::instance().get("example")->camera().projection(projection);
}

//glutReshapeFunc
void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
	aspect = float(w) / float(h);

	recalculateProjection();	
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

void update()
{
	Storage<Scene>::instance().get("example")->update();
}

void timerFPS()
{
	update();
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
	zoom -= 0.05*direction;
	recalculateProjection();
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
	static int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	static int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	if (x != centerX || y != centerY) glutWarpPointer(centerX, centerY);

	float rotx = x - centerX;
	float roty = y - centerY;

	orbit.rotate(rotx, roty);
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
	glutTimerFunc(16, timer, 1);
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

//#define VERT_SHADER_FILE "assets/basic_shader.vert"
//#define FRAG_SHADER_FILE "assets/basic_shader.frag"
//#define VERT_SHADER_FILE "assets/basic_color.vert"
//#define FRAG_SHADER_FILE "assets/basic_color.frag"
#define VERT_LAND_FILE "assets/blinn_phong.vert"
#define FRAG_LAND_FILE "assets/blinn_phong.frag"
//#define FRAG_LAND_FILE "assets/height_shader.frag"
#define FRAG_WATER_FILE "assets/water_bp.frag"
//#define MUNKEY_FILE "assets/munkey.obj"	
//#define MUNKEY_FILE "assets/bunny.obj"	
//#define MUNKEY_FILE "assets/icosphere.obj"	
#define LAND_FILE "assets/icosphere2.obj"	
#define WATER_FILE "assets/icosphere.obj"

ShaderProgram shaderProgram;
VertexShader vertexShader;
FragmentShader fragmentShader;

ShaderProgram waterShaderProgram;
VertexShader waterVertexShader;
FragmentShader waterFragmentShader;

void createShaderProgram()
{
	vertexShader.load(VERT_LAND_FILE);
	fragmentShader.load(FRAG_LAND_FILE);

	shaderProgram.create()
		.attach(&vertexShader) //if it's not compiled, it compiles the shader
		.attach(&fragmentShader)
		.bindAttribute(VERTICES, "in_Position")
		.bindAttribute(NORMALS, "in_Normal")
		.link();
	
	//Add to the storage so it can be accessed by the rest of the engine
	//It's not necessary, but it's being used to test this feature
	Storage<ShaderProgram>::instance().add("land", &shaderProgram);


	waterVertexShader.load(VERT_LAND_FILE);
	waterFragmentShader.load(FRAG_WATER_FILE);

	waterShaderProgram.create()
		.attach(&waterVertexShader) //if it's not compiled, it compiles the shader
		.attach(&waterFragmentShader)
		.bindAttribute(VERTICES, "in_Position")
		.bindAttribute(NORMALS, "in_Normal")
		.link();

	//Add to the storage so it can be accessed by the rest of the engine
	//It's not necessary, but it's being used to test this feature
	Storage<ShaderProgram>::instance().add("water", &waterShaderProgram);
}

Mesh land_mesh;
Mesh water_mesh;

void createMeshes()
{
	PerlinFilter perlin(1.5f, 0.19f, -0.025f, 8, 1.8);


	land_mesh.load(LAND_FILE, perlin);
	water_mesh.load(WATER_FILE);

	Storage<Mesh>::instance().add("land",  &land_mesh);
	Storage<Mesh>::instance().add("water", &water_mesh);
}



void setupCamera()
{
	aspect = float(WinX) / float(WinY);
	mat4 projection = glm::ortho(-aspect*zoom, aspect*zoom, -zoom, zoom, -4.0f, 4.0f);
	orbit = OrbitControl(2.0f, 0.5f, 0.0f);
	camera = Camera(&orbit, projection);
}

Scene scene;
Node land;
Node water;

void planetRotate(Node& munkey)
{
	munkey.transform().rotateY(0.1f / 60.0f);
}


void createScene()
{
	scene = Scene(camera);
	land.mesh(*Storage<Mesh>::instance().get("land"));
	land.shader(*Storage<ShaderProgram>::instance().get("land"));
	land.updateFunc(planetRotate);
	scene.root()->addChild(&land);

	water.mesh(*Storage<Mesh>::instance().get("water"));
	water.shader(*Storage<ShaderProgram>::instance().get("water"));
	land.addChild(&water);
	
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