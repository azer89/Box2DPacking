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

int numShape = 0;

class Confined : public Test
{
public:

	enum
	{
		e_columnCount = 0,
		e_rowCount = 0
	};

	Confined()
	{
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;

			b2Vec2 w1(-15.0f, 0.0f);   // bottomleft
			b2Vec2 w2(10.0f, 0.0f);    // bottomright
			b2Vec2 w3(-10.0f, 20.0f);  // topleft
			b2Vec2 w4(10.0f, 20.0f);   // topright

			b2Vec2 w5(0.0f, 15.0f);   // topright

			// Floor
			shape.Set(w1, w2);
			ground->CreateFixture(&shape, 0.0f);

			// Left wall
			shape.Set(w1, w3);
			ground->CreateFixture(&shape, 0.0f);

			// Right wall
			shape.Set(w2, w4);
			ground->CreateFixture(&shape, 0.0f);

			// Roof
			//shape.Set(w3, w4);
			//ground->CreateFixture(&shape, 0.0f);
			shape.Set(w3, w5);
			ground->CreateFixture(&shape, 0.0f);
			shape.Set(w5, w4);
			ground->CreateFixture(&shape, 0.0f);
		}

		float32 radius = 0.5f;
		b2CircleShape shape;
		shape.m_p.SetZero();
		shape.m_radius = radius;

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 1.0f;
		fd.friction = 0.1f;

		for (int32 j = 0; j < e_columnCount; ++j)
		{
			for (int i = 0; i < e_rowCount; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-10.0f + (2.1f * j + 1.0f + 0.01f * i) * radius, (2.0f * i + 1.0f) * radius);
				b2Body* body = m_world->CreateBody(&bd);

				body->CreateFixture(&fd);
			}
		}

		// ---------- no gravity ----------
		m_world->SetGravity(b2Vec2(0.0f, 0.0f));
	}

	void CreateCircle()
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

		b2Vec2 p(RandomFloat(), 10.0f + RandomFloat());
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position = p;
		//bd.allowSleep = false;
		b2Body* body = m_world->CreateBody(&bd);

		body->CreateFixture(&fd);

		numShape++;
	}

	void CreateArrowHead()
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
		triangle1.m_radius = 0.2; // radius skin

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
		triangle2.m_radius = 0.2; // radius skin

		b2FixtureDef fd2;
		fd2.density = 1.0f;
		fd2.friction = 0.0f;
		fd2.shape = &triangle2;

		b2Vec2 p(RandomFloat(), 10.0f + RandomFloat());
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position = p;
		//bd.allowSleep = false;
		b2Body* body = m_world->CreateBody(&bd);

		body->CreateFixture(&fd1);
		body->CreateFixture(&fd2);

		numShape++;
	}

	void CreateBox()
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
		fd.density = 1.0f;
		fd.friction = 0.0f;
		fd.shape = &shape;

		b2Vec2 p(RandomFloat(), 10.0f + RandomFloat());
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position = p;
		//bd.allowSleep = false;
		b2Body* body = m_world->CreateBody(&bd);

		body->CreateFixture(&fd);

		numShape++;
	}

	void Keyboard(int key)
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

	void Step(Settings* settings)
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

	static Test* Create()
	{
		return new Confined;
	}
};

#endif
