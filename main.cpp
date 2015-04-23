#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include "Model.h"
#include "VertexPositions.h"

#include "PxPhysicsAPI.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
	"in vec2 vertUV;\n"
	"smooth out vec2 fragUV;\n"
	"uniform mat4 modelMatrix;\n"
	"uniform mat4 viewMatrix;\n"
	"uniform mat4 projectionMatrix;\n"
	"smooth out vec4 fragColour;\n" //send colours to frag shader
	"void main()\n"
	"{\n"
	"   gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;\n"
	"	fragColour = colour;\n"
	"	fragUV = vertUV;\n"
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
	"smooth in vec4 fragColour;\n"
	"uniform sampler2D textureSampler;\n"
	"smooth in vec2 fragUV;\n"
	"out vec4 outputColour;\n"
	"void main()\n"
	"{\n"
	"	outputColour = texture(textureSampler, fragUV);\n"
	//"   outputColour = fragColour;\n"
	"}\n"
	);


//our variables
bool done = false;

Model* modelx;

const Uint8 *movement; //movement variable for viewpoint changes

float angle = 90.0f;

float viewChangeX = 0.0f; //Change the view matrix over time
float viewChangeY = 0.0f; //Change the view matrix over time
float viewChangeZ = 0.0f; //Change the view matrix over time

float eyeCentreX = -4.0f;
float eyeCentreY = 0.5f;
float eyeCentreZ = 0.0f;

float centreX = 0.0f;
float centreY = 0.0f;
float centreZ = 0.0f;

Sint32 mouseX = 0;
Sint32 mouseY = 0;

float movementRotationAngle = 0.0f;
float movementRadius = 2.0f;

float moveSpeedX = -5.0f; //0.0f
float moveSpeedY = 0.5f;
float moveSpeedZ = 0.0f;

string currentView = "player";
string currentScreen = "menu";

GLint textureLocation;
GLint textureSamplerLocation;
GLuint textureID;
GLuint textureID2;
GLuint textureID3;
GLuint textureID4;
GLuint textureID5;
GLuint textureID6;

//Score Textures
GLuint textureID7;
GLuint textureID8;
GLuint textureID9;
GLuint textureID10;
GLuint textureID11;
GLuint textureID12;
GLuint textureID13;
GLuint textureID14;
GLuint textureID15;
GLuint textureID16;
GLuint textureID17;
GLuint textureID18;
GLuint textureID19;
GLuint textureID20;

float enemyMoveX = 0.0f;
float enemyMoveY = 0.0f;
float enemyMoveZ = 0.0f;

float playerEnemyX = 0.0f;
float playerEnemyY = 0.0f;
float playerEnemyZ = 0.0f;

float moveSpeed = 0.02f;
float enemyMoveSpeed = 0.015f;

//our GL and GLSL variables

glm::mat4 modelMatrix;
glm::mat4 modelMatrix2;
glm::mat4 modelMatrix3;
glm::mat4 modelMatrix4;
glm::mat4 modelMatrix5;
glm::mat4 modelMatrix6;
glm::mat4 modelMatrix7;
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
GLuint playerBufferObject;
GLuint vao;

Mix_Music *music = NULL;
Mix_Music *intro = NULL;
Mix_Chunk *hitFX = NULL;

int gameMiliseconds = 0;
int gameTimer = 0;
int powerupTimer = 0;
int loading = 0;

bool isHurt = false;
float playerHealth = 1.0f;

bool score = false;
int totalScore = 0;

float powerupMove = 0.0f;
bool powerupDown = true;

float levelLO = -4.5f;
float levelHI = 4.5f;
float powerupPosX;
float powerupPosZ;
bool pickup = true;
bool powerup = false;
bool enemyPickup = true;
bool enemyPowerup = false;

bool playerCollided = true;

int volume = 50;

void getPlayerRotation();
void initialise();
void createWindow();
void setGLAttributes();
void createContext();
void initGlew();
GLuint createShader(GLenum eShaderType, const std::string &strShaderFile);
GLuint createProgram(const std::vector<GLuint> &shaderList);
void initializeProgram();
void initializeVertexBuffer();
void loadAssets();
void updateSimulation(float simLength);
void handleInput();
void getPlayerRotation();
void render();
void cleanUp();
int main(int argc, char* args[]);
void playMusic();
void initializeTexturesAndSamplers();
void enemyMovement();


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

	SDL_WINDOW_RESIZABLE;

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
	matrixLocation = glGetUniformLocation(theProgram, "modelMatrix");
	viewLocation = glGetUniformLocation(theProgram, "viewMatrix");
	projectionLocation = glGetUniformLocation(theProgram, "projectionMatrix");
	textureLocation = glGetAttribLocation(theProgram, "vertUV");
	textureSamplerLocation = glGetUniformLocation(theProgram, "textureSampler");

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

	glGenBuffers(1, &playerBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, playerBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(modelx->vertices), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	std::cout << "playerBufferObject created OK! GLUint is: " << playerBufferObject << std::endl;
}

void initializeTexturesAndSamplers()
{
	//################################################################################################################################ TEXTURES

	//################################################################################################ HUD
	SDL_Surface* image = SDL_LoadBMP("assets/Health.bmp");
	if (image == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image->format->BytesPerPixel, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image);

	cout << "texture created OK! GLUint is: " << textureID << std::endl;


	//################################################################################################ Walls
	SDL_Surface* image2 = SDL_LoadBMP("assets/Walls.bmp");
	if (image2 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID2); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, image2->format->BytesPerPixel, image2->w, image2->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image2->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image2);

	cout << "texture created OK! GLUint is: " << textureID2 << std::endl;

	//################################################################################################ Death cube
	SDL_Surface* image3 = SDL_LoadBMP("assets/Death.bmp");
	if (image3 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID3); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, image3->format->BytesPerPixel, image3->w, image3->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image3->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image3);

	cout << "texture created OK! GLUint is: " << textureID3 << std::endl;


	//################################################################################################ Menu
	SDL_Surface* image4 = SDL_LoadBMP("assets/Menu.bmp");
	if (image4 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID4); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image4->format->BytesPerPixel, image4->w, image4->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image4->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image4);

	cout << "texture created OK! GLUint is: " << textureID4 << std::endl;

	//################################################################################################ Game Over
	SDL_Surface* image5 = SDL_LoadBMP("assets/GameOver.bmp");
	if (image5 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID5); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image5->format->BytesPerPixel, image5->w, image5->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image5->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image5);

	cout << "texture created OK! GLUint is: " << textureID5 << std::endl;

	//################################################################################################ Loading
	SDL_Surface* image6 = SDL_LoadBMP("assets/Loading.bmp");
	if (image6 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID6); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image6->format->BytesPerPixel, image6->w, image6->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image6->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image6);

	cout << "texture created OK! GLUint is: " << textureID6 << std::endl;


	//################################################################################################ 1
	SDL_Surface* image7 = SDL_LoadBMP("assets/1.bmp");
	if (image7 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID7); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image7->format->BytesPerPixel, image7->w, image7->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image7->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image7);

	cout << "texture created OK! GLUint is: " << textureID7 << std::endl;


	//################################################################################################ 2
	SDL_Surface* image8 = SDL_LoadBMP("assets/2.bmp");
	if (image8 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID8); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image8->format->BytesPerPixel, image8->w, image8->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image8->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image8);

	cout << "texture created OK! GLUint is: " << textureID8 << std::endl;


	//################################################################################################ 3
	SDL_Surface* image9 = SDL_LoadBMP("assets/3.bmp");
	if (image9 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID9); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image9->format->BytesPerPixel, image9->w, image9->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image9->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image9);

	cout << "texture created OK! GLUint is: " << textureID9 << std::endl;


	//################################################################################################ 4
	SDL_Surface* image10 = SDL_LoadBMP("assets/4.bmp");
	if (image10 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID10); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image10->format->BytesPerPixel, image10->w, image10->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image10->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image10);

	cout << "texture created OK! GLUint is: " << textureID10 << std::endl;


	//################################################################################################ 5
	SDL_Surface* image11 = SDL_LoadBMP("assets/5.bmp");
	if (image11 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID11); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID11);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image11->format->BytesPerPixel, image11->w, image11->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image11->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image11);

	cout << "texture created OK! GLUint is: " << textureID11 << std::endl;


	//################################################################################################ 6
	SDL_Surface* image12 = SDL_LoadBMP("assets/6.bmp");
	if (image12 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID12); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID12);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image12->format->BytesPerPixel, image12->w, image12->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image12->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image12);

	cout << "texture created OK! GLUint is: " << textureID12 << std::endl;


	//################################################################################################ 7
	SDL_Surface* image13 = SDL_LoadBMP("assets/7.bmp");
	if (image13 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID13); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID13);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image13->format->BytesPerPixel, image13->w, image13->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image13->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image13);

	cout << "texture created OK! GLUint is: " << textureID13 << std::endl;


	//################################################################################################ 8
	SDL_Surface* image14 = SDL_LoadBMP("assets/8.bmp");
	if (image14 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID14); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID14);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image14->format->BytesPerPixel, image14->w, image14->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image14->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image14);

	cout << "texture created OK! GLUint is: " << textureID14 << std::endl;


	//################################################################################################ 9
	SDL_Surface* image15 = SDL_LoadBMP("assets/9.bmp");
	if (image15 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID15); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID15);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image15->format->BytesPerPixel, image15->w, image15->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image15->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image15);

	cout << "texture created OK! GLUint is: " << textureID15 << std::endl;


	//################################################################################################ 0
	SDL_Surface* image16 = SDL_LoadBMP("assets/0.bmp");
	if (image16 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID16); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID16);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image16->format->BytesPerPixel, image16->w, image16->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image16->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image16);

	cout << "texture created OK! GLUint is: " << textureID16 << std::endl;


	//################################################################################################ WOOD WALLS
	SDL_Surface* image17 = SDL_LoadBMP("assets/Wood.bmp");
	if (image17 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID17); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID17);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image17->format->BytesPerPixel, image17->w, image17->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image17->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image17);

	cout << "texture created OK! GLUint is: " << textureID17 << std::endl;


	//################################################################################################ GREY FLOOR
	SDL_Surface* image18 = SDL_LoadBMP("assets/Floor.bmp");
	if (image18 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID18); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID18);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, image18->format->BytesPerPixel, image18->w, image18->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image18->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image18);

	cout << "texture created OK! GLUint is: " << textureID18 << std::endl;


	//################################################################################################ Score cube
	SDL_Surface* image19 = SDL_LoadBMP("assets/Death.bmp");
	if (image19 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID19); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID19);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, image19->format->BytesPerPixel, image19->w, image19->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image19->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image19);

	cout << "texture created OK! GLUint is: " << textureID19 << std::endl;


	//################################################################################################ Pickups
	SDL_Surface* image20 = SDL_LoadBMP("assets/Pickup.bmp");
	if (image20 == NULL)
	{
		cout << "image loading (for texture) failed." << std::endl;
		SDL_Quit();
		exit(1);
	}

	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	glGenTextures(1, &textureID20); //generate a texture ID and store it
	glBindTexture(GL_TEXTURE_2D, textureID20);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, image20->format->BytesPerPixel, image20->w, image20->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image20->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image20);

	cout << "texture created OK! GLUint is: " << textureID20 << std::endl;
}

void loadAssets()
{
	initializeProgram(); //create GLSL Shaders, link into a GLSL program

	initializeVertexBuffer(); //load data into a vertex buffer

	initializeTexturesAndSamplers();

	modelx = new Model();

	glGenVertexArrays(1, &vao); //create a Vertex Array Object
	glBindVertexArray(vao); //make the VAO active
	cout << "Vertex Array Object created OK! GLUint is: " << vao << std::endl;

	cout << "Loaded Assets OK!\n";
}

void updateSimulation(float simLength) //update simulation with an amount of time to simulate for (in seconds)
{
	angle += simLength;

	if (currentScreen == "game")
	{
		//Enemy AI movement
		if (moveSpeedX + 1.2f > enemyMoveX)
		{
			enemyMoveX += 0.015;
		}
		else if (moveSpeedX + 1.2f < enemyMoveX)
		{
			enemyMoveX -= 0.015;
		}

		if (moveSpeedZ - 0.2f > enemyMoveZ)
		{
			enemyMoveZ += 0.015;
		}
		else if (moveSpeedZ - 0.2f < enemyMoveZ)
		{
			enemyMoveZ -= 0.015;
		}

		//Player collide with death cube
		if (currentView == "player")
		{
			if (moveSpeedX + 1.0f < enemyMoveX && moveSpeedX + 1.4 > enemyMoveX && moveSpeedZ - 0.4f < enemyMoveZ && moveSpeedZ - 0.0 > enemyMoveZ)
			{
				isHurt = true;
			}
		}
		else
		{
			if (moveSpeedX + 1.0f < playerEnemyX && moveSpeedX + 1.4 > playerEnemyX && moveSpeedZ - 0.4f < playerEnemyZ && moveSpeedZ - 0.0 > playerEnemyZ)
			{
				isHurt = true;
			}
		}

		//Player collide with score cube
		if (moveSpeedX < -1.0f && moveSpeedX > -1.4f && moveSpeedZ < 0.3f && moveSpeedZ > -0.26)
		{
			score = true;
		}

		//Player collide with powerup
		if (moveSpeedX + 1.0f < powerupPosX && moveSpeedX + 1.4 > powerupPosX && moveSpeedZ - 0.4f < powerupPosZ && moveSpeedZ - 0.0 > powerupPosZ)
		{
			powerup = true;
			pickup = true;
		}

		//Enemy player collide with object
		if (playerEnemyX - 0.5f < powerupPosX && playerEnemyX + 0.5f > powerupPosX && playerEnemyZ - 0.5f < powerupPosZ && playerEnemyZ + 0.5f> powerupPosZ)
		{
			enemyPowerup = true;
			enemyPickup = true;
		}

		//Enemy and player wall collisions
		if (!(moveSpeedX > -6.4f && moveSpeedX < 3.0f && moveSpeedZ > -4.5f && moveSpeedZ < 4.5f))
		{
			if (!(moveSpeedX > -6.4f))
				moveSpeedX += moveSpeed;
			if (!(moveSpeedX < 3.0f))
				moveSpeedX -= moveSpeed;
			if (!(moveSpeedZ > -4.5f))
				moveSpeedZ += moveSpeed;
			if (!(moveSpeedZ < 4.5f))
				moveSpeedZ -= moveSpeed;
		}

		if (!(playerEnemyX > -5.0f && playerEnemyX < 4.0f && playerEnemyZ > -4.5f && playerEnemyZ < 4.5f))
		{
			if (!(playerEnemyX > -5.0f))
				playerEnemyX += enemyMoveSpeed;
			if (!(playerEnemyX < 4.0f))
				playerEnemyX -= enemyMoveSpeed;
			if (!(playerEnemyZ > -4.5f))
				playerEnemyZ += enemyMoveSpeed;
			if (!(playerEnemyZ < 4.5f))
				playerEnemyZ -= enemyMoveSpeed;
		}
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
		{
			movementRotationAngle -= 0.1f;
			//SDL_WarpMouseInWindow(win, 300, 300);
		}
		else
		{
			movementRotationAngle += 0.1f;
			//SDL_WarpMouseInWindow(win, 300, 300);
		}

		mouseX = camera.motion.x;
		camera.motion.x = 0;

		//glfwSetCursorPos(window, 300, 300);

		//cout << (camera.motion.x) << (", ") << (camera.motion.y) << endl; //Prints the 2D mouse location
		break;
	}


	//############################################################################################################## PLAYER CONTROLS
	
	if(movement[SDL_SCANCODE_W])
	{
		moveSpeedX += moveSpeed;
		//getPlayerRotation();
		
		if(currentView.compare("player") == 0)
		{
			//Movement
			eyeCentreX = moveSpeedX;
			eyeCentreY = 0.5f;
			eyeCentreZ = moveSpeedZ;
			
			centreX = moveSpeedX;
			centreY = 0.0f;
			centreZ = moveSpeedZ;
		}
	}

	if(movement[SDL_SCANCODE_S])
	{
		moveSpeedX -= moveSpeed;

		if(currentView.compare("player") == 0)
		{
			//Move movement
			eyeCentreX = moveSpeedX;
			eyeCentreY = 0.5f;
			eyeCentreZ = moveSpeedZ;

			centreX = moveSpeedX;
			centreY = 0.0f;
			centreZ = moveSpeedZ;
		}
	}


	//Angle of rotation within the view sphere
	if(movement[SDL_SCANCODE_D])
	{
		moveSpeedZ += moveSpeed;

		if(currentView.compare("player") == 0)
		{
			//Move movement
			eyeCentreX = moveSpeedX;
			eyeCentreY = 0.5f;
			eyeCentreZ = moveSpeedZ;

			centreX = moveSpeedX;
			centreY = 0.0f;
			centreZ = moveSpeedZ;
		}
	}

	if(movement[SDL_SCANCODE_A])
	{
		moveSpeedZ -= moveSpeed;

		if(currentView.compare("player") == 0)
		{
			//Move movement
			eyeCentreX = moveSpeedX;
			eyeCentreY = 0.5f;
			eyeCentreZ = moveSpeedZ;

			centreX = moveSpeedX;
			centreY = 0.0f;
			centreZ = moveSpeedZ;
		}
	}
	

	//############################################################################################################## PLAYER CONTROLS
	if (movement[SDL_SCANCODE_UP])
	{
		playerEnemyX += enemyMoveSpeed;
	}

	if (movement[SDL_SCANCODE_DOWN])
	{
		playerEnemyX -= enemyMoveSpeed;
	}

	if (movement[SDL_SCANCODE_LEFT])
	{
		playerEnemyZ -= enemyMoveSpeed;
	}

	if (movement[SDL_SCANCODE_RIGHT])
	{
		playerEnemyZ += enemyMoveSpeed;
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
		eyeCentreY = 0.5f;
		eyeCentreZ = moveSpeedZ;

		centreX = moveSpeedX;
		centreY = 0.0f;
		centreZ = moveSpeedZ;

		currentView = "player";
	}

	if(movement[SDL_SCANCODE_2])
	{
		//Change view centre to freecam
		eyeCentreX = 0.0f;
		eyeCentreY = 10.0f;
		eyeCentreZ = 0.0f;

		centreX = 0.0f;
		centreY = 0.0f;
		centreZ = 0.0f;

		enemyMoveX = 0.0f;
		enemyMoveY = 0.0f;
		enemyMoveZ = 0.0f;

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

	if (movement[SDL_SCANCODE_F])
		SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);

	if (movement[SDL_SCANCODE_RETURN])
	{
		if (currentScreen == "menu")
		{
			currentScreen = "loading";
		}

		if (currentScreen == "gameOver")
		{
			playerHealth = 1.0f;

			score = false;
			totalScore = 0;

			angle = 90.0f;

			viewChangeX = 0.0f; //Change the view matrix over time
			viewChangeY = 0.0f; //Change the view matrix over time
			viewChangeZ = 0.0f; //Change the view matrix over time

			eyeCentreX = -4.0f;
			eyeCentreY = 0.5f;
			eyeCentreZ = 0.0f;

			centreX = 0.0f;
			centreY = 0.0f;
			centreZ = 0.0f;

			mouseX = 0;
			mouseY = 0;

			movementRotationAngle = 0.0f;
			movementRadius = 2.0f;

			moveSpeedX = -5.0f;
			moveSpeedY = 0.5f;
			moveSpeedZ = 0.0f;

			currentView = "player";

			enemyMoveX = 0.0f;
			enemyMoveY = 0.0f;
			enemyMoveZ = 0.0f;

			currentScreen = "game";
			playMusic();
		}
	}
}

void getPlayerRotation()
{
	double playerRotation = movementRotationAngle * (180 / M_PI);
	double playerDirection = 0;
	float rotationSize = playerRotation / 1000; //Used to determine the moveSpeed
	bool positive = false;

	if (playerRotation > 0)
	{
		positive = true;

		if (playerRotation >= 360) //Determine the direction the player is facing
		{
			do
			{
				playerDirection = playerRotation - 360;
				playerRotation = playerDirection;
			} while (playerDirection >= 360);
		}
		else
			playerDirection = playerRotation;
	}
	else
	{
		positive = false;

		if (playerRotation <= -360) //Determine the direction the player is facing
		{
			
			do
			{
				cout << "player rotation" << playerRotation << endl;
				playerDirection = (playerRotation + 360);
				playerRotation += 360;
			} while (playerDirection < -360);
			playerDirection = -playerDirection;
		}
		else
			playerDirection = -playerRotation;

		
		cout << playerDirection << endl;
	}

	if (playerRotation >= 90) //Determine the amount of rotation within the 90 degree rotation
	{
		do
		{
			playerRotation -= 90;
		} while (playerRotation >= 90);
	}	

	cout << playerDirection << endl;

	if (playerDirection == 0)
	{
		//Z = X
		moveSpeedZ += (moveSpeed + rotationSize);
		moveSpeedX += (moveSpeed - rotationSize);
	}
	else
	{
		if (positive == true)
		{
			//Z > X
			if (playerDirection < 90)
			{
				moveSpeedZ += (moveSpeed + rotationSize);
				moveSpeedX -= (moveSpeed - rotationSize);
			}
			else if (playerDirection < 180)
			{
				moveSpeedZ += (moveSpeed + rotationSize);
				moveSpeedX += (moveSpeed - rotationSize);
			}
			else if (playerDirection < 270)
			{
				moveSpeedZ -= (moveSpeed + rotationSize);
				moveSpeedX += (moveSpeed - rotationSize);
			}
			else if (playerDirection < 360)
			{
				moveSpeedZ -= (moveSpeed + rotationSize);
				moveSpeedX -= (moveSpeed - rotationSize);
			}
		}
		else
		{
			//Z < X
			if (playerDirection < 90)
			{
				moveSpeedZ -= (moveSpeed - rotationSize);
				moveSpeedX -= (moveSpeed + rotationSize);
			}
			else if (playerDirection < 180)
			{
				moveSpeedZ -= (moveSpeed - rotationSize);
				moveSpeedX += (moveSpeed + rotationSize);
			}
			else if (playerDirection < 270)
			{
				moveSpeedZ += (moveSpeed - rotationSize);
				moveSpeedX += (moveSpeed + rotationSize);
			}
			else if (playerDirection < 360)
			{
				moveSpeedZ += (moveSpeed - rotationSize);
				moveSpeedX -= (moveSpeed + rotationSize);
			}
		}
	}
}

void render()
{
	glUseProgram(theProgram); //installs the program object specified by program as part of current rendering state

	//load data to GLSL that **may** have changed

	eyeCentreX = moveSpeedX;
	eyeCentreY = 0.5f;
	eyeCentreZ = moveSpeedZ;

	centreX = moveSpeedX;
	centreY = 0.0f;
	centreZ = moveSpeedZ;


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
	glVertexAttribPointer(colourLocation, 4, GL_FLOAT, GL_FALSE, 0, (void *) 1152); //define **how** values are reader from positionBufferObject in Attrib 0


	//############################################################################################################## INITIALISE TEXTURES
	glEnableVertexAttribArray(textureLocation);
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, (void *)1632); //Start at "TEXTURES" in vertexPositions

	glUniform1i(textureSamplerLocation, 0);
	glActiveTexture(GL_TEXTURE0);


	if (currentScreen == "loading")
	{
		//############################################################################################################## LEVEL
		glBindTexture(GL_TEXTURE_2D, textureID6); //Load Menu texture

		glDrawArrays(GL_TRIANGLES, 0, 6); //Generates the plane

		glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


		//############################################################################################################## CUBE
		glBindTexture(GL_TEXTURE_2D, textureID3); //Load Cube texture

		glm::mat4 rotateMatrix5 = glm::rotate(glm::mat4(), angle, glm::normalize(glm::vec3(1, 0, 1)));//Initialisation Rotation
		glm::mat4 translateMatrix6 = glm::translate(glm::vec3(-1.2f, 1.5f, -0.3f)); //Initialisation Translation

		modelMatrix5 = translateMatrix6 * rotateMatrix5;

		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix5));

		glDrawArrays(GL_TRIANGLES, 30, 36); //Generates the plane

		glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


		//############################################################################################################## PROJECTION MATRIX
		float fovy = glm::radians(90.0f); //Default = 1.0f               - i.e. the view "cone"
		float aspect = 4.0f / 4.0f; //Default = 4.0f/3.0f     - i.e. aspect ratio 4:3
		float zNear = 0.1f; //Default = 0.1f              - i.e. how close before clipping
		float zFar = 100.0f; //Default = 1.0f                 - i.e. how far before clipping

		projectionMatrix = glm::perspective(fovy, aspect, zNear, zFar);

		//############################################################################################################## VIEW MATRIX
		glm::vec3 viewEye = glm::vec3(-0.0001f, 4.0f, 0.0f);
		glm::vec3 viewCenter = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);

		viewMatrix = glm::lookAt(viewEye, viewCenter, viewUp);
	}


	if (currentScreen == "menu")
	{
		//############################################################################################################## LEVEL
		glBindTexture(GL_TEXTURE_2D, textureID4); //Load Menu texture

		glDrawArrays(GL_TRIANGLES, 0, 6); //Generates the plane

		glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


		//############################################################################################################## CUBE
		glBindTexture(GL_TEXTURE_2D, textureID3); //Load Cube texture

		glm::mat4 rotateMatrix5 = glm::rotate(glm::mat4(), angle, glm::normalize(glm::vec3(1, 0, 1)));//Initialisation Rotation
		glm::mat4 translateMatrix6 = glm::translate(glm::vec3(-1.2f, 1.5f, -0.3f)); //Initialisation Translation

		modelMatrix5 = translateMatrix6 * rotateMatrix5;

		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix5));
		
		glDrawArrays(GL_TRIANGLES, 30, 36); //Generates the plane

		glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


		//############################################################################################################## PROJECTION MATRIX
		float fovy = glm::radians(90.0f); //Default = 1.0f               - i.e. the view "cone"
		float aspect = 4.0f / 4.0f; //Default = 4.0f/3.0f     - i.e. aspect ratio 4:3
		float zNear = 0.1f; //Default = 0.1f              - i.e. how close before clipping
		float zFar = 100.0f; //Default = 1.0f                 - i.e. how far before clipping

		projectionMatrix = glm::perspective(fovy, aspect, zNear, zFar);

		//############################################################################################################## VIEW MATRIX
		glm::vec3 viewEye = glm::vec3(-0.0001f, 4.0f, 0.0f);
		glm::vec3 viewCenter = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);

		viewMatrix = glm::lookAt(viewEye, viewCenter, viewUp);
	}

	if (currentScreen == "game")
	{
		//############################################################################################################## PROJECTION MATRIX
		float fovy = glm::radians(90.0f); //Default = 1.0f               - i.e. the view "cone"
		float aspect = 4.0f / 4.0f; //Default = 4.0f/3.0f     - i.e. aspect ratio 4:3
		float zNear = 0.1f; //Default = 0.1f              - i.e. how close before clipping
		float zFar = 100.0f; //Default = 1.0f                 - i.e. how far before clipping

		projectionMatrix = glm::perspective(fovy, aspect, zNear, zFar);

		if (currentView == "player")
		{
			//############################################################################################################## LEVEL
			glBindTexture(GL_TEXTURE_2D, textureID2); //Bind wall texture

			glDrawArrays(GL_TRIANGLES, 0, 30);

			glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


			//############################################################################################################## VIEW MATRIX - Camera follow
			glm::vec3 viewEye = glm::vec3(eyeCentreX, eyeCentreY, eyeCentreZ); //Default = (0.0f, 0.0f, -1.0f)
			glm::vec3 viewCenter = glm::vec3(centreX + 0.3f, centreY + 0.3f, centreZ); //Default = (0.0f, 0.0f, 0.0f)
			glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f); //Default = (0.0f, 1.0f, 0.0f)

			viewMatrix = glm::lookAt(viewEye, viewCenter, viewUp);


			//############################################################################################################## HUD
			glBindTexture(GL_TEXTURE_2D, textureID); //Bind health texture

			glm::mat4 translateMatrix4 = glm::translate(glm::vec3(0.0, 0.0, 0.0)); //Initialisation Translation
			glm::mat4 rotateMatrix4 = glm::rotate(glm::mat4(), -0.3f, glm::normalize(glm::vec3(0, 0, 1)));//Initialisation Rotation
			glm::mat4 translateMatrix5 = glm::translate(glm::vec3(eyeCentreX + 5.0, -1.0, eyeCentreZ)); //Initialisation Translation
			glm::mat4 scaleMatrix = glm::scale(glm::vec3(1.0f, 1.0f, playerHealth));

			modelMatrix4 = translateMatrix5 * scaleMatrix * rotateMatrix4 * translateMatrix4;

			glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix4));
			glDrawArrays(GL_TRIANGLES, 66, 6);

			glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


			//############################################################################################################## ENEMY CUBE
			glBindTexture(GL_TEXTURE_2D, textureID3); //Bind death texture

			glm::mat4 translateMatrix3 = glm::translate(glm::vec3(enemyMoveX, 0.0, enemyMoveZ)); //Initialisation Translation

			modelMatrix3 = translateMatrix3;

			glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix3));
			glDrawArrays(GL_TRIANGLES, 30, 36);

			glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures
		}
		else if (currentView == "freecam")
		{
			//############################################################################################################## FLOOR
			glBindTexture(GL_TEXTURE_2D, textureID18); //Bind floor texture

			glDrawArrays(GL_TRIANGLES, 0, 6);

			glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


			//############################################################################################################## WALLS
			glBindTexture(GL_TEXTURE_2D, textureID17); //Bind wall texture

			glDrawArrays(GL_TRIANGLES, 6, 24);

			glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


			//############################################################################################################## VIEW MATRIX - Camera static
			glm::vec3 viewEye = glm::vec3(-4.5f, 2.0f, 0.0f); //Default = (0.0f, 0.0f, -1.0f)
			glm::vec3 viewCenter = glm::vec3(centreX + 0.3f, centreY + 0.3f, centreZ); //Default = (0.0f, 0.0f, 0.0f)
			glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f); //Default = (0.0f, 1.0f, 0.0f)

			viewMatrix = glm::lookAt(viewEye, viewCenter, viewUp);


			//############################################################################################################## ENEMY CUBE
			glBindTexture(GL_TEXTURE_2D, textureID3); //Bind death texture

			glm::mat4 translateMatrix3 = glm::translate(glm::vec3(playerEnemyX, 0.0, playerEnemyZ)); //Initialisation Translation

			modelMatrix3 = translateMatrix3;

			glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix3));
			glDrawArrays(GL_TRIANGLES, 30, 36);

			glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


			//############################################################################################################## POWERUP CUBE
			glBindTexture(GL_TEXTURE_2D, textureID20); //Load Cube texture

			if (pickup == true || enemyPickup == true)
			{
				powerupPosX = levelLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (levelHI - levelLO)));
				powerupPosZ = levelLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (levelHI - levelLO)));
				pickup = false;
				enemyPickup = false;
			}

			glm::mat4 translateMatrix8 = glm::translate(glm::vec3(powerupPosX, 0.0f, powerupPosZ)); //Initialisation Translation
			glm::mat4 translateMatrix9 = glm::translate(glm::vec3(0.0f, powerupMove, 0.0f)); //Initialisation Translation

			modelMatrix7 = translateMatrix9 * translateMatrix8;

			glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix7));

			glDrawArrays(GL_TRIANGLES, 30, 36); //Generates the plane

			glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures
		}


		//############################################################################################################## SCORE CUBE
		glBindTexture(GL_TEXTURE_2D, textureID19); //Load Cube texture

		glm::mat4 translateMatrix7 = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)); //Initialisation Translation

		modelMatrix6 = translateMatrix7;

		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix6));

		glDrawArrays(GL_TRIANGLES, 30, 36); //Generates the plane

		glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


		//############################################################################################################## LOAD PLAYER MODEL
		glBindBuffer(GL_ARRAY_BUFFER, playerBufferObject); //bind playerBufferObject
		glEnableVertexAttribArray(positionLocation);

		glm::mat4 translateMatrix = glm::translate(glm::vec3(-0.6f, 0.5f, 1.36f)); //Initialisation Translation
		glm::mat4 translateMatrix2 = glm::translate(glm::vec3(eyeCentreX + 1.5f, eyeCentreY - 2.0f, eyeCentreZ)); //Walking Translation
		glm::mat4 rotateMatrix = glm::rotate(glm::mat4(), 1.6f, glm::normalize(glm::vec3(0, eyeCentreY, 0)));//Initialisation Rotation
		glm::mat4 rotateMatrix2 = glm::rotate(glm::mat4(), -movementRotationAngle, glm::normalize(glm::vec3(0, 1, 0)));//Walking Rotation

		modelMatrix2 = translateMatrix2 * rotateMatrix2 * rotateMatrix * translateMatrix;// * rotateMatrix;

		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix2));
		modelx->Render();


		glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
		glEnableVertexAttribArray(positionLocation);
	}


	if (currentScreen == "gameOver")
	{
		//############################################################################################################## LEVEL
		glBindTexture(GL_TEXTURE_2D, textureID5); //Load game over texture

		glDrawArrays(GL_TRIANGLES, 0, 6); //Generates the plane

		glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


		//############################################################################################################### SCORE
		int tempScore = totalScore;
		int tens = 0;
		float numChange = 0.0f;

		if (totalScore > 9)
		{
			do //Find out tens
			{
				tens++;
				tempScore /= 10;
			} while (tempScore > 9);

			for (int iCounter = tens; iCounter > 0; iCounter--)
			{
				glBindTexture(GL_TEXTURE_2D, textureID16);

				glm::mat4 translateMatrix6 = glm::translate(glm::vec3(-0.2f, 2.0f, 0.5f - numChange)); //-0.3f

				modelMatrix5 = translateMatrix6;

				glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix5));

				glDrawArrays(GL_TRIANGLES, 48, 6); //Generates the plane

				numChange += 0.5f;
			}

			glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures
		}

		switch (tempScore)
		{
		case 0:
			glBindTexture(GL_TEXTURE_2D, textureID16);
			break;
		case 1:
			glBindTexture(GL_TEXTURE_2D, textureID7);
			break;
		case 2:
			glBindTexture(GL_TEXTURE_2D, textureID8);
			break;
		case 3:
			glBindTexture(GL_TEXTURE_2D, textureID9);
			break;
		case 4:
			glBindTexture(GL_TEXTURE_2D, textureID10);
			break;
		case 5:
			glBindTexture(GL_TEXTURE_2D, textureID11);
			break;
		case 6:
			glBindTexture(GL_TEXTURE_2D, textureID12);
			break;
		case 7:
			glBindTexture(GL_TEXTURE_2D, textureID13);
			break;
		case 8:
			glBindTexture(GL_TEXTURE_2D, textureID14);
			break;
		case 9:
			glBindTexture(GL_TEXTURE_2D, textureID15);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D, textureID16);
			break;
		}

		glm::mat4 translateMatrix6 = glm::translate(glm::vec3(-0.2f, 2.0f, 0.0f - numChange)); //-0.3f

		modelMatrix5 = translateMatrix6;

		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix5));

		glDrawArrays(GL_TRIANGLES, 48, 6); //Generates the plane

		glBindTexture(GL_TEXTURE_2D, 0); //Clear the texture buffer for other textures


		//############################################################################################################## PROJECTION MATRIX
		float fovy = glm::radians(90.0f); //Default = 1.0f               - i.e. the view "cone"
		float aspect = 4.0f / 4.0f; //Default = 4.0f/3.0f     - i.e. aspect ratio 4:3
		float zNear = 0.1f; //Default = 0.1f              - i.e. how close before clipping
		float zFar = 100.0f; //Default = 1.0f                 - i.e. how far before clipping

		projectionMatrix = glm::perspective(fovy, aspect, zNear, zFar);

		//############################################################################################################## VIEW MATRIX
		glm::vec3 viewEye = glm::vec3(-0.0001f, 4.0f, 0.0f);
		glm::vec3 viewCenter = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);

		viewMatrix = glm::lookAt(viewEye, viewCenter, viewUp);
	}


	//############################################################################################################## CLEAN UP
	glDisableVertexAttribArray(0); //cleanup
	glUseProgram(0); //clean up

}

void playMusic()
{
	Mix_VolumeMusic(50);

	if (!SDL_WasInit(SDL_INIT_AUDIO))
	{
		cout << "SDL2 Audio failed to initialise. " << SDL_GetError() << "\n";
		system("pause");
		SDL_Quit();
		exit(1);
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cout << "SDL2_Mixer failed to initialise. " << Mix_GetError() << "\n";
		system("pause");
		SDL_Quit();
		exit(1);
	}

	music = Mix_LoadMUS("Hubris_MIDI.wav");
	intro = Mix_LoadMUS("Intro.wav");
	hitFX = Mix_LoadWAV("PlayerHit.wav");

	if (music == NULL || hitFX == NULL)
	{
		cout << "An audio file failed to load.\n";
		system("pause");
		SDL_Quit();
		exit(1);
	}
	else
	{
		cout << "Music File Loaded\n";

		if (currentScreen == "game")
			Mix_PlayMusic(music, -1);
		else
			Mix_PlayMusic(intro, -1);
	}
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
	srand(static_cast <unsigned> (time(0)));
	initialise();
	createWindow();
	setGLAttributes();
	createContext();
	initGlew();
	playMusic();
	
	SDL_ShowCursor(false);

	glEnable(GL_DEPTH_TEST);

	//load stuff from files
	//- usually do just once
	loadAssets();

	while (!done) //Game loop - Loops until 'esc' key is pressed
	{
		handleInput(); //Check for user input

		updateSimulation(0.25f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render();

		SDL_GL_SwapWindow(win);; //Present the frame buffer to the display (swapBuffers)

		//Basic timer
		gameMiliseconds++;
		powerupTimer++;

		if (gameMiliseconds > 0) //Intro fade out
		{
			if (currentScreen == "loading")
			{
				volume -= 0.00000000000000355;

				Mix_VolumeMusic(volume);
			}
		}

		if (gameMiliseconds == 60)
		{
			gameTimer++;
			gameMiliseconds = 0;
			if (isHurt == true)
			{
				playerHealth -= 0.2f;
				Mix_PlayChannel(-1, hitFX, 0);
				isHurt = false;
				cout << "Player hit by Death Cube" << endl;
			}
			
			if (score == true)
			{
				totalScore += 1;
				score = false;
			}

			if (currentScreen == "loading")
			{
				if (loading == 5)
				{
					loading = 0;
					currentScreen = "game";
					playMusic();
					Mix_VolumeMusic(50);
				}
				else
					loading++;
			}
		}

		if (playerHealth <= 0.0f)
		{
			Mix_HaltMusic();
			currentScreen = "gameOver";
		}

		//Powerup animation
		if (powerupMove >= 0.0f)
			powerupDown = true;
		else if (powerupMove <= -0.5f)
			powerupDown = false;

		if (powerupDown == true)
			powerupMove -= 0.01f;
		else
			powerupMove += 0.01f;

		//Powerup handling
		if (powerupTimer >= 240)
		{
			powerup = false;
			enemyPowerup = false;
		}

		if (powerup == true)
		{
			moveSpeed = 0.03;
		}
		else
		{
			moveSpeed = 0.02;
		}

		if (enemyPowerup == true)
		{
			enemyMoveSpeed = 0.03;
		}
		else
		{
			enemyMoveSpeed = 0.02;
		}
		
		//Powerup timer handler
		if (powerup == true || enemyPowerup == true)
		{
			powerupTimer++;
		}
		else
		{
			powerupTimer = 0;
		}

		//cout << powerupTimer << endl;
	}

	//Cleanup and exit
	cleanUp();
	SDL_Quit();

	return 0;
}