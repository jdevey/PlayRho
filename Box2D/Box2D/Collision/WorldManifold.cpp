/*
 * Original work Copyright (c) 2007-2009 Erin Catto http://www.box2d.org
 * Modified work Copyright (c) 2016 Louis Langholtz https://github.com/louis-langholtz/Box2D
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

#include <Box2D/Collision/WorldManifold.hpp>
#include <Box2D/Collision/Manifold.hpp>

namespace box2d {

	static inline WorldManifold GetForCircles(const Manifold& manifold,
											  const Transformation& xfA, const float_t radiusA,
											  const Transformation& xfB, const float_t radiusB)
	{
		assert(manifold.GetPointCount() == 1);
		
		switch (manifold.GetPointCount())
		{
			case 1:
			{
				const auto pointA = Transform(manifold.GetLocalPoint(), xfA);
				const auto pointB = Transform(manifold.GetPoint(0).localPoint, xfB);
				const auto delta = pointB - pointA;
				const auto normal = (almost_equal(LengthSquared(delta), 0))? Vec2{float_t{1}, float_t{0}}: GetUnitVector(delta);
				const auto cA = pointA + (radiusA * normal);
				const auto cB = pointB - (radiusB * normal);
				const auto p0 = (cA + cB) / float_t{2};
				const auto s0 = Dot(cB - cA, normal);
				return WorldManifold{normal, WorldManifold::PointSeparation{p0, s0}};
			}
			default: break;
		}
		
		// should never be reached
		return WorldManifold{Vec2{float_t{1}, float_t{0}}};
	}
	
	static inline WorldManifold GetForFaceA(const Manifold& manifold,
											const Transformation& xfA, const float_t radiusA,
											const Transformation& xfB, const float_t radiusB)
	{
		const auto normal = Rotate(manifold.GetLocalNormal(), xfA.q);
		const auto planePoint = Transform(manifold.GetLocalPoint(), xfA);
		const auto pointFn = [&](Manifold::size_type index) {
			const auto clipPoint = Transform(manifold.GetPoint(index).localPoint, xfB);
			const auto cA = clipPoint + (radiusA - Dot(clipPoint - planePoint, normal)) * normal;
			const auto cB = clipPoint - (radiusB * normal);
			return WorldManifold::PointSeparation{(cA + cB) / float_t{2}, Dot(cB - cA, normal)};
		};
		
		assert(manifold.GetPointCount() <= 2);
		
		switch (manifold.GetPointCount())
		{
			case 0: return WorldManifold{normal};
			case 1: return WorldManifold{normal, pointFn(0)};
			case 2: return WorldManifold{normal, pointFn(0), pointFn(1)};
			default: break; // should never be reached
		}
		
		// should never be reached
		return WorldManifold{normal};
	}
	
	static inline WorldManifold GetForFaceB(const Manifold& manifold,
											const Transformation& xfA, const float_t radiusA,
											const Transformation& xfB, const float_t radiusB)
	{
		const auto normal = Rotate(manifold.GetLocalNormal(), xfB.q);
		const auto planePoint = Transform(manifold.GetLocalPoint(), xfB);
		const auto pointFn = [&](Manifold::size_type index) {
			const auto clipPoint = Transform(manifold.GetPoint(index).localPoint, xfA);
			const auto cB = clipPoint + (radiusB - Dot(clipPoint - planePoint, normal)) * normal;
			const auto cA = clipPoint - (radiusA * normal);
			return WorldManifold::PointSeparation{(cA + cB) / float_t{2}, Dot(cA - cB, normal)};
		};
		
		assert(manifold.GetPointCount() <= 2);
		
		// Negate normal given to world manifold constructor to ensure it points from A to B.
		switch (manifold.GetPointCount())
		{
			case 0: return WorldManifold{-normal};
			case 1: return WorldManifold{-normal, pointFn(0)};
			case 2: return WorldManifold{-normal, pointFn(0), pointFn(1)};
			default: break; // should never be reached
		}
		
		// should never be reached
		return WorldManifold{-normal};
	}
	
	WorldManifold GetWorldManifold(const Manifold& manifold,
								   const Transformation& xfA, const float_t radiusA,
								   const Transformation& xfB, const float_t radiusB)
	{
		const auto type = manifold.GetType();
		assert((type == Manifold::e_circles) || (type == Manifold::e_faceA) || (type == Manifold::e_faceB) || (type == Manifold::e_unset));
		switch (type)
		{
			case Manifold::e_circles: return GetForCircles(manifold, xfA, radiusA, xfB, radiusB);
			case Manifold::e_faceA: return GetForFaceA(manifold, xfA, radiusA, xfB, radiusB);
			case Manifold::e_faceB: return GetForFaceB(manifold, xfA, radiusA, xfB, radiusB);
			case Manifold::e_unset: return WorldManifold{Vec2_zero};
		}
		
		// should never be reached
		return WorldManifold{Vec2_zero};
	}	

} /* namespace box2d */