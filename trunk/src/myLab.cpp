#include "myLab.h"		//Global defines
#include "CMyRenderer.h"

#include "CMyUiEvents.h"
//#include "TImageHandler.h"

//CONSTANTS
const int KWindowPositionX = 50;
const int KWindowPositionY = 50;


void initLights()
	{
	////Materials settings
	//GLfloat mat_ambient[]= { 0.2f, 0.2f, 0.2f, 0.0f };
	//GLfloat mat_diffuse[]= { 0.2f, 0.2f, 0.2f, 0.0f };
	//GLfloat mat_specular[]= { 1.0f, 1.0f, 1.0f, 0.0f };
	//GLfloat mat_shininess[]= { 1.0f };

	////Materials initialization and activation
	//glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient);
	//glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv (GL_FRONT, GL_POSITION, mat_shininess);  

	////SETUP OPENGL LIGHTs
	//GLfloat light_ambient[]= { 0.4f, 0.4f, 0.4f, 0.4f };
	//GLfloat light_diffuse[]= { 1.0f, 0.4f, 0.4f, 0.8f };
	//GLfloat light_specular[]= { 0.4f, 0.4f, 0.4f, 0.2f };

	//GLfloat light_position[]= { 20.0f, 2.0f, -10.0f, 1.0f };

	//glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient);
	//glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse); 
	//glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular);
	//glLightfv (GL_LIGHT1, GL_POSITION, light_position);

	//glEnable (GL_LIGHT1);

	////glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glEnable (GL_LIGHTING);


 //Simple
GLfloat mat_diffuse[]= { 0.8f, 0.8f, 0.8f, 0.8f };
GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 0.5 };
GLfloat mat_shininess[] = { 50.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_DEPTH_TEST);

glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	}

void initOpenGL( CMyRenderer* renderer )
	{
	//Init Window
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition( KWindowPositionX, KWindowPositionY );
	glutInitWindowSize( renderer->Width(), renderer->Height() ); //800,600);
	glutCreateWindow("DOFinite Engine 0.1");

	initLights();

	//Set some Glut functions
	glutKeyboardFunc( ProcessNormalKeysWithUi );
	glutSpecialFunc(  ProcessCursorKeysWithUi );
	glutMouseFunc(    ProcessMouseEventWithUi );

	glutDisplayFunc(  RenderSceneWithRenderer );
	glutIdleFunc(     RenderSceneWithRenderer );
	//glutReshapeFunc(reshape );

	//Init view
	glClearColor (0.0, 0.1, 0.0, 0.0);

	// Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations
    glLoadIdentity(); // We initialize the projection matrix as identity
	gluPerspective(60.0, 1.0, 1.5, 20.0);
		//(45.0f, (GLfloat)renderer->Width()/(GLfloat)renderer->Height(), -1.0f,20.0f); //angle, ratio, near, far


	glShadeModel(GL_SMOOTH); //GL_FLAT | GL_SMOOTH;
	//glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Texture mapping perspective correction
    //glEnable(GL_TEXTURE_2D); // Texture mapping ON
    //glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)

	glEnable(GL_CULL_FACE); // Enable the back face culling

	//glDepthRange( 0.1f, 0.3f);
    glEnable(GL_DEPTH_TEST); // Enable the depth test (z-buffer)

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity ();
	}


/**
* Main method.
* This method is called first an it will do the following:
*  -# Creates the renderer and ui objects
*  -# Initializes Glut
*  -# Calls renderer->CreateMesh to create a CMesh
*  -# Starts the glutMainLoop()
* @param argc amount of arguments for glutInit().
* @param argv arguments for glutInit().
* @return 0
*/
int main(int argc, char **argv)
	{
	//Init renderer
	CMyRenderer* renderer = new CMyRenderer( 800, 600);
	CMyUiEvents* ui = new CMyUiEvents( renderer );

	//This static data enables to pass renderer and ui functions to OpenGL
	CMyRenderer::iCurrentRenderer = renderer;
	CMyUiEvents::iCurrentUi = ui;

	//Initialize GL using GLUT
	glutInit(&argc, argv);
	initOpenGL( renderer );

	renderer->CreateScene();

	//START GL loop
	glutMainLoop();

	delete ui;
	delete renderer;
	return 0;
	}
