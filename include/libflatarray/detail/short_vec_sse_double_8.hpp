/**
 * Copyright 2014-2017 Andreas Schäfer
 * Copyright 2015 Kurt Kanzenbach
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef FLAT_ARRAY_DETAIL_SHORT_VEC_SSE_DOUBLE_8_HPP
#define FLAT_ARRAY_DETAIL_SHORT_VEC_SSE_DOUBLE_8_HPP

#if (LIBFLATARRAY_WIDEST_VECTOR_ISA == LIBFLATARRAY_SSE) ||             \
    (LIBFLATARRAY_WIDEST_VECTOR_ISA == LIBFLATARRAY_SSE2) ||            \
    (LIBFLATARRAY_WIDEST_VECTOR_ISA == LIBFLATARRAY_SSE4_1)

#include <libflatarray/detail/short_vec_helpers.hpp>
#include <libflatarray/config.h>
#include <libflatarray/short_vec_base.hpp>

// disable certain warnings from system headers when compiling with
// Microsoft Visual Studio:
#ifdef _MSC_BUILD
#pragma warning( push )
#pragma warning( disable : 4514 )
#endif

#include <emmintrin.h>
#ifdef LIBFLATARRAY_WITH_CPP14
#include <initializer_list>
#endif

#ifdef _MSC_BUILD
#pragma warning( pop )
#endif

namespace LibFlatArray {

template<typename CARGO, std::size_t ARITY>
class short_vec;

#ifdef __ICC
// disabling this warning as implicit type conversion is exactly our goal here:
#pragma warning push
#pragma warning (disable: 2304)
#endif

template<>
class short_vec<double, 8> : public short_vec_base<double, 8>
{
public:
    static const std::size_t ARITY = 8;
    typedef short_vec<double, 8> mask_type;
    typedef short_vec_strategy::sse strategy;

    template<typename _CharT, typename _Traits>
    friend std::basic_ostream<_CharT, _Traits>& operator<<(
        std::basic_ostream<_CharT, _Traits>& __os,
        const short_vec<double, 8>& vec);

    inline
    short_vec(const double data = 0) :
        val{_mm_set1_pd(data),
            _mm_set1_pd(data),
            _mm_set1_pd(data),
            _mm_set1_pd(data)}
    {}

    inline
    short_vec(const double *data)
    {
        load(data);
    }

    inline
    short_vec(const __m128d& val1, const __m128d& val2, const __m128d& val3, const __m128d& val4) :
        val{val1,
            val2,
            val3,
            val4}
    {}

#ifdef LIBFLATARRAY_WITH_CPP14
    inline
    short_vec(const std::initializer_list<double>& il)
    {
        const double *ptr = static_cast<const double *>(&(*il.begin()));
        load(ptr);
    }
#endif

    inline
    bool any() const
    {
        __m128d buf1 = _mm_or_pd(_mm_or_pd(val[ 0], val[ 1]),
                                 _mm_or_pd(val[ 2], val[ 3]));

#ifdef __SSE4_1__
        return (0 == _mm_testz_si128(
                    _mm_castpd_si128(buf1),
                    _mm_castpd_si128(buf1)));
#else
        __m128d buf2 = _mm_shuffle_pd(buf1, buf1, 1);
        return _mm_cvtsd_f64(buf1) || _mm_cvtsd_f64(buf2);
#endif
    }

    inline
    double operator[](int i) const
    {
        __m128d buf = val[i >> 1];
        i &= 1;

        if (i == 0) {
            return _mm_cvtsd_f64(buf);
        }

        buf = _mm_shuffle_pd(buf, buf, 1);
        return _mm_cvtsd_f64(buf);
    }

    inline
    void operator-=(const short_vec<double, 8>& other)
    {
        val[ 0] = _mm_sub_pd(val[ 0], other.val[ 0]);
        val[ 1] = _mm_sub_pd(val[ 1], other.val[ 1]);
        val[ 2] = _mm_sub_pd(val[ 2], other.val[ 2]);
        val[ 3] = _mm_sub_pd(val[ 3], other.val[ 3]);
    }

    inline
    short_vec<double, 8> operator-(const short_vec<double, 8>& other) const
    {
        return short_vec<double, 8>(
            _mm_sub_pd(val[ 0], other.val[ 0]),
            _mm_sub_pd(val[ 1], other.val[ 1]),
            _mm_sub_pd(val[ 2], other.val[ 2]),
            _mm_sub_pd(val[ 3], other.val[ 3]));
    }

    inline
    void operator+=(const short_vec<double, 8>& other)
    {
        val[ 0] = _mm_add_pd(val[ 0], other.val[ 0]);
        val[ 1] = _mm_add_pd(val[ 1], other.val[ 1]);
        val[ 2] = _mm_add_pd(val[ 2], other.val[ 2]);
        val[ 3] = _mm_add_pd(val[ 3], other.val[ 3]);
    }

    inline
    short_vec<double, 8> operator+(const short_vec<double, 8>& other) const
    {
        return short_vec<double, 8>(
            _mm_add_pd(val[ 0], other.val[ 0]),
            _mm_add_pd(val[ 1], other.val[ 1]),
            _mm_add_pd(val[ 2], other.val[ 2]),
            _mm_add_pd(val[ 3], other.val[ 3]));
    }

    inline
    void operator*=(const short_vec<double, 8>& other)
    {
        val[ 0] = _mm_mul_pd(val[ 0], other.val[ 0]);
        val[ 1] = _mm_mul_pd(val[ 1], other.val[ 1]);
        val[ 2] = _mm_mul_pd(val[ 2], other.val[ 2]);
        val[ 3] = _mm_mul_pd(val[ 3], other.val[ 3]);
    }

    inline
    short_vec<double, 8> operator*(const short_vec<double, 8>& other) const
    {
        return short_vec<double, 8>(
            _mm_mul_pd(val[ 0], other.val[ 0]),
            _mm_mul_pd(val[ 1], other.val[ 1]),
            _mm_mul_pd(val[ 2], other.val[ 2]),
            _mm_mul_pd(val[ 3], other.val[ 3]));
    }

    inline
    void operator/=(const short_vec<double, 8>& other)
    {
        val[ 0] = _mm_div_pd(val[ 0], other.val[ 0]);
        val[ 1] = _mm_div_pd(val[ 1], other.val[ 1]);
        val[ 2] = _mm_div_pd(val[ 2], other.val[ 2]);
        val[ 3] = _mm_div_pd(val[ 3], other.val[ 3]);
    }

    inline
    short_vec<double, 8> operator/(const short_vec<double, 8>& other) const
    {
        return short_vec<double, 8>(
            _mm_div_pd(val[ 0], other.val[ 0]),
            _mm_div_pd(val[ 1], other.val[ 1]),
            _mm_div_pd(val[ 2], other.val[ 2]),
            _mm_div_pd(val[ 3], other.val[ 3]));
    }

    inline
    short_vec<double, 8> operator/(const sqrt_reference<double, 8>& other) const;

    inline
    short_vec<double, 8> operator<(const short_vec<double, 8>& other) const
    {
        return short_vec<double, 8>(
            _mm_cmplt_pd(val[ 0], other.val[ 0]),
            _mm_cmplt_pd(val[ 1], other.val[ 1]),
            _mm_cmplt_pd(val[ 2], other.val[ 2]),
            _mm_cmplt_pd(val[ 3], other.val[ 3]));
    }

    inline
    short_vec<double, 8> operator<=(const short_vec<double, 8>& other) const
    {
        return short_vec<double, 8>(
            _mm_cmple_pd(val[ 0], other.val[ 0]),
            _mm_cmple_pd(val[ 1], other.val[ 1]),
            _mm_cmple_pd(val[ 2], other.val[ 2]),
            _mm_cmple_pd(val[ 3], other.val[ 3]));
    }

    inline
    short_vec<double, 8> operator==(const short_vec<double, 8>& other) const
    {
        return short_vec<double, 8>(
            _mm_cmpeq_pd(val[ 0], other.val[ 0]),
            _mm_cmpeq_pd(val[ 1], other.val[ 1]),
            _mm_cmpeq_pd(val[ 2], other.val[ 2]),
            _mm_cmpeq_pd(val[ 3], other.val[ 3]));
    }

    inline
    short_vec<double, 8> operator>(const short_vec<double, 8>& other) const
    {
        return short_vec<double, 8>(
            _mm_cmpgt_pd(val[ 0], other.val[ 0]),
            _mm_cmpgt_pd(val[ 1], other.val[ 1]),
            _mm_cmpgt_pd(val[ 2], other.val[ 2]),
            _mm_cmpgt_pd(val[ 3], other.val[ 3]));
    }

    inline
    short_vec<double, 8> operator>=(const short_vec<double, 8>& other) const
    {
        return short_vec<double, 8>(
            _mm_cmpge_pd(val[ 0], other.val[ 0]),
            _mm_cmpge_pd(val[ 1], other.val[ 1]),
            _mm_cmpge_pd(val[ 2], other.val[ 2]),
            _mm_cmpge_pd(val[ 3], other.val[ 3]));
    }

    inline
    short_vec<double, 8> sqrt() const
    {
        return short_vec<double, 8>(
            _mm_sqrt_pd(val[ 0]),
            _mm_sqrt_pd(val[ 1]),
            _mm_sqrt_pd(val[ 2]),
            _mm_sqrt_pd(val[ 3]));
    }

    inline
    void load(const double *data)
    {
        val[ 0] = _mm_loadu_pd(data + 0);
        val[ 1] = _mm_loadu_pd(data + 2);
        val[ 2] = _mm_loadu_pd(data + 4);
        val[ 3] = _mm_loadu_pd(data + 6);
    }

    inline
    void load_aligned(const double *data)
    {
        SHORTVEC_ASSERT_ALIGNED(data, 16);
        val[ 0] = _mm_load_pd(data + 0);
        val[ 1] = _mm_load_pd(data + 2);
        val[ 2] = _mm_load_pd(data + 4);
        val[ 3] = _mm_load_pd(data + 6);
    }

    inline
    void store(double *data) const
    {
        _mm_storeu_pd(data + 0, val[ 0]);
        _mm_storeu_pd(data + 2, val[ 1]);
        _mm_storeu_pd(data + 4, val[ 2]);
        _mm_storeu_pd(data + 6, val[ 3]);
    }

    inline
    void store_aligned(double *data) const
    {
        SHORTVEC_ASSERT_ALIGNED(data, 16);
        _mm_store_pd(data + 0, val[ 0]);
        _mm_store_pd(data + 2, val[ 1]);
        _mm_store_pd(data + 4, val[ 2]);
        _mm_store_pd(data + 6, val[ 3]);
    }

    inline
    void store_nt(double *data) const
    {
        SHORTVEC_ASSERT_ALIGNED(data, 16);
        _mm_stream_pd(data + 0, val[ 0]);
        _mm_stream_pd(data + 2, val[ 1]);
        _mm_stream_pd(data + 4, val[ 2]);
        _mm_stream_pd(data + 6, val[ 3]);
    }

    inline
    void gather(const double *ptr, const int *offsets)
    {
        val[ 0] = _mm_loadl_pd(val[ 0], ptr + offsets[0]);
        val[ 0] = _mm_loadh_pd(val[ 0], ptr + offsets[1]);
        val[ 1] = _mm_loadl_pd(val[ 1], ptr + offsets[2]);
        val[ 1] = _mm_loadh_pd(val[ 1], ptr + offsets[3]);
        val[ 2] = _mm_loadl_pd(val[ 2], ptr + offsets[4]);
        val[ 2] = _mm_loadh_pd(val[ 2], ptr + offsets[5]);
        val[ 3] = _mm_loadl_pd(val[ 3], ptr + offsets[6]);
        val[ 3] = _mm_loadh_pd(val[ 3], ptr + offsets[7]);
    }

    inline
    void scatter(double *ptr, const int *offsets) const
    {
        _mm_storel_pd(ptr + offsets[0], val[ 0]);
        _mm_storeh_pd(ptr + offsets[1], val[ 0]);
        _mm_storel_pd(ptr + offsets[2], val[ 1]);
        _mm_storeh_pd(ptr + offsets[3], val[ 1]);
        _mm_storel_pd(ptr + offsets[4], val[ 2]);
        _mm_storeh_pd(ptr + offsets[5], val[ 2]);
        _mm_storel_pd(ptr + offsets[6], val[ 3]);
        _mm_storeh_pd(ptr + offsets[7], val[ 3]);
    }

    inline
    void blend(const mask_type& mask, const short_vec<double, 8>& other)
    {
#ifdef __SSE4_1__
        val[ 0] = _mm_blendv_pd(val[ 0], other.val[ 0], mask.val[ 0]);
        val[ 1] = _mm_blendv_pd(val[ 1], other.val[ 1], mask.val[ 1]);
        val[ 2] = _mm_blendv_pd(val[ 2], other.val[ 2], mask.val[ 2]);
        val[ 3] = _mm_blendv_pd(val[ 3], other.val[ 3], mask.val[ 3]);
#else
        val[ 0] = _mm_or_pd(
            _mm_and_pd(mask.val[ 0], other.val[ 0]),
            _mm_andnot_pd(mask.val[ 0], val[ 0]));
        val[ 1] = _mm_or_pd(
            _mm_and_pd(mask.val[ 1], other.val[ 1]),
            _mm_andnot_pd(mask.val[ 1], val[ 1]));
        val[ 2] = _mm_or_pd(
            _mm_and_pd(mask.val[ 2], other.val[ 2]),
            _mm_andnot_pd(mask.val[ 2], val[ 2]));
        val[ 3] = _mm_or_pd(
            _mm_and_pd(mask.val[ 3], other.val[ 3]),
            _mm_andnot_pd(mask.val[ 3], val[ 3]));
#endif
    }

private:
    __m128d val[4];
};

#ifdef __ICC
#pragma warning pop
#endif

inline
void operator<<(double *data, const short_vec<double, 8>& vec)
{
    vec.store(data);
}

inline
short_vec<double, 8> sqrt(const short_vec<double, 8>& vec)
{
    return vec.sqrt();
}

template<typename _CharT, typename _Traits>
std::basic_ostream<_CharT, _Traits>&
operator<<(std::basic_ostream<_CharT, _Traits>& __os,
           const short_vec<double, 8>& vec)
{
    const double *data1 = reinterpret_cast<const double *>(&vec.val[ 0]);
    const double *data2 = reinterpret_cast<const double *>(&vec.val[ 1]);
    const double *data3 = reinterpret_cast<const double *>(&vec.val[ 2]);
    const double *data4 = reinterpret_cast<const double *>(&vec.val[ 3]);
    __os << "["
         << data1[0] << ", " << data1[1] << ", "
         << data2[0] << ", " << data2[1] << ", "
         << data3[0] << ", " << data3[1] << ", "
         << data4[0] << ", " << data4[1] << "]";
    return __os;
}

}

#endif

#endif
