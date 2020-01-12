// 
// Cubic mapping (Environment Mapping)
// 
// 執行前的準備工作
// 關閉 CShape.h 中的 #define PERVERTEX_LIGHTING，使用 PERPIXEL_LIGHTING 才會有作用
// 設定 #define MULTITEXTURE  (DIFFUSE_MAP|LIGHT_MAP|NORMAL_MAP)
// 開啟 #define CUBIC_MAP 1

#include "header/Angel.h"
#include "Common/CQuad.h"
#include "Common/CSolidCube.h"
#include "Common/CSolidSphere.h"
#include "Common/CWireSphere.h"
#include "Common/CWireCube.h"
#include "Common/CChecker.h"
#include "Common/CCamera.h"
#include "Common/CTexturePool.h"
#include "png_loader.h"
//#include "Common/ModelPool.h"
#include "Common/C2DSprite.h"
#include "Common/Room.h"

#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 20 // must be an even number

#define RED_BUTTON   0
#define GREEN_BUTTON 1
#define BLUE_BUTTON  2
#define WHITE_BUTTON 3

// For Model View and Projection Matrix
mat4 g_mxModelView(1.0f);
mat4 g_mxProjection;

// For View Point
GLfloat g_fRadius = 80.0;
GLfloat g_fTheta = 45.0f*DegreesToRadians;
GLfloat g_fPhi = 45.0f*DegreesToRadians;

point4  g_vEye(g_fRadius*sin(g_fTheta)*cos(g_fPhi), g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), 1.0);
point4  g_vAt(0.0, 0.0, 0.0, 1.0);
vec4    g_vUp(0.0, 1.0, 0.0, 0.0);

// 2D 介面所需要的相關變數
C2DSprite *g_p2DBtn[4];
mat4  g_2DView = mat4(1, 0, 0, 0
	, 0, 1, 0, 0
	, 0, 0, 1, 0
	, 0, 0, -1, 1);;
mat4  g_2DProj;

Room *room1,*room2, *room3, *room4, *room5, *room6;

// 函式的原型宣告
extern void IdleProcess();
void UIGenerator();
void UIAction(vec2 pt);

int playerState;
int UpdatePlayerState();

enum PLAYERSTATE
{
	OUTSIDE=0,
	ROOM1=1,
	ROOM2,
	ROOM3,
	ROOM4,
	ROOM5,
	ROOM6,
};

void init( void )
{
	mat4 mxT;
	vec4 vT, vColor;
	// 產生所需之 Model View 與 Projection Matrix
	// 產生所需之 Model View 與 Projection Matrix

	//point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	//point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi)-40.0f, g_fRadius*cos(g_fTheta)-50.0f, g_fRadius*sin(g_fTheta)*cos(g_fPhi)-40.0f, 1.0f);
	point4  at(0.0f, 2.0f, 0.0f, 1.0f);

	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);


	UIGenerator();

	room1 = new Room2(0.0f,0.0f,0.0f,eye);
	room2 = new Room1(20.001f, 0.0f, 0.0f, eye);
	room3 = new Room3(20.001f, 0.0f, -20.001f, eye);
	room4 = new Room4(0.0f, 0.0f, -20.001f, eye);
	room5 = new Room5(0.0f, 0.0f, -40.002f , eye);
	room6 = new Room6(20.001f, 0.0f, -40.002f, eye);

	// 因為本範例不會動到 Projection Matrix 所以在這裡設定一次即可
	// 就不寫在 OnFrameMove 中每次都 Check
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);

	room1->SetProjectionMatrix(mpx);
	room2->SetProjectionMatrix(mpx);
	room3->SetProjectionMatrix(mpx);
	room4->SetProjectionMatrix(mpx);
	room5->SetProjectionMatrix(mpx);
	room6->SetProjectionMatrix(mpx);
}

void GL_Display( void )
{
	auto camera = CCamera::getInstance();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

	room1->Draw(camera->getViewPosition());
	room2->Draw(camera->getViewPosition());
	room3->Draw(camera->getViewPosition());
	room4->Draw(camera->getViewPosition());
	room5->Draw(camera->getViewPosition());
	room6->Draw(camera->getViewPosition());

	for (int i = 0; i <4; i++) g_p2DBtn[i]->Draw();

	glutSwapBuffers();	// 交換 Frame Buffer
}

void onFrameMove(float delta)
{
	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view 與 projection matrix 是否需要更新給物件
	auto camera = CCamera::getInstance();
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		room1->SetViewMatrix(mvx, camera->getViewPosition());
		room2->SetViewMatrix(mvx, camera->getViewPosition());
		room3->SetViewMatrix(mvx, camera->getViewPosition());
		room4->SetViewMatrix(mvx, camera->getViewPosition());
		room5->SetViewMatrix(mvx, camera->getViewPosition());
		room6->SetViewMatrix(mvx, camera->getViewPosition());
	}

	playerState = UpdatePlayerState();
	//Print(playerState);
	room1->Update(delta);
	room2->Update(delta);
	room3->Update(delta);
	room4->Update(delta);
	room5->Update(delta);
	room6->Update(delta);

	GL_Display();
}

void Win_Keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case  SPACE_KEY:

		break;

// Part 2 : for single light source
	case 65: // A key
	case 97: // a key
		room3->g_bAutoRotating = !room3->g_bAutoRotating;
		break;
	case 82: // R key
		if(room1->g_fLightR <= 0.95f ) room1->g_fLightR += 0.05f;
		room1->g_Light[0].diffuse.x = room1->g_fLightR;
		room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
		break;
	case 114: // r key
		if(room1->g_fLightR >= 0.05f ) room1->g_fLightR -= 0.05f;
		room1->g_Light[0].diffuse.x = room1->g_fLightR;
		room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
		break;
	case 71: // G key
		if(room1->g_fLightG <= 0.95f ) room1->g_fLightG += 0.05f;
		room1->g_Light[0].diffuse.y = room1->g_fLightG;
		room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
		break;
	case 103: // g key
		if(room1->g_fLightG >= 0.05f ) room1->g_fLightG -= 0.05f;
		room1->g_Light[0].diffuse.y = room1->g_fLightG;
		room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
		break;
	case 66: // B key
		if(room1->g_fLightB <= 0.95f ) room1->g_fLightB += 0.05f;
		room1->g_Light[0].diffuse.z = room1->g_fLightB;
		room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
		break;
	case 98: // b key
		if(room1->g_fLightB >= 0.05f ) room1->g_fLightB -= 0.05f;
		room1->g_Light[0].diffuse.z = room1->g_fLightB;
		room1->g_pLight[0].SetColor(room1->g_Light[0].diffuse);
		break;

    case 033:
		glutIdleFunc( NULL );
		CCamera::getInstance()->destroyInstance();
		CTexturePool::getInstance()->destroyInstance();

		for (int i = 0; i < 4; i++)
		{
			delete g_p2DBtn[i];
		}

		delete room1;
		delete room2;
		delete room3;
		delete room4;
		delete room5;
		delete room6;

        exit( EXIT_SUCCESS );
        break;
    }
}

inline void ScreenToUICoordinate(int x, int y, vec2 &pt)
{
	pt.x = 2.0f*(float)x / SCREEN_SIZE - 1.0f;
	pt.y = 2.0f*(float)(SCREEN_SIZE - y) / SCREEN_SIZE - 1.0f;
}

void Win_Mouse(int button, int state, int x, int y) {
	vec2 pt;
	switch(button) {
		case GLUT_LEFT_BUTTON:   // 目前按下的是滑鼠左鍵
			if (state == GLUT_DOWN) {
				ScreenToUICoordinate(x, y, pt);
				UIAction(pt);
			}
			break;
		case GLUT_MIDDLE_BUTTON:  // 目前按下的是滑鼠中鍵 ，換成 Y 軸
			//if ( state == GLUT_DOWN ) ; 
			break;
		case GLUT_RIGHT_BUTTON:   // 目前按下的是滑鼠右鍵
			//if ( state == GLUT_DOWN ) ;
			break;
		default:
			break;
	} 
}

void Win_SpecialKeyboard(int key, int x, int y) {

	switch(key) {
		case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵

			break;
		case GLUT_KEY_RIGHT:	// 目前按下的是向右方向鍵

			break;
		case GLUT_KEY_UP:	// 目前按下的是向右方向鍵
			CCamera::getInstance()->moveForward(playerState);
			break;
		case GLUT_KEY_DOWN:	// 目前按下的是向右方向鍵
			CCamera::getInstance()->moveBackward(playerState);
			break;
		default:
			break;
	}
}

void Win_PassiveMotion(int x, int y) {
	//g_fPhi = (float)-M_PI*(x - HALF_SIZE) / (HALF_SIZE);  // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	//g_fTheta = (float)M_PI*(float)y / SCREEN_SIZE;;
	//point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	//point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	//auto camera = CCamera::getInstance();
	//camera->updateViewLookAt(eye, at);


	g_fPhi = (float)-M_PI*(x - HALF_SIZE) / (HALF_SIZE);   // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)-M_PI*y / (SCREEN_SIZE);

	point4  at(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);

	CCamera::getInstance()->updateLookAt(at);

	//room3->RotateBillboard(g_fPhi);
	//Print(g_fPhi);
}

void Win_MouseMotion(int x, int y) {
	//g_fPhi = (float)-M_PI*(x - HALF_SIZE) / (HALF_SIZE); // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	//g_fTheta = (float)M_PI*(float)y / SCREEN_SIZE;
	//point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	//point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	//auto camera = CCamera::getInstance();
	//camera->updateViewLookAt(eye, at);


	g_fPhi = (float)-M_PI*(x - HALF_SIZE) / (HALF_SIZE);   // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)-M_PI*y / (SCREEN_SIZE);

	point4  at(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	CCamera::getInstance()->updateLookAt(at);

	//room3->RotateBillboard(g_fPhi);
	Print(g_fPhi);
}

void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
	glEnable(GL_DEPTH_TEST);
}

int main( int argc, char **argv )
{
    
	glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( SCREEN_SIZE, SCREEN_SIZE );

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow("Compositing and Blending");

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE; 
    glewInit();  

    init();

	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);  
    glutKeyboardFunc( Win_Keyboard );	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutSpecialFunc( Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
    glutDisplayFunc( GL_Display );
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc( IdleProcess );
	
    glutMainLoop();
    return 0;
}


void UIGenerator() {
	// 以下為利用平行投影產生 2D 的介面
	// 範圍在 X/Y 平面的  -1 到 1 之間，介面都放在 Z = 0 
	mat4 mxT2D, mxS2D;
	vec4 vColor2D = vec4(0, 0, 0, 1);

	//白色按鈕
	g_p2DBtn[0] = new C2DSprite; g_p2DBtn[0]->SetShader_2DUI();
	vColor2D.x = 1; vColor2D.y = 0; vColor2D.z = 0; g_p2DBtn[0]->SetDefaultColor(vColor2D);
	mxS2D = Scale(0.1f, 0.1f, 1.0f);
	mxT2D = Translate(0.2f, -0.85f, 0);
	g_p2DBtn[0]->SetTRSMatrix(mxT2D*mxS2D);
	g_p2DBtn[0]->SetViewMatrix(g_2DView);
	g_p2DBtn[0]->SetViewMatrix(g_2DProj);

	//藍色按鈕
	g_p2DBtn[1] = new C2DSprite; g_p2DBtn[1]->SetShader_2DUI();
	vColor2D.x = 0; vColor2D.y = 1; vColor2D.z = 0; g_p2DBtn[1]->SetDefaultColor(vColor2D);
	mxT2D = Translate(0.4f, -0.85f, 0);
	g_p2DBtn[1]->SetTRSMatrix(mxT2D*mxS2D);
	g_p2DBtn[1]->SetViewMatrix(g_2DView);
	g_p2DBtn[1]->SetViewMatrix(g_2DProj);

	//綠色按鈕
	g_p2DBtn[2] = new C2DSprite; g_p2DBtn[2]->SetShader_2DUI();
	vColor2D.x = 0; vColor2D.y = 0; vColor2D.z = 1; g_p2DBtn[2]->SetDefaultColor(vColor2D);
	mxT2D = Translate(0.6f, -0.85f, 0);
	g_p2DBtn[2]->SetTRSMatrix(mxT2D*mxS2D);
	g_p2DBtn[2]->SetViewMatrix(g_2DView);
	g_p2DBtn[2]->SetViewMatrix(g_2DProj);

	//紅色按鈕
	g_p2DBtn[3] = new C2DSprite; g_p2DBtn[3]->SetShader_2DUI();
	vColor2D.x = 0.85; vColor2D.y = 0.85; vColor2D.z = 0.85; g_p2DBtn[3]->SetDefaultColor(vColor2D);
	mxT2D = Translate(0.8f, -0.85f, 0);
	g_p2DBtn[3]->SetTRSMatrix(mxT2D*mxS2D);
	g_p2DBtn[3]->SetViewMatrix(g_2DView);
	g_p2DBtn[3]->SetViewMatrix(g_2DProj);
}

void UIAction(vec2 pt) {
	if (g_p2DBtn[RED_BUTTON]->OnTouches(pt)) {
		if (g_p2DBtn[0]->getButtonStatus()) {
			printf("紅色關\n");
			//room1->g_Light[1].isLighting = false;
		}
		else {
			printf("紅色開\n");
			//room1->g_Light[1].isLighting = true;
		}
	}
	//藍色按鈕→控制主燈光的旋轉On/Off
	if (g_p2DBtn[GREEN_BUTTON]->OnTouches(pt)) {
		if (g_p2DBtn[1]->getButtonStatus()) {
			printf("綠色關\n");
			//room1->g_Light[2].isLighting = false;
		}
		else {
			printf("綠色開\n");
			//room1->g_Light[2].isLighting = true;
		}
	}
	//綠色按鈕→控制所有側燈光的照明On/Off
	if (g_p2DBtn[BLUE_BUTTON]->OnTouches(pt)) {
		if (g_p2DBtn[2]->getButtonStatus()) {
			printf("藍色關\n");
			//room1->g_Light[3].isLighting = false;
		}
		else {
			printf("藍色開\n");
			//room1->g_Light[3].isLighting = true;
		}
	}
	//紅色按鈕→控制所有側燈光聚集於一點
	if (g_p2DBtn[3]->OnTouches(pt)) {
		if (g_p2DBtn[3]->getButtonStatus()) {
			printf("白色官\n");
			//room1->g_Light[0].isLighting = false;
		}
		else {
			printf("白色開\n");
			//room1->g_Light[0].isLighting = true;
		}
	}
}

int UpdatePlayerState() {
	vec4 cameravp = CCamera::getInstance()->_viewPosition;

	if (cameravp.z > -9.5f && cameravp.z < 9.5f && cameravp.x < 9.5f && cameravp.x > -9.5f) {
		return PLAYERSTATE::ROOM1;
	}
	else if (cameravp.z > -9.5f && cameravp.z < 9.5f  && cameravp.x >= 9.5f && cameravp.x < 27.0f) {
		return PLAYERSTATE::ROOM2;
	}
	else if (cameravp.z <= -9.5f && cameravp.z > -29.5f  &&  cameravp.x < 9.5f && cameravp.x > -9.5f) {
		return PLAYERSTATE::ROOM4;
	}
	else if (cameravp.z <= -9.5f && cameravp.z > -29.5f  && cameravp.x >= 9.5f && cameravp.x < 27.0f) {
		return PLAYERSTATE::ROOM3;
	}
	else if (cameravp.z <= -29.5f && cameravp.z > -49.5f  &&  cameravp.x < 9.5f && cameravp.x > -9.5f) {
		return PLAYERSTATE::ROOM5;
	}
	else if (cameravp.z <= -29.5f && cameravp.z > -49.5f  && cameravp.x >= 9.5f && cameravp.x < 27.0f) {
		return PLAYERSTATE::ROOM6;
	}
	return PLAYERSTATE::OUTSIDE;
}