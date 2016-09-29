/*
* Copyright (c) 2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

// ---------- BOX2D packing ----------

/* edited by Reza (azer.darkblade@gmail.com) */

#ifndef CONFINED_H
#define CONFINED_H

#include <iostream>
#include "Test.h"

// packing library
#include <vector>

#include "APath.h"
#include "ArtData.h"
#include "VFRegion.h"
//#include "BlobTracer.h"
#include "LRFunctions.h"
#include "PathIO.h"
#include "Box2DSystemParams.h"

//int numShape = 0;

class Confined : public Test
{
public:

	enum
	{
		e_columnCount = 0,
		e_rowCount = 0
	};

	Confined();

	//void CreateCircle();
	//void CreateArrowHead();
	//void CreateBox();

	void Keyboard(int key);

	void Step(Settings* settings);

	static Test* Create()
	{
		return new Confined;
	}

public:
	void LoadArtData();
	void MakePhysicsObjectsFromArtData();
	void RepulsionForce();

public:
	PathIO _pathIO;

	std::vector<VFRegion> _uVFRegions;
	std::vector<VFRegion> _dVFRegions;

	std::vector<std::vector<AVector>> _dRegionBoundary;

	std::vector<ArtData> _artDataArray;
};

#endif
