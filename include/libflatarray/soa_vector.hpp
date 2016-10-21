/**
 * Copyright 2016 Andreas Schäfer
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef FLAT_ARRAY_SOA_VECTOR_HPP
#define FLAT_ARRAY_SOA_VECTOR_HPP

#include <libflatarray/soa_accessor.hpp>
#include <libflatarray/soa_grid.hpp>
#include <stdexcept>

namespace LibFlatArray {

/**
 * This is the runtime resizable counterpart to soa_array. The goal is
 * to provide an interface similar to std::vector and simultaneously
 * have a callback to expose the struct-of-arrays layout.
 */
template<typename T>
class soa_vector
{
public:
    typedef T value_type;

    inline
    __host__ __device__
    explicit soa_vector(std::size_t count = 0) :
        grid(count, 1, 1),
        count(count)
    {}

    inline
    __host__ __device__
    explicit soa_vector(std::size_t count, const value_type& value) :
        grid(count, 1, 1),
        count(count)
    {
        grid.broadcast(0, 0, 0, value, count);
    }

    /**
     * Copies an element to the given index. We're intentionally not
     * using at() or operator[] to avoid mismatched expectations here:
     * we can't yield references to a T here.
     */
    inline
    __host__ __device__
    void set(std::size_t index, const T& element)
    {
        grid.set(index, 0, 0, element);
    }

    /**
     * Copy out an element. Again we're not using at() or operator[]
     * here to avoid confusion with the API: we can't return
     * references from an SoA container.
     */
    inline
    __host__ __device__
    T get(std::size_t index) const
    {
        return grid.get(index, 0, 0);
    }

    inline
    __host__ __device__
    std::size_t size() const
    {
        return count;
    }

    inline
    __host__ __device__
    void clear()
    {
        count = 0;
    }

private:
    soa_grid<T> grid;
    std::size_t count;

    // fixme: retrieval of multiple elements
    // fixme: emplace
    // fixme: at()
    // fixme: operator[]
    // fixme: resize
    // fixme: reserve
    // fixme: size
    // fixme: begin
    // fixme: end
    // fixme: push_back
};

}

#endif

