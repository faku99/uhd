//
// Copyright 2010 Ettus Research LLC
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef INCLUDED_UHD_TYPES_RANGES_HPP
#define INCLUDED_UHD_TYPES_RANGES_HPP

#include <uhd/config.hpp>
#include <uhd/utils/pimpl.hpp>
#include <string>
#include <vector>
#include <string>

namespace uhd{

    /*!
     * A range object describes a set of discrete values of the form:
     * y = start + step*n, where n is an integer between 0 and (stop - start)/step
     */
    template <typename T> class range_t{
    public:
        /*!
         * Create a range from a single value.
         * The step size will be taken as zero.
         * \param value the only possible value in this range
         */
        range_t(const T &value = T(0));

        /*!
         * Create a range from a full set of values.
         * A step size of zero implies infinite precision.
         * \param start the minimum value for this range
         * \param stop the maximum value for this range
         * \param step the step size for this range
         */
        range_t(const T &start, const T &stop, const T &step = T(0));

        //! Get the start value for this range.
        const T start(void) const;

        //! Get the stop value for this range.
        const T stop(void) const;

        //! Get the step value for this range.
        const T step(void) const;

        //! Convert this range to a printable string
        const std::string to_pp_string(void) const;

    private: 
        UHD_PIMPL_DECL(impl) _impl;
    };

    /*!
     * A meta-range object holds a list of individual ranges.
     */
    template <typename T> struct meta_range_t : std::vector<range_t<T> >{

        //! A default constructor for an empty meta-range
        meta_range_t(void);

        /*!
         * Input iterator constructor:
         * Makes boost::assign::list_of work.
         * \param first the begin iterator
         * \param last the end iterator
         */
        template <typename InputIterator>
        meta_range_t(InputIterator first, InputIterator last);

        /*!
         * A convenience constructor for a single range.
         * A step size of zero implies infinite precision.
         * \param start the minimum value for this range
         * \param stop the maximum value for this range
         * \param step the step size for this range
         */
        meta_range_t(const T &start, const T &stop, const T &step = T(0));

        //! Get the overall start value for this meta-range.
        const T start(void) const;

        //! Get the overall stop value for this meta-range.
        const T stop(void) const;

        //! Get the overall step value for this meta-range.
        const T step(void) const;

        /*!
         * Clip the target value to a possible range value.
         * \param value the value to clip to this range
         * \param clip_step if true, clip to steps as well
         * \return a value that is in one of the ranges
         */
         const T clip(const T &value, bool clip_step = false) const;

        //! Convert this meta-range to a printable string
        const std::string to_pp_string(void) const;

    };

    //!typedef for a gain meta-range
    typedef meta_range_t<float> gain_range_t;

    //!typedef for a frequency meta-range
    typedef meta_range_t<double> freq_range_t;


} //namespace uhd

#include <uhd/types/ranges.ipp>

#endif /* INCLUDED_UHD_TYPES_RANGES_HPP */
