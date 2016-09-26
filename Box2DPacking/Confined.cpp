
#include "Confined.h"

Confined::Confined()
{
	// load regions
	this->_uVFRegions = _pathIO.LoadRegions(SystemParams::_image_folder + SystemParams::_artName + ".path");
	for (int a = 0; a < _uVFRegions.size(); a++)
	    { this->_dVFRegions.push_back(_uVFRegions[a].Scale(Box2DSystemParams::_box2DDownScaling)); }

	// region scaling (0.1 from the normal size)
	for (int a = 0; a < _dVFRegions.size(); a++)
	    { _dRegionBoundary.push_back(VFRegion::ConvertPathsToVectors(_dVFRegions[a]._boundaryPaths)[0]); }

	// make fixtures of region boundaries
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
}

void Confined::CreateCircle()
{
	std::cout << "circle\n";

	float32 radius = std::abs(RandomFloat()) + 0.5;
	b2CircleShape shape;
	shape.m_p.SetZero();
	shape.m_radius = radius;

	b2FixtureDef fd;
	fd.density = 1.0f;
	fd.friction = 0.0f;
	fd.shape = &shape;

	b2Vec2 p(RandomFloat() + 20, 30.0f + RandomFloat());
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position = p;
	//bd.allowSleep = false;
	b2Body* body = m_world->CreateBody(&bd);

	body->CreateFixture(&fd);

	//numShape++;
}

void Confined::CreateArrowHead()
{
	std::cout << "arrowhead\n";

	/*b2Transform xf1;
	xf1.q.Set(0.3524f * b2_pi); // rotation
	xf1.p = xf1.q.GetXAxis();

	b2Vec2 vertices[3];

	b2PolygonShape triangle1;
	vertices[0] = b2Mul(xf1, b2Vec2(-1.0f, 0.0f));
	vertices[1] = b2Mul(xf1, b2Vec2(2.0f, 0.0f));
	vertices[2] = b2Mul(xf1, b2Vec2(0.0f, 0.5f));
	triangle1.Set(vertices, 3);

	b2FixtureDef fd1;
	fd1.density = 1.0f;
	fd1.friction = 0.0f;
	fd1.shape = &triangle1;

	b2Transform xf2;
	xf2.q.Set(-0.3524f * b2_pi); // rotation
	xf2.p = -xf2.q.GetXAxis();

	b2PolygonShape triangle2;
	vertices[0] = b2Mul(xf2, b2Vec2(-2.0f, 0.0f));
	vertices[1] = b2Mul(xf2, b2Vec2(1.0f, 0.0f));
	vertices[2] = b2Mul(xf2, b2Vec2(0.0f, 0.5f));
	triangle2.Set(vertices, 3);*/

	b2Transform xf1;
	xf1.q.Set(0 * b2_pi); // rotation
	xf1.p = xf1.q.GetXAxis();

	b2Vec2 vertices[3];

	b2PolygonShape triangle1;
	vertices[0] = b2Mul(xf1, b2Vec2(-1.0f, 0.0f));
	vertices[1] = b2Mul(xf1, b2Vec2(1.0f, 0.0f));
	vertices[2] = b2Mul(xf1, b2Vec2(0.0f, 1.5f));
	triangle1.Set(vertices, 3);
	triangle1.m_radius = 0.5; // radius skin

	b2FixtureDef fd1;
	fd1.density = 1.0f;
	fd1.friction = 0.0f;
	fd1.shape = &triangle1;

	b2Transform xf2;
	xf2.q.Set(b2_pi); // rotation
	//xf2.p = -xf2.q.GetXAxis();
	xf2.p = xf2.q.GetXAxis();

	b2PolygonShape triangle2;
	vertices[0] = b2Mul(xf2, b2Vec2(-3.0f, 0.0f));
	vertices[1] = b2Mul(xf2, b2Vec2(-1.0f, 0.0f));
	vertices[2] = b2Mul(xf2, b2Vec2(-2.0f, 2.5f));
	triangle2.Set(vertices, 3);
	triangle2.m_radius = 0.5; // radius skin

	b2FixtureDef fd2;
	fd2.density = 1.0f;
	fd2.friction = 0.0f;
	fd2.shape = &triangle2;

	b2Vec2 p(RandomFloat() + 20, 30.0f + RandomFloat());
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position = p;
	//bd.allowSleep = false;
	b2Body* body = m_world->CreateBody(&bd);

	body->CreateFixture(&fd1);
	body->CreateFixture(&fd2);

	//numShape++;
}

void Confined::CreateBox()
{
	std::cout << "box\n";

	float32 a = 1.0f;
	b2Vec2 position;
	position.x = 0.0f;
	position.y = 0.0f;
	b2PolygonShape shape;
	float32 width = std::abs(RandomFloat()) + 0.5;
	float32 height = std::abs(RandomFloat()) + 0.5;
	shape.SetAsBox(width, height, position, 0.0f);

	b2FixtureDef fd;
	//fd.density = 1.0f;
	//fd.friction = 0.0f;
	fd.shape = &shape;

	b2Vec2 p(RandomFloat(), 10.0f + RandomFloat());
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position = p;
	//bd.allowSleep = false;
	b2Body* body = m_world->CreateBody(&bd);

	body->CreateFixture(&fd);

	//numShape++;
}

void Confined::Keyboard(int key)
{
	switch (key)
	{
	case GLFW_KEY_A:
		CreateArrowHead();
		break;
	case GLFW_KEY_B:
		CreateBox();
		break;
	case GLFW_KEY_C:
		CreateCircle();
		break;
	}
}

void Confined::Step(Settings* settings)
{
	bool sleeping = true;
	for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetType() != b2_dynamicBody)
		{
			continue;
		}

		if (b->IsAwake())
		{
			sleeping = false;
		}
	}

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