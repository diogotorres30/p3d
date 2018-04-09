///////////////////////////////////////////////////////////////////////
//
// P3D Course
// (c) 2016 by Jo„o Madeiras Pereira
// TEMPLATE: Whitted Ray Tracing NFF scenes and drawing points with Modern OpenGL
//
//You should develop your rayTracing( Ray ray, int depth, float RefrIndex) which returns a color and
// to develop your load_NFF function
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <algorithm>  
#include <random>

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "NFFLoader.h"
#include "Scene.h"
#include "Color.h"
#include "Ray.h"

#define CAPTION "ray tracer"

#define VERTEX_COORD_ATTRIB 0
#define COLOR_ATTRIB 1

#define MAX_DEPTH 6
#define SAMPLE_NUMBER 16

// Points defined by 2 attributes: positions which are stored in vertices array and colors which are stored in colors array
float *colors;
float *vertices;

int size_vertices;
int size_colors;

GLfloat m[16];  //projection matrix initialized by ortho function

GLuint VaoId;
GLuint VboId[2];

GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

Scene* scene = NULL;
int RES_X;
int RES_Y;

/* Draw Mode: 0 - point by point; 1 - line by line; 2 - full frame */
int draw_mode=1;

int WindowHandle = 0;

//buffer to keep each keyboard key
bool keyBuffer[256];

//random generator
std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0.0, 1.0);

///////////////////////////////////////////////////////////////////////  RAY-TRACE SCENE

Color rayTracing(Ray ray, int depth, float RefrIndex)
{
	//variable to hold the color of the pixel
	Color color;
	//variable to hold the nearest t where an object was intersected 
	float nearestT = 100.0f;
	//variable to hold the current t
	float t = 0.0f;
	//variable to hold a pointer to the object that was intersected the closest to the origin
	Mesh *nearestMesh = nullptr;
	Vector3 fixedPoint = Vector3(0.0f);
	Vector3 reflected;

	//loop for intersection of the ray with every object in the scene
	std::vector<Mesh*> meshes = scene->getMeshes();
	for (std::vector<Mesh*>::iterator itMesh = meshes.begin(); itMesh != meshes.end(); ++itMesh)
	{
		t = (*itMesh)->intersect(ray);
		if (t > 0.0f && t < nearestT)
		{
			nearestT = t;
			nearestMesh = (*itMesh);
		}			
	}

	if (nearestT == 100.0f)
	{
		return scene->getBackground();
	}
	else
	{
		//calculate the intersection point to use for the secondary rays
		ray.intersectionPoint(nearestT);

		//Ambient component
		color = /*nearestMesh->getMaterial()->getColor() * nearestMesh->getMaterial()->getKd()*/Color(0.0f, 0.0f, 0.0f);
		Vector3 normal = normalized(nearestMesh->getNormal(ray));

		float epsilonOne, epsilonTwo;
		std::vector<Light*> lights = scene->getLights();
		for (std::vector<Light*>::iterator itLight = lights.begin(); itLight != lights.end(); ++itLight)
		{
			// With soft Shadows
			if (keyBuffer['S'] || keyBuffer['s'])
			{
				Vector3 positionInLight;

				for (int p = 0; p < SAMPLE_NUMBER; p++)
				{
					for (int q = 0; q < SAMPLE_NUMBER; q++)
					{
						epsilonOne = dis(gen);
						epsilonTwo = dis(gen);
						
						positionInLight.x = (*itLight)->getPosition().x + (p + epsilonOne) / SAMPLE_NUMBER;
						positionInLight.y = (*itLight)->getPosition().y + (q + epsilonTwo) / SAMPLE_NUMBER;
						positionInLight.z = (*itLight)->getPosition().z;

						Vector3 L = normalized(positionInLight - ray.point);
						float LDistance = norm(positionInLight - ray.point);
						float LNormal = std::max(dot(normal, L), 0.0f);

						//Fixed point for ray in the light direction
						fixedPoint = ray.point + 0.0001f * L;
						//create shadow ray
						Ray shadowRay = Ray(fixedPoint, L);
						t = 0.0f;
						nearestT = 100.0f;
						bool shadowIntersection = false;

						for (std::vector<Mesh*>::iterator itMesh = meshes.begin(); itMesh != meshes.end(); ++itMesh)
						{
							t = (*itMesh)->intersect(shadowRay);
							if (t > 0.0f && t < nearestT)
							{
								shadowIntersection = true;
								nearestT = t;
							}
						}

						//Calculate the reflected direction
						reflected = normalized(2.0f * dot((-ray.direction), normal) * normal - (-ray.direction));

						//Vector3 v = normalized(scene->getCamera()->getFrom() - ray.point);

						//attenuation function considering the direction to each light
						float attenuation = 1.0 / (1.0f + 0.09f * LDistance + 0.0002f * (LDistance * LDistance));

						//halfway vector creates too bright colors
						Vector3 halfwayDir = normalized(L + (-ray.direction));

						if ((!shadowIntersection) && LNormal > 0.0f)
						{
							//ambient component, not used
							//Color ca = (*itLight)->getColor() * nearestMesh->getMaterial()->getKd() * nearestMesh->getMaterial()->getColor();
							//diffuse component
							Color cd = (*itLight)->getColor() * nearestMesh->getMaterial()->getKd() * nearestMesh->getMaterial()->getColor() * LNormal/* * attenuation*/;
							//specular component with halfway direction
							float specular = std::max(dot(halfwayDir, normal), 0.0f);
							//added attenuation to improve results
							Color cs = (*itLight)->getColor() * nearestMesh->getMaterial()->getKs() * nearestMesh->getMaterial()->getColor() * pow(specular, nearestMesh->getMaterial()->getShine()) * attenuation;
							//specular component with reflected vector
							//Color cs = (*itLight)->getColor() * nearestMesh->getMaterial()->getKs() * nearestMesh->getMaterial()->getColor() * pow(std::max(dot(reflected, (-ray.direction)), 0.0f), nearestMesh->getMaterial()->getShine()) /** attenuation*/;
							//add each component of the light according to Blinn-Phong
							color += (cd + cs) / (SAMPLE_NUMBER*SAMPLE_NUMBER);
						}
					}
				}
			}
			// with hard shadows
			else
			{
				Vector3 L = normalized((*itLight)->getPosition() - ray.point);
				float LDistance = norm((*itLight)->getPosition() - ray.point);
				float LNormal = std::max(dot(normal, L), 0.0f);

				//Fixed point for ray in the light direction
				fixedPoint = ray.point + 0.0001f * L;
				//create shadow ray
				Ray shadowRay = Ray(fixedPoint, L);
				t = 0.0f;
				nearestT = 100.0f;
				bool shadowIntersection = false;

				for (std::vector<Mesh*>::iterator itMesh = meshes.begin(); itMesh != meshes.end(); ++itMesh)
				{
					t = (*itMesh)->intersect(shadowRay);
					if (t > 0.0f && t < nearestT)
					{
						shadowIntersection = true;
						nearestT = t;
					}
				}

				//Calculate the reflected direction
				reflected = normalized(2.0f * dot((-ray.direction), normal) * normal - (-ray.direction));

				//Vector3 v = normalized(scene->getCamera()->getFrom() - ray.point);

				//attenuation function considering the direction to each light
				float attenuation = 1.0 / (1.0f + 0.09f * LDistance + 0.0002f * (LDistance * LDistance));

				//halfway vector creates too bright colors
				Vector3 halfwayDir = normalized(L + (-ray.direction));

				if ((!shadowIntersection) && LNormal > 0.0f)
				{
					//ambient component, not used
					//Color ca = (*itLight)->getColor() * nearestMesh->getMaterial()->getKd() * nearestMesh->getMaterial()->getColor();
					//diffuse component
					Color cd = (*itLight)->getColor() * nearestMesh->getMaterial()->getKd() * nearestMesh->getMaterial()->getColor() * LNormal/* * attenuation*/;
					//specular component with halfway direction
					float specular = std::max(dot(halfwayDir, normal), 0.0f);
					//added attenuation to improve results
					Color cs = (*itLight)->getColor() * nearestMesh->getMaterial()->getKs() * nearestMesh->getMaterial()->getColor() * pow(specular, nearestMesh->getMaterial()->getShine()) * attenuation;
					//specular component with reflected vector
					//Color cs = (*itLight)->getColor() * nearestMesh->getMaterial()->getKs() * nearestMesh->getMaterial()->getColor() * pow(std::max(dot(reflected, (-ray.direction)), 0.0f), nearestMesh->getMaterial()->getShine()) /** attenuation*/;
					//add each component of the light according to Blinn-Phong
					color += cd + cs;
				}
			}
		}

		if (depth >= MAX_DEPTH)
		{
			return color;
		}
       
		//Reflection
		if ((1.0f - nearestMesh->getMaterial()->getT()) > 0.0f)
		{
			//Fixed point for reflection
            fixedPoint = ray.point + 0.0001f * reflected;
			//create reflected ray
			Ray rRay = Ray(fixedPoint, reflected);
			//shoot reflected ray
			Color rColor = rayTracing(rRay, depth + 1, RefrIndex);
			//add reflection color to the pixel color
            color += rColor * nearestMesh->getMaterial()->getKs() /** (1.0f - nearestMesh->getMaterial()->getT())*/;
		}

		//Refraction
        if (nearestMesh->getMaterial()->getT() > 0.0f)
        {
            float cosI = dot(ray.direction, normal);
            float iorI = RefrIndex;
            float iorT = nearestMesh->getMaterial()->getIndex();

			//decide the case where we are inside or outside of the object
            if(cosI < 0){
                cosI = -cosI;
            }
            else {
                std::swap(iorI, iorT);
                normal = -normal;
            }
            float n = iorI/iorT;
            float k = 1 - n * n * (1 - cosI * cosI);
			//Calculate the refracted direction
            Vector3 rt = normalized(ray.direction * n + normal * (n * cosI - sqrt(k)));
			//Fixed point for refraction
            fixedPoint = ray.point + 0.0001f * rt;
			//create refracted ray
            Ray tRay = Ray(fixedPoint, rt);
			//shoot refracted ray
            Color tColor = rayTracing(tRay, depth + 1, RefrIndex);
			//add refraction color to the pixel color
            color += tColor * nearestMesh->getMaterial()->getT();

        }
		return color;
	}
}

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
    bool isError = false;
    GLenum errCode;
    const GLubyte *errString;
    while ((errCode = glGetError()) != GL_NO_ERROR) {
        isError = true;
        errString = gluErrorString(errCode);
        std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
    }
    return isError;
}

void checkOpenGLError(std::string error)
{
    if(isOpenGLError()) {
        std::cerr << error << std::endl;
        //exit(EXIT_FAILURE);
    }
}
/////////////////////////////////////////////////////////////////////// SHADERs
const GLchar* VertexShader =
{
    "#version 330 core\n"
    
    "in vec2 in_Position;\n"
    "in vec3 in_Color;\n"
    "uniform mat4 Matrix;\n"
    "out vec4 color;\n"
    
    "void main(void)\n"
    "{\n"
    "    vec4 position = vec4(in_Position, 0.0, 1.0);\n"
    "    color = vec4(in_Color, 1.0);\n"
    "    gl_Position = Matrix * position;\n"
    
    "}\n"
};

const GLchar* FragmentShader =
{
    "#version 330 core\n"
    
    "in vec4 color;\n"
    "out vec4 out_Color;\n"
    
    "void main(void)\n"
    "{\n"
    "    out_Color = color;\n"
    "}\n"
};

void createShaderProgram()
{
    VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderId, 1, &VertexShader, 0);
    glCompileShader(VertexShaderId);
    
    FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
    glCompileShader(FragmentShaderId);
    
    ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);
    
    glBindAttribLocation(ProgramId, VERTEX_COORD_ATTRIB, "in_Position");
    glBindAttribLocation(ProgramId, COLOR_ATTRIB, "in_Color");
    
    glLinkProgram(ProgramId);
    UniformId = glGetUniformLocation(ProgramId, "Matrix");
    
    //checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
    glUseProgram(0);
    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);
    
    glDeleteShader(FragmentShaderId);
    glDeleteShader(VertexShaderId);
    glDeleteProgram(ProgramId);
    
    checkOpenGLError("ERROR: Could not destroy shaders.");
}
/////////////////////////////////////////////////////////////////////// VAOs & VBOs
void createBufferObjects()
{
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
    glGenBuffers(2, VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    
    /* N„o È necess·rio fazer glBufferData, ou seja o envio dos pontos para a placa gr·fica pois isso
     È feito na drawPoints em tempo de execuÁ„o com GL_DYNAMIC_DRAW */
    
    glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glVertexAttribPointer(VERTEX_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
    glEnableVertexAttribArray(COLOR_ATTRIB);
    glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
    
    // unbind the VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glDisableVertexAttribArray(COLOR_ATTRIB);
    //checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
    glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glDisableVertexAttribArray(COLOR_ATTRIB);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glDeleteBuffers(1, VboId);
    glDeleteVertexArrays(1, &VaoId);
    checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

void drawPoints()
{
    glBindVertexArray(VaoId);
    glUseProgram(ProgramId);
    
    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
    glBufferData(GL_ARRAY_BUFFER, size_colors, colors, GL_DYNAMIC_DRAW);
    
    glUniformMatrix4fv(UniformId, 1, GL_FALSE, m);
    glDrawArrays(GL_POINTS, 0, RES_X*RES_Y);
    glFinish();
    
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    #ifdef __APPLE__
    #else
       checkOpenGLError("ERROR: Could not draw scene.");
    #endif
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

// Render function by primary ray casting from the eye towards the scene's objects

void renderScene()
{
    int index_pos=0;
    int index_col=0;
	Ray ray;
	Color color;
	float epsilon;

    for (int y = 0; y < RES_Y; y++)
    {
        for (int x = 0; x < RES_X; x++)
        {

			//TODO: create lens for depth of field


			if (keyBuffer['A'] || keyBuffer['a'])
			{
				for (int p = 0; p < SAMPLE_NUMBER; p++)
				{
					for (int q = 0; q < SAMPLE_NUMBER; q++)
					{
						epsilon = dis(gen);
						ray = Ray(scene->getCamera(), Vector2(x + (p + epsilon)/ SAMPLE_NUMBER, y + (q + epsilon)/ SAMPLE_NUMBER));
						color += rayTracing(ray, 1, 1.0f);
					}
				}

				color = color / (SAMPLE_NUMBER*SAMPLE_NUMBER);
			}
			else
			{
				//YOUR 2 FUNTIONS:
				ray = Ray(scene->getCamera(), Vector2(x, y));
				color = rayTracing(ray, 1, 1.0f);
			}
           
            
            vertices[index_pos++]= (float)x;
            vertices[index_pos++]= (float)y;
            colors[index_col++]= (float)color.r;
            colors[index_col++]= (float)color.g;
            colors[index_col++]= (float)color.b;
          
            if(draw_mode == 0) {  // desenhar o conte˙do da janela ponto a ponto
                drawPoints();
                index_pos=0;
                index_col=0;
            }
        }
        printf("line %d", y);
        if(draw_mode == 1) {  // desenhar o conte˙do da janela linha a linha
            drawPoints();
            index_pos=0;
            index_col=0;
        }
    }
    
    if(draw_mode == 2) //preenchar o conte˙do da janela com uma imagem completa
        drawPoints();
    
    printf("Terminou!\n");
}

void keysPressed(unsigned char key, int x, int y)
{
	/* 
		Anti-Aliasing On/Off - key A || a
		Soft Shadows On/Off - key S || s
		Uniform Grid On/Off - key G || g
		Depth of Field On/Off - key D || d 
	*/
	if(keyBuffer[key])
		keyBuffer[key] = false;
	else
		keyBuffer[key] = true;
	glutPostRedisplay();
}

void cleanup()
{
    destroyShaderProgram();
    destroyBufferObjects();
}

void ortho(float left, float right, float bottom, float top,
           float nearp, float farp)
{
    m[0 * 4 + 0] = 2 / (right - left);
    m[0 * 4 + 1] = 0.0;
    m[0 * 4 + 2] = 0.0;
    m[0 * 4 + 3] = 0.0;
    m[1 * 4 + 0] = 0.0;
    m[1 * 4 + 1] = 2 / (top - bottom);
    m[1 * 4 + 2] = 0.0;
    m[1 * 4 + 3] = 0.0;
    m[2 * 4 + 0] = 0.0;
    m[2 * 4 + 1] = 0.0;
    m[2 * 4 + 2] = -2 / (farp - nearp);
    m[2 * 4 + 3] = 0.0;
    m[3 * 4 + 0] = -(right + left) / (right - left);
    m[3 * 4 + 1] = -(top + bottom) / (top - bottom);
    m[3 * 4 + 2] = -(farp + nearp) / (farp - nearp);
    m[3 * 4 + 3] = 1.0;
}

void reshape(int w, int h)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, w, h);
    ortho(0, (float)RES_X, 0, (float)RES_Y, -1.0, 1.0);
}

/////////////////////////////////////////////////////////////////////// SETUP
void setupCallbacks()
{
    #ifdef __APPLE__
    #else
        glutCloseFunc(cleanup);
    #endif
    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);

	//Keyboard Functions
	glutKeyboardFunc(keysPressed);
}

void setupGLEW() {
    glewExperimental = GL_TRUE;
    GLenum result = glewInit() ;
    if (result != GLEW_OK) {
        std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
        exit(EXIT_FAILURE);
    }
    GLenum err_code = glGetError();
    printf ("Vendor: %s\n", glGetString (GL_VENDOR));
    printf ("Renderer: %s\n", glGetString (GL_RENDERER));
    printf ("Version: %s\n", glGetString (GL_VERSION));
    printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
}

void setupGLUT(int argc, char* argv[])
{
    glutInit(&argc, argv);
    
    #ifdef __APPLE__
        glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    #else
        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
        glutInitContextVersion(3, 3);
        glutInitContextProfile(GLUT_CORE_PROFILE );
        glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    #endif
    
    glutInitWindowPosition(640,100);
    glutInitWindowSize(RES_X, RES_Y);
    
    
    
    #ifdef __APPLE__
    #else
    glDisable(GL_DEPTH_TEST);
    #endif
    WindowHandle = glutCreateWindow(CAPTION);
    if(WindowHandle < 1) {
        std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void init(int argc, char* argv[])
{
    setupGLUT(argc, argv);
    setupGLEW();
    std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    createShaderProgram();
    createBufferObjects();
    setupCallbacks();
}

int main(int argc, char* argv[])
{
    //INSERT HERE YOUR CODE FOR PARSING NFF FILES
	NFFLoader loader = NFFLoader();
    #ifdef __APPLE__
        std::string filename = std::string("mount_low.nff");
    #else
       std::string filename = std::string("../../RayTracing/TurnerRayTracing/src/nffs/ball.nff");
    #endif
	scene = loader.createScene(filename);
	scene->getCamera()->calculate();

    RES_X = scene->getCamera()->getResX();
    RES_Y = scene->getCamera()->getResY();
    
    if(draw_mode == 0) { // desenhar o conte˙do da janela ponto a ponto
        size_vertices = 2*sizeof(float);
        size_colors = 3*sizeof(float);
        printf("DRAWING MODE: POINT BY POINT\n");
    }
    else if(draw_mode == 1) { // desenhar o conte˙do da janela linha a linha
        size_vertices = 2*RES_X*sizeof(float);
        size_colors = 3*RES_X*sizeof(float);
        printf("DRAWING MODE: LINE BY LINE\n");
    }
    else if(draw_mode == 2) { // preencher o conte˙do da janela com uma imagem completa
        size_vertices = 2*RES_X*RES_Y*sizeof(float);
        size_colors = 3*RES_X*RES_Y*sizeof(float);
        printf("DRAWING MODE: FULL IMAGE\n");
    }
    else {
        printf("Draw mode not valid \n");
        exit(0);
    }
    printf("resx = %d  resy= %d.\n", RES_X, RES_Y);
    
    vertices = (float*)malloc(size_vertices);
    if (vertices==NULL) exit (1);
    
    colors = (float*)malloc(size_colors);
    if (colors==NULL) exit (1);
    
    init(argc, argv);
    glutMainLoop();
    exit(EXIT_SUCCESS);
}
///////////////////////////////////////////////////////////////////////
