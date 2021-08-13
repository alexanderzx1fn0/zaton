#ifndef LOAD_SCENE_H_
#define LOAD_SCENE_H_
#include "src/game/objects.h"
#include "src/graphics/opengl/OpenGLExtensions.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "src/graphics/data/vbocube.h"

#include "src/game/Terrain.h"

namespace Sc
{
   bool terrain = true; 
}
void InitObject(int number)
{
	for (int i = 0; i<maxbuf; i++)
	{
		objects[number].buf[i].vao = 0;
		objects[number].buf[i].vbo = 0;
		objects[number].buf[i].ebo = 0;
	}
}
void UpdateMemoryObj()
{
	if (objCount>0)
	{
		Object_ *objRes = new Object_ [objCount+1];
		for (int i = 0;i<objCount;i++)
			objRes[i] = objects[i];

		delete[] objects;
		objects = objRes;//new Object_ [objCount+1];
	}
	else
		objects = new Object_ [objCount+1];
}

void ReleaseLocalObject(int number)
{
	for (int j = 0; j<maxbuf; j++)
	{
		if(objects[number].buf[j].vao)
                {
                    glBindVertexArray(objects[number].buf[j].vao);
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    if (objects[number].buf[j].vbo)
                    {
                        glBindBuffer(GL_ARRAY_BUFFER, 0);
                        glDeleteBuffers(1, &objects[number].buf[j].vbo);
                    }
                    if (objects[number].buf[j].ebo)
                    {
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                        glDeleteBuffers(1, &objects[number].buf[j].ebo);
                    }
                    glBindVertexArray(0);
                    glDeleteVertexArrays(1, &objects[number].buf[j].vao);
                }
	}
    
}
void CleanObjects()
{
        // TODO: deallocate gpu memory
	for (int i = 0; i<objCount; i++)
        {
		ReleaseLocalObject(i);
        }
	objCount = 0;

	delete[] objects;
}
void CreateLocalObject(int number, int lod, bool optionslod)
{
	if (objects[number].obj == objType::vb_cube)
		optionslod = false;

	objects[number].optionslod = optionslod;

	if (optionslod) lod = 40;


	float scX =	objects[number].sc.x;
	float scY =	objects[number].sc.y;
	float scZ =	objects[number].sc.z;
	float scu = objects[number].scU;
	float scv = objects[number].scV;
	float scu1 = objects[number].scU1;
	float scv1 = objects[number].scV1;

	InitObject(number);

	switch(objects[number].obj)
	{
		case objType::vb_cube:
			objects[number].buf[0] = CreateBuffersCube(scX,scY,scZ,scu,scv,scu1,scv1);
			break;
		default:
			objects[number].local = false;
			break;
	}
}

void SetMatrixObj(int number)
{
        // TODO: create class Transform
	glm::mat4 mTranslation(1.0f);
        glm::mat4 mScaling(1.0f);
        glm::mat4 mRotationX(1.0f);
        glm::mat4 mRotationY(1.0f);
        glm::mat4 mRotationZ(1.0f);

	if(objects[number].local)// && objects[number].obj != objType::vb_cube
//		&& objects[number].obj != objType::vb_polygoneV)
		mScaling = glm::scale(mScaling,  glm::vec3(1.0f, 1.0f, 1.0f));
	else
		mScaling = glm::scale(mScaling,  glm::vec3(objects[number].sc.x, objects[number].sc.y, objects[number].sc.z));

        mTranslation = glm::translate(mTranslation, glm::vec3(objects[number].pos.x, objects[number].pos.y, objects[number].pos.z));

        mRotationX = glm::rotate(mRotationX, objects[number].rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
        mRotationY = glm::rotate(mRotationY, objects[number].rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
        mRotationZ = glm::rotate(mRotationZ, objects[number].rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

        // TODO: at this moment I don''t know righw of seq of matrix multiplication
	objects[number].ObjWorldMatrices = mScaling*mRotationX*mRotationY*mRotationZ*mTranslation;
	//objects[number].ObjWorldMatrices = mTranslation * mRotationZ * mRotationY * mRotationX * mScaling;
}

//Ôóíêöèÿ ñîçäàíèÿ ìàòðèöû îáúåêòà
void SetMatrixObj(int number, float trX,float trY,float trZ, float scX, float scY, float scZ,float rtX,float rtY,float rtZ)
{
	objects[number].pos.x = trX;
	objects[number].pos.y = trY;
	objects[number].pos.z = trZ;

	objects[number].sc.x = scX;
	objects[number].sc.y = scY;
	objects[number].sc.z = scZ;

	objects[number].rot.x = rtX;
	objects[number].rot.y = rtY;
	objects[number].rot.z = rtZ;

	SetMatrixObj(number);
}

void CrObjParam ( int number, int ObjType, float trX,float trY,float trZ, float scX, float scY, float scZ, 
				 float r,float g,float b,float a, int tex, bool terrain,bool local,float scu,float scv,float scu1,float scv1)
{
	objects[number].obj = ObjType;

	if (ObjType<objType::vb_max)
		objects[number].modelColl = ObjType;

	objects[number].local = local;

	if(Sc::terrain && terrain) 
	{
		float a = adjTerrainPointer->getHeight( trX, trZ );
		float m = TheTerrain->getHeight( trX, trZ );

		if (a<m) a = m;

		trY = trY + a +scY;//*0.5f;
	}

	SetMatrixObj(number,trX,trY,trZ,scX,scY,scZ,0,0,0);

	//objects[number].mtrl = InitMtrl(D3DXCOLOR(r,g,b,a),D3DXCOLOR(r,g,b,a),D3DXCOLOR(0,0,0,0),D3DXCOLOR(0,0,0,0),2);
	if (a >= 1.0f)
		objects[number].blend = false;
	else
		objects[number].blend = true;

	objects[number].texture = tex;
	objects[number].phisic = true;
	objects[number].show = true;
	objects[number].doubleRender = false;
	objects[number].sort = false;
	objects[number].scU = scu;
	objects[number].scV = scv;
	objects[number].scU1 = scu1;
	objects[number].scV1 = scv1;

	if (local)
		CreateLocalObject(number,40,true);
}


void AddObj( int ObjType, float trX,float trY,float trZ, float scX, float scY, float scZ, 
			float r,float g,float b,float a, int tex, bool terrain,bool local,float scU,float scV,float scU1,float scV1)
{
	UpdateMemoryObj();
	InitObject(objCount);
	CrObjParam(objCount++,ObjType,trX,trY,trZ,scX,scY,scZ,r,g,b,a,tex,terrain,local,scU,scV,scU1,scV1);
}

void AddDoorCupe(float trX,float trY,float trZ,float scX,float scY,float scZ,float direction,float speed)
{
	float tv = 1/scY;
	float tu;

	if(scX>scZ)
		tu = 1/scX;
	else
		tu = 1/scZ;

	AddObj(objType::vb_cube,trX,trY,trZ,scX,scY,scZ,1,1,1,1,objType::vb_door_cupe, true,true,tu,tv,tu,tv);//,1/scY); - íå îøèáêà!!!!

	int i = objCount-1;
	objects[i].modelColl = objType::vb_door_cupe;
	objects[i].blend = true;
	objects[i].doubleRender = false;
	objects[i].activate = false;
	objects[i].animated = false;
	objects[i].speedAnimated = speed;
	objects[i].timeNotShow = direction;//äàííûé ïàðàìåòð èñìîëüçóåòñÿ äëÿ îïðåäåëåíèÿ ñîñò îòêð/çàêð äëÿ äâåðè

	if (scX>scZ)
	{
		objects[i].vec.x = trX+2*scX*direction;//ïåðåìåùåíèå ïðè îòêðûòèè
		objects[i].vec.y = trX;//ïåðåìåùåíèå ïðè çàêðûòèè
	}
	else
	{
		objects[i].vec.x = trZ+2*scZ*direction;
		objects[i].vec.y = trZ;
	}
}

void AddBunker_1(float trX,float trY,float trZ)
{
	//ðàçìåðû áóíêåðà
	float scx = 15.0f;
	float scy = 8.0f;
	float scz = 15.0f;
	//ðàçìåðû äâåðè
	float dscx = 0.125f;
	float dscy = 6.0f;
	float dscz = 3.2f;

	if (Sc::terrain)
        {
		trY += TheTerrain->getHeight( trX, trZ )+scy;
        }
	else
        {
		trY += scy;
        }

	AddObj ( objType::vb_cube,   trX-scx,trY,trZ, 1.0f, scy, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj ( objType::vb_cube,   trX,trY,trZ+scz, scx, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);

	AddObj ( objType::vb_cube,   trX,trY,trZ-scz, scx, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//ïîë è ïîòîëîê
	AddObj ( objType::vb_cube,   trX,trY+scy,trZ, scx+1.5f, 1.0f, scz+1.5f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj ( objType::vb_cube,   trX,trY-scy-0.4f,trZ, scx+3.0f, 1.0f, scz+3.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//âõîäíàÿ ÷àñòü
	AddObj ( objType::vb_cube,   trX+scx,trY,trZ-(scz+dscz)/2, 1.0f, scy, (scz-dscz)/2+0.1f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj ( objType::vb_cube,   trX+scx,trY,trZ+(scz+dscz)/2, 1.0f, scy, (scz-dscz)/2+0.1f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj ( objType::vb_cube,   trX+scx,trY+dscy,trZ, 1.0f, scy-dscy, dscz-0.1f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);

	AddDoorCupe (trX+scx,trY,trZ, dscx, dscy, dscz,-1.0f,4.0f);
	objects[objCount-1].pos.y = trY +dscy-scy;
	SetMatrixObj(objCount-1);
}
void AddBunker_2(float trX,float trY,float trZ)
{
	//размеры бункера
	float scx = 15.0f;
	float scy = 8.0f;
	float scz = 15.0f;
	//размеры двери
	float dscx = 3.2f;
	float dscy = 6.0f;
	float dscz = 0.125f;

	if (Sc::terrain)
        {
		trY += TheTerrain->getHeight( trX, trZ )+scy;
        }
	else
        {
		trY += scy;
        }
	//стены
	AddObj (objType::vb_cube,   trX,trY,trZ-scz, scx, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+scx,trY,trZ, 1.0f, scy, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX-scx,trY,trZ, 1.0f, scy, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//пол и потолок
	AddObj (objType::vb_cube,   trX,trY+scy,trZ, scx+1.5f, 1.0f, scz+1.5f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY-scy-0.4f,trZ, scx+3.0f, 1.0f, scz+3.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//входная часть
	AddObj (objType::vb_cube,   trX-(scx+dscx)/2,trY,trZ+scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+(scx+dscx)/2,trY,trZ+scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY+dscy,trZ+scz, dscx-0.1f, scy-dscy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//дверь
	AddDoorCupe (trX,trY,trZ+scz, dscx, dscy, dscz,-1.0f,4.0f);
	objects[objCount-1].pos.y = trY +dscy-scy;
	SetMatrixObj(objCount-1);
}
void AddBunker_3(float trX,float trY,float trZ)
{
	//ðàçìåðû áóíêåðà
	float scx = 15.0f;
	float scy = 8.0f;
	float scz = 15.0f;
	//ðàçìåðû äâåðè
	float dscx = 3.2f;
	float dscy = 6.0f;
	float dscz = 0.125f;

	if (Sc::terrain)
        {
		trY += TheTerrain->getHeight( trX, trZ )+scy;
        }
	else
        {
		trY += scy;
        }

	//ñòåíû
	AddObj (objType::vb_cube,   trX,trY,trZ+scz, scx, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+scx,trY,trZ, 1.0f, scy, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX-scx,trY,trZ, 1.0f, scy, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//ïîë è ïîòîëîê
	AddObj (objType::vb_cube,   trX,trY+scy,trZ, scx+1.5f, 1.0f, scz+1.5f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY-scy-1.0f,trZ, scx+3.0f, 1.0f, scz+3.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//âõîäíàÿ ÷àñòü
	AddObj (objType::vb_cube,   trX-(scx+dscx)/2,trY,trZ-scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+(scx+dscx)/2,trY,trZ-scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY+dscy,trZ-scz, dscx-0.1f, scy-dscy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//äâåðü
	//AddDoor (rX,trY,trZ-scz, dscx, dscy, dscz);
	//objects[objCount-1].pos.y = trY +dscy-scy;
	//SetMatrixObj(objCount-1);
}

void AddTurret_1(float trX,float trY,float trZ)
{
	//размеры башни
	float scx = 8.0f;
	float scy = 20.0f;
	float scz = 8.0f;
	//размеры прохода
	float dscx = 4.0f;
	float dscy = 6.0f;
	float dscz = 0.125f;
	float fh = 0.125f;//фундамент

	if (Sc::terrain)
        {
		trY += TheTerrain->getHeight( trX, trZ )+scy;
        }
	else
        {
		trY += scy;
        }
	//стены
	AddObj (objType::vb_cube,   trX,trY,trZ+scz, scx, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+scx,trY,trZ, 1.0f, scy, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX-scx,trY,trZ, 1.0f, scy, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//пол, верхний пол и потолок
	AddObj (objType::vb_cube,   trX,trY+scy,trZ, scx+1.5f, 1.0f, scz+1.5f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY+5.0f,trZ-scz/2, scx, 1.0f, scz/2, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY-scy-fh,trZ, scx+3.0f, 0.125f, scz+3.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//входная часть
	AddObj (objType::vb_cube,   trX-(scx+dscx)/2,trY,trZ-scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+(scx+dscx)/2,trY,trZ-scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY,trZ-scz, dscx-0.1f, scy-dscy*2, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//лестница и дверь
        /*
	AddStairs(rX+scx-1.2f,0.0f,trZ+3.0f, 0.0001f, 2*scy/3, 2.0f);
	objects[objCount-1].pos.y = trY+2*scy/3-scy;//+scy/2-scy;
	SetMatrixObj(objCount-1);
	AddDoor (rX,0.0f,trZ-scz, dscx, dscy, dscz);
	objects[objCount-1].pos.y = trY +dscy-scy;
	SetMatrixObj(objCount-1);
        */
}
void AddTurret_2(float trX,float trY,float trZ)
{
	//размеры башни
	float scx = 8.0f;
	float scy = 20.0f;
	float scz = 8.0f;
	//размеры прохода
	float dscx = 4.0f;
	float dscy = 6.0f;
	float dscz = 0.25f;
	float fh = 0.125f;//фундамент
	float lh = scy+5.5f;//высота поднятия лифта

	if (Sc::terrain)
        {
		trY += TheTerrain->getHeight( trX, trZ )+scy;
        }
	else
        {
		trY += scy;
        }
	//стены
	AddObj (objType::vb_cube,   trX,trY,trZ+scz, scx, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+scx,trY,trZ, 1.0f, scy, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX-scx,trY,trZ, 1.0f, scy, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//пол, верхний пол и потолок
	AddObj (objType::vb_cube,   trX,trY+scy,trZ, scx, 1.0f, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY+5.0f,trZ-scz/2, scx, 1.0f, scz/2, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY-scy-fh,trZ, scx+3.0f, 0.125f, scz+3.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//входная часть
	AddObj (objType::vb_cube,   trX-(scx+dscx)/2,trY,trZ-scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+(scx+dscx)/2,trY,trZ-scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY,trZ-scz, dscx-0.1f, scy-dscy*2, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
/*
	//лифт
	AddLift(pd3dDevice,trX,0.0f,trZ+scz/2,scx,dscz,scz/2,lh,5.0f,true);
	objects[objCount-1].pos.y = trY-scy;//trY +dscz-scy*3;
	objects[objCount-1].vec.y = objects[objCount-1].pos.y;
	objects[objCount-1].vec.x = lh+objects[objCount-1].pos.y;
	SetMatrixObj(objCount-1);
	//Лестница
	AddStairs(pd3dDevice,trX+scx+1.2f,0.0f,trZ+3.0f, 0.0001f, scy, 2.0f);
	objects[objCount-1].pos.y = trY;// +scy/2-scy;
	SetMatrixObj(objCount-1);
*/
}

void AddTurret_3(float trX,float trY,float trZ)
{
	//размеры башни
	float scx = 8.0f;
	float scy = 20.0f;
	float scz = 8.0f;
	//размеры прохода
	float dscx = 4.0f;
	float dscy = 6.0f;
	float dscz = 0.25f;
	float fh = 0.125f;//фундамент
	float lh = scy+5.625f;//высота поднятия лифта
	if (Sc::terrain)
        {
		trY += TheTerrain->getHeight( trX, trZ )+scy;
        }
	else
        {
		trY += scy;
        }
	//стены
	AddObj (objType::vb_cube,   trX,trY,trZ+scz, scx, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+scx,trY,trZ, 1.0f, scy, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX-scx,trY,trZ, 1.0f, scy, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//пол, верхний пол и потолок
	AddObj (objType::vb_cube,   trX,trY+scy,trZ, scx, 1.0f, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY+5.0f,trZ-scz/2, scx, 1.0f, scz/2, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY-scy-fh,trZ, scx+3.0f, 0.125f, scz+3.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//входная часть
	AddObj (objType::vb_cube,   trX-(scx+dscx)/2,trY,trZ-scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+(scx+dscx)/2,trY,trZ-scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY+dscy,trZ-scz, dscx-0.1f, scy-dscy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
/*
	//лифт
	AddLift(pd3dDevice,trX,0.0f,trZ+scz/2,scx,dscz,scz/2,lh,5.0f,true);
	objects[objCount-1].pos.y = trY-scy;//trY +dscz-scy*3;
	objects[objCount-1].vec.y = objects[objCount-1].pos.y;
	objects[objCount-1].vec.x = lh+objects[objCount-1].pos.y;
	SetMatrixObj(objCount-1);
	//Лестница
	AddStairs(pd3dDevice,trX+scx+1.2f,0.0f,trZ+3.0f, 0.0001f, scy, 2.0f);
	objects[objCount-1].pos.y = trY;// +scy/2;//scy+scy/2-scy;
	SetMatrixObj(objCount-1);
*/
}

void AddTurret_4(float trX,float trY,float trZ)
{
	//размеры башни
	float scx = 6.0f;
	float scy = 40.0f;
	float scz = 6.0f;
	//размеры прохода
	float dscx = 4.0f;
	float dscy = 6.0f;
	float fh = 0.125f;//фундамент
	if (Sc::terrain)
        {
		trY += TheTerrain->getHeight( trX, trZ )+scy;
        }
	else
        {
		trY += scy;
        }
	//стены
	AddObj (objType::vb_cube,   trX,trY,trZ+scz, scx, scy+6.0f, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+scx,trY,trZ, 1.0f, scy+4.0f, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX-scx,trY,trZ, 1.0f, scy+4.0f, scz, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//пол и потолок
	AddObj (objType::vb_cube,   trX,trY+scy,trZ-scz/2-4.0f, scx, 1.0f, scz/2+2.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY-scy-fh,trZ, scx+3.0f, 0.125f, scz+3.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	//входная часть
	AddObj (objType::vb_cube,   trX-(scx+dscx)/2,trY,trZ-scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX+(scx+dscx)/2,trY,trZ-scz, (scx-dscx)/2+0.1f, scy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
	AddObj (objType::vb_cube,   trX,trY+dscy,trZ-scz, dscx-0.1f, scy-dscy, 1.0f, 1,1,1, 1, objType::tex_wall, false,true,0.25f,0.25f,0.25f,0.25f);
/*
	//Лестница
	AddStairs(pd3dDevice,trX,0.0f,trZ+scz-1.2f, 2.0f, scy, 0.0001f);
	objects[objCount-1].pos.y = trY;// + scy-8.0f;//+dscy-scy;
	SetMatrixObj(objCount-1);
*/
}


void CreateStandartObjects()
{
/*
			AddTree(pd3dDevice,-423.7f,0.0f,-373.8f); 
			AddTree(pd3dDevice,-419.5f,0.0f,-263.3f); 
			AddTree(pd3dDevice,-262.3f,0.0f,-336.1f); 
			AddTree(pd3dDevice,-240.1f,0.0f,-222.5f); 
			AddTree(pd3dDevice,-246.1f,0.0f,-100.5f); 
			AddTree(pd3dDevice,-378.7f,0.0f,-45.1f); 
			AddTree(pd3dDevice,-437.6f,0.0f,53.4f); 
			AddTree(pd3dDevice,-320.4f,0.0f,81.9f); 
			AddTree(pd3dDevice,-245.4f,0.0f,84.5f); 

			AddBush_1(pd3dDevice,-363.4f, 0.0f , -92.9f ); 
			AddBush_2(pd3dDevice,-320.4f, 0.0f , -52.5f ); 
			AddBush_1(pd3dDevice,-292.4f, 0.0f ,  55.0f );  
			AddBush_2(pd3dDevice,-245.4f, 0.0f ,  72.2f );
			AddBush_2(pd3dDevice,-373.4f, 0.0f ,  30.8f ); 
			AddBush_1(pd3dDevice,-441.9f, 0.0f , -59.9f ); 
			AddBush_2(pd3dDevice,-324.1f, 0.0f , -111.1f ); 
			AddBush_2(pd3dDevice,-382.6f, 0.0f , -344.3f ); 
			AddBush_2(pd3dDevice,-284.7f, 0.0f , -452.5f ); 
			AddBush_2(pd3dDevice,-263.5f, 0.0f , -377.8f ); 
			AddBush_2(pd3dDevice,-427.7f, 0.0f , -328.7f ); 


			AddTree(pd3dDevice, -414.0f, 0.0f, 192.7f ) ;
			AddTree(pd3dDevice, -290.9f, 0.0f, 195.1f ) ; 
			AddTree(pd3dDevice, -240.1f, 0.0f, 232.0f ) ; 
			AddTree(pd3dDevice, -266.9f, 0.0f, 331.1f ) ; 
			AddTree(pd3dDevice, -342.5f, 0.0f, 329.8f ) ; 
			AddTree(pd3dDevice, - 389.5f , 0.0f , 304.9f ) ;
			AddTree(pd3dDevice, - 408.8f , 0.0f , 262.1f ) ;
			AddTree(pd3dDevice, - 297.6f , 0.0f , 300.0f ) ;
			AddTree(pd3dDevice, - 320.6f , 0.0f , 203.7f ) ;
			AddBush_1(pd3dDevice, - 144.8f , 0.0f , 300.5f ) ;
			AddTree(pd3dDevice, - 119.3f , 0.0f , 404.3f ) ;
			AddTree(pd3dDevice, - 95.8f , 0.0f , 429.1f ) ;
			AddBush_1(pd3dDevice, 30.0f, 0.0f, 407.8f ) ;
			AddTree(pd3dDevice, 1.9f, 0.0f , 350.7f ) ;
			AddTree(pd3dDevice, -39.9f , 0.0f , 231.7f) ;
			AddTree(pd3dDevice, -117.2f , 0.0f , 152.6f ) ;
			AddTree(pd3dDevice, 42.0f , 0.0f , 103.7f ) ;
			AddBush_1(pd3dDevice, 13.0f , 0.0f , -88.8f ) ;
			AddTree(pd3dDevice, -7.5f , 0.0f , -221.4f ) ;
			AddTree(pd3dDevice, -123.2f , 0.0f , -188.8f );


			AddBush_2(pd3dDevice,80.8f , 0.0f , 193.6f ); 
			AddBush_1(pd3dDevice,-9.5f , 0.0f , 124.9f ); 
			AddBush_2(pd3dDevice,-30.9f , 0.0f , 47.3f ); 
			AddBush_2(pd3dDevice,-52.3f , 0.0f , -76.2f ); 
			AddBush_1(pd3dDevice,-7.0f , 0.0f , -126.9f ); 
			AddBush_2(pd3dDevice,-60.5f , 0.0f , -184.5f); 
			AddTree(pd3dDevice,-87.1f , 0.0f , -420.7f ); 
			AddBush_2(pd3dDevice, -7.3f, 0.0f , - 422.3f ); 
			AddBush_2(pd3dDevice,90.9f , 0.0f, - 410.1f ); 
			AddBush_2(pd3dDevice,181.3f , 0.0f , -405.8f ); 
			AddBush_1(pd3dDevice,274.7f , 0.0f , -393.8f ); 
			AddBush_2(pd3dDevice,308.0f , 0.0f , -402.5f ); 
			AddBush_2(pd3dDevice,359.2f , 0.0f , -364.3f ); 


			AddTree(pd3dDevice, 268.1f , 0.0f , -342.4f ) ; 
 			AddTree(pd3dDevice,313.3f , 0.0f , -434.9f ) ; 
 			AddTree(pd3dDevice,429.5f , 0.0f , -209.9f ) ; 
 			AddTree(pd3dDevice,366.7f , 0.0f , -165.6f ) ; 
 			AddTree(pd3dDevice,292.0f , 0.0f , -237.2f ) ; 
 			AddTree(pd3dDevice,205.7f , 0.0f , -148.0f ) ; 
 			AddTree(pd3dDevice,134.9f , 0.0f , -206.6f ) ; 
 			AddTree(pd3dDevice,129.3f , 0.0f , -3.9f ) ; 
 			AddTree(pd3dDevice,118.9f , 0.0f , -59.2f ) ; 
 			AddTree(pd3dDevice,174.1f , 0.0f , 21.4f ) ; 
 			AddTree(pd3dDevice,247.8f , 0.0f , -53.5f ) ; 
 			AddTree(pd3dDevice,340.6f , 0.0f , -46.8f ) ; 
 			AddTree(pd3dDevice,299.5f , 0.0f , 35.3f ) ; 
 			AddTree(pd3dDevice,427.0f , 0.0f , -10.0f ) ; 
 			AddTree(pd3dDevice,430.6f , 0.0f , 58.6f ) ; 
 			AddTree(pd3dDevice,313.6f , 0.0f , 159.9f ) ; 
 			AddTree(pd3dDevice,409.3f , 0.0f , 182.1f ) ; 
 			AddTree(pd3dDevice,297.5f , 0.0f , 207.5f ) ; 
 			AddTree(pd3dDevice,353.7f , 0.0f , 266.5f ) ; 

			AddBush_2(pd3dDevice,413.7f , 0.0f , 126.9f ) ; 
			AddBush_2(pd3dDevice,402.9f , 0.0f , 66.5f ) ; 
			AddBush_2(pd3dDevice,347.4f , 0.0f , 140.4f ) ; 
			AddBush_2(pd3dDevice,326.4f , 0.0f , 19.0f ) ; 
			AddBush_2(pd3dDevice,204.0f , 0.0f , -8.6f ) ; 
			AddBush_2(pd3dDevice,194.1f , 0.0f , -166.5f ) ; 
			AddBush_2(pd3dDevice,268.6f , 0.0f , -203.7f ) ; 
			AddBush_2(pd3dDevice,375.5f , 0.0f , -205.0f ) ; 
			AddBush_2(pd3dDevice,409.0f , 0.0f , -251.7f ) ; 
			AddBush_2(pd3dDevice,-25.5f , 0.0f , -377.8f ) ; 

			AddGrass_1(pd3dDevice,-322.4f , 0.0f , -312.7f ) ; 
			AddGrass_2(pd3dDevice, -365.4f , 0.0f , -253.4f ) ; 
			AddGrass_1(pd3dDevice, -373.3f , 0.0f , -145.9f ) ; 
			AddGrass_2(pd3dDevice, -342.9f , 0.0f , 25.1f ) ; 
			AddGrass_1(pd3dDevice, -336.4f , 0.0f , 217.2f ) ; 
			AddGrass_2(pd3dDevice, -101.4f , 0.0f , 366.3f ) ; 
			AddGrass_1(pd3dDevice, 0.2f , 0.0f , 269.7f ) ; 
			AddGrass_2(pd3dDevice, -49.0f , 0.0f , 97.5f ) ; 
			AddGrass_1(pd3dDevice, -32.7f , 0.0f , -151.4f ) ; 
			AddGrass_2(pd3dDevice, -52.2f , 0.0f , -399.6f ) ; 
			AddGrass_1(pd3dDevice, 174.1f , 0.0f , -432.7f ) ; 
			AddGrass_2(pd3dDevice, 339.7f , 0.0f , -197.4f ) ; 
			AddGrass_1(pd3dDevice, 154.8f , 0.0f , -127.7f ) ; 
			AddGrass_2(pd3dDevice, 221.2f , 0.0f , -23.9f ) ; 
			AddGrass_1(pd3dDevice, 270.3f , 0.0f , -17.0f ) ; 
			AddGrass_2(pd3dDevice, 338.6f , 0.0f , -8.8f ) ; 
			AddGrass_1(pd3dDevice, 360.4f , 0.0f , 55.0f ) ; 
			AddGrass_2(pd3dDevice, 375.6f , 0.0f , 106.6f ) ; 

			AddArmour(pd3dDevice,-383.3f , 1.0f , -217.2f ) ; 
			AddKit(pd3dDevice,-272.4f , 1.0f , -58.8f ) ; 

			AddArmourMini(pd3dDevice,-421.6f , 1.0f , 232.6f ) ; 
			AddArmourMini(pd3dDevice,-405.2f , 1.0f , 218.9f ) ; 

			for (float i = 0.0f; i<3.0f;i++)
				for (float j = 0.0f; j<3.0f;j++)
				{
					AddStake(pd3dDevice,-97.3f +i, 0.0f , 121.0f +j) ; 
 					AddSoul(pd3dDevice,-54.4f +i, 0.0f , 9.4f +j) ; 
					AddJavelin(pd3dDevice,-110.3f +i, 0.0f , -423.3f +j) ; 
					AddStake(pd3dDevice,340.1f +i, 0.0f , -427.9f +j) ; 
					AddJavelin(pd3dDevice,413.3f +i, 0.0f , -171.7f +j) ; 
					AddSoul(pd3dDevice,310.1f +i, 0.0f , 84.3f +j) ; 
					AddSoul(pd3dDevice,417.0f +i, 0.0f , 156.5f +j) ;
				}

			AddKit(pd3dDevice,362.8f , 2.0f , -60.0f ) ; 

			AddSpade(pd3dDevice,-324.0f,-0.5f,-300.0f);

			AddBush_1(pd3dDevice,-334.0f,0.0f,-400.0f);
			AddBush_2(pd3dDevice,-300.0f,0.0f,-350.0f);
			AddArmour(pd3dDevice,-300.0f,1.0f,-430.0f);
*/
			AddTurret_1(-270.0f,0.0f,-60.5f);
			AddTurret_2(-383.0f,0.0f,-212.5f);

/*
			AddDoorCupe (pd3dDevice,-344.5f,-0.5f,141.0f,20.0f,30.0f,1.0f,1.0f,4.0f);
			AddDoorCupe (pd3dDevice,-384.5f,-0.5f,141.0f,20.0f,30.0f,1.0f,-1.0f,4.0f);
			objects[objCount-1].pos.y = objects[objCount-2].pos.y;
			SetMatrixObj(objCount-1);
*/
			AddBunker_1(-414.0f,0.0f,229.0f);
			//AddTeleport(26.0f,0.0f,256.0f,166.0f,4.0f,409.0f);
			//AddTeleport(190.0f,0.0f,104.0f,18.0f,4.0f,193.0f);
			//AddFence(91.0f,0.0f,191.0f,0.00001f,10.0f,150.0f,1.0f,0.5f);
			AddTurret_4(-100.0f,1.0f, 120.5f);
			AddBunker_2(343.0f,0.0f,-417.0f);
			AddBunker_1(317.0f,0.0f,85.0f);
			AddTurret_3(137.0f,0.0f, -224.5f);
			AddBunker_1(367.0f,0.0f, -50.5f);
			AddTurret_1(422.0f,0.0f, 161.5f);
/*
			AddEndLavel(pd3dDevice,318.0f,0.0f,300.0f);
			AddSecretPlace(pd3dDevice,444.0f,0.0f,410.0f);
			AddArmour(pd3dDevice,444.0f,1.0f,424.0f);
			AddKit(pd3dDevice,419.0f,1.0f,444.0f);
			AddArtefact(pd3dDevice,448.0f,3.0f,440.0f);

			for(int i = 0; i<5;i++)
				for(int j = 0; j<5;j++)
				AddSoul(pd3dDevice,422.0f+j,0.0f,421.0f+i);

			AddGrass_1(pd3dDevice,424.0f,0.0f,305.0f);
			AddGrass_1(pd3dDevice,410.0f,0.0f,329.0f);
			AddGrass_1(pd3dDevice,400.0f,0.0f,309.0f);
			AddGrass_2(pd3dDevice,-359.0f,0.0f,427.0f);

			AddSecretPlace(pd3dDevice,-435.0f,0.0f,419.0f);
			AddKit(pd3dDevice,-409.0f,1.0f,430.0f);
			AddArmour(pd3dDevice,-391.0f,1.0f,407.0f);


			AddObj (pd3dDevice, objType::vb_cube,   -84.0f,0.0f,-304.0f,20.0f, 10.0f, 20.0f, 1,1,1, 1, objType::tex_wall, true,true,0.25f,0.25f,0.25f,0.25f);
			AddJumpRope(pd3dDevice,-84.0f,0.05f,-271.0f,5.0f,0.25f,5.0f,0.0f,10.0f,-5.0f);
*/
			AddBunker_2(-118.0f,0.0f,-416.0f);
			AddBunker_1(418.0f,0.0f,-183.0f);
/*
			AddSecretPlace(443.0f,0.0f,-419.0f);
			AddArtefact(449.0f,1.0f,-412.0f);



			AddObj (objType::vb_cylinder, -294.6f, 0.0f, -316.0f ,     2.4f, 3, 2.4f, 1 ,1, 1.0f, 0.5f, objType::tex_barrel, true);
			AddObj (objType::vb_cylinder, -361.1f , 0.0f, -234.7f ,     2.4f, 3, 2.4f, 1 ,1, 1.0f, 0.5f, objType::tex_barrel, true);
			AddObj (objType::vb_cylinder, -57.2f , 0.0f, 53.0f ,     2.4f, 3, 2.4f, 1 ,1, 1.0f, 0.5f, objType::tex_barrel, true);
			AddObj ( objType::vb_cone,	 0.0f,8.0f,50.0f,20, 4, 20, 1,1,1, 1, objType::tex_wall, true,true,1,1,0.25f,0.25f);
			AddObj ( objType::vb_cylinder,0.0f,0.0f,50.0f,20, 4, 20, 1,1,1, 1, objType::tex_wall, true,true,1,0.25f,0.25f,0.25f);
			AddObj ( objType::vb_cylinder,0.0f,0.0f,20.0f, 2, 4,  2, 1,1,1, 1, objType::tex_wall, true,true,1,1,0.5f,0.5f);
*/

			AddBunker_1(-50.0f,0.0f,0.0f);

			AddTurret_1(-100.0f,0.0f,-80.5f);
			AddTurret_4(-100.0f,1.0f, 120.5f);


/*

			AddBush_1(-364.8f,0.0f,-441.8f);
			AddBush_1(-405.9f,0.0f,-428.2f);
			AddBush_1(-291.1f,0.0f,-441.7f);
			AddBush_2(-227.0f,0.0f,-414.2f);
			AddGrass_1(-277.4f,0.0f,-334.7f);
			AddGrass_2(-249.3f,0.0f,-287.3f);
			AddGrass_2(-303.3f,0.0f,-261.8f);
			AddGrass_2(-400.0f,0.0f,-294.8f);
			AddGrass_1(-440.9f,0.0f,-241.5f);
			AddGrass_2(-428.2f,0.0f,-150.1f);
			AddBush_1(-305.0f,0.0f,-177.1f);
			AddBush_2(-255.0f,0.0f,-251.3f);
			AddBush_1(-240.6f,0.0f,-245.9f);
			AddBush_2(-252.1f,0.0f,-165.8f);
			AddBush_1(-289.1f,0.0f,-105.8f);
			AddBush_1(-412.9f,0.0f,-19.5f);
			AddBush_1(-358.6f,0.0f,82.9f);
			AddBush_2(-247.0f,0.0f,35.1f);
			AddBush_2(-301.2f,0.0f,-11.9f);
			AddGrass_1(-342.7f,0.0f,169.7f);
			AddGrass_2(-383.8f,0.0f,191.1f);
			AddGrass_1(-413.2f,0.0f,205.2f);
			AddGrass_2(-383.3f,0.0f,254.1f);
			AddGrass_1(-377.1f,0.0f,291.6f);
			AddGrass_2(-342.6f,0.0f,310.2f);
			AddGrass_2(-290.4f,0.0f,316.4f);
			AddBush_2(-296.3f,0.0f,273.9f);
			AddBush_2(-355.2f,0.0f,238.9f);
			AddBush_1(-228.6f,0.0f,293.3f);
			AddGrass_2(-142.1f,0.0f,363.0f);
			AddBush_1(-104.0f,0.0f,405.6f);
			AddBush_1(-53.0f,0.0f,433.0f);
			AddBush_1(6.7f,0.0f,430.5f);
			AddBush_2(-5.3f,0.0f,390.6f);
			AddBush_2(-63.9f,0.0f,363.4f);
			AddGrass_1(-104.3f,0.0f,326.6f);
			AddGrass_2(-138.2f,0.0f,285.2f);
			AddGrass_2(-79.0f,0.0f,248.4f);
			AddGrass_1(24.0f,0.0f,229.2f);
			AddGrass_1(88.5f,0.0f,219.4f);
			AddGrass_2(83.9f,0.0f,203.0f);
			AddBush_1(84.5f,0.0f,174.0f);
			AddBush_1(51.8f,0.0f,184.2f);
			AddBush_2(22.5f,0.0f,163.2f);
			AddGrass_1(-50.7f,0.0f,137.8f);
			AddBush_2(-102.0f,0.0f,176.7f);
			AddGrass_2(-130.4f,0.0f,83.5f);
			AddBush_1(-84.8f,0.0f,65.5f);
			AddBush_2(-17.4f,0.0f,16.2f);
			AddGrass_1(34.5f,0.0f,-22.4f);
			AddBush_1(-41.8f,0.0f,-37.5f);
			AddBush_1(27.8f,0.0f,-235.6f);
			AddBush_1(-98.1f,0.0f,-225.0f);
			AddBush_2(-105.0f,0.0f,-171.1f);
			AddBush_2(-98.8f,0.0f,-95.5f);
			AddGrass_1(-71.1f,0.0f,-250.7f);
			AddGrass_2(-104.4f,0.0f,-328.5f);
			AddGrass_1(-69.7f,0.0f,-329.7f);
			AddGrass_2(-44.4f,0.0f,-324.5f);
			AddGrass_1(-19.6f,0.0f,-338.3f);
			AddGrass_2(-92.3f,0.0f,-391.4f);
			AddBush_1(-73.1f,0.0f,-445.1f);
			AddBush_2(-16.1f,0.0f,-445.1f);
			AddBush_2(34.9f,0.0f,-445.2f);
			AddBush_1(55.0f,0.0f,-388.3f);
			AddBush_1(71.2f,0.0f,-358.9f);
			AddBush_2(116.7f,0.0f,-342.8f);
			AddGrass_1(184.1f,0.0f,-331.7f);
			AddGrass_2(216.9f,0.0f,-393.2f);
			AddBush_1(197.7f,0.0f,-446.9f);
			AddBush_2(258.7f,0.0f,-455.5f);
			AddBush_1(306.4f,0.0f,-389.2f);
			AddBush_2(332.4f,0.0f,-333.9f);
			AddGrass_1(415.0f,0.0f,-344.5f);
			AddBush_1(456.7f,0.0f,-277.1f);
			AddBush_1(404.4f,0.0f,-246.8f);
			AddBush_1(330.8f,0.0f,-247.8f);
			AddBush_2(307.2f,0.0f,-150.2f);
			AddBush_2(204.9f,0.0f,-237.2f);
			AddGrass_1(152.5f,0.0f,-163.0f);
			AddGrass_2(113.1f,0.0f,-123.2f);
			AddGrass_1(158.5f,0.0f,-80.0f);
			AddGrass_2(128.8f,0.0f,-29.1f);
			AddGrass_1(160.8f,0.0f,-9.3f);
			AddGrass_2(232.0f,0.0f,-2.6f);
			AddBush_2(264.7f,0.0f,-54.4f);
			AddBush_1(317.3f,0.0f,-55.3f);
			AddGrass_2(360.0f,0.0f,-17.8f);
			AddGrass_1(406.3f,0.0f,-31.8f);
			AddGrass_2(422.5f,0.0f,11.0f);
			AddGrass_1(416.6f,0.0f,80.1f);
			AddGrass_2(374.9f,0.0f,112.8f);
			AddBush_1(338.8f,0.0f,61.9f);
			AddBush_1(326.1f,0.0f,164.2f);
			AddBush_1(301.2f,0.0f,195.0f);
			AddBush_2(361.6f,0.0f,232.0f);
			AddBush_2(374.2f,0.0f,175.2f);
			AddGrass_1(318.6f,0.0f,260.6f);


			AddBush_2(-414.2f,0.0f,-452.8f);
			AddBush_1(-433.0f,0.0f,-411.9f);
			AddBush_2(-424.3f,0.0f,-347.4f);
			AddBush_1(-442.1f,0.0f,-290.0f);
			AddBush_2(-420.1f,0.0f,-209.8f);
			AddBush_1(-440.1f,0.0f,-155.9f);
			AddBush_2(-424.0f,0.0f,-112.0f);
			AddBush_1(-438.9f,0.0f,-48.3f);
			AddBush_2(-425.0f,0.0f,2.3f);
			AddBush_1(-364.4f,0.0f,-36.3f);
			AddBush_2(-399.8f,0.0f,-103.3f);
			AddBush_1(-404.0f,0.0f,-158.2f);
			AddBush_2(-346.3f,0.0f,-199.8f);
			AddBush_1(-393.8f,0.0f,183.5f);
			AddBush_2(-343.8f,0.0f,206.6f);
			AddBush_1(-322.0f,0.0f,301.7f);
			AddBush_2(-245.7f,0.0f,268.4f);
			AddBush_1(-202.8f,0.0f,338.6f);
			AddBush_2(-117.4f,0.0f,377.9f);
			AddBush_1(-6.9f,0.0f,315.4f);
			AddBush_2(-28.4f,0.0f,263.4f);
			AddBush_1(-63.2f,0.0f,254.0f);
			AddBush_2(-105.4f,0.0f,236.8f);
			AddBush_1(-127.9f,0.0f,212.3f);
			AddBush_2(-61.5f,0.0f,158.7f);
			AddBush_1(-74.2f,0.0f,98.4f);
			AddBush_2(-26.0f,0.0f,77.0f);
			AddBush_1(19.8f,0.0f,73.2f);
			AddBush_2(24.0f,0.0f,12.9f);
			AddBush_1(24.4f,0.0f,-48.6f);
			AddBush_2(-6.5f,0.0f,-74.2f);
			AddBush_1(-6.4f,0.0f,-107.4f);
			AddBush_2(27.1f,0.0f,-154.2f);
			AddBush_1(-5.8f,0.0f,-185.5f);
			AddBush_2(-44.4f,0.0f,-158.3f);
			AddBush_1(-37.7f,0.0f,-81.8f);
			AddBush_2(-40.2f,0.0f,-47.3f);
			AddBush_1(-103.7f,0.0f,-20.9f);
			AddBush_2(-142.0f,0.0f,42.3f);
			AddBush_1(-143.7f,0.0f,124.0f);
			AddBush_2(-128.4f,0.0f,-351.4f);
			AddBush_1(-139.6f,0.0f,-383.3f);
			AddBush_2(-27.3f,0.0f,-329.9f);
			AddBush_1(10.5f,0.0f,-336.2f);
			AddBush_2(19.3f,0.0f,-393.8f);
			AddBush_2(47.9f,0.0f,-423.5f);
			AddBush_1(101.6f,0.0f,-450.7f);
			AddBush_1(131.9f,0.0f,-436.1f);
			AddBush_2(160.1f,0.0f,-340.8f);
			AddBush_2(217.0f,0.0f,-348.2f);
			AddBush_1(260.7f,0.0f,-405.7f);
			AddBush_2(385.1f,0.0f,-399.9f);
			AddBush_1(369.2f,0.0f,-320.9f);
			AddBush_2(450.3f,0.0f,-297.4f);
			AddBush_2(405.2f,0.0f,-213.4f);
			AddBush_1(303.7f,0.0f,-174.8f);
			AddBush_1(285.6f,0.0f,-232.2f);
			AddBush_2(241.2f,0.0f,-241.6f);
			AddBush_1(229.2f,0.0f,-192.1f);
			AddBush_1(249.0f,0.0f,-157.0f);
			AddBush_2(175.0f,0.0f,-125.3f);
			AddBush_1(112.1f,0.0f,-154.1f);
			AddBush_2(125.7f,0.0f,-91.2f);
			AddBush_1(150.6f,0.0f,-44.1f);
			AddBush_1(187.1f,0.0f,11.2f);
			AddBush_1(197.5f,0.0f,-64.6f);
			AddBush_2(316.9f,0.0f,-0.9f);
			AddBush_1(390.6f,0.0f,-2.6f);
			AddBush_1(398.6f,0.0f,56.0f);
			AddBush_2(324.6f,0.0f,124.3f);
			AddBush_1(378.4f,0.0f,218.4f);
			AddBush_1(307.7f,0.0f,224.4f);
			AddBush_2(357.7f,0.0f,80.1f);


			AddBush_2(-341.2f,0.0f,-140.8f);
			AddBush_1(-312.4f,0.0f,-233.9f);
			AddBush_2(-316.5f,0.0f,-124.9f);
			AddBush_1(-297.1f,0.0f,-73.9f);
			AddBush_2(-343.6f,0.0f,-36.6f);
			AddBush_1(-312.9f,0.0f,22.6f);
			AddBush_2(-263.8f,0.0f,-11.5f);
			AddBush_1(-416.0f,0.0f,79.2f);
			AddBush_2(-398.3f,0.0f,-22.3f);
			AddBush_1(-378.9f,0.0f,-125.3f);
			AddBush_2(-374.3f,0.0f,-184.8f);
			AddBush_2(-276.6f,0.0f,86.4f);
			AddBush_1(-356.1f,0.0f,169.3f);
			AddBush_1(-375.3f,0.0f,259.3f);
			AddBush_1(-389.0f,0.0f,293.1f);
			AddBush_2(-355.4f,0.0f,305.9f);
			AddBush_2(-306.6f,0.0f,233.9f);
			AddBush_2(-267.3f,0.0f,221.2f);
			AddBush_1(-245.2f,0.0f,246.7f);
			AddBush_2(-251.7f,0.0f,335.8f);
			AddBush_2(-156.4f,0.0f,306.9f);
			AddBush_1(-135.7f,0.0f,356.5f);
			AddBush_2(-72.3f,0.0f,409.2f);
			AddBush_2(-30.4f,0.0f,390.8f);
			AddBush_1(-35.5f,0.0f,325.1f);
			AddBush_2(4.2f,0.0f,297.5f);
			AddBush_2(11.9f,0.0f,218.0f);
			AddBush_1(-29.6f,0.0f,214.6f);
			AddBush_2(-105.2f,0.0f,199.5f);
			AddBush_1(-137.7f,0.0f,165.9f);
			AddBush_2(-112.2f,0.0f,91.3f);
			AddBush_1(-48.3f,0.0f,64.4f);
			AddBush_2(35.6f,0.0f,124.6f);
			AddBush_1(72.4f,0.0f,185.0f);
			AddBush_2(-5.2f,0.0f,200.3f);
			AddBush_1(112.0f,0.0f,406.1f);
			AddBush_2(154.6f,0.0f,407.8f);
			AddBush_1(200.0f,0.0f,408.9f);
			AddBush_1(217.2f,0.0f,382.7f);
			AddBush_1(190.2f,0.0f,377.0f);
			AddBush_2(146.8f,0.0f,368.5f);
			AddBush_2(116.0f,0.0f,368.1f);
			AddBush_2(145.1f,0.0f,340.1f);
			AddBush_2(189.6f,0.0f,325.8f);
			AddBush_1(188.8f,0.0f,272.2f);
			AddBush_1(163.1f,0.0f,275.6f);
			AddBush_1(135.1f,0.0f,273.4f);
			AddBush_1(117.4f,0.0f,251.1f);
			AddBush_1(122.2f,0.0f,215.6f);
			AddBush_2(147.9f,0.0f,203.8f);
			AddBush_2(180.5f,0.0f,194.7f);
			AddBush_2(215.3f,0.0f,167.7f);
			AddBush_2(187.0f,0.0f,136.8f);
			AddBush_2(155.7f,0.0f,128.5f);
			AddBush_1(124.3f,0.0f,146.2f);
			AddBush_1(166.5f,0.0f,99.0f);
			AddBush_2(186.4f,0.0f,88.9f);
			AddBush_1(216.3f,0.0f,112.2f);
			AddBush_2(30.4f,0.0f,-11.0f);
			AddBush_1(-20.5f,0.0f,-145.7f);
			AddBush_2(-34.4f,0.0f,-217.2f);
			AddBush_1(-22.1f,0.0f,-246.4f);
			AddBush_2(13.6f,0.0f,-249.7f);
			AddBush_1(43.2f,0.0f,-205.2f);
			AddBush_2(-89.7f,0.0f,-144.1f);
			AddBush_1(-113.6f,0.0f,-110.2f);
			AddBush_2(-122.3f,0.0f,-60.6f);
			AddBush_2(-126.5f,0.0f,-251.0f);
			AddBush_2(-101.9f,0.0f,-350.3f);
			AddBush_2(-125.5f,0.0f,-381.6f);
			AddBush_1(-59.2f,0.0f,-405.0f);
			AddBush_1(9.5f,0.0f,-413.1f);
			AddBush_2(85.9f,0.0f,-400.0f);
			AddBush_2(138.8f,0.0f,-366.3f);
			AddBush_1(207.9f,0.0f,-420.7f);
			AddBush_2(287.5f,0.0f,-418.4f);
			AddBush_1(358.8f,0.0f,-382.0f);
			AddBush_2(411.1f,0.0f,-330.0f);
			AddBush_1(459.4f,0.0f,-248.3f);
			AddBush_1(382.3f,0.0f,-241.7f);
			AddBush_2(346.0f,0.0f,-173.2f);
			AddBush_2(271.9f,0.0f,-219.5f);
			AddBush_1(176.6f,0.0f,-229.0f);
			AddBush_1(136.8f,0.0f,-177.5f);
			AddBush_1(148.7f,0.0f,-92.5f);
			AddBush_2(191.0f,0.0f,-46.7f);
			AddBush_2(229.5f,0.0f,-34.7f);
			AddBush_1(276.2f,0.0f,3.5f);
			AddBush_2(340.8f,0.0f,6.0f);
			AddBush_1(353.6f,0.0f,34.5f);
			AddBush_2(427.3f,0.0f,116.0f);
			AddBush_1(361.7f,0.0f,149.5f);
			AddBush_2(328.5f,0.0f,184.1f);
			AddBush_1(342.0f,0.0f,226.6f);
			AddBush_2(337.3f,0.0f,259.2f);
			AddBush_1(433.5f,0.0f,274.9f);
			AddBush_2(418.2f,0.0f,292.8f);
			AddBush_1(404.2f,0.0f,350.8f);
			AddBush_2(385.6f,0.0f,381.1f);
			AddBush_1(373.9f,0.0f,381.9f);
			AddBush_2(378.3f,0.0f,342.8f);
			AddBush_2(327.9f,0.0f,428.1f);
			AddBush_1(374.8f,0.0f,456.3f);

			AddBush_1(-329.4f,0.0f,-178.0f);
			AddBush_2(-321.1f,0.0f,-77.9f);
			AddBush_2(-325.0f,0.0f,-9.7f);
			AddBush_2(-371.2f,0.0f,52.2f);
			AddBush_2(-330.6f,0.0f,239.9f);
			AddBush_1(-227.3f,0.0f,339.7f);
			AddBush_2(-119.1f,0.0f,316.0f);
			AddBush_1(-57.2f,0.0f,300.5f);
			AddBush_2(-79.7f,0.0f,184.1f);
			AddBush_1(-44.7f,0.0f,126.5f);
			AddBush_2(-126.9f,0.0f,21.4f);
			AddBush_1(-75.2f,0.0f,-70.9f);
			AddBush_2(-78.9f,0.0f,-167.9f);
			AddBush_1(-90.1f,0.0f,-200.7f);
			AddBush_2(-57.8f,0.0f,-222.0f);
			AddBush_1(-34.7f,0.0f,-251.5f);
			AddBush_2(23.3f,0.0f,-191.6f);
			AddBush_2(-75.1f,0.0f,-357.4f);
			AddBush_2(-16.0f,0.0f,-369.4f);
			AddBush_1(25.1f,0.0f,-360.9f);     
*/
}

/*
class Scene
{
public:
    Scene();
    ~Scene();
    bool LoadScene(); 
    void Render();

    Material*    mMaterialList;
    Shaders*     mShaderList;
    Textures*    mTextureList;
    Light*       mLightList; 
    unsigned int mMaterialCount;
    unsigned int mTextureCount;
    unsigned int mLightCount;
    unsigned int mShaderCount;
};
*/
#endif

