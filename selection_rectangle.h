//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2022/10/05
//

#ifndef __SelectionRectangle_h__
#define __SelectionRectangle_h__

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

using namespace Ogre;

class SelectionRectangle : public ManualObject
{
public:
    SelectionRectangle(const String &name)
        : ManualObject(name)
    {
        setUseIdentityProjection(true);
        setUseIdentityView(true);
        setRenderQueueGroup(RENDER_QUEUE_OVERLAY);
        setQueryFlags(0);
		setVisibilityFlags(0x0002);
    }

	void setCorners(float left, float top, float right, float bottom)
	{
       
		//Mapping to Projection Plane Space

		//BEGIN Using TrayMgr
		left = left * 2 - 1;
		right = right * 2 - 1;
		top = -1 - top * 2;
		bottom = -1 -bottom * 2;
		//End USING TrayMgr
		//////////////////////////
		Real z = -1;
		clear();
		/*
		begin("Examples/Hilite/Yellow", RenderOperation::OT_LINE_STRIP);
		colour(1, 1, 1);
		position(left, top, z);
		position(right, top, z);
		position(right, bottom, z);
		position(left, bottom, z);
		position(left, top, z);
		*/
		
/////////////////////////////////////////
		//
		//  1----0
		//  | \  | 
		//  |  \ |
		//  3----2 
		
		//begin("Examples/Hilite/Yellow", RenderOperation::OT_TRIANGLE_STRIP);
		begin("Examples/SelectionRegion", RenderOperation::OT_TRIANGLE_STRIP);
		position(right, top, z);
		position(left, top, z);
		position(right, bottom, z);
		position(left, bottom, z);
		
/////////////////////////////////////////
		end();

		AxisAlignedBox box;
		box.setInfinite();
		setBoundingBox(box);
	}
};


#endif