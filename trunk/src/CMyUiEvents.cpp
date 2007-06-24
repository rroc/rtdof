
//INCLUDE FILES
#include "CMyUiEvents.h"

//CONSTANTS
#ifdef USE_RADIANS
const float KAngleChangeAmount = 0.001f;
#elif defined USE_DEGREES
const float KAngleChangeAmount = 0.05f;
#endif

const float KScaleChangeAmount = 0.005f;

//INIT STATIC DATA
CMyUiEvents* CMyUiEvents::iCurrentUi = 0;

//Constructor, saves a pointer to the renderer
CMyUiEvents::CMyUiEvents( CMyRenderer* aRenderer )
	{
	iRenderer = aRenderer;
	}

//Destructor
CMyUiEvents::~CMyUiEvents()
	{
	}

//Used to handle the keyboard input (ASCII Characters)
void CMyUiEvents::ProcessNormalKeys(unsigned char key, int x, int y)
	{
	TAngles changes = iRenderer->RotAnglesChange();
	TAngles savedChanges(changes);
	int renderType(0);

    switch (key)
		{
		//ESC
		case 27:
			exit(0);
			break;
		case ',':
		case ';':
			iRenderer->SetScale( iRenderer->Scale() + KScaleChangeAmount );
			break;

		//DISPLAY ADDITIONAL VISUAL AID
		case 'x':
			iRenderer->SetAxisDrawn( !iRenderer->AxisDrawn() );
			break;
		case 'n':
			iRenderer->SetNormalsDrawn( !iRenderer->NormalsDrawn() );
			break;

		//CHANGE RENDERER
		case '-':
			renderType = iRenderer->RenderType();
			if( 0 > --renderType )
				{
				renderType = (CMyRenderer::EEmpty)-1;
				}
			iRenderer->SetRenderType( static_cast<CMyRenderer::TRenderType>(renderType) );
			break;

		case '+':
			renderType = iRenderer->RenderType();
			if( CMyRenderer::EEmpty == ++renderType )
				{
				renderType = CMyRenderer::ERenderVertices;
				}
				iRenderer->SetRenderType( static_cast<CMyRenderer::TRenderType>(renderType) );
			break;

		//SCALE MESH
		case '.':
		case ':':
			iRenderer->SetScale( iRenderer->Scale()-KScaleChangeAmount );
			break;

		//CHANGE MESH
		case 'j':
			iRenderer->iMeshIndex++;
			break;
		case 'k':
			iRenderer->iMeshIndex--;
			break;

		//MODIFY MESH X
        case 'd':
			changes.iRotX += KAngleChangeAmount;
			break;
        case 'a':
			changes.iRotX -= KAngleChangeAmount;
			break;
		//MODIFY MESH Y
        case 'w':
			changes.iRotY += KAngleChangeAmount;
			break;
        case 's':
			changes.iRotY -= KAngleChangeAmount;
		    break;
		//MODIFY MESH Z
		case '3':
			changes.iRotZ += KAngleChangeAmount;
			break;
        case 'e':
			changes.iRotZ -= KAngleChangeAmount;
		    break;

		//RESET MESH Base angle as well
		case '1':
			changes.iRotX = changes.iRotY = changes.iRotZ = 0.0;
			//Wait until change is ok
			while(iRenderer->TransformInProcess());
			iRenderer->SetRotAngles(TAngles(0,0,0));
		    break;
		//FREEZE MESH ROTATION
		case 'q':
			changes.iRotX = changes.iRotY = changes.iRotZ = 0.0;
			break;
		//---temp
		case '8':
			iRenderer->iAmountOfTriangles++;
			break;
		//---temp
		case '7':
			iRenderer->iAmountOfTriangles--;
			break;
		//undefined key
		default:
			break;
		}
	if(changes != savedChanges)
		{
		//Wait until change is ok
		while(iRenderer->TransformInProcess());
		iRenderer->SetRotAngles( changes );
		}
	}

// Used to handle the keyboard input (not ASCII Characters)
void CMyUiEvents::ProcessCursorKeys(int key, int x, int y)
	{
	TAngles changes = iRenderer->AnglesChange();

	    switch (key)
	    {
		//MODIFY X
        case GLUT_KEY_UP:
			changes.iRotY += KAngleChangeAmount;
			break;
        case GLUT_KEY_DOWN:
			changes.iRotY -= KAngleChangeAmount;
			break;
		//MODIFY Y
        case GLUT_KEY_LEFT:
			changes.iRotX -= KAngleChangeAmount;
			break;
        case GLUT_KEY_RIGHT:
			changes.iRotX += KAngleChangeAmount;
		    break;
		//MODIFY Z
		case GLUT_KEY_PAGE_UP:
			changes.iRotZ += KAngleChangeAmount;
			break;
        case GLUT_KEY_PAGE_DOWN:
			changes.iRotZ -= KAngleChangeAmount;
		    break;
		//RESET Base angle as well
		case GLUT_KEY_HOME:
			changes.iRotX = changes.iRotY = changes.iRotZ = 0.0;

			//Wait until change is ok
			while(iRenderer->TransformInProcess());
			iRenderer->SetCurrentAngles(0,0,0);
		    break;
		//FREEZE
		default:
			changes.iRotX = changes.iRotY = changes.iRotZ = 0.0;
			break;
		}
	//Wait until change is ok
	while(iRenderer->TransformInProcess());
	iRenderer->SetAnglesChange( changes );
	}

void CMyUiEvents::ProcessMouseEvent( int button, int state, int x, int y )
	{
	switch (button)
		{
		//Set point of rotation
		case GLUT_LEFT_BUTTON:
			if(GLUT_UP == state)
				{
				//Wait until change is ok
				//while(iRenderer->TransformInProcess());
				//TVector3 p = iRenderer->PointOfRotation();
				//iRenderer->SetPointOfRotation( x, y, p.Z() );
				}
			break;
		//Set mesh position
		case GLUT_RIGHT_BUTTON:
			if(GLUT_UP == state)
				{
				//Wait until change is ok
				//while(iRenderer->TransformInProcess());
				//TVector3 mp = iRenderer->MeshPosition();
				//iRenderer->SetMeshPosition( x, y, mp.Z() );
				}
			break;
		default:
			break;
		}

	}

//EXTERNAL FUNCTIONS TO USE GLUT CALLBACKS
void ProcessNormalKeysWithUi( unsigned char key, int x, int y )
	{
	if (CMyUiEvents::iCurrentUi != 0 )
		CMyUiEvents::iCurrentUi->ProcessNormalKeys( key, x, y  );
	}

void ProcessCursorKeysWithUi( int key, int x, int y )
	{
	if (CMyUiEvents::iCurrentUi != 0 )
		CMyUiEvents::iCurrentUi->ProcessCursorKeys( key, x, y );
	}
void ProcessMouseEventWithUi( int button, int state, int x, int y)
	{
	if (CMyUiEvents::iCurrentUi != 0 )
		CMyUiEvents::iCurrentUi->ProcessMouseEvent( button, state, x, y );
	}

