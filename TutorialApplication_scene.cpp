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

	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	ColourValue fadeColour(1.0, 1.0, 1.0);
	mSceneMgr->setFog(FOG_LINEAR, fadeColour, 0, 1400, 1600);

	mSelectionRect = new SelectionRectangle("selectionRect");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mSelectionRect);
	
	//mSelectionRect->setLightMask(0);
	//mSelectionRect->setCastShadows(false);

	mPetCounter = new DIGIT_STRING_DIALOGUE(mSceneMgr);

	mFlgSelectNow = false;
}

void BasicTutorial_00::createLights()
{
	Light* light = mSceneMgr->createLight("Light1");
	light->setType(Light::LT_POINT);
	light->setPosition(Vector3(0, 0, 0));
	light->setDiffuseColour(0.0, 0.0, 0.0);		
	light->setSpecularColour(0.0, 0.0, 0.0);	
	mLight_Point = light;
	mLightSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mLightSceneNode->attachObject(light);
}

void BasicTutorial_00::createPlaneObjectResource()
{
	//
	Plane plane(Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane(
		"ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
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
	ent = mSceneMgr->createEntity(
		"GroundEntity_02", "ground");
	//ent->setQueryFlags(0x0);

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

void BasicTutorial_00::createObjects()
{
	//Ogre::LogManager::getSingletonPtr()->logMessage("*** createObjects( ) ***");
	int i;
	std::string petMeshName = READER_DATA::getMeshName_Pet();
	float scale = READER_DATA::getMeshScale_Pet();
	float yaw_angle_offset = READER_DATA::getYawAngleDegreeOffset_Pet();

	mNumofObjects = 0;
	int numRobots = 2;
	Real r = 200;
	for (int k = 0; k < numRobots; ++k, ++mNumofObjects)
	{

		i = k;
		String name;
		genNameUsingIndex("r", i, name);

		mEntityArr[i]
			= mSceneMgr
			->createEntity(name, petMeshName);
			//->createEntity(name, "robot.mesh");
		//->createEntity( name, "penguin.mesh" ); 

		//mEntityArr[i]->setVisibilityFlags(VIS_MASK_MINMAP);


		mAnimationStateArr[i] = mEntityArr[i]->getAnimationState("Idle");
		mMotionStateArr[i] = MOTION_TYPE_IDLE;

		mAnimationStateArr[i]->setEnabled(true);
		mAnimationStateArr[i]->setLoop(true);

		mSceneNodeArr[i]
			= mSceneMgr
			->getRootSceneNode()
			->createChildSceneNode();

		mSceneNodeArr[i]->attachObject(mEntityArr[i]);
		
		mSceneNodeArr[i]->setScale(scale, scale, scale);

		if (i == 0) {
			mSceneNodeArr[i]->setScale(5, 5, 5);
		}
	}



	//
	mCurrentObject = mSceneNodeArr[0];

}
void BasicTutorial_00::createSphere()
{
	mSphere_Position = Vector3(0.0, 0.0, 0.0);
	SceneNode* sphere_scn = mSphere_Node = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	Entity* sphere_ent
		= mSceneMgr
		->createEntity("s", "sphere.mesh");
	sphere_scn->attachObject(sphere_ent);
	sphere_scn->setScale(0.2, 0.2, 0.2);
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