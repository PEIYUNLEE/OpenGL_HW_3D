#pragma once

#include "../header/Angel.h"

#include "ModelPool.h"
#include "CQuad.h"
#include "CWireSphere.h"
#include "CLineSegment.h"
#include "CTexturePool.h"

#include "CSolidCube.h"
#include "CSolidSphere.h"
#include "../png_loader.h"

class Room
{
private:
public:

	enum ROOMSTATE
	{
		LEVEL0 = 0,	//
		LEVEL1,	//貓吃老鼠按鈕出現
		LEVEL2,
		LEVEL3,
		DONE = 6,
	};

	int roomState;

	Room();
	~Room();

	int lightCount;
	int doorCount;
	float roomPosX, roomPosY,roomPosZ;

	bool g_bAutoRotating;

	float g_fLightR;
	float g_fLightG;
	float g_fLightB;

	float g_fElapsedTime;
	float g_fLightRadius;
	float g_fLightTheta;

	virtual void SetProjectionMatrix(mat4 mpx) = 0;
	virtual void SetViewMatrix(mat4 mvx,vec4 cameraViewPosition) = 0;
	virtual void Draw(vec4 cameraPos) = 0;
	virtual void Update(float delta) = 0;

	LightSource *g_Light;

	CWireSphere *g_pLight;
	CLineSegment *g_LightLine;

	// Texture 
	int g_iTexWidth, g_iTexHeight;
	GLuint *g_uiFTexID; // 三個物件分別給不同的貼圖

	virtual void RotateBillboard(float g_fPhi)=0;
	virtual void ChangeLevel(int tolevel,bool &rBtnGet, bool &gBtnGet, bool &bBtnGet) = 0;
protected:
	CQuad *g_LeftWall, *g_RightWall;
	CQuad *g_FrontWall, *g_BackWall;
	CQuad *g_BottomWall, *g_TopWall;

	CQuad *g_pDoor;

	virtual void LightGenerator(float x, float y, float z, int count) = 0;
	virtual void ObjectGenerator(float x, float y, float z, point4 eye) = 0;
	virtual void DoorGenerator(float x, float y, float z, int count) = 0;
	virtual void TurnObj()=0;
};


class Room1 :public Room
{
private:
	ModelPool *g_pCat;
	ModelPool *g_pDeer;
	ModelPool *g_pRat;
	ModelPool *g_pWolf;
	ModelPool *g_pBtn1, *g_pBtn2, *g_pBtn3;
public:
	bool rBtnGet, gBtnGet, bBtnGet;

	Room1(float px, float py, float pz, point4 eye);
	~Room1();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
	void ChangeLevel(int tolevel, bool &rBtnGet, bool &gBtnGet, bool &bBtnGet);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
	void TurnObj();
};

class Room2 :public Room
{
private:
	ModelPool *g_pCar1,*g_pCar2;
	mat4 carInitPos1, carInitPos2;
	float timer1, timer2;
public:
	Room2(float px, float py, float pz, point4 eye);
	~Room2();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
	void ChangeLevel(int tolevel, bool &rBtnGet, bool &gBtnGet, bool &bBtnGet);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
	void TurnObj();
	void CarMove(float dt);
};

class Room3 :public Room
{
private:
	CQuad *g_pSpiderFly, *g_pG, *g_pSpiderDown;

public:
	Room3(float px, float py, float pz, point4 eye);
	~Room3();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
	void ChangeLevel(int tolevel, bool &rBtnGet, bool &gBtnGet, bool &bBtnGet);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
	void TurnObj();
};

class Room4 :public Room
{
private:
	CSolidSphere  *g_pSphere, *g_pEarth, *g_pSun, *g_pMoon;

	float sunRevTimer, earthRevTimer, earthRotTimer, moonRevTimer, moonRotTimer;
	float enRotTimer;
	mat4 mxSunT, mxEarthT, mxMoonT, mxEmT;
	// Texture 
	GLuint g_uiSphereCubeMap; // for Cubic Texture

public:
	Room4(float px, float py, float pz, point4 eye);
	~Room4();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
	void ChangeLevel(int tolevel, bool &rBtnGet, bool &gBtnGet, bool &bBtnGet);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
	void TurnObj();
	void PlanetUpdate(float dt);
};

class Room5 :public Room
{
private:
	CSolidCube *g_pCube;
public:
	Room5(float px, float py, float pz, point4 eye);
	~Room5();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
	void ChangeLevel(int tolevel, bool &rBtnGet, bool &gBtnGet, bool &bBtnGet);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
	void TurnObj();
};

class Room6 :public Room
{
private:
	ModelPool *g_pBoxClosed, *g_pBoxOpen;
	CQuad *g_pPaper;

public:
	Room6(float px, float py, float pz, point4 eye);
	~Room6();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
	void ChangeLevel(int tolevel, bool &rBtnGet, bool &gBtnGet, bool &bBtnGet);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
	void TurnObj();
};


class Room7 :public Room
{
private:
public:
	Room7(float px, float py, float pz, point4 eye);
	~Room7();

	void SetProjectionMatrix(mat4 mpx);
	void SetViewMatrix(mat4 mvx, vec4 cameraViewPosition);
	void Draw(vec4 cameraPos);
	void Update(float delta);
	void RotateBillboard(float g_fPhi);
	void ChangeLevel(int tolevel, bool &rBtnGet, bool &gBtnGet, bool &bBtnGet);
protected:
	void LightGenerator(float x, float y, float z, int count);
	void ObjectGenerator(float x, float y, float z, point4 eye);
	void TextureGenerator(int count);
	void UpdateLightPosition(float dt);
	void DoorGenerator(float px, float py, float pz, int count);
	void TurnObj();
};