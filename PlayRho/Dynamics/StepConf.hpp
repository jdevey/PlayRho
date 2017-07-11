/*
 * Original work Copyright (c) 2006-2011 Erin Catto http://www.box2d.org
 * Modified work Copyright (c) 2017 Louis Langholtz https://github.com/louis-langholtz/Box2D
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

#ifndef B2_STEP_CONF_HPP
#define B2_STEP_CONF_HPP

/// @file
/// Declarations of the StepConf class, and free functions associated with it.

#include <PlayRho/Common/Settings.hpp>
#include <PlayRho/Common/BoundedValue.hpp>

namespace box2d {

/// @brief Step configuration.
/// @details
/// Provides the primary means for configuring the per-step world physics simulation. All
/// the values have defaults. These defaults are intended to most likely be the values desired.
/// @note Be sure to confirm that the delta time (the time-per-step i.e. <code>dt</code>) is
///   correct for your use.
/// @note This data structure is 100-bytes large (with 4-byte Real on at least one 64-bit platform).
/// @sa World::Step.
class StepConf
{
public:
    /// @brief Step iterations type.
    /// @details A type for countining iterations per-step.
    /// @note The special value of -1 is reserved for signifying an invalid iteration value.
    using iteration_type = ts_iters_t;

    /// @brief Invalid iteration value.
    static constexpr auto InvalidIteration = static_cast<iteration_type>(-1);

    /// @brief Gets the delta time (time amount for this time step).
    /// @sa SetTime(Real).
    /// @return Time step amount in seconds.
    Time GetTime() const noexcept { return time; }

    /// @brief Gets the inverse delta-t value.
    /// @return 1/dt or 0 if dt is 0.
    /// @sa GetTime().
    Frequency GetInvTime() const noexcept { return invTime; }
    
    /// @brief Sets the delte time and inverse time from the given value and its inverse respectively.
    /// @note Used in both the regular and TOI phases of step processing.
    /// @post Getting the delta time will return this value.
    /// @post The inverse delta time value is the inverse of the given value or zero if the value is zero.
    /// @sa GetTime().
    /// @sa GetInvTime().
    /// @param value Elapsed time amount.
    constexpr StepConf& SetTime(Time value) noexcept
    {
        time = value;
        invTime = (value != Time{0})? Real{1} / value: Hertz * Real{0};
        return *this;
    }

    /// @brief Sets the inverse time and delta time from the given value and its inverse respectively.
    /// @note Used in both the regular and TOI phases of step processing.
    /// @post Getting the inverse delta time will return this value.
    /// @post The delta time value is the inverse of the given value or zero if the value is zero.
    /// @sa GetTime().
    /// @sa GetInvTime().
    /// @param value Inverse time amount.
    constexpr StepConf& SetInvTime(Frequency value) noexcept
    {
        invTime = value;
        time = (value != Frequency{0})? Time{Real{1} / value}: Time{0};
        return *this;
    }
    
    /// @brief Delta time ratio.
    /// @details This is the delta-time multiplied by the inverse delta time from the previous
    ///    world step. The value of 1 indicates that the time step has not varied.
    /// @note Used in the regular phase processing of the step.
    Real dtRatio = 1;

    /// @brief Minimum still time to sleep.
    /// @details The time that a body must be still before it will be put to sleep.
    /// @note Set to an invalid value to disable sleeping.
    /// @note Used in the regular phase processing of the step.
    Time minStillTimeToSleep = DefaultMinStillTimeToSleep;

    /// @brief Linear slop.
    /// @details Linear slop for position resolution.
    /// @note Must be greater than 0.
    /// @note Used in both the regular and TOI phases of step processing.
    Positive<Length> linearSlop = DefaultLinearSlop;
    
    /// @brief Angular slop.
    /// @note Must be greater than 0.
    /// @note Used in both the regular and TOI phases of step processing.
    Positive<Angle> angularSlop = DefaultAngularSlop;
    
    /// @brief Regular resolution rate.
    /// @details
    /// This scale factor controls how fast positional overlap is resolved.
    /// Ideally this would be 1 so that overlap is removed in one time step.
    /// However using values close to 1 often lead to overshoot.
    /// @note Must be greater than 0 for any regular-phase positional resolution to get done.
    /// @note Used in the regular phase of step processing.
    Real regResolutionRate = Real{2} / 10; // aka 0.2.
    
    /// @brief Regular minimum separation.
    /// @details
    /// This is the minimum amount of separation there must be between regular-phase interacting
    /// bodies for intra-step position resolution to be considered successful and end before all
    /// of the regular position iterations have been done.
    /// @note Used in the regular phase of step processing.
    /// @sa regPositionIterations.
    Length regMinSeparation = -DefaultLinearSlop * Real{3};
    
    /// @brief Time of impact resolution rate.
    /// @details
    /// This scale factor controls how fast positional overlap is resolved.
    /// Ideally this would be 1 so that overlap is removed in one time step.
    /// However using values close to 1 often lead to overshoot.
    /// @note Used in the TOI phase of step processing.
    /// @note Must be greater than 0 for any TOI-phase positional resolution to get done.
    Real toiResolutionRate = Real{75} / 100; // aka .75

    /// @brief Time of impact minimum separation.
    /// @details
    /// This is the minimum amount of separation there must be between TOI-phase interacting
    /// bodies for intra-step position resolution to be considered successful and end before all
    /// of the TOI position iterations have been done.
    /// @note Used in the TOI phase of step processing.
    /// @sa toiPositionIterations.
    Length toiMinSeparation = -DefaultLinearSlop * Real(1.5f);

    /// @brief Target depth.
    /// @details Target depth of overlap for calculating the TOI for CCD elligible bodies.
    /// @note Must be greater than 0.
    /// @note Must not be subnormal.
    /// @note Must be less than twice the world's minimum vertex radius.
    /// @note Used in the TOI phase of step processing.
    Positive<Length> targetDepth = DefaultLinearSlop * Real{3};
    
    /// @brief Tolerance.
    /// @details The acceptable plus or minus tolerance from the target depth for TOI calculations.
    /// @note Must be greater than 0.
    /// @note Must not be subnormal.
    /// @note Must be less than the target depth.
    /// @note Used in the TOI phase of step processing.
    Positive<Length> tolerance = DefaultLinearSlop / Real{4};

    /// @brief Velocity threshold.
    /// @details A velocity threshold for elastic collisions. Any collision with a relative linear
    /// velocity below this threshold will be treated as inelastic.
    /// @note Used in both the regular and TOI phases of step processing.
    LinearVelocity velocityThreshold = DefaultVelocityThreshold;

    /// @brief Maximum translation.
    /// @details The maximum linear velocity of a body.
    /// @note This limit is very large and is used to prevent numerical problems.
    /// You shouldn't need to adjust this.
    /// @note Used in both the regular and TOI phases of step processing.
    Length maxTranslation = Meter * Real(4); // originally 2
    
    /// @brief Maximum rotation.
    /// @details The maximum angular velocity of a body.
    /// @note This limit is very large and is used to prevent numerical problems.
    /// You shouldn't need to adjust this.
    /// @note Used in both the regular and TOI phases of step processing.
    Angle maxRotation = Radian * (Pi / 2);

    /// @brief Maximum linear correction.
    /// @note Must be greater than 0 for any positional resolution to get done.
    /// @note This value should be greater than the linear slop value.
    /// @note Used in both the regular and TOI phases of step processing.
    Length maxLinearCorrection = DefaultMaxLinearCorrection;
    
    /// @brief Maximum angular correction.
    /// @note Used in both the regular and TOI phases of step processing.
    Angle maxAngularCorrection = DefaultMaxAngularCorrection;

    /// @brief Linear sleep tolerance.
    /// @note Used in the regular phase of step processing.
    LinearVelocity linearSleepTolerance = DefaultLinearSleepTolerance;

    /// @brief Angular sleep tolerance.
    /// @note Used in the regular phase of step processing.
    AngularVelocity angularSleepTolerance = DefaultAngularSleepTolerance;
    
    /// @brief Displacement multiplier for directional AABB fattening.
    Real displaceMultiplier = DefaultDistanceMultiplier;
    
    /// @brief AABB extension.
    /// @details This is the extension that will be applied to Axis Aligned Bounding Box
    ///    objects used in "broadphase" collision detection. This fattens AABBs in the
    ///    dynamic tree. This allows proxies to move by a small amount without triggering
    ///    a tree adjustment.
    /// @note Should be greater than 0.
    Length aabbExtension = DefaultAabbExtension;

    /// @brief Max. circles ratio.
    /// @details When the ratio of the closest face's length to the vertex radius is
    ///   more than this amount, then face-manifolds are forced, else circles-manifolds
    ///   may be computed for new contact manifolds.
    /// @note This is used in the calculation of new contact manifolds.
    Real maxCirclesRatio = DefaultCirclesRatio;

    /// @brief Regular velocity iterations.
    /// @details The number of iterations of velocity resolution that will be done in the step.
    /// @note Used in the regular phase of step processing.
    iteration_type regVelocityIterations = 8;
    
    /// @brief Regular position iterations.
    /// @details
    /// This is the maximum number of iterations of position resolution that will
    /// be done before leaving any remaining unsatisfied positions for the next step.
    /// In this context, positions are satisfied when the minimum separation is greater than
    /// or equal to the regular minimum separation amount.
    /// @note Used in the regular phase of step processing.
    /// @sa regMinSeparation.
    iteration_type regPositionIterations = 3;

    /// @brief TOI velocity iterations.
    /// @details
    /// This is the number of iterations of velocity resolution that will be done in the step.
    /// @note Used in the TOI phase of step processing.
    iteration_type toiVelocityIterations = 8;

    /// @brief TOI position iterations.
    /// @details
    /// This value is the maximum number of iterations of position resolution that will
    /// be done before leaving any remaining unsatisfied positions for the next step.
    /// In this context, positions are satisfied when the minimum separation is greater than
    /// or equal to the TOI minimum separation amount.
    /// @note Used in the TOI phase of step processing.
    /// @sa toiMinSeparation.
    iteration_type toiPositionIterations = 20;
    
    /// @brief Max TOI root finder iterations.
    /// @note Used in the TOI phase of step processing.
    iteration_type maxToiRootIters = DefaultMaxToiRootIters;
    
    /// @brief Max TOI iterations.
    /// @note Used in the TOI phase of step processing.
    iteration_type maxToiIters = DefaultMaxToiIters;
    
    /// @brief Max distance iterations.
    /// @note Used in the TOI phase of step processing.
    iteration_type maxDistanceIters = DefaultMaxDistanceIters;

    /// @brief Maximum sub steps.
    /// @details
    /// This is the maximum number of sub-steps per contact in continuous physics simulation.
    /// In other words, this is the maximum number of times in a world step that a contact will
    /// have continuous collision resolution done for it.
    /// @note Used in the TOI phase of step processing.
    iteration_type maxSubSteps = DefaultMaxSubSteps;
    
    /// @brief Do warm start.
    /// @details Whether or not to perform warm starting (in the regular phase).
    /// @note Used in the regular phase of step processing.
    bool doWarmStart = true;
    
    /// @brief Do TOI.
    /// @details Whether or not to perform continuous collision detection.
    /// @note Used in the TOI phase of step processing.
    bool doToi = true;

    /// @brief Do blocksolve algorithm.
    bool doBlocksolve = true;

private:
    /// @brief Delta time.
    /// @details This is the time step in seconds.
    Time time = DefaultStepTime;

    /// @brief Inverse time step.
    /// @details The inverse time step. Specifically: 1/time or 0 if time == 0.
    /// @see time.
    Frequency invTime = DefaultStepFrequency;
};

inline Length GetMaxRegLinearCorrection(const StepConf& conf) noexcept
{
    return conf.maxLinearCorrection * static_cast<Real>(conf.regPositionIterations);
}

bool IsMaxTranslationWithinTolerance(const StepConf& conf) noexcept;

} // namespace box2d

#endif