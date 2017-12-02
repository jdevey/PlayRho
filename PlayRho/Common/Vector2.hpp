/*
 * Original work Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
 * Modified work Copyright (c) 2017 Louis Langholtz https://github.com/louis-langholtz/PlayRho
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

#ifndef PLAYRHO_COMMON_VECTOR2_HPP
#define PLAYRHO_COMMON_VECTOR2_HPP

#include <PlayRho/Common/Settings.hpp>
#include <PlayRho/Common/InvalidArgument.hpp>
#include <PlayRho/Common/Vector.hpp>

namespace playrho
{
    /// @brief Vector with 2-elements.
    /// @note This is just a C++11 alias template for 2-element uses of the Vector template.
    template <typename T>
    using Vector2 = Vector<T, 2>;
    
    /// @brief Vector with 2 Real elements.
    /// @note This data structure is two-times the size of the <code>Real</code> type
    ///   (or 8 using Real of float).
    using Vec2 = Vector2<Real>;
    
    /// @brief 2-element vector of Length quanties.
    /// @note Often used as a 2-dimensional distance or location vector.
    using Length2 = Vector2<Length>;

    /// @brief 2-element vector of LinearVelocity quantities.
    /// @note Often used as a 2-dimensional speed vector.
    using LinearVelocity2 = Vector2<LinearVelocity>;
    
    /// @brief 2-element vector of LinearAcceleration quantities.
    /// @note Often used as a 2-dimensional linear acceleration vector.
    using LinearAcceleration2 = Vector2<LinearAcceleration>;
    
    /// @brief 2-element vector of Force quantities.
    /// @note Often used as a 2-dimensional force vector.
    using Force2 = Vector2<Force>;
    
    /// @brief 2-element vector of Mass quantities.
    using Mass2 = Vector2<Mass>;

    /// @brief 2-element vector of InvMass quantities.
    using InvMass2 = Vector2<InvMass>;
    
    /// @brief 2-element vector of Momentum quantities.
    /// @note Often used as a 2-dimensional momentum vector.
    using Momentum2 = Vector2<Momentum>;
    
    /// @brief Earthly gravity in 2-dimensions.
    /// @details Linear acceleration in 2-dimensions of an earthly object due to Earth's mass.
    /// @sa EarthlyLinearAcceleration
    constexpr auto EarthlyGravity2D = LinearAcceleration2{0_mps2, EarthlyLinearAcceleration};

    /// @brief Gets the given value as a Vec2.
    constexpr inline Vec2 GetVec2(const Vector2<Real> value)
    {
        return {value};
    }

    /// @brief Gets an invalid value for the Vec2 type.
    template <>
    constexpr inline Vec2 GetInvalid() noexcept
    {
        return Vec2{GetInvalid<Vec2::value_type>(), GetInvalid<Vec2::value_type>()};
    }

    /// @brief Determines whether the given vector contains finite coordinates.
    template <typename TYPE>
    constexpr inline bool IsValid(const Vector2<TYPE>& value) noexcept
    {
        return IsValid(Get<0>(value)) && IsValid(Get<1>(value));
    }
    
#ifdef USE_BOOST_UNITS
    /// @brief Gets an invalid value for the Length2 type.
    template <>
    constexpr Length2 GetInvalid() noexcept
    {
        return Length2{GetInvalid<Length>(), GetInvalid<Length>()};
    }
    
    /// @brief Gets an invalid value for the LinearVelocity2 type.
    template <>
    constexpr LinearVelocity2 GetInvalid() noexcept
    {
        return LinearVelocity2{GetInvalid<LinearVelocity>(), GetInvalid<LinearVelocity>()};
    }
    
    /// @brief Gets an invalid value for the Force2 type.
    template <>
    constexpr Force2 GetInvalid() noexcept
    {
        return Force2{GetInvalid<Force>(), GetInvalid<Force>()};
    }
    
    /// @brief Gets an invalid value for the Momentum2 type.
    template <>
    constexpr Momentum2 GetInvalid() noexcept
    {
        return Momentum2{GetInvalid<Momentum>(), GetInvalid<Momentum>()};
    }
    
    constexpr inline Vec2 GetVec2(const Length2 value)
    {
        return Vec2{
            Get<0>(value) / Meter,
            Get<1>(value) / Meter
        };
    }
    
    constexpr inline Vec2 GetVec2(const LinearVelocity2 value)
    {
        return Vec2{
            Get<0>(value) / MeterPerSecond,
            Get<1>(value) / MeterPerSecond
        };
    }
    
    constexpr inline Vec2 GetVec2(const Momentum2 value)
    {
        return Vec2{
            Get<0>(value) / (Kilogram * MeterPerSecond),
            Get<1>(value) / (Kilogram * MeterPerSecond)
        };
    }
    
    constexpr inline Vec2 GetVec2(const Force2 value)
    {
        return Vec2{
            Get<0>(value) / Newton,
            Get<1>(value) / Newton
        };
    }
#endif
} // namespace playrho

#endif // PLAYRHO_COMMON_VECTOR2_HPP
