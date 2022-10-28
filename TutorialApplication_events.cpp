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

	mRaySceneQuery->setSortByDistance(true);
	mRaySceneQuery->setRay(mRay);

	// Perform the scene query
	RaySceneQueryResult& result = mRaySceneQuery->execute();
	RaySceneQueryResult::iterator itr;

	// Get the results, set the camera height
	// We are interested in the first intersection. It is ok to traverse all the results.

	for (itr = result.begin(); itr != result.end(); ++itr)
	{
		if (itr->movable && itr->movable->getName().substr(0, 5) == "Robot")
		{
			mCurrentObject = itr->movable->getParentSceneNode();
			bool flgShow = mCurrentObject->getShowBoundingBox();
			mCurrentObject->showBoundingBox(!flgShow);
			numSelectedObj = 1;
			break;
		}
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
	if (bottom < top) {
		Real tmp = bottom;
		bottom = top;
		top = tmp;
	}

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
	Ray topLeft     = mTrayMgr->screenToScene(mCamera, Vector2(nleft , ntop));
	Ray topRight    = mTrayMgr->screenToScene(mCamera, Vector2(nright, ntop));
	Ray bottomLeft  = mTrayMgr->screenToScene(mCamera, Vector2(nleft , nbottom));
	Ray bottomRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, nbottom));

	PlaneBoundedVolume vol;
	int np = 100;
	double nd = SystemParameter::nearClipDistance;

	vol.planes.push_back(Plane(topLeft.getPoint(nd), topRight.getPoint(nd), bottomRight.getPoint(nd)));
	vol.planes.push_back(Plane(topLeft.getOrigin(), topLeft.getPoint(np), topRight.getPoint(np)));
	vol.planes.push_back(Plane(topLeft.getOrigin(), bottomLeft.getPoint(np), topLeft.getPoint(np)));
	vol.planes.push_back(Plane(bottomLeft.getOrigin(), bottomRight.getPoint(np), bottomLeft.getPoint(np)));
	vol.planes.push_back(Plane(bottomRight.getOrigin(), topRight.getPoint(np), bottomRight.getPoint(np)));

	PlaneBoundedVolumeList volList;
	volList.push_back(vol);
	mVolQuery->setVolumes(volList);

	SceneQueryResult result = mVolQuery->execute();
	SceneQueryResultMovableList::iterator itr;

	for (itr = result.movables.begin(); itr != result.movables.end(); itr++)
	{
		if (*itr && (*itr)->getName().substr(0, 5) == "Robot")
		{
			mCurrentObject = (*itr)->getParentSceneNode();
			bool flgShow = mCurrentObject->getShowBoundingBox();
			mCurrentObject->showBoundingBox(!flgShow);
			numSelectedObj++;
		}
	}

	return numSelectedObj;

}

void BasicTutorial_00::computeTargetPosition()
{
	Ray mRay = mTrayMgr->getCursorRay(mCamera);

	mRaySceneQuery->setSortByDistance(true);
	mRaySceneQuery->setRay(mRay);

	// Perform the scene query
	RaySceneQueryResult& result = mRaySceneQuery->execute();
	RaySceneQueryResult::iterator itr;

	for (itr = result.begin(); itr != result.end(); ++itr)
	{
		if (itr->movable && itr->movable->getName() == "Floor")
		{
			mTargetPosition = mRay.getPoint(itr->distance);
			mFlgTarget = true;
			break;
		}
	}
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
		if (left == right && top == bottom)
		{
			mNumberOfPets = singleClickSelect(arg, id);
		}
		else
		{
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
