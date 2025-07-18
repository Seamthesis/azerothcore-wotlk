/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AC_SPLINE_H
#define AC_SPLINE_H

#include "Errors.h"
#include "MovementTypedefs.h"
#include <G3D/Vector3.h>
#include <limits>

namespace Movement
{
    class SplineBase
    {
    public:
        using index_type = int;
        using ControlArray = std::vector<Vector3>;
        enum EvaluationMode
        {
            ModeLinear,
            ModeCatmullrom,
            ModeBezier3_Unused,
            UninitializedMode,
            ModesEnd
        };

    protected:
        ControlArray points;

        index_type index_lo{0};
        index_type index_hi{0};

        uint8 m_mode{UninitializedMode};
        bool cyclic{false};

        enum
        {
            // could be modified, affects segment length evaluation precision
            // lesser value saves more performance in cost of lover precision
            // minimal value is 1
            // client's value is 20, blizzs use 2-3 steps to compute length
            STEPS_PER_SEGMENT = 3
        };
        static_assert(STEPS_PER_SEGMENT > 0, "shouldn't be lesser than 1");

    protected:
        void EvaluateLinear(index_type, float, Vector3&) const;
        void EvaluateCatmullRom(index_type, float, Vector3&) const;
        void EvaluateBezier3(index_type, float, Vector3&) const;
        typedef void (SplineBase::*EvaluationMethtod)(index_type, float, Vector3&) const;
        static EvaluationMethtod evaluators[ModesEnd];

        void EvaluateDerivativeLinear(index_type, float, Vector3&) const;
        void EvaluateDerivativeCatmullRom(index_type, float, Vector3&) const;
        void EvaluateDerivativeBezier3(index_type, float, Vector3&) const;
        static EvaluationMethtod derivative_evaluators[ModesEnd];

        [[nodiscard]] float SegLengthLinear(index_type) const;
        [[nodiscard]] float SegLengthCatmullRom(index_type) const;
        [[nodiscard]] float SegLengthBezier3(index_type) const;
        typedef float (SplineBase::*SegLenghtMethtod)(index_type) const;
        static SegLenghtMethtod seglengths[ModesEnd];

        void InitLinear(const Vector3*, index_type, bool, index_type);
        void InitCatmullRom(const Vector3*, index_type, bool, index_type);
        void InitBezier3(const Vector3*, index_type, bool, index_type);
        typedef void (SplineBase::*InitMethtod)(const Vector3*, index_type, bool, index_type);
        static InitMethtod initializers[ModesEnd];

        void UninitializedSplineEvaluationMethod(index_type, float, Vector3&) const { ABORT(); }
        [[nodiscard]] float UninitializedSplineSegLenghtMethod(index_type) const { ABORT(); }
        void UninitializedSplineInitMethod(Vector3 const*, index_type, bool, index_type) { ABORT(); }

    public:
        explicit SplineBase()  = default;

        /** Caclulates the position for given segment Idx, and percent of segment length t
            @param t - percent of segment length, assumes that t in range [0, 1]
            @param Idx - spline segment index, should be in range [first, last)
         */
        void evaluate_percent(index_type Idx, float u, Vector3& c) const {(this->*evaluators[m_mode])(Idx, u, c);}

        /** Caclulates derivation in index Idx, and percent of segment length t
            @param Idx - spline segment index, should be in range [first, last)
            @param t  - percent of spline segment length, assumes that t in range [0, 1]
         */
        void evaluate_derivative(index_type Idx, float u, Vector3& hermite) const {(this->*derivative_evaluators[m_mode])(Idx, u, hermite);}

        /**  Bounds for spline indexes. All indexes should be in range [first, last). */
        [[nodiscard]] index_type first() const { return index_lo;}
        [[nodiscard]] index_type last()  const { return index_hi;}

        [[nodiscard]] bool empty() const { return index_lo == index_hi;}
        [[nodiscard]] EvaluationMode mode() const { return (EvaluationMode)m_mode;}
        [[nodiscard]] bool isCyclic() const { return cyclic;}

        // Xinef: DO NOT USE EXCEPT FOR SPLINE INITIALIZATION!!!!!!
        [[nodiscard]] const ControlArray& getPoints() const { return points;}
        [[nodiscard]] index_type getPointCount() const { return points.size();}
        [[nodiscard]] const Vector3& getPoint(index_type i) const { return points[i];}

        /** Initializes spline. Don't call other methods while spline not initialized. */
        void init_spline(const Vector3* controls, index_type count, EvaluationMode m);
        void init_cyclic_spline(const Vector3* controls, index_type count, EvaluationMode m, index_type cyclic_point);

        /** As i can see there are a lot of ways how spline can be initialized
            would be no harm to have some custom initializers. */
        template<class Init> inline void init_spline_custom(Init& initializer)
        {
            initializer(m_mode, cyclic, points, index_lo, index_hi);
        }

        void clear();

        /** Calculates distance between [i; i+1] points, assumes that index i is in bounds. */
        [[nodiscard]] float SegLength(index_type i) const { return (this->*seglengths[m_mode])(i);}

        [[nodiscard]] std::string ToString() const;
    };

    template<typename length_type>
    class Spline : public SplineBase
    {
    public:
        using LengthType = length_type;
        using LengthArray = std::vector<length_type>;
    protected:
        LengthArray lengths;

        [[nodiscard]] index_type computeIndexInBounds(length_type length) const;
    public:
        explicit Spline() = default;

        /** Calculates the position for given t
            @param t - percent of spline's length, assumes that t in range [0, 1]. */
        void evaluate_percent(float t, Vector3& c) const;

        /** Calculates derivation for given t
            @param t - percent of spline's length, assumes that t in range [0, 1]. */
        void evaluate_derivative(float t, Vector3& hermite) const;

        /** Calculates the position for given segment Idx, and percent of segment length t
            @param t = partial_segment_length / whole_segment_length
            @param Idx - spline segment index, should be in range [first, last). */
        void evaluate_percent(index_type Idx, float u, Vector3& c) const { SplineBase::evaluate_percent(Idx, u, c);}

        /** Caclulates derivation for index Idx, and percent of segment length t
            @param Idx - spline segment index, should be in range [first, last)
            @param t  - percent of spline segment length, assumes that t in range [0, 1]. */
        void evaluate_derivative(index_type Idx, float u, Vector3& c) const { SplineBase::evaluate_derivative(Idx, u, c);}

        // Assumes that t in range [0, 1]
        [[nodiscard]] index_type computeIndexInBounds(float t) const;
        void computeIndex(float t, index_type& out_idx, float& out_u) const;

        /** Initializes spline. Don't call other methods while spline not initialized. */
        void init_spline(const Vector3* controls, index_type count, EvaluationMode m) { SplineBase::init_spline(controls, count, m);}
        void init_cyclic_spline(const Vector3* controls, index_type count, EvaluationMode m, index_type cyclic_point) { SplineBase::init_cyclic_spline(controls, count, m, cyclic_point);}

        /**  Initializes lengths with SplineBase::SegLength method. */
        void initLengths();

        /** Initializes lengths in some custom way
            Note that value returned by cacher must be greater or equal to previous value. */
        template<class T> inline void initLengths(T& cacher)
        {
            index_type i = index_lo;
            lengths.resize(index_hi + 1);
            length_type prev_length = 0, new_length = 0;
            while (i < index_hi)
            {
                new_length = cacher(*this, i);
                // length overflowed, assign to max positive value
                if (new_length < 0)
                    new_length = std::numeric_limits<length_type>::max();
                lengths[++i] = new_length;

                ASSERT(prev_length <= new_length);
                prev_length = new_length;
            }
        }

        /** Returns length of the whole spline. */
        [[nodiscard]] length_type length() const { return lengths[index_hi];}
        /** Returns length between given nodes. */
        [[nodiscard]] length_type length(index_type first, index_type last) const { return lengths[last] - lengths[first];}
        [[nodiscard]] length_type length(index_type Idx) const { return lengths[Idx];}

        void set_length(index_type i, length_type length) { lengths[i] = length;}
        void clear();
    };
}

#include "SplineImpl.h"

#endif // AC_SPLINE_H
