#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <conio.h>
#include <windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>
using namespace std;

//#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <GL/glut.h>
//#include "vgl.h"
#include "LoadShaders.h"
#include "objloader.hpp"
#include "objloader.cpp"
//#include "shader.h"
//#include "background.cpp"
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "particle/shader.cpp"
#include "particle/texture.cpp"
using namespace glm;

//#include "camera.cpp"
// 包含纹理加载类
#include "texture.cpp"

#define PARTSNUM 18
#define BODY 0
#define LEFTSHOUDER 1
#define ULEFTARM 2
#define DLEFTARM 3
#define LEFTHAND 4

void updateModels();

void init();
void initshader();
void background();
void ChangeSize(int w,int h);
void display();
void display2();
void Keyboard(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void Mouse(int button,int state,int x,int y);

void menuEvents(int option);
void ActionMenuEvents(int option);
void ModeMenuEvents(int option);
void ShaderMenuEvents(int option);

void idle(int dummy);

mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);
mat4 rotate(float angle1,float x,float y,float z);

void Obj2Buffer(char* obj,int i);
void load2Buffer( char* obj,int i);

void updateObj(int);
void resetObj(int);
void updateObj2(int);

bool isFrame;

GLuint VAO;
GLuint VBO;
GLuint uVBO;
GLuint nVBO;
GLuint mVBO;
GLuint UBO;
GLuint VBOs[PARTSNUM];
GLuint uVBOs[PARTSNUM];
GLuint nVBOs[PARTSNUM];
GLuint cubeVAO, cubeVBO;
GLuint floorVAO, floorVBO;
GLuint quadVAO, quadVBO;
GLuint framebuffer;
GLuint framebuffer2;
GLuint rbo;
GLuint rbo2;
GLuint program;
GLuint program2;
GLuint program3;
GLuint program4;
int pNo;

float angles[PARTSNUM];
float position = 0.0;
float positionz = 0.0;

float position_Longinus_y = 0.0;
float position_Longinus_z = 0.0;

float position_lefthand_y = 0.0;
float position_lefthand_z = 0.0;

float position_righthand_y = 0.0;
float position_righthand_z = 0.0;

float position_leftfoot_x = 0.0;
float position_leftfoot_y = 0.0;
float position_leftfoot_z = 0.0;

float position_rightfoot_x = 0.0;
float position_rightfoot_y = 0.0;
float position_rightfoot_z = 0.0;


float position_lefthanddown_y = 0.0;
float position_lefthanddown_z = 0.0;

float position_righthanddown_y = 0.0;
float position_righthanddown_z = 0.0;

float position_leftfootdown_x = 0.0;
float position_leftfootdown_y = 0.0;
float position_leftfootdown_z = 0.0;

float position_rightfootdown_x = 0.0;
float position_rightfootdown_y = 0.0;
float position_rightfootdown_z = 0.0;
float angle1 = 0.0;
float rotatesky = 0.0;
float rotatesky2 = 0.0;
float eyeAngley = 0.0;
float eyedistance = 20.0;
float size = 1;
GLfloat movex,movey;
GLint MatricesIdx;
GLuint ModelID;

int vertices_size[PARTSNUM];
int uvs_size[PARTSNUM];
int normals_size[PARTSNUM];
int materialCount[PARTSNUM];

std::vector<std::string> mtls[PARTSNUM];//use material
std::vector<unsigned int> faces[PARTSNUM];//face count
map<string,vec3> KDs;//mtl-name&Kd
map<string,vec3> KSs;//mtl-name&Ks

mat4 Projection ;
mat4 View;
mat4 Model;
mat4 Models[PARTSNUM];

#define leftHand 0
#define rightHand 1
#define leftFoot 2
#define rightFoot 3
#define WALK 1
#define IDLE 0
#define Javelin 2
int mode;
int action;

int w = 800;
int h = 600;




// CPU representation of a particle
struct Particle{
	glm::vec3 pos, speed;
	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

const int MaxParticles = 1000;
Particle ParticlesContainer[MaxParticles];
int LastUsedParticle = 0;
bool particle_NUM = 0;
int ParticlesCount;
int newparticles;
GLuint programID;
int particle_flag =0;

GLuint CameraRight_worldspace_ID  ;
GLuint CameraUp_worldspace_ID  ;
GLuint ViewProjMatrixID ;

// fragment shader
GLuint TextureID  ;
GLuint Texture ;

GLuint billboard_vertex_buffer;
GLuint particles_position_buffer;
GLuint particles_color_buffer;

mat4 ViewProjectionMatrix;

mat4 ProjectionMatrix ;
mat4 ViewMatrix;





GLuint VertexArrayID;



static const GLfloat g_vertex_buffer_data[] = { 
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};

static GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];
static GLubyte* g_particule_color_data         = new GLubyte[MaxParticles * 4];


double delta = 0.01;
int FindUnusedParticle();
void SortParticles();

int mtl_flag = 0;
int mtl_flag2 = 0;
void reset2();


bool noise_flag = 0;
int control = 0;

int cnt=1;
int i;
    
float totalTime = 0;
float crrentTime;
int t = 0;

//bool refraction = 0;
//bool reflection = 0;

struct Vertex {
	float x, y, z;
	float u, v;
};
Vertex cubeVertices[] = {
        // Positions          // Texture Coords
       -1.3f,-1.f,0.f,    0.f,0.f, 
        1.3f,1.f,0.f,      1.f,1.f,
        -1.3f,1.f,0.f,     0.f,1.f,
        -1.3f,-1.f,0.f,    0.f,0.f,
        1.3f,-1.f,0.f,     1.f,0.f,
        1.3f,1.f,0.f,      1.f,1.f,

    
    };
GLfloat floorVertices[] = {
        // Positions          // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
        5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

        5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        5.0f,  -0.5f, -5.0f,  2.0f, 2.0f								
    };
GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // Positions   // TexCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };


void initFBO();

bool flag = 0;
bool walk_flag = 0;
bool javelin_flag=0;
int background_num = 0;
//vec3 camera = vec3(0,0,20);
int num = 0;
GLint backgroundLOC;
GLint noiseLOC;
GLint noisecontrolLOC;
GLuint cameraLOC;
bool light = true;
GLuint lightLOC;
GLuint skyBoxTextId;
GLuint skyBoxVAOId, skyBoxVBOId;

GLuint cubeTexture;
GLuint floorTexture ;
GLuint textureColorbuffer;
GLuint textureColorbuffer2;

static unsigned createTexture(int w, int h, const void *data);
static unsigned createTextureFromImage(const char *fileName);

int picturetype=0;
GLuint pictureLOC;
int screenshot = 0;
