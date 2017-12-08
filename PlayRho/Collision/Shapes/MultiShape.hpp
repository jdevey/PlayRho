/*
 * Copyright (c) 2017 Louis Langholtz https://github.com/louis-langholtz/PlayRho
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef PLAYRHO_COLLISION_SHAPES_MULTISHAPE_HPP
#define PLAYRHO_COLLISION_SHAPES_MULTISHAPE_HPP

#include <PlayRho/Collision/Shapes/Shape.hpp>
#include <PlayRho/Collision/Shapes/ShapeDef.hpp>
#include <vector>

namespace playrho {
    
    class VertexSet;
    
    /// @brief The "multi-shape" shape.
    /// @details Composes zero or more convex shapes into what can be a concave shape.
    /// @ingroup PartsGroup
    class MultiShape: public Shape
    {
    public:
        /// @brief Convex hull.
        class ConvexHull
        {
        public:
            
            /// @brief Gets the convex hull for the given set of vertices.
            static ConvexHull Get(const VertexSet& pointSet);

            /// @brief Gets the distance proxy for this convex hull.
            DistanceProxy GetDistanceProxy(Length vertexRadius) const
            {
                return DistanceProxy{
                    vertexRadius, static_cast<VertexCounter>(vertices.size()),
                    vertices.data(), normals.data()
                };
            }

        private:
            /// @brief Initializing constructor.
            ConvexHull(std::vector<Length2> verts, std::vector<UnitVec2> norms):
                vertices{verts}, normals{norms}
            {}

            /// Array of vertices.
            /// @details Consecutive vertices constitute "edges" of the polygon.
            std::vector<Length2> vertices;
            
            /// Normals of edges.
            /// @details
            /// These are 90-degree clockwise-rotated unit-vectors of the vectors defined by
            /// consecutive pairs of elements of vertices.
            std::vector<UnitVec2> normals;
        };
        
        /// @brief Gets the default vertex radius for the MultiShape.
        static PLAYRHO_CONSTEXPR inline Length GetDefaultVertexRadius() noexcept
        {
            return DefaultLinearSlop * Real{2};
        }
        
        /// @brief Configuration data for multi-shape shapes.
        struct Conf: public ShapeDefBuilder<Conf>
        {
            inline Conf(): ShapeDefBuilder{ShapeConf{}.UseVertexRadius(GetDefaultVertexRadius())}
            {
                // Intentionally empty.
            }
            
            /// Creates a convex hull from the given set of local points.
            /// The size of the set must be in the range [1, MaxShapeVertices].
            /// @warning the points may be re-ordered, even if they form a convex polygon
            /// @warning collinear points are handled but not removed. Collinear points
            ///   may lead to poor stacking behavior.
            void AddConvexHull(const VertexSet& pointSet) noexcept;

            std::vector<ConvexHull> children; ///< Children.
        };
        
        /// @brief Gets the default configuration for a MultiShape.
        static inline Conf GetDefaultConf() noexcept
        {
            return Conf{};
        }
        
        /// @brief Default constructor.
        /// @details Constructs a polygon shape with a 0,0 centroid and vertex count of 0.
        /// @note Polygons with a vertex count less than 1 are "degenerate" and should be
        ///   treated as invalid.
        explicit MultiShape(const Conf& conf = GetDefaultConf()) noexcept:
            Shape{conf}, m_children{conf.children}
        {
            // Intentionally empty.
        }
        
        /// @brief Copy constructor.
        MultiShape(const MultiShape&) = default;
        
        ~MultiShape() override = default;
        
        ChildCounter GetChildCount() const noexcept override;
        
        DistanceProxy GetChild(ChildCounter index) const override;
        
        MassData GetMassData() const noexcept override;
                
        void Accept(ShapeVisitor& visitor) const override;

    private:
        std::vector<ConvexHull> m_children; ///< Children.
    };
    
    inline ChildCounter MultiShape::GetChildCount() const noexcept
    {
        return static_cast<ChildCounter>(m_children.size());
    }
    
    inline DistanceProxy MultiShape::GetChild(ChildCounter index) const
    {
        if (index >= GetChildCount())
        {
            throw InvalidArgument("index out of range");
        }
        const auto& child = m_children.at(index);
        return child.GetDistanceProxy(GetVertexRadius());
    }

} // namespace playrho

#endif // PLAYRHO_COLLISION_SHAPES_MULTISHAPE_HPP
