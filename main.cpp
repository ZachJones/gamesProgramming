#include <iostream>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <SDL.h>

//#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS //force glm to use radians //must do **before** including GLM headers
//NOTE: GLSL uses radians, so will do the same, for consistency

#include <glm/glm.hpp> //include the main glm header
#include <glm/gtc/matrix_transform.hpp> //include functions to ease the calculation of the view and projection matrices
#include <glm/gtc/type_ptr.hpp> //include functionality for converting a matrix object into a float array for usage in OpenGL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace std;

/////////////////////
// global variables

std::string exeName;
SDL_Window *win; //pointer to the SDL_Window
SDL_GLContext context; //the SDL_GLContext

//string holding the **source** of our vertex shader, to save loading from a file
const std::string strVertexShader(
	#ifdef OPENGL_VERSION_3_1
		"#version 140\n"
	#endif
	#ifdef OPENGL_VERSION_3_3
		"#version 140\n"
	#endif
	"in vec4 position;\n"
	"in vec4 colour;\n"
	"uniform mat4 modelMatrix;\n"
	"uniform mat4 viewMatrix;\n"
	"uniform mat4 projectionMatrix;\n"
	"out vec4 fragColour;\n" //send colours to frag shader
	"void main()\n"
	"{\n"
	"   gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;\n"
	"	fragColour = colour;\n"
	"}\n"
	);

//string holding the **source** of our fragment shader, to save loading from a file
const std::string strFragmentShader(
	#ifdef OPENGL_VERSION_3_1
		"#version 140\n"
	#endif
	#ifdef OPENGL_VERSION_3_3
		"#version 140\n"
	#endif
	"in vec4 fragColour;\n"
	"out vec4 outputColor;\n"
	"void main()\n"
	"{\n"
	"   outputColor = fragColour;\n"
	"}\n"
	);


//our variables
bool done = false;

const float vertexPositions[] = {

	//############################################################################################################## POSITIONS

	//############################################################################# PLANE
	
	5.0f, -1.0f, -5.0f, 1.0f,
	-5.0f, -1.0f, 5.0f, 1.0f,
	-5.0f, -1.0f, -5.0f, 1.0f,
	
	5.0f, -1.0f, -5.0f, 1.0f,
	5.0f, -1.0f, 5.0f, 1.0f,
	-5.0f, -1.0f, 5.0f, 1.0f,

	//Walls
	-5.0f, -1.0f, 5.0f, 1.0f,
	5.0f, -1.0f, 5.0f, 1.0f,
	-5.0f, 5.0f, 5.0f, 1.0f,
	
	5.0f, 5.0f, 5.0f, 1.0f,
	-5.0f, 5.0f, 5.0f, 1.0f,
	5.0f, -1.0f, 5.0f, 1.0f,
	
	5.0f, -1.0f, -5.0f, 1.0f,
	-5.0f, -1.0f, -5.0f, 1.0f,
	-5.0f, 5.0f, -5.0f, 1.0f,
	
	5.0f, -1.0f, -5.0f, 1.0f,
	-5.0f, 5.0f, -5.0f, 1.0f,
	5.0f, 5.0f, -5.0f, 1.0f,

	-5.0f, 5.0f, 5.0f, 1.0f,
	-5.0f, 5.0f, -5.0f, 1.0f,
	-5.0f, -1.0f, -5.0f, 1.0f,
	
	-5.0f, 5.0f, 5.0f, 1.0f,
	-5.0f, -1.0f, -5.0f, 1.0f,
	-5.0f, -1.0f, 5.0f, 1.0f,

	5.0f, 5.0f, -5.0f, 1.0f,
	5.0f, 5.0f, 5.0f, 1.0f,
	5.0f, -1.0f, -5.0f, 1.0f,

	5.0f, -1.0f, -5.0f, 1.0f,
	5.0f, 5.0f, 5.0f, 1.0f,
	5.0f, -1.0f, 5.0f, 1.0f,

	//############################################################################################################## COLOURS

	//############################################################################################################## PLANE
	
	0.9f, 0.9f, 1.0f, 1.0f,
	0.9f, 0.9f, 1.0f, 1.0f,
	0.9f, 0.9f, 1.0f, 1.0f,

	0.9f, 0.9f, 1.0f, 1.0f,
	0.9f, 0.9f, 1.0f, 1.0f,
	0.9f, 0.9f, 1.0f, 1.0f,

	//Blue Wall
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,

	//Cyan Wall
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,

	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,

	//Pink wall
	1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,

	1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,

	//Red Wall
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

};

const Uint8 *movement; //movement variable for viewpoint changes

//the offset we'll pass to the GLSL
double offsetX = -0.5; //using different values from CPU and static GLSL examples, to make it clear this is working
double offsetY = -0.5; //NOTE: we could use an array and pass the pointer, to be simpler & more efficent
double offsetXSpeed = 0.2; //rate of change of offsetX in units per second
double offsetYSpeed = 0.2; //rate of change of offsetY in units per second

float angle = 0.0f;
float angle2 = 0.0f;

float viewChangeX = 0.0f; //Change the view matrix over time
float viewChangeY = 0.0f; //Change the view matrix over time
float viewChangeZ = 0.0f; //Change the view matrix over time

float eyeCentreX = 0.0f;
float eyeCentreY = -0.9f;
float eyeCentreZ = 0.0f;

float centreX = 0.0f;
float centreY = -0.9f;
float centreZ = 0.0f;

Sint32 mouseX = 0;
Sint32 mouseY = 0;

//GLFWwindow* window;

float movementRotationAngle = 0.0f;
float movementRadius = 2.0f;

float moveSpeedX = 0.0f;
float moveSpeedY = 0.0f;
float moveSpeedZ = 0.0f;

string currentView = "player";

//our GL and GLSL variables

glm::mat4 modelMatrix;
glm::mat4 modelMatrix2;
glm::mat4 modelMatrix3;
glm::mat4 modelMatrix4;
glm::mat4 modelMatrix5;
GLuint matrixLocation; //<---------------------------------------------------------------- Should change to "modelLocation"

glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); //Default view (identity matrix)
GLuint viewLocation;

glm::mat4 projectionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
GLuint projectionLocation;

GLint colourLocation;

GLuint theProgram; //GLuint that we'll fill in to refer to the GLSL program (only have 1 at this point)
GLint positionLocation; //GLuint that we'll fill in with the location of the `offset` variable in the GLSL
GLint offsetLocation; //GLuint that we'll fill in with the location of the `offset` variable in the GLSL

GLuint positionBufferObject;
GLuint vao;

// end Global Variables
/////////////////////////


void initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	cout << "SDL initialised OK!\n";
}

void createWindow()
{
	//get executable name, and use as window title
	int beginIdxWindows = exeName.rfind("\\"); //find last occurrence of a backslash
	int beginIdxLinux = exeName.rfind("/"); //find last occurrence of a forward slash
	int beginIdx = max(beginIdxWindows, beginIdxLinux);
	std::string exeNameEnd = exeName.substr(beginIdx + 1);
	const char *exeNameCStr = exeNameEnd.c_str();

	//create window
	win = SDL_CreateWindow(exeNameCStr, 100, 100, 600, 600, SDL_WINDOW_OPENGL); //same height and width makes the window square ...

	//error handling
	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "SDL CreatedWindow OK!\n";
}

void setGLAttributes()
{
	#ifdef OPENGL_VERSION_3_1
		cout << "Built for OpenGL Version 3.1" << endl;
		// set the opengl context version
		int major = 3;
		int minor = 1;
	#endif
	#ifdef OPENGL_VERSION_3_3
		cout << "Built for OpenGL Version 3.3" << endl;
		// set the opengl context version
		int major = 3;
		int minor = 3;
	#endif
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //core profile
	cout << "Set OpenGL context to version " << major << "." << minor << " OK!\n";
}

void createContext()
{
	context = SDL_GL_CreateContext(win);
	if (context == nullptr){
		SDL_DestroyWindow(win);
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "Created OpenGL context OK!\n";
}

void initGlew()
{
	GLenum rev;
	glewExperimental = GL_TRUE; //GLEW isn't perfect - see https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
	rev = glewInit();
	if (GLEW_OK != rev){
		std::cout << "GLEW Error: " << glewGetErrorString(rev) << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLEW Init OK!\n";
	}
}

GLuint createShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint createProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}

void initializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, strFragmentShader));

	theProgram = createProgram(shaderList);
	if (theProgram == 0)
	{
		cout << "GLSL program creation error." << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLSL program creation OK! GLUint is: " << theProgram << std::endl;
	}

	positionLocation = glGetAttribLocation(theProgram, "position");
	colourLocation = glGetAttribLocation(theProgram, "colour");
	//offsetLocation = glGetUniformLocation(theProgram, "offset");
	matrixLocation = glGetUniformLocation(theProgram, "modelMatrix");
	viewLocation = glGetUniformLocation(theProgram, "viewMatrix");
	projectionLocation = glGetUniformLocation(theProgram, "projectionMatrix");

	//clean up shaders (we don't need them anymore as they are no in theProgram
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

void initializeVertexBuffer()
{
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "positionBufferObject created OK! GLUint is: " << positionBufferObject << std::endl;
}

void loadAssets()
{
	initializeProgram(); //create GLSL Shaders, link into a GLSL program

	initializeVertexBuffer(); //load data into a vertex buffer

	glGenVertexArrays(1, &vao); //create a Vertex Array Object
	glBindVertexArray(vao); //make the VAO active
	cout << "Vertex Array Object created OK! GLUint is: " << vao << std::endl;

	cout << "Loaded Assets OK!\n";
}

void updateSimulation(double simLength) //update simulation with an amount of time to simulate for (in seconds)
{
	//offsetX += offsetXSpeed * simLength;
	//offsetY += offsetYSpeed * simLength;

	angle2 += 0.02f;

	if(movement[SDL_SCANCODE_W])
	{
		//angle += 0.02f; //Changes the model matrix
	}
}

void handleInput()
{
	//############################################################################################################## ENABLE USER INPUT AND MOUSE DETECTION

	SDL_PumpEvents();
	movement = SDL_GetKeyboardState(NULL);
	SDL_Event camera;
	SDL_PollEvent(&camera);


	//############################################################################################################## CAMERA MOVEMENT
	switch (camera.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEMOTION:
		if (camera.motion.x < mouseX)
			movementRotationAngle -= 0.1f;
		else
			movementRotationAngle += 0.1f;

		mouseX = camera.motion.x;
		camera.motion.x = 0;

		//glfwSetCursorPos(window, 300, 300);

		//cout << (camera.motion.x) << (", ") << (camera.motion.y) << endl; //Prints the 2D mouse location
		break;
	}


	//############################################################################################################## PLAYER CONTROLS
	
	if(movement[SDL_SCANCODE_S])
	{
		moveSpeedX += 0.002f;
		
		if(currentView.compare("player") == 0)
		{
			//Move movement
			eyeCentreX = moveSpeedX;
			eyeCentreY = -0.9f;
			eyeCentreZ = moveSpeedZ;
			
			centreX = moveSpeedX;
			centreY = -0.9f;
			centreZ = moveSpeedZ;
		}
	}

	if(movement[SDL_SCANCODE_W])
	{
		moveSpeedX -= 0.002f;

		if(currentView.compare("player") == 0)
		{
			//Move movement
			eyeCentreX = moveSpeedX;
			eyeCentreY = -0.9f;
			eyeCentreZ = moveSpeedZ;

			centreX = moveSpeedX;
			centreY = -0.9f;
			centreZ = moveSpeedZ;
		}
	}


	//Angle of rotation within the view sphere
	if(movement[SDL_SCANCODE_A])
	{
		moveSpeedZ += 0.002f;

		if(currentView.compare("player") == 0)
		{
			//Move movement
			eyeCentreX = moveSpeedX;
			eyeCentreY = -0.9f;
			eyeCentreZ = moveSpeedZ;

			centreX = moveSpeedX;
			centreY = -0.9f;
			centreZ = moveSpeedZ;
		}
	}

	if(movement[SDL_SCANCODE_D])
	{
		moveSpeedZ -= 0.002f;

		if(currentView.compare("player") == 0)
		{
			//Move movement
			eyeCentreX = moveSpeedX;
			eyeCentreY = -0.9f;
			eyeCentreZ = moveSpeedZ;

			centreX = moveSpeedX;
			centreY = -0.9f;
			centreZ = moveSpeedZ;
		}
	}
	

	//############################################################################################################## movement CONTROLS


	//Radius of the view sphere
	if(movement[SDL_SCANCODE_S] && movement[SDL_SCANCODE_LSHIFT])
	{
		movementRadius += 0.02f;
	}

	if(movement[SDL_SCANCODE_W] && movement[SDL_SCANCODE_LSHIFT])
	{
		movementRadius -= 0.02f;
	}


	//Angle of rotation within the view sphere
	if(movement[SDL_SCANCODE_A] && movement[SDL_SCANCODE_LSHIFT])
	{
		movementRotationAngle += 0.02f;
	}

	if(movement[SDL_SCANCODE_D] && movement[SDL_SCANCODE_LSHIFT])
	{
		movementRotationAngle -= 0.02f;
	}
	

	//Vertical orientation (Currently not part of the view sphere)
	if(movement[SDL_SCANCODE_Q] && movement[SDL_SCANCODE_LSHIFT])
	{
		viewChangeY -= 0.02f;
	}
	
	if(movement[SDL_SCANCODE_E] && movement[SDL_SCANCODE_LSHIFT])
	{
		viewChangeY += 0.02f;
	}


	//Change centred view
	if(movement[SDL_SCANCODE_1])
	{
		//Change view centre to player
		eyeCentreX = moveSpeedX;
		eyeCentreY = -0.9f;
		eyeCentreZ = moveSpeedZ;

		centreX = moveSpeedX;
		centreY = -0.9f;
		centreZ = moveSpeedZ;

		currentView = "player";
	}

	if(movement[SDL_SCANCODE_2])
	{
		//Change view centre to freecam
		eyeCentreX = 0.0f;
		eyeCentreY = 0.0f;
		eyeCentreZ = 0.0f;

		centreX = 0.0f;
		centreY = 0.0f;
		centreZ = 0.0f;

		currentView = "freecam";
	}


	//Determine view change using SOHCAHTOA
	viewChangeX = movementRadius * cosf(movementRotationAngle);
	viewChangeZ = movementRadius * sinf(movementRotationAngle);


	//############################################################################################################## EXIT CONTROLS
	if(movement[SDL_SCANCODE_ESCAPE])
	{
		done = true;
	}

}

void render()
{
	glUseProgram(theProgram); //installs the program object specified by program as part of current rendering state

	//load data to GLSL that **may** have changed

	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject); //bind positionBufferObject

	glEnableVertexAttribArray(positionLocation); //this 0 corresponds to the location = 0 in the GLSL for the vertex shader.
		//more generically, use glGetAttribLocation() after GLSL linking to obtain the assigned attribute location.

	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, 0, 0); //define **how** values are reader from positionBufferObject in Attrib 0

	glEnable(GL_CULL_FACE); //Enables face culling to discard faces of the cube that cannot be seen
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnableVertexAttribArray(colourLocation);
	glVertexAttribPointer(colourLocation, 4, GL_FLOAT, GL_FALSE, 0, (void *) 480); //define **how** values are reader from positionBufferObject in Attrib 0


	glDrawArrays(GL_TRIANGLES, 0, 30);

	//############################################################################################################## PROJECTION MATRIX
	float fovy = glm::radians(90.0f); //Default = 1.0f               - i.e. the view "cone"
	float aspect = 4.0f/4.0f; //Default = 4.0f/3.0f     - i.e. aspect ratio 4:3
	float zNear = 0.1f; //Default = 0.1f              - i.e. how close before clipping
	float zFar = 100.0f; //Default = 1.0f                 - i.e. how far before clipping

	projectionMatrix = glm::perspective(fovy, aspect, zNear, zFar);


	//############################################################################################################## VIEW MATRIX
	glm::vec3 viewEye = glm::vec3(eyeCentreX,eyeCentreY,eyeCentreZ); //Default = (0.0f, 0.0f, -1.0f)
	glm::vec3 viewCenter = glm::vec3(viewChangeX + centreX, viewChangeY + centreY, viewChangeZ + centreZ); //Default = (0.0f, 0.0f, 0.0f)
	glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f); //Default = (0.0f, 1.0f, 0.0f)

	viewMatrix = glm::lookAt(viewEye, viewCenter, viewUp);


	//############################################################################################################## CLEAN UP
	glDisableVertexAttribArray(0); //cleanup
	glUseProgram(0); //clean up

}

void cleanUp()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	cout << "Cleaning up OK!\n";
}

int main( int argc, char* args[] )
{
	exeName = args[0];
	//setup
	//- do just once
	initialise();
	createWindow();
	setGLAttributes();
	createContext();
	initGlew();

	glEnable(GL_DEPTH_TEST);

	//load stuff from files
	//- usually do just once
	loadAssets();


	while (!done && (SDL_GetTicks() < 500000000000000)) //LOOP FROM HERE, for 2000ms (or if done flag is set)
		//WARNING: SDL_GetTicks is only accurate to milliseconds, use SDL_GetPerformanceCounter and SDL_GetPerformanceFrequency for higher accuracy
	{
		//GET INPUT HERE - PLACEHOLDER

		handleInput();

		updateSimulation(0.02); //call update simulation with an amount of time to simulate for (in seconds)
		  //WARNING - we are always updating by a constant amount of time. This should be tied to how long has elapsed
		    // see, for example, http://headerphile.blogspot.co.uk/2014/07/part-9-no-more-delays.html

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render(); //RENDER HERE - PLACEHOLDER

		SDL_GL_SwapWindow(win);; //present the frame buffer to the display (swapBuffers)

	} //LOOP TO HERE

	//cleanup and exit
	cleanUp();
	SDL_Quit();

	return 0;
}