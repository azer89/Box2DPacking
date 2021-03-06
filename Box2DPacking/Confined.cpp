
#include "Confined.h"

#include "MySVGRenderer.h"

#include "NANOFLANNWrapper.h"

Confined::Confined()
{
	// ---------- load regions ----------
	this->_uVFRegions = _pathIO.LoadRegions(SystemParams::_image_folder + SystemParams::_artName + ".path");
	for (int a = 0; a < _uVFRegions.size(); a++)
	    { this->_dVFRegions.push_back(_uVFRegions[a].Scale(Box2DSystemParams::_box2DDownScaling)); }

	
	// ---------- region scaling (0.1 from the normal size) ----------
	for (int a = 0; a < _dVFRegions.size(); a++)
	    { _dRegionBoundary.push_back(VFRegion::ConvertPathsToVectors(_dVFRegions[a]._boundaryPaths)[0]); }

	// ---------- make fixtures of region boundaries ----------
	b2BodyDef bd;
	b2Body* ground = m_world->CreateBody(&bd);
	b2EdgeShape shape;
	for (int a = 0; a < _dRegionBoundary.size(); a++)
	{
		int sz = _dRegionBoundary[a].size();
		for (int b = 0; b < sz - 1; b++)
		{
			AVector p1 = _dRegionBoundary[a][b];
			AVector p2 = _dRegionBoundary[a][b+1];

			shape.Set(b2Vec2(p1.x, p1.y), b2Vec2(p2.x, p2.y));
			ground->CreateFixture(&shape, 0.0f);
		}
		AVector p1 = _dRegionBoundary[a][sz - 1];
		AVector p2 = _dRegionBoundary[a][0];
		shape.Set(b2Vec2(p1.x, p1.y), b2Vec2(p2.x, p2.y));
		ground->CreateFixture(&shape, 0.0f);
	}

	// ---------- no gravity ----------
	m_world->SetGravity(b2Vec2(0.0f, 0.0f));

	// ---------- load art data and process them ----------
	LoadArtData();
	MakePhysicsObjectsFromArtData();
}

void Confined::LoadArtData()
{
	std::cout << "LoadOrnaments\n";

	// ===== load deformed ornaments =====
	std::stringstream aDss;
	aDss << SystemParams::_image_folder << SystemParams::_artName << ".artdata";
	_artDataArray = _pathIO.LoadArtData(aDss.str());

	std::cout << "_artDataArray size: " << _artDataArray.size() << "\n";
}

void Confined::MakePhysicsObjectsFromArtData()
{
	// Need to have non intersecting polygons before triangulation
	for (int a = 0; a < _artDataArray.size(); a++)
		{ _artDataArray[a].FixSelfIntersection(); }

	// triangulate
	for (int a = 0; a < _artDataArray.size(); a++)
		{ _artDataArray[a].Triangulate(); }

	//MySVGRenderer::SaveOrnamentsToSVG(SystemParams::_image_folder + SystemParams::_artName + "_simple.svg", _artDataArray);
	//MySVGRenderer::SaveTrianglesToSVG(SystemParams::_image_folder + SystemParams::_artName + "_tri.svg", _artDataArray);


	for (int a = 0; a < _artDataArray.size(); a++)
	{
		std::vector<std::vector<AVector>>       triPoints = _artDataArray[a]._triPoints;
		std::vector<std::vector<AnIdxTriangle>> triangles = _artDataArray[a]._triangles;

		// bug 
		//if (triPoints.size() == 0 || triangles.size() == 0) { continue; }

		// bounding box
		AVector topLeftBB;
		topLeftBB.x = _artDataArray[a]._triLeft;
		topLeftBB.y = _artDataArray[a]._triTop;

		b2Vec2 p(topLeftBB.x * Box2DSystemParams::_box2DDownScaling, topLeftBB.y * Box2DSystemParams::_box2DDownScaling);
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position = p;
		b2Body* body = m_world->CreateBody(&bd);
		
		for (int b = 0; b < triangles.size(); b++)
		{
			if (triangles[b].size() == 0) { continue; } // ?

			for (int c = 0; c < triangles[b].size(); c++)
			{
				//std::cout << ".";
				AnIdxTriangle tri = triangles[b][c];
				AVector pt0 = (triPoints[b][tri.idx0] - topLeftBB) * Box2DSystemParams::_box2DDownScaling;
				AVector pt1 = (triPoints[b][tri.idx1] - topLeftBB) * Box2DSystemParams::_box2DDownScaling;
				AVector pt2 = (triPoints[b][tri.idx2] - topLeftBB) * Box2DSystemParams::_box2DDownScaling;

				// bug, if a triangle is too small, box2d will create a rectangular shape
				// heron's formula
				float triArea = std::abs(pt0.x * (pt1.y - pt2.y) + pt1.x * (pt2.y - pt0.y) + pt2.x * (pt0.y - pt1.y)) / 2.0f;
				if (triArea < 1e-1 * Box2DSystemParams::_box2DDownScaling) { continue; } // if it's too small, continue

				b2PolygonShape triangle;
				b2Vec2 vertices[3] = { b2Vec2(pt0.x, pt0.y), b2Vec2(pt1.x, pt1.y), b2Vec2(pt2.x, pt2.y) };
				triangle.Set(vertices, 3);
				//triangle.m_radius = 0.1; // radius skin

				b2FixtureDef fd;
				fd.density = 1.0f;
				fd.friction = 0.0f;
				fd.shape = &triangle;

				body->CreateFixture(&fd);
			}
		} // for 
	}

	std::cout << "DONE !!!\n";
}



void Confined::Keyboard(int key)
{
	switch (key)
	{
	case GLFW_KEY_A:
		//CreateArrowHead();
		break;
	case GLFW_KEY_B:
		//CreateBox();
		break;
	case GLFW_KEY_C:
		//CreateCircle();
		break;
	}
}

void Confined::RepulsionForce()
{
	// ---------- create list of bodies ----------
	std::vector<b2Body*> dBodies;
	for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetType() != b2_dynamicBody) { continue; }
		dBodies.push_back(b);
	}

	// ---------- iterate bodies ----------
	for (int a = 0; a < dBodies.size(); a++)
	{
		b2Body* aBody = dBodies[a]; // current body 

		// ---------- build kd-tree ----------
		NANOFLANNWrapper tree;
		std::vector<AVector> points;
		for (int b = 0; b < dBodies.size(); b++) // iterate neighbors
		{
			if (b == a) { continue; } // other than the current one
			b2Body* neighborBody = dBodies[b];
			b2Vec2 bodyPos = neighborBody->GetPosition();

			for (b2Fixture* f = neighborBody->GetFixtureList(); f; f = f->GetNext())
			{
				if (b2PolygonShape* poly = dynamic_cast< b2PolygonShape* >(f->GetShape()))
				{
					if (poly->GetVertexCount() == 3)
					{
						b2Vec2 p0 = poly->GetVertex(0) + bodyPos;
						b2Vec2 p1 = poly->GetVertex(1) + bodyPos;
						b2Vec2 p2 = poly->GetVertex(2) + bodyPos;

						// add to list
						points.push_back(AVector(p0.y, p0.y));
						points.push_back(AVector(p1.y, p1.y));
						points.push_back(AVector(p2.y, p2.y));
					}
				}
			} // neighbor fixtures
		}
		// ---------- build kd-tree ----------
		tree.SetPointData(points);
		tree.CreatePointKDTree();

		// ---------- apply force ----------
		b2Vec2 bodyPos = aBody->GetPosition();

		for (b2Fixture* f = aBody->GetFixtureList(); f; f = f->GetNext())
		{
			if (b2PolygonShape* poly = dynamic_cast<b2PolygonShape*>(f->GetShape()))
			{
				if (poly->GetVertexCount() == 3)
				{
					float k = 0.1f; // force magnitude
					int n = 1;      // num of closest points

					AVector p0(poly->GetVertex(0).x + bodyPos.x, poly->GetVertex(0).y + bodyPos.y);
					AVector p1(poly->GetVertex(1).x + bodyPos.x, poly->GetVertex(1).y + bodyPos.y);
					AVector p2(poly->GetVertex(2).x + bodyPos.x, poly->GetVertex(2).y + bodyPos.y);

					std::vector<AVector> f0 = tree.GetClosestPoints(p0, n);
					std::vector<AVector> f1 = tree.GetClosestPoints(p1, n);
					std::vector<AVector> f2 = tree.GetClosestPoints(p2, n);

					// 0
					for (int a = 0; a < n; a++)
					{
						AVector aForce = (p0 - f0[a]).Norm() * k / p0.Distance(f0[a]);
						aBody->ApplyForce(b2Vec2(aForce.x, aForce.y),
							              b2Vec2(p0.x, p0.y), 
										  true);
					}

					// 1
					for (int a = 0; a < n; a++)
					{
						AVector aForce = (p1 - f1[a]).Norm() * k / p1.Distance(f1[a]);
						aBody->ApplyForce(b2Vec2(aForce.x, aForce.y),
							              b2Vec2(p1.x, p1.y), 
										  true);
					}

					// 2
					for (int a = 0; a < n; a++)
					{
						AVector aForce = (p2 - f2[a]).Norm() * k / p2.Distance(f2[a]);
						aBody->ApplyForce(b2Vec2(aForce.x, aForce.y),
							              b2Vec2(p2.x, p2.y), 
										  true);
					}
				}
			}
		} // fixture iteration
	}

	dBodies.clear();
}

void Confined::Step(Settings* settings)
{
	bool sleeping = true;
	for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetType() != b2_dynamicBody) { continue; }
		if (b->IsAwake()) { sleeping = false; }
	} 

	//RepulsionForce();

	if (m_stepCount == 180)
	{
		m_stepCount += 0;
	}

	//if (sleeping)
	//{
	//	CreateCircle();
	//}

	Test::Step(settings);

	for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetType() != b2_dynamicBody)
		{
			continue;
		}

		b2Vec2 p = b->GetPosition();
		if (p.x <= -10.0f || 10.0f <= p.x || p.y <= 0.0f || 20.0f <= p.y)
		{
			p.x += 0.0f;
		}
	}

	//g_debugDraw.DrawString(5, m_textLine, "Press 'c' to create a circle.");
	m_textLine += DRAW_STRING_NEW_LINE;
}

//void Confined::CreateCircle()
//{
//	std::cout << "circle\n";
//
//	float32 radius = std::abs(RandomFloat()) + 0.5;
//	b2CircleShape shape;
//	shape.m_p.SetZero();
//	shape.m_radius = radius;
//
//	b2FixtureDef fd;
//	fd.density = 1.0f;
//	fd.friction = 0.0f;
//	fd.shape = &shape;
//
//	b2Vec2 p(RandomFloat() + 20, 30.0f + RandomFloat());
//	b2BodyDef bd;
//	bd.type = b2_dynamicBody;
//	bd.position = p;
//	//bd.allowSleep = false;
//	b2Body* body = m_world->CreateBody(&bd);
//
//	body->CreateFixture(&fd);
//
//	//numShape++;
//}
//
//void Confined::CreateArrowHead()
//{
//	std::cout << "arrowhead\n";
//
//	/*b2Transform xf1;
//	xf1.q.Set(0.3524f * b2_pi); // rotation
//	xf1.p = xf1.q.GetXAxis();
//
//	b2Vec2 vertices[3];
//
//	b2PolygonShape triangle1;
//	vertices[0] = b2Mul(xf1, b2Vec2(-1.0f, 0.0f));
//	vertices[1] = b2Mul(xf1, b2Vec2(2.0f, 0.0f));
//	vertices[2] = b2Mul(xf1, b2Vec2(0.0f, 0.5f));
//	triangle1.Set(vertices, 3);
//
//	b2FixtureDef fd1;
//	fd1.density = 1.0f;
//	fd1.friction = 0.0f;
//	fd1.shape = &triangle1;
//
//	b2Transform xf2;
//	xf2.q.Set(-0.3524f * b2_pi); // rotation
//	xf2.p = -xf2.q.GetXAxis();
//
//	b2PolygonShape triangle2;
//	vertices[0] = b2Mul(xf2, b2Vec2(-2.0f, 0.0f));
//	vertices[1] = b2Mul(xf2, b2Vec2(1.0f, 0.0f));
//	vertices[2] = b2Mul(xf2, b2Vec2(0.0f, 0.5f));
//	triangle2.Set(vertices, 3);*/
//
//	b2Transform xf1;
//	xf1.q.Set(0 * b2_pi); // rotation
//	xf1.p = xf1.q.GetXAxis();
//
//	b2Vec2 vertices[3];
//
//	b2PolygonShape triangle1;
//	vertices[0] = b2Mul(xf1, b2Vec2(-1.0f, 0.0f));
//	vertices[1] = b2Mul(xf1, b2Vec2(1.0f, 0.0f));
//	vertices[2] = b2Mul(xf1, b2Vec2(0.0f, 1.5f));
//	triangle1.Set(vertices, 3);
//	triangle1.m_radius = 0.5; // radius skin
//
//	b2FixtureDef fd1;
//	fd1.density = 1.0f;
//	fd1.friction = 0.0f;
//	fd1.shape = &triangle1;
//
//	b2Transform xf2;
//	xf2.q.Set(b2_pi); // rotation
//	//xf2.p = -xf2.q.GetXAxis();
//	xf2.p = xf2.q.GetXAxis();
//
//	b2PolygonShape triangle2;
//	vertices[0] = b2Mul(xf2, b2Vec2(-3.0f, 0.0f));
//	vertices[1] = b2Mul(xf2, b2Vec2(-1.0f, 0.0f));
//	vertices[2] = b2Mul(xf2, b2Vec2(-2.0f, 2.5f));
//	triangle2.Set(vertices, 3);
//	triangle2.m_radius = 0.5; // radius skin
//
//	b2FixtureDef fd2;
//	fd2.density = 1.0f;
//	fd2.friction = 0.0f;
//	fd2.shape = &triangle2;
//
//	b2Vec2 p(RandomFloat() + 20, 30.0f + RandomFloat());
//	b2BodyDef bd;
//	bd.type = b2_dynamicBody;
//	bd.position = p;
//	bd.linearDamping = 0.5f; /////////////
//	//bd.allowSleep = false;
//	b2Body* body = m_world->CreateBody(&bd);
//
//	body->CreateFixture(&fd1);
//	body->CreateFixture(&fd2);
//
//	//numShape++;
//}
//
//void Confined::CreateBox()
//{
//	std::cout << "box\n";
//
//	//float32 a = 1.0f;
//	b2Vec2 position;
//	position.x = 0.0f;
//	position.y = 0.0f;
//	b2PolygonShape shape;
//	float32 width = std::abs(RandomFloat()) + 0.5;
//	float32 height = std::abs(RandomFloat()) + 0.5;
//	shape.SetAsBox(width, height, position, 0.0f);
//
//	b2FixtureDef fd;
//	//fd.density = 1.0f;
//	//fd.friction = 0.0f;
//	fd.shape = &shape;
//
//	b2Vec2 p(RandomFloat(), 10.0f + RandomFloat());
//	b2BodyDef bd;
//	bd.type = b2_dynamicBody;
//	bd.position = p;
//	//bd.allowSleep = false;
//	b2Body* body = m_world->CreateBody(&bd);
//
//	body->CreateFixture(&fd);
//
//	//numShape++;
//}