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

///////////////////////////////////////////////////////////////////////  RAY-TRACE SCENE

Color rayTracing(Ray ray, int depth, float RefrIndex)
{
	Color color;
	float nearestT = 100.0f;
	float t = 0.0f;
	Mesh *nearestMesh = nullptr;
	Vector3 intersectionPoint = Vector3(0.0f);
	Vector3 fixedPoint = Vector3(0.0f), reflected = Vector3(0.0f);
    Vector3 v = Vector3(0.0f);

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
		ray.intersectionPoint(nearestT);

		//Ambient component
		color = /*nearestMesh->getMaterial()->getColor() * nearestMesh->getMaterial()->getKd()*/Color(0.0f, 0.0f, 0.0f);
		Vector3 normal = normalized(nearestMesh->getNormal(ray));

		std::vector<Light*> lights = scene->getLights();
		for (std::vector<Light*>::iterator itLight = lights.begin(); itLight != lights.end(); ++itLight)
		{
			Vector3 L = normalized((*itLight)->getPosition() - ray.point);
			float LDistance = norm((*itLight)->getPosition() - ray.point);
			float LNormal = std::max(dot(normal, L), 0.0f);

            fixedPoint = ray.point + 0.0001f;
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

            v = normalized(scene->getCamera()->getFrom() - ray.point);
            reflected = 2.0f * dot(v, normal) * normal - v;

            //float attenuation = 1.0 / (1.0f + 0.09f * LDistance + 0.0002f * (LDistance * LDistance));
            //Vector3 halfwayDir = normalized(L + v);

            if ((!shadowIntersection))
            {
                //Color ca = (*itLight)->getColor() * nearestMesh->getMaterial()->getKd() * nearestMesh->getMaterial()->getColor();
                Color cd = (*itLight)->getColor() * nearestMesh->getMaterial()->getKd() * nearestMesh->getMaterial()->getColor() * LNormal /** attenuation*/;
                Color cs = (*itLight)->getColor() * nearestMesh->getMaterial()->getKs() * nearestMesh->getMaterial()->getColor() * pow(std::max(dot(reflected, L), 0.0f), nearestMesh->getMaterial()->getShine())/* * attenuation*/;
                color += cd + cs;
            }
		}

		if (depth >= MAX_DEPTH)
		{
			return color;
		}
       
		if ((1.0f - nearestMesh->getMaterial()->getT()) > 0.0f)
		{
			Ray rRay = Ray(fixedPoint, reflected);
			Color rColor = rayTracing(rRay, depth + 1, nearestMesh->getMaterial()->getIndex());
            color += rColor * nearestMesh->getMaterial()->getKs() * nearestMesh->getMaterial()->getColor() * (1.0f - nearestMesh->getMaterial()->getT());
		}

        if (nearestMesh->getMaterial()->getT() > 0.0f)
        {
            Vector3 vt = dot(v, normal) * normal - v;
            float sinthetai = norm(vt);
            float sinthetat = (RefrIndex/nearestMesh->getMaterial()->getIndex()) * sinthetai;
            float costhetat = sqrt((1- (sinthetat * sinthetat)));
            Vector3 tHat = (1 / sinthetai) * vt;
            Vector3 rt = sinthetat * tHat + costhetat * (-normal);
            Ray tRay = Ray(fixedPoint, rt);
            Color tColor = rayTracing(tRay, depth + 1, nearestMesh->getMaterial()->getIndex());
            color += tColor * nearestMesh->getMaterial()->getT();
        }

		return color;
	}
	//	if (reflective object) {
	//		rRay = calculate ray in the reflected direction;
	//		rColor = trace(scene, point, rRay direction, depth + 1);
	//		reduce rColor by the specular reflection coefficient and add to color;
	//	}
	//	if (translucid object) {
	//		tRay = calculate ray in the refracted direction;
	//		tColor = trace(scene, point, tRay direction, depth + 1);
	//		reduce tColor by the transmittance coefficient and add to color;
	//	}
	//	return color;
	//}

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
    
    for (int y = 0; y < RES_Y; y++)
    {
        for (int x = 0; x < RES_X; x++)
        {
            
            //YOUR 2 FUNTIONS:
            Ray ray = Ray(scene->getCamera(), Vector2(x, y));
            Color color = rayTracing(ray, 1, 1.0f);
            
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
        std::string filename = std::string("test_scene.nff");
    #else
       std::string filename = std::string("../../RayTracing/TurnerRayTracing/src/nffs/test_scene.nff");
    #endif
	scene = loader.createScene(filename);
	scene->getCamera()->calculate();

	//scene = new Scene();
	//std::string filename = std::string("jap.nff");
	//if(!(scene->load_nff(filename))) return 0;
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
