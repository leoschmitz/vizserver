//-----------------------------------------------------------------------------
//
// Copyright (C) 2008-2009 VizTrader.com All rights reserved.
//
//-----------------------------------------------------------------------------
#ifndef __VIZMATH_H__
#define __VIZMATH_H__
//-----------------------------------------------------------------------------
#include "VizTypes.h"
#include <cmath>
#include <limits>
//-----------------------------------------------------------------------------
namespace viz
{
namespace core
{
//-----------------------------------------------------------------------------
const f32 ROUNDING_ERROR_32 = 0.000001f;
const f64 ROUNDING_ERROR_64 = 0.00000001f;
//-----------------------------------------------------------------------------
inline bool equals(const f32 a, const f32 b, const f32 tolerance = ROUNDING_ERROR_32)
{
	return (a + tolerance >= b) && (a - tolerance <= b);
}
//-----------------------------------------------------------------------------
inline const f32& min(const f32& a, const f32& b)
{
	return a < b ? a : b;
}
//-----------------------------------------------------------------------------
inline const f32& min(const f32& a, const f32& b, const f32& c)
{
	return a < b ? min(a, c) : min(b, c);
}
//-----------------------------------------------------------------------------
inline const f32& max(const f32& a, const f32& b)
{
	return a < b ? b : a;
}
//-----------------------------------------------------------------------------
inline const f32& max(const f32& a, const f32& b, const f32& c)
{
	return a < b ? max(b, c) : max(a, c);
}
//-----------------------------------------------------------------------------
inline long long clampLog10ll(const long long x)
{
	long double d = static_cast<long double>(x);
	d = (d > 1.0) ? log10(d) : 0.0;
	return static_cast<long long>(d);
} 
//-----------------------------------------------------------------------------
inline f32 clampLog10f(const f32 x)
{
	return (x > 1.0f) ? log10f(x) : 0.0f;
} 
//-----------------------------------------------------------------------------
}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
