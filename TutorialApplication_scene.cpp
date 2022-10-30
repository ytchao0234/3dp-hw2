//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2022/10/05
//
//
// Purpose: Set up the scenes. 
//
//
//
#include "TutorialApplication.h"
#include "BasicTools.h"
#include "SystemParameter.h"
#include "reader_data.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <math.h>

using namespace Ogre;


void BasicTutorial_00::createScene_Setup_FX(void) {
	initParticleSystemForExplosion();
	Vector3 pos;
	pos = mSceneNodeArr[0]->getPosition();
	setOffParticleSystem(mParticleNode, "explosion", pos);


	mFXManager = new SpecialEffectManager(
		mSceneMgr,
		SystemParameter::maxNumParticleSystems);
	mFXManager->init();

	for (int i = 0;
		i < mFXManager->getNumParticleSystems();
		++i) 
	{
		if (i >= mNumofObjects) break;
		mFXManager->setOffParticleSystem(
			mSceneNodeArr[i]->getPosition()
		);
		if (i == 0) break;
	}
}

//
//
//
void BasicTutorial_00::createScene_Setup(void)
{
	PlaneBoundedVolumeList volList;
	mVolQuery = mSceneMgr->createPlaneBoundedVolumeQuery(volList);
	mRaySceneQuery = mSceneMgr->createRayQuery(Ray());

	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

	// ColourValue fadeColour(1.0, 1.0, 1.0);
	// mSceneMgr->setFog(FOG_LINEAR, fadeColour, 0, 1400, 1600);

	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");

	mSelectionRect = new SelectionRectangle("selectionRect");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mSelectionRect);
	
	mSelectionRect->setLightMask(0);
	mSelectionRect->setCastShadows(false);

	mPetCounter = new DIGIT_STRING_DIALOGUE(mSceneMgr);
	mMovingObjectsCounter = new DIGIT_STRING_DIALOGUE(mSceneMgr);

	mFlgSelectNow = false;
}

void BasicTutorial_00::createLights()
{
	Light* light = mSceneMgr->createLight("Light1");
	light->setType(Light::LT_POINT);
	light->setPosition(SystemParameter::radius * std::cos(0.0), 600, SystemParameter::radius * std::sin(0.0));
	light->setDiffuseColour(0.6, 0.6, 0.6);		
	light->setSpecularColour(1.0, 1.0, 1.0);	
	mLight_Point = light;
	mLightSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mLightSceneNode->attachObject(light);
}

void BasicTutorial_00::createPlaneObjectResource()
{
	//
	Plane floor(Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane(
		"floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		floor,
		800, 800, 	// width, height
		2, 2, 		// x- and y-segments
		true, 		// normal
		1, 		// num texture sets
		5, 5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
	);
	Plane ground(Vector3::UNIT_Y, -30);
	MeshManager::getSingleton().createPlane(
		"ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		ground,
		800, 800, 	// width, height
		2, 2, 		// x- and y-segments
		true, 		// normal
		1, 		// num texture sets
		5, 5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
	);
}

void BasicTutorial_00::createGround()
{
	Entity* ent = nullptr;
	ent = mSceneMgr->createEntity("Floor", "floor");
	//ent->setQueryFlags(0x0);
    ent->setMaterialName("Examples/Rocky");
    ent->setCastShadows(false);

	mEntity_Floor = ent;
	//
	mSceneNode_Floor = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( );

	mSceneNode_Floor->attachObject(ent);

	ent = mSceneMgr->createEntity("Ground", "ground");
	//ent->setQueryFlags(0x0);
    ent->setMaterialName("Examples/BeachStones");
    ent->setCastShadows(false);

	mEntity_Ground = ent;
	//
	mSceneNode_Ground = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( );

	mSceneNode_Ground->attachObject(ent);
	//
}

void BasicTutorial_00::createFloor()
{
}

void BasicTutorial_00::createObjectGroup(int start, int number, int radius)
{
	std::string petMeshName = READER_DATA::getMeshName_Pet();
	float scale = READER_DATA::getMeshScale_Pet();
	float yaw_angle_offset = READER_DATA::getYawAngleDegreeOffset_Pet();
    
	double x = 0, y = 20, z = 0, fx = 0;
	int i = start;

    for (int k = 0; k < number; k += 1, i +=1)
    {
        String entityName;
		genNameUsingIndex("Robot", i, entityName);
		mEntityArr[i] = mSceneMgr
			->createEntity(entityName, petMeshName);

        fx = k / (double) number;
        x = radius * cos(fx * PI * 2);
        z = radius * sin(fx * PI * 2);

		mAnimationStateArr[i] = mEntityArr[i]->getAnimationState("Idle");
		mMotionStateArr[i] = MOTION_TYPE_IDLE;

		mAnimationStateArr[i]->setEnabled(true);
		mAnimationStateArr[i]->setLoop(true);

		mSceneNodeArr[i] = mSceneMgr
			->getRootSceneNode()
			->createChildSceneNode();

		mSceneNodeArr[i]->attachObject(mEntityArr[i]);
		mSceneNodeArr[i]->setScale(scale, scale, scale);
		mSceneNodeArr[i]->setPosition(x, 0, z);
		mSceneNodeArr[i]->lookAt(Vector3(0, 0, 0), Node::TS_WORLD);
		mSceneNodeArr[i]->yaw(Degree(yaw_angle_offset));

		if (i == 0) {
			mSceneNodeArr[i]->setScale(2, 2, 2);
		}
    }
}

void BasicTutorial_00::createObjects()
{
	//Ogre::LogManager::getSingletonPtr()->logMessage("*** createObjects( ) ***");

	int numRobot1 = 30, radius1 = 300;
	int numRobot2 = 20, radius2 = 200;
	mNumofObjects = numRobot1 + numRobot2;

	createObjectGroup(        0, numRobot1, radius1);
	createObjectGroup(numRobot1, numRobot2, radius2);

	//
	mCurrentObject = mSceneNodeArr[0];
}
void BasicTutorial_00::createSphere()
{
	mSphere_Position = Vector3(0.0, 0.0, 0.0);
	SceneNode* sphere_scn = mSphere_Node = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	Entity* sphere_ent
		= mSceneMgr
		->createEntity("Sphere", "sphere.mesh");
	sphere_ent->setMaterialName("Examples/SphereMappedRustySteel");
	sphere_scn->attachObject(sphere_ent);
	auto bb = sphere_ent->getBoundingBox();
	Real sizeX = bb.getMaximum().x - bb.getMinimum().x;
	Real sf = mSphere_Radius / sizeX;
	sphere_scn->setScale(sf, sf, sf);
}

void BasicTutorial_00::createScene(void)
{
	createScene_Setup( );
	createLights( );
	createPlaneObjectResource();
	createGround();
	createFloor();
	createObjects();
	createSphere();
	createScene_Setup_FX();
}