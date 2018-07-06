/*
 main

 Copyright 2012 Thomas Dalling - http://tomdalling.com/

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

 // third-party libraries
#include "glew.h"
#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

 // standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <list>

 // tdogl classes
#include "Program.h"
#include "Texture.h"
#include "Camera.h"

// application classes
#include "ModelAsset.h"
#include "ModelInstance.h"
#include "Light.h"
#include "Sphere.h"
#include "Capsule.h"
#include "Quad.h"
#include "cube.h"
#include "bumpcube.h"
#include "Object.h"
#include "collision.h"
#include "Projectile.h"
#include "fontplot.h"
#include "objloader.h"
#include "skybox.h"
// constants
const glm::vec2 SCREEN_SIZE(960, 540);

// globals
GLFWwindow* gWindow = NULL;
tdogl::Camera gCamera;
std::list<Object*> gObjectList;
Light gLight[4];
float gScrollY = 0.0f;
ModelAsset *g_sphere;
ModelAsset *g_capsule;
ModelAsset *g_smallSphere;
ModelAsset *g_cube;
ModelAsset *g_ogre;
FontPlot *g_fontPlot;
SkyBox *g_skyBox;
int score = 0;

// convenience function that returns a translation matrix
glm::mat4 translate(GLfloat x, GLfloat y, GLfloat z) 
{
	return glm::translate(glm::mat4(), glm::vec3(x, y, z));
}


// convenience function that returns a scaling matrix
glm::mat4 scale(GLfloat x, GLfloat y, GLfloat z) 
{
	return glm::scale(glm::mat4(), glm::vec3(x, y, z));
}


// convenience function that returns a rotation matrix
glm::mat4 rotate(GLfloat degrees, GLfloat x, GLfloat y, GLfloat z)
{
	return glm::rotate(glm::mat4(), degrees, glm::vec3(x, y, z));
}


static void AddFloor()
{
	ModelAsset *pFloorAsset = new ModelAsset();
	pFloorAsset->LoadShaders("GL/shaders/vertex-shader.glsl", "GL/shaders/fragment-shader-4light.glsl");
	pFloorAsset->LoadTexture("GL/textures/sci-fi-floor.png");
	GLfloat vertexData[] =
	{
	-100.0f, 0.0, -100.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-100.0f, 0.0, 100.0f, 0.0f, 20.0f, 0.0f, 1.0f, 0.0f,
		100.0f, 0.0, 100.0f, 20.0f, 20.0f, 0.0f, 1.0f, 0.0f,
		-100.0f, 0.0, -100.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		100.0f, 0.0, 100.0f, 20.0f, 20.0f, 0.0f, 1.0f, 0.0f,
		100.0f, 0.0, -100.0f, 20.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	};
	pFloorAsset->SetVertexData(vertexData, 6, 8);
	Object *floorObj = new Object(translate(0, 0, 0));
	floorObj->AddComponent(new ModelInstance(*pFloorAsset));
	floorObj->AddCollision(new PlaneColl(vec3(0, 0, 0), vec3(0, 1, 0)));
	gObjectList.push_back(floorObj);
}


void AddWalls()
{
	Object *wall0 = new Object(translate(0, 0, 0));
	wall0->SetTransform(translate(15, 0, 0) * scale(1, 8, 10));
	wall0->AddComponent(new ModelInstance(*g_cube));
	wall0->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 20.0f, 2.0f, 16.0f));
	gObjectList.push_back(wall0);

	Object *wall1 = new Object(translate(0, 0, 0));
	wall1->SetTransform(translate(-15, 0, 5) * scale(1, 8, 16));
	wall1->AddComponent(new ModelInstance(*g_cube));
	wall1->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 32.0f, 2.0f, 16.0f));
	gObjectList.push_back(wall1);

	Object *wall2 = new Object(translate(0, 0, 0));
	wall2->SetTransform(translate(0, 0, 21) * scale(30, 8, 1));
	wall2->AddComponent(new ModelInstance(*g_cube));
	wall2->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 2.0f, 60.0f, 16.0f));
	gObjectList.push_back(wall2);

	Object *wall3 = new Object(translate(0, 0, 0));
	wall3->SetTransform(translate(0, 0, -11) * scale(16, 8, 1));
	wall3->AddComponent(new ModelInstance(*g_cube));
	wall3->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 2.0f, 32.0f, 16.0f));
	gObjectList.push_back(wall3);

	Object *wall4 = new Object(translate(0, 0, 0));
	wall4->SetTransform(translate(25, 0, -20) * scale(1, 8, 40));
	wall4->AddComponent(new ModelInstance(*g_cube));
	wall4->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 80.0f, 2.0f, 16.0f));
	gObjectList.push_back(wall4);

	Object *wall5 = new Object(translate(0, 0, 0));
	wall5->SetTransform(translate(-8, 0, -23) * scale(23, 8, 1));
	wall5->AddComponent(new ModelInstance(*g_cube));
	wall5->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 2.0f, 46.0f, 16.0f));
	gObjectList.push_back(wall5);

	Object *wall6 = new Object(translate(0, 0, 0));
	wall6->SetTransform(translate(-30, 0, 8) * scale(1, 8, 30));
	wall6->AddComponent(new ModelInstance(*g_cube));
	wall6->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 60.0f, 2.0f, 16.0f));
	gObjectList.push_back(wall6);

	Object *wall7 = new Object(translate(0, 0, 0));
	wall7->SetTransform(translate(-22, 0, -35) * scale(40, 8, 1));
	wall7->AddComponent(new ModelInstance(*g_cube));
	wall7->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 2.0f, 80.0f, 16.0f));
	gObjectList.push_back(wall7);

	Object *wall8 = new Object(translate(0, 0, 0));
	wall8->SetTransform(translate(17, 0, -47) * scale(1, 8, 13));
	wall8->AddComponent(new ModelInstance(*g_cube));
	wall8->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 26.0f, 2.0f, 16.0f));
	gObjectList.push_back(wall8);

	Object *wall9 = new Object(translate(0, 0, 0));
	wall9->SetTransform(translate(21, 0, -61) * scale(5, 8, 1));
	wall9->AddComponent(new ModelInstance(*g_cube));
	wall9->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 2.0f, 10.0f, 16.0f));
	gObjectList.push_back(wall9);

	Object *wall10 = new Object(translate(0, 0, 0));
	wall10->SetTransform(translate(-40, 0, 0) * scale(1, 8, 35));
	wall10->AddComponent(new ModelInstance(*g_cube));
	wall10->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 75.0f, 2.0f, 16.0f));
	gObjectList.push_back(wall10);

	Object *wall11 = new Object(translate(0, 0, 0));
	wall11->SetTransform(translate(-28, 0, 46) * scale(29, 8, 1));
	wall11->AddComponent(new ModelInstance(*g_cube));
	wall11->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 2.0f, 58.0f, 16.0f));
	gObjectList.push_back(wall11);

	Object *wall12 = new Object(translate(0, 0, 0));
	wall12->SetTransform(translate(-55, 0, 5) * scale(1, 8, 40));
	wall12->AddComponent(new ModelInstance(*g_cube));
	wall12->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 80.0f, 2.0f, 16.0f));
	gObjectList.push_back(wall12);

	Object *wall13 = new Object(translate(0, 0, 0));
	wall13->SetTransform(translate(-20, 0, 36) * scale(1, 8, 9));
	wall13->AddComponent(new ModelInstance(*g_cube));
	wall13->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 18.0f, 2.0f, 16.0f));
	gObjectList.push_back(wall13);

	Object *wall14 = new Object(translate(0, 0, 0));
	wall14->SetTransform(translate(-10, 0, 30) * scale(1, 8, 9));
	wall14->AddComponent(new ModelInstance(*g_cube));
	wall14->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 18.0f, 2.0f, 16.0f));
	gObjectList.push_back(wall14);

	Object *wall15 = new Object(translate(0, 0, 0));
	wall15->SetTransform(translate(0, 0, 45) * scale(1, 8, 16));
	wall15->AddComponent(new ModelInstance(*g_cube));
	wall15->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 18.0f, 2.0f, 16.0f));
	gObjectList.push_back(wall15);

	Object *wall16 = new Object(translate(0, 0, 0));
	wall16->SetTransform(translate(30, 0, 40) * scale(1, 8, 20));
	wall16->AddComponent(new ModelInstance(*g_cube));
	wall16->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 40.0f, 2.0f, 16.0f));
	gObjectList.push_back(wall16);

	Object *wall17 = new Object(translate(0, 0, 0));
	wall17->SetTransform(translate(15, 0, 60) * scale(15, 8, 1));
	wall17->AddComponent(new ModelInstance(*g_cube));
	wall17->AddCollision(new CubeColl(vec3(0.0f, 0.0f, 0.0f), 2.0f, 30.0f, 16.0f));
	gObjectList.push_back(wall17);
}

void AddEnemies()
{
	// load an object
	g_ogre = ObjLoader::Load("GL/models/ogre/", "OgreOBJ.obj", "GL/shaders/vertex-shader.glsl", "GL/shaders/fragment-shader-4light.glsl");

	mat4 trans = glm::scale(glm::mat4(), glm::vec3(0.03, 0.03, 0.03));

	Object *enemy0 = new Object(trans);
	enemy0->AddComponent(new ModelInstance(*g_ogre));
	enemy0->SetTransform(translate(20, 0, 0));
	enemy0->AddCollision(new CapsuleColl(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), 2.0f));
	gObjectList.push_back(enemy0);

	Object *enemy1 = new Object(trans);
	enemy1->AddComponent(new ModelInstance(*g_ogre));
	enemy1->SetTransform(translate(10, 0, -29) * rotate(1.5708f, 0, 1, 0));
	enemy1->AddCollision(new CapsuleColl(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), 2.0f));
	gObjectList.push_back(enemy1);

	Object *enemy2 = new Object(trans);
	enemy2->AddComponent(new ModelInstance(*g_ogre));
	enemy2->SetTransform(translate(-35, 0, 0) * rotate(3.1415f, 0, 1, 0));
	enemy2->AddCollision(new CapsuleColl(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), 2.0f));
	gObjectList.push_back(enemy2);

	Object *enemy3 = new Object(trans);
	enemy3->AddComponent(new ModelInstance(*g_ogre));
	enemy3->SetTransform(translate(-45, 0, 40) * rotate(1.5708f, 0, 1, 0));
	enemy3->AddCollision(new CapsuleColl(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), 2.0f));
	gObjectList.push_back(enemy3);
}


// draws a single frame
static void Render() {
	// clear everything
	glClearColor(0, 0, 0, 1); // black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_skyBox->Render( gCamera );
	// render all the instances
	for (Object *pObj : gObjectList)
	{
		pObj->Render(gCamera, gLight, 4);
	}

	g_fontPlot->DrawString(64, 64, "Score:  " + std::to_string(score), vec4(0.0f, 1.0f, 0.0f, 1.0f));

	// swap the display buffers (displays what was just drawn)
	glfwSwapBuffers(gWindow);
}


void Shoot(const glm::vec3 &pos, const glm::vec3 &vel)
{
	Object *pObj = new Object(glm::translate(glm::mat4(), pos));
	ModelInstance *inst = new ModelInstance(*g_smallSphere);
	Projectile *proj = new Projectile(pos, vec3(vel.x, 0.0f, vel.z), glm::vec3(0.0f, -9.81f, 0.0f));
	pObj->AddComponent(inst);
	pObj->AddComponent(proj);
	pObj->AddCollision(new SphereColl(vec3(0.0f, 0.0f, 0.0f), 1.0f));
	gObjectList.push_back(pObj);
}


bool CollideObjects(Object *pObj0, Object *pObj1, vec3 &norm, float &depth)
{
	COLL_TYPE type0 = pObj0->GetCollision()->GetType();
	COLL_TYPE type1 = pObj1->GetCollision()->GetType();

	// arrange in ascending order of type, so we only need one case per pair
	if (type0 > type1)
	{
		Object *swap = pObj1;
		pObj1 = pObj0;
		pObj0 = swap;
		type0 = pObj0->GetCollision()->GetType();
		type1 = pObj1->GetCollision()->GetType();
	}

	bool collision = false;

	if (type0 == CTYPE_SPHERE && type1 == CTYPE_SPHERE)
	{
		vec3 r0, r1;
		float rad0, rad1;
		r0 = ((SphereColl*)(pObj0->GetCollision()))->GetPos(pObj0->GetTransform());
		r1 = ((SphereColl*)(pObj1->GetCollision()))->GetPos(pObj1->GetTransform());
		rad0 = ((SphereColl*)(pObj0->GetCollision()))->GetRadius();
		rad1 = ((SphereColl*)(pObj1->GetCollision()))->GetRadius();

		collision = Collision::SphereSphereCollision(r0, rad0, r1, rad1, norm, depth);
	}
	else if (type0 == CTYPE_SPHERE && type1 == CTYPE_CAPSULE)
	{
		vec3 r0, r1, rsph;
		float rad0, rad1;
		r0 = ((CapsuleColl*)(pObj1->GetCollision()))->GetR0(pObj1->GetTransform());
		r1 = ((CapsuleColl*)(pObj1->GetCollision()))->GetR1(pObj1->GetTransform());
		rsph = ((SphereColl*)(pObj0->GetCollision()))->GetPos(pObj0->GetTransform());
		rad1 = ((CapsuleColl*)(pObj1->GetCollision()))->GetRadius();
		rad0 = ((SphereColl*)(pObj0->GetCollision()))->GetRadius();

		collision = Collision::SphereCapsuleCollision(rsph, rad0, r0 ,r1, rad1, norm, depth);

	}
	else if (type0 == CTYPE_SPHERE && type1 == CTYPE_PLANE)
	{
		vec3 np, rp, rsph;
		float rad;

		rsph = ((SphereColl*)(pObj0->GetCollision()))->GetPos(pObj0->GetTransform());
		rad = ((SphereColl*)(pObj0->GetCollision()))->GetRadius();
		rp = ((PlaneColl*)(pObj1->GetCollision()))->GetPos(pObj1->GetTransform());
		np = ((PlaneColl*)(pObj1->GetCollision()))->GetNormal(pObj1->GetTransform());
		collision = Collision::SpherePlaneCollision(rsph, rad, rp, np, norm, depth);
	}
	else if (type0 == CTYPE_SPHERE && type1 == CTYPE_CUBE)
	{
		vec3 cMin, cMax, centreSphere;
		float rad;

		centreSphere = ((SphereColl*)(pObj0->GetCollision()))->GetPos(pObj0->GetTransform());
		rad = ((SphereColl*)(pObj0->GetCollision()))->GetRadius();
				
		cMin.x = (((CubeColl*)(pObj1->GetCollision()))->getCentre(pObj1->GetTransform()).x - (((CubeColl*)(pObj1->GetCollision()))->getWidth() / 2));
		cMin.y = (((CubeColl*)(pObj1->GetCollision()))->getCentre(pObj1->GetTransform()).y - (((CubeColl*)(pObj1->GetCollision()))->getHeight()));
		cMin.z = (((CubeColl*)(pObj1->GetCollision()))->getCentre(pObj1->GetTransform()).z - (((CubeColl*)(pObj1->GetCollision()))->getLength() / 2));

		cMax.x = (((CubeColl*)(pObj1->GetCollision()))->getCentre(pObj1->GetTransform()).x + (((CubeColl*)(pObj1->GetCollision()))->getWidth() / 2));
		cMax.y = (((CubeColl*)(pObj1->GetCollision()))->getCentre(pObj1->GetTransform()).y + (((CubeColl*)(pObj1->GetCollision()))->getHeight()));
		cMax.z = (((CubeColl*)(pObj1->GetCollision()))->getCentre(pObj1->GetTransform()).z + (((CubeColl*)(pObj1->GetCollision()))->getLength() / 2));

		collision = Collision::SphereCubeCollision(cMin, cMax, centreSphere, rad, norm, depth);
	}

	return collision;
}


// update the scene based on the time elapsed since last update
static void Update(float secondsElapsed) {

	//move position of camera based on WASD keys
	const float moveSpeed = 20.0f;
	const float panSpeed = 50.0;
	glm::vec3 floorForward;
	glm::vec3 floorRight;

	floorForward = gCamera.forward(); floorForward.y = 0.0f; floorForward = glm::normalize(floorForward);
	floorRight = gCamera.right(); floorRight.y = 0.0f; floorRight = glm::normalize(floorRight);

	// camera movement
	vec3 oldPos, newPos;
	oldPos = gCamera.position();
	newPos = oldPos;

	if (glfwGetKey(gWindow, 'S'))
		newPos += secondsElapsed * moveSpeed * -floorForward;
	else if (glfwGetKey(gWindow, 'W'))
		newPos += secondsElapsed * moveSpeed * floorForward;
	if (glfwGetKey(gWindow, 'A'))
		newPos += secondsElapsed * moveSpeed * -floorRight;
	else if (glfwGetKey(gWindow, 'D'))
		newPos += secondsElapsed * moveSpeed * floorRight;
	// camera collision
	bool cameraCollision = false;
	float camRadius = 2.0f;
	vec3 collNorm;
	float collDepth;

	for (auto obj : gObjectList)
	{
		if (obj->GetCollision() != nullptr)
		{
			COLL_TYPE type = obj->GetCollision()->GetType();
			if (type == CTYPE_SPHERE)
			{
				vec3 rSph = ((SphereColl*)(obj->GetCollision()))->GetPos(obj->GetTransform());
				float rad = ((SphereColl*)(obj->GetCollision()))->GetRadius();
				if (Collision::SphereSphereCollision(rSph, rad, newPos, camRadius, collNorm, collDepth))
				{
					cameraCollision = true;
					break;
				}
			}
			else if (type == CTYPE_CAPSULE)
			{
				vec3 r0 = ((CapsuleColl*)(obj->GetCollision()))->GetR0(obj->GetTransform());
				vec3 r1 = ((CapsuleColl*)(obj->GetCollision()))->GetR1(obj->GetTransform());
				float rad = ((CapsuleColl*)(obj->GetCollision()))->GetRadius();
				if (Collision::SphereCapsuleCollision(newPos, camRadius, r0, r1, rad, collNorm, collDepth))
				{
					cameraCollision = true;
					break;
				}
			}
			else if (type == CTYPE_PLANE)
			{
				vec3 rp = ((PlaneColl*)(obj->GetCollision()))->GetPos(obj->GetTransform());
				vec3 np = ((PlaneColl*)(obj->GetCollision()))->GetNormal(obj->GetTransform());
				if (Collision::SpherePlaneCollision(newPos, camRadius, rp, np, collNorm, collDepth))
				{
					cameraCollision = true;
					break;
				}

			}
			else if (type == CTYPE_CUBE)
			{
				vec3 cMin, cMax;

				cMin.x = (((CubeColl*)(obj->GetCollision()))->getCentre(obj->GetTransform()).x - (((CubeColl*)(obj->GetCollision()))->getWidth() / 2));
				cMin.y = (((CubeColl*)(obj->GetCollision()))->getCentre(obj->GetTransform()).y - (((CubeColl*)(obj->GetCollision()))->getHeight()));
				cMin.z = (((CubeColl*)(obj->GetCollision()))->getCentre(obj->GetTransform()).z - (((CubeColl*)(obj->GetCollision()))->getLength() / 2));

				cMax.x = (((CubeColl*)(obj->GetCollision()))->getCentre(obj->GetTransform()).x + (((CubeColl*)(obj->GetCollision()))->getWidth() / 2));
				cMax.y = (((CubeColl*)(obj->GetCollision()))->getCentre(obj->GetTransform()).y + (((CubeColl*)(obj->GetCollision()))->getHeight()));
				cMax.z = (((CubeColl*)(obj->GetCollision()))->getCentre(obj->GetTransform()).z + (((CubeColl*)(obj->GetCollision()))->getLength() / 2));

				if (Collision::SphereCubeCollision(cMin, cMax, newPos, camRadius, collNorm, collDepth))
				{
					cameraCollision = true;
					break;
				}
			}
		}
	}
	if (cameraCollision)
	{
		newPos += collNorm * collDepth;
		newPos.y = oldPos.y;
	}
	gCamera.setPosition(newPos);

	float xOffs = 0.0f;
	float yOffs = 0.0f;
	static bool shooting = false;

	if (glfwGetKey(gWindow, GLFW_KEY_LEFT))
		yOffs -= panSpeed * secondsElapsed;
	if (glfwGetKey(gWindow, GLFW_KEY_RIGHT))
		yOffs += panSpeed * secondsElapsed;
	if (glfwGetKey(gWindow, GLFW_KEY_UP))
		xOffs -= panSpeed * secondsElapsed;
	if (glfwGetKey(gWindow, GLFW_KEY_DOWN))
		xOffs += panSpeed * secondsElapsed;
	if (glfwGetKey(gWindow, GLFW_KEY_SPACE))
	{
		if (!shooting)
		{
			Shoot(gCamera.position()+gCamera.forward()*2.5f, gCamera.forward() * 30.0f);
			shooting = true;
		}
	}
	else
		shooting = false;

	gCamera.offsetOrientation(xOffs, yOffs);

	// call update on all instances
	auto iter = gObjectList.begin();

	while (iter != gObjectList.end())
	{
		Object *pObj = *iter;
		pObj->Update(secondsElapsed);

		if (pObj->GetPosition().y < -10.0)
		{
			iter = gObjectList.erase(iter);
			delete pObj;
			score++;
		}
		else
			iter++;
	}

	// collision - loop over all pairs of objects. 
	// delete any objects which are intersecting
	auto iter1 = gObjectList.begin(); // start at the beginning
	while (iter1 != gObjectList.end())
	{
		// iterate over the remainder of the list
		auto iter2 = std::next(iter1);
		bool collision = false;
		while (iter2 != gObjectList.end()) 
		{
			// do both objects have collision?
			if ((*iter1)->GetCollision() && (*iter2)->GetCollision())
			{
				vec3 norm;
				float depth;
				if (CollideObjects(*iter1, *iter2, norm, depth))
				{
					COLL_TYPE cT0, cT1;
					
					cT0 = (COLL_TYPE)((*iter1)->GetCollision()->GetType());
					cT1 = (COLL_TYPE)((*iter2)->GetCollision()->GetType());

					// we have a collision - get the velocities and positions
					Projectile *p0, *p1;
					vec3 vel0(0, 0, 0), vel1(0, 0, 0);
					vec3 pos0(0, 0, 0), pos1(0, 0, 0);

					p0 = (Projectile*)((*iter1)->GetComponentByName("Projectile"));
					p1 = (Projectile*)((*iter2)->GetComponentByName("Projectile"));

					if (p0 != nullptr)
					{
						vel0 = p0->GetVelocity();
						pos0 = p0->GetPosition();
					}
					if (p1 != nullptr)
					{
						vel1 = p1->GetVelocity();
						pos1 = p1->GetPosition();
					}

					// quickly check if our collision is with the floor or with a cube, if it is continue else delete the objects
					if (pos0.y >= 1 || pos1.y >= 1 && cT0 != CTYPE_CUBE && cT1 != CTYPE_CUBE)
					{
						delete *iter2;
						gObjectList.erase(iter2);
						collision = true;
						break;
					}

					// collision normal goes from 0 to 1. Check dot product
					float cr = 0.9f;

					if ( dot(norm,vel1-vel0) < 0.0f )
					{ 
						if (!p0 && p1)
						{
							vel1 = vel1 - (1.0f+cr)*norm*(dot(vel1,norm));
							p1->SetVelocity(vel1);
						}
						else if (p0 && p1)
						{
							vec3 deltaVel = norm*(dot(norm, vel1 - vel0)) * (1.0f + cr) * 0.5f;
							vel1 -= deltaVel;
							vel0 += deltaVel;
							p1->SetVelocity(vel1);
							p0->SetVelocity(vel0);

						}
					}
				}
			}
			iter2++;
		}

		if (collision)
		{
			delete *iter1;
			iter1 = gObjectList.erase(iter1);
			score++;
			g_fontPlot->DrawString(64, 64, "Score:  " + std::to_string(score), vec4(0.0f, 1.0f, 0.0f, 1.0f));
		}
		else
		{
			iter1++;
		}
	}
}


// records how far the y axis has been scrolled
void OnScroll(GLFWwindow* window, double deltaX, double deltaY) {
    gScrollY += deltaY;
}


void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}


// the program starts here
void AppMain() {
    // initialise GLFW
    glfwSetErrorCallback(OnError);
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");

    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "OpenGL Tutorial", NULL, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");

    // GLFW settings
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, 0, 0);
    glfwSetScrollCallback(gWindow, OnScroll);
    glfwMakeContextCurrent(gWindow);

    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // GLEW throws some errors, so discard all the errors so far
    while(glGetError() != GL_NO_ERROR) {}

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_3)
        throw std::runtime_error("OpenGL 3.3 API is not available.");

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// add a skybox
	std::string cubeMaps[] =
	{
		std::string("GL\\textures\\cubemaps\\top.png"),
		std::string("GL\\textures\\cubemaps\\bottom.png"),
		std::string("GL\\textures\\cubemaps\\front.png"),
		std::string("GL\\textures\\cubemaps\\back.png"),
		std::string("GL\\textures\\cubemaps\\left.png"),
		std::string("GL\\textures\\cubemaps\\right.png"),
	};
	g_skyBox = new SkyBox(cubeMaps, "\GL\\shaders\\vertex-skybox.glsl", "GL\\shaders\\fragment-skybox.glsl");

	// Create assets and populate instance list
	AddFloor();
	// small sphere for projectile
	g_smallSphere = Sphere::Generate(0.2f, "GL/textures/purple.png", "GL/shaders/vertex-shader.glsl", "GL/shaders/fragment-shader-4light.glsl", 20);
	// large sphere for static objects
	g_sphere = Sphere::Generate(1.0f, "GL/textures/grid.jpg", "GL/shaders/vertex-shader.glsl", "GL/shaders/fragment-shader-4light.glsl", 20);
	g_capsule = Capsule::Generate( 2.0f, 4.0f, "GL/textures/grid.jpg", "GL/shaders/vertex-shader.glsl", "GL/shaders/fragment-shader-4light.glsl", 20);
	g_cube = BumpCube::Generate(2.0f, 1.0f, "GL/textures/sci-fi-wall.png", "GL/textures/sci-fi-wall-n.png", "GL/shaders/vertex-shader-bump.glsl", "GL/shaders/fragment-shader-4light-bump.glsl");

	AddWalls();

	AddEnemies();

	// create font plotter
	g_fontPlot = new FontPlot("GL/Textures/lucida32", "GL/shaders/vertex-fontplot.glsl", "GL/shaders/fragment-fontplot.glsl", SCREEN_SIZE.x, SCREEN_SIZE.y);

	// setup gCamera
    gCamera.setPosition(glm::vec3(0,4,0));
    gCamera.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);
    gCamera.setNearAndFarPlanes(0.5f, 500.0f);
	gCamera.setFieldOfView(70.0f);

	// setup gLights
	gLight[0].position = glm::vec3(-20, 10, 20);
	gLight[0].intensities = glm::vec3(1, 1, 1); //white
	gLight[0].attenuation = 0.005f;
	gLight[0].ambientCoefficient = 0.01f;

	gLight[1].position = glm::vec3(20, 10, 20);
	gLight[1].intensities = glm::vec3(1, 0, 0); //red
	gLight[1].attenuation = 0.005f;
	gLight[1].ambientCoefficient = 0.01f;

	gLight[2].position = glm::vec3(-20, 10, -20);
	gLight[2].intensities = glm::vec3(0, 1, 0); //green
	gLight[2].attenuation = 0.005f;
	gLight[2].ambientCoefficient = 0.01f;

	gLight[3].position = glm::vec3(20, 10, -20);
	gLight[3].intensities = glm::vec3(0, 0, 1); //blue
	gLight[3].attenuation = 0.005f;
	gLight[3].ambientCoefficient = 0.01f;

    // run while the window is open
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();

        // update the scene based on the time elapsed since last update
        double thisTime = glfwGetTime();
        Update((float)(thisTime - lastTime));
        lastTime = thisTime;

        // draw one frame
        Render();

        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << std::endl;

        //exit program if escape key is pressed
        if(glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(gWindow, GL_TRUE);
    }

    // clean up and exit
    glfwTerminate();
}


int main(int argc, char *argv[]) {
    try {
        AppMain();
    } catch (const std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

