/*
* Copyright (c) 2006-2010 Erin Catto http://www.box2d.org
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

#include <Box2D/Dynamics/Contacts/EdgeAndPolygonContact.h>
#include <Box2D/Common/BlockAllocator.h>
#include <Box2D/Dynamics/Fixture.h>
#include <Box2D/Collision/Shapes/EdgeShape.h>
#include <Box2D/Collision/Shapes/PolygonShape.h>

#include <new>

using namespace box2d;

Contact* EdgeAndPolygonContact::Create(Fixture* fixtureA, child_count_t,
										   Fixture* fixtureB, child_count_t,
										   BlockAllocator* allocator)
{
	void* mem = allocator->Allocate(sizeof(EdgeAndPolygonContact));
	return new (mem) EdgeAndPolygonContact(fixtureA, fixtureB);
}

void EdgeAndPolygonContact::Destroy(Contact* contact, BlockAllocator* allocator)
{
	(static_cast<EdgeAndPolygonContact*>(contact))->~EdgeAndPolygonContact();
	allocator->Free(contact, sizeof(EdgeAndPolygonContact));
}

EdgeAndPolygonContact::EdgeAndPolygonContact(Fixture* fixtureA, Fixture* fixtureB)
: Contact(fixtureA, 0, fixtureB, 0)
{
	assert(m_fixtureA->GetType() == Shape::e_edge);
	assert(m_fixtureB->GetType() == Shape::e_polygon);
}

Manifold EdgeAndPolygonContact::Evaluate(const Transform& xfA, const Transform& xfB)
{
	return CollideShapes(*static_cast<EdgeShape*>(m_fixtureA->GetShape()), xfA, *static_cast<PolygonShape*>(m_fixtureB->GetShape()), xfB);
}
