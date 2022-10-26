//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2022/10/05
//
//
// The functions will be invoked at the runtime stage.
//
// Example: 
// Handle key events to control the camera.
// Handle key events to set viewports.
// Handle key events to obtain the camera information.
// Handle key events to activate the objects/items, e.g., spheres, robots, etc
// And others
// 
// A key handler handles simple tasks, 
// such as setting the states of game objects. 
//
//

#include "TutorialApplication.h"
#include "BasicTools.h"

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

bool BasicTutorial_00::mouseMoved(const OIS::MouseEvent& arg)
{
	Ogre::String ss = Ogre::StringConverter::toString(arg.state.Z.rel);

	Ogre::LogManager::getSingletonPtr()->logMessage(ss);

	if (arg.state.Z.rel != 0) {
		Vector3 p = mCamera->getPosition();

		Real dy = arg.state.Z.rel / 20.0;
		p.y -= dy;
		mCamera->setPosition(p);
	}

	if (mFlgSelectNow == true) {
		Ray mRay = mTrayMgr->getCursorRay(mCamera);

		Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
		left = scn.x;
		top = scn.y;

		Ogre::String ss;
		ss = Ogre::StringConverter::toString(left);
		Ogre::LogManager::getSingletonPtr()->logMessage("left:" + ss);

		ss = Ogre::StringConverter::toString(top);
		Ogre::LogManager::getSingletonPtr()->logMessage("top:" + ss);

		mSelectionRect->setCorners(left, top, right, bottom);
		mSelectionRect->setVisible(true);
	}
	return BaseApplication::mouseMoved(arg);

}

int BasicTutorial_00::singleClickSelect(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	int numSelectedObj = 0;	// number of selected objects

	Ray mRay = mTrayMgr->getCursorRay(mCamera);

	RaySceneQuery* mRaySceneQuery = mSceneMgr->createRayQuery(Ray());

	mRaySceneQuery->setSortByDistance(true);

	mRaySceneQuery->setRay(mRay);
	// Perform the scene query
	RaySceneQueryResult& result = mRaySceneQuery->execute();
	RaySceneQueryResult::iterator itr = result.begin();

	// Get the results, set the camera height
	// We are interested in the first intersection. It is ok to traverse all the results.

	mCurrentObject = mSceneNodeArr[0];
	if (mCurrentObject) {
		bool flgShow = mCurrentObject->getShowBoundingBox();
		mCurrentObject->showBoundingBox(!flgShow);
		numSelectedObj = 1;
	}

	return numSelectedObj;
}

int BasicTutorial_00::volumeSelection(
	const OIS::MouseEvent& arg,
	OIS::MouseButtonID id)
{
	int numSelectedObj = 0;
	
	//
	// correction for the left, right, top, bottom
	//
	if (left > right) {
		Real tmp = left;
		left = right;
		right = tmp;
	}
	/*
	if (bottom < top) {
		Real tmp = bottom;
		bottom = top;
		top = tmp;
	}
	*/

	//
	// Do you know the mechanism?
	// Need to convert from one coordinate system to another.
	// The followings are correct.
	Real nleft = left;
	Real nright = right;
	Real ntop = top + 1;
	Real nbottom = bottom + 1;

	//
	// Add your own stuff here
	//

	return numSelectedObj;

}

void BasicTutorial_00::computeTargetPosition()
{
	static bool flg = false;

	mFlgTarget = true;
	mTargetPosition = Vector3(400, 0, 0);
	if (flg) mTargetPosition = Vector3::ZERO;
	flg = !flg;
}


bool BasicTutorial_00::keyPressed(const OIS::KeyEvent& arg)
{
	if (arg.key == OIS::KC_B)
	{
		mEnabledObjectAnimation = !mEnabledObjectAnimation;

	}

	if (arg.key == OIS::KC_V)
	{
		mEnabledLightAnimation = !mEnabledLightAnimation;

	}

	if (arg.key == OIS::KC_SLASH)
	{
		// minimap
	}

	if (arg.key == OIS::KC_N) {
		mFlg_EnableParticleSystem = !mFlg_EnableParticleSystem;
		return true;
	}

	if (arg.key == OIS::KC_P) {
		mFlg_Floor_Motion = !mFlg_Floor_Motion;
		return true;
	}

	if (arg.key == OIS::KC_1) {
		return true;
	}

	if (arg.key == OIS::KC_2) {
		return true;
	}

	if (arg.key == OIS::KC_3) {
	}

	if (arg.key == OIS::KC_F1) {
		//if (mEntity_Floor) mEntity_Floor->setMaterialName("Examples/Rocky");

		return true;
	}

	if (arg.key == OIS::KC_F2) {
		//if (mEntity_Floor) mEntity_Floor->setMaterialName("Examples/Tens");

		return true;
	}

	if (arg.key == OIS::KC_F3) {
		//if (mEntity_Floor) mEntity_Floor->setMaterialName("Examples/WaterStream");

		return true;
	}

	if (arg.key == OIS::KC_9) {
	}

	if (arg.key == OIS::KC_0) {

	}

	if (arg.key == OIS::KC_7) {
	}

	if (arg.key == OIS::KC_8) {
	}




	return BaseApplication::keyPressed(arg);
}

bool BasicTutorial_00::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right) {
		computeTargetPosition();
		mFlgAllObjectsReachedTarget = false;
		return BaseApplication::mouseReleased(arg, id);
	}
	mFlgTarget = false;
	if (mFlgSelectNow == true) {

		for (int i = 0; i < mNumofObjects; ++i)
		{
			mSceneNodeArr[i]->showBoundingBox(false);
		}
		if (left == right
			&&
			top == bottom)
		{
			mNumberOfPets = singleClickSelect(arg, id);
		}
		else {
			mNumberOfPets = volumeSelection(arg, id);
		}
		mFlgSelectNow = false;
		mSelectionRect->setVisible(false);
	}

	return BaseApplication::mouseReleased(arg, id);

}

bool BasicTutorial_00::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right) {
		return BaseApplication::mousePressed(arg, id);
	}

	Ogre::String ss = Ogre::StringConverter::toString(id);

	Ogre::LogManager::getSingletonPtr()->logMessage(ss);


	mFlgSelectNow = true;

	Ray mRay = mTrayMgr->getCursorRay(mCamera);

	Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
	left = scn.x;
	top = scn.y;
	right = scn.x;
	bottom = scn.y;

	mSelectionRect->setCorners(left, top, right, bottom);
	mSelectionRect->setVisible(true);
	return BaseApplication::mousePressed(arg, id);
}
