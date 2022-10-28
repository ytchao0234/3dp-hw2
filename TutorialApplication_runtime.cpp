//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2022/10/05
//
//
// Purpose: Update the game state, 
// including object states, game progress, physics simulation, 
// collision detection, collision response, and etc.
//
//
// The functions will be invoked at the runtime stage.
//

#include "TutorialApplication.h"
#include "BasicTools.h"
#include "reader_data.h"
#include "SystemParameter.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <math.h>

//
// using namespace Ogre;
// Enable to use the functions/structures defined in the namespace Ogre.
//
using namespace Ogre;


//
// For an object pair, push the objects away from each other if they collide.
// objA, objB: object indices
// rA, rB: object radiuses
//
void BasicTutorial_00
::resolveCollisionObjectPair(
	int objA
	, int objB
	, float rA
	, float rB)
{

}

//
// Detect and resolve collisions of objects.
//
void BasicTutorial_00::resolveCollisionForObjects()
{
	float ri = 25; // object radius
	float rj = 25; // object radius
	for (int i = 0; i < mNumofObjects; ++i)
	{
		for (int j = i + 1; j < mNumofObjects; ++j) {
			resolveCollisionObjectPair(i, j, ri, rj);
		}
	}
}

void BasicTutorial_00::resolveCollisionBetweenObjectsAndSphere()
{

}
//
// Perform collision handling.
//
// Step 1. Resolve collision between objects.
// Step 2. Resolve collision between objects and the large sphere.
//
void BasicTutorial_00::performCollisionHandling(Real dt)
{
	// The collision response does not resolve all objects at one time.
	// Call resolveCollisionForObjects for multiple times.
	for (int i = 0; i < 10; ++i) {
		resolveCollisionForObjects();
	}
	resolveCollisionBetweenObjectsAndSphere();
}

//
// This function is correct.
// Create scene node, mParticleNode.
// Create one particle system.
// Attach the particle system to scene node, mParticleNode.
//
void BasicTutorial_00::initParticleSystemForExplosion( ) {
	mParticleNode = static_cast<SceneNode*>(
		mSceneMgr->getRootSceneNode()->createChild());

	// Create particle system with name "explosion".
	ParticleSystem* p = mSceneMgr->createParticleSystem(
		"explosion"			
		, "Examples/GreenyNimbus");

	ParticleEmitter* e = p->getEmitter(0);
	e->setEnabled(false);
	mParticleNode->attachObject(p);
	mParticleNode->setPosition(Vector3::ZERO);
	mParticleNode->setVisible(false);
}

//
// This function is correct.
// Enable the particle system which is attached to 
// scene node fNode.
// pname is the particle system name.
//
void BasicTutorial_00::setOffParticleSystem(
	Ogre::SceneNode* fNode
	, const Ogre::String& pname
	, const Ogre::Vector3& pos)
{
	fNode->setPosition(pos);
	fNode->setVisible(true);

	//Get the movable object with name pname.
	MovableObject* s = fNode->getAttachedObject(pname);	

	//
	//Convert the movable object, s, to a particle system.
	//Note that: in initParticleSystemForExplosion(...), 
	//a particle system is attached to the scene node fNode.
	//Thus, the conversion static_cast<ParticleSystem*>(s) makes sense.
	// 
	ParticleSystem* p = static_cast<ParticleSystem*>(s);
	if (p == NULL) return;
	p->setVisible(true);					// set it to be visible
	ParticleEmitter* e = p->getEmitter(0);	// get the emitter with index 0
	e->setEnabled(true);					// enable the emitter
}

void BasicTutorial_00::updateObjects_Positions(Real dt)
{
	mNumOfMovingObj = 0;
	if (mFlgTarget == false) return;

	for (int i = 0; i < mNumofObjects; ++i)
	{
		bool flgShow = mSceneNodeArr[i]->getShowBoundingBox();
		if (flgShow == false) continue;
		
		++mNumOfMovingObj;
		Vector3 direction = mTargetPosition - mSceneNodeArr[i]->getPosition();
		Real robotTargetDistance = direction.length();
		if (robotTargetDistance != 0.0)
			direction.normalise();
		Real walkSpeed = READER_DATA::getWalkSpeed();
		Real walkDistance = walkSpeed * dt;

		if (robotTargetDistance < 1.0) {
			mSceneNodeArr[i]->showBoundingBox(false);
			continue;
		}

		mObjectDisplacement[i] = direction * walkDistance;
		Vector3 robotLookAtPosition = mTargetPosition;
		robotLookAtPosition.y = mSceneNodeArr[i]->getPosition().y;
		mSceneNodeArr[i]->translate(mObjectDisplacement[i]);
		mSceneNodeArr[i]->lookAt(robotLookAtPosition, Node::TS_WORLD);
		mSceneNodeArr[i]->yaw(Degree(READER_DATA::getYawAngleDegreeOffset_Pet()));
	}
}

void BasicTutorial_00::updateObjects_Animation(Real dt)
{
	if (!mEnabledObjectAnimation) return;
	for (int i = 0; i < mNumofObjects; ++i) {


		bool flgShow = mSceneNodeArr[i]->getShowBoundingBox();
		if (flgShow == false) {
			if (mAnimationStateArr[i]) {
				mAnimationStateArr[i]->setEnabled(false);
				mAnimationStateArr[i]->setLoop(false);
			}
			mAnimationStateArr[i] = mEntityArr[i]->getAnimationState("Idle");
			mAnimationStateArr[i]->setEnabled(true);
			mAnimationStateArr[i]->setLoop(true);
		}
		else {
			if (mAnimationStateArr[i]) {
				mAnimationStateArr[i]->setEnabled(false);
				mAnimationStateArr[i]->setLoop(false);
			}
			mAnimationStateArr[i] = mEntityArr[i]->getAnimationState("Walk");
			mAnimationStateArr[i]->setEnabled(true);
			mAnimationStateArr[i]->setLoop(true);
		}
	}
	
	for (int i = 0; i < mNumofObjects; ++i) {
		mAnimationStateArr[i]->addTime(dt * 2);
	}
}

void BasicTutorial_00::checkObjects_ReachingTarget(Real dt)
{

}

//
// Update the objects.
//
void BasicTutorial_00::updateObjects(Real dt)
{
	updateObjects_Positions( dt );

	checkObjects_ReachingTarget( dt );

	updateObjects_Animation(dt);
}


//
// Update the particle systems
//
void BasicTutorial_00::updateParticleSystems()
{
	Vector3 pos;
	pos = mSceneNodeArr[0]->getPosition();
	setOffParticleSystem(mParticleNode, "explosion", pos);

	if (mFlg_EnableParticleSystem) {
		for (int i = 0; i < mNumofObjects; ++i)
		{
			mFXManager->setOffParticleSystem(
				i,
				mSceneNodeArr[i]->getPosition()
			);
		}
	}
}

//
// change the light position
//
void BasicTutorial_00::updateLight(Real dt) {
	if (!mEnabledLightAnimation) return;

	static Real angle = 0.0f;
	Vector3 position = mLightSceneNode->getPosition();
	angle += dt;
	angle = (angle > 2 * PI) ? angle - 2 * PI : angle;
	position.x = SystemParameter::radius * std::cos(angle);
	position.z = SystemParameter::radius * std::sin(angle);
	mLightSceneNode->setPosition(position);
}

//
// Show the information about the scene
// Example:
// The number of objects, selected objects, etc.
//
void BasicTutorial_00::updateObjectInformation() {
	
}

//
// frameStarted(...) is the main function 
// which updates the game logic/animation/life points/energy etc.
//
bool BasicTutorial_00::frameStarted(const FrameEvent& evt)
{
	Real dt = evt.timeSinceLastFrame;

	updateObjects( dt );
	performCollisionHandling( dt );
	updateParticleSystems();
	updateLight( dt );
	updateObjectInformation( );

	return BaseApplication::frameStarted(evt);
}

