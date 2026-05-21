//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Fixed point arithemtics, implementation.
//


#ifndef __M_FIXED__
#define __M_FIXED__




//
// Fixed point, 32bit as 16.16.
//
#define FRACBITS		16
#define FRACUNIT		(1<<FRACBITS)
#define FIXED2DOUBLE(x)		(x / (float)FRACUNIT) // [cronopio] f64 -> f32 (translator rejects double)

typedef int fixed_t;

// [cronopio] (int64)(a*b - c*d)/divisor without i64 (impl in m_fixed_cvm.c).
int CVM_CrossDiv(int a, int b, int c, int d, int divisor);
// [cronopio] (int64)(a1*b1 + a2*b2 + a3*b3)/s without i64.
int CVM_ProjDiv(int a1, int b1, int a2, int b2, int a3, int b3, int s);

fixed_t FixedMul	(fixed_t a, fixed_t b);
fixed_t FixedDiv	(fixed_t a, fixed_t b);

// [cronopio] Out-of-line float abs. Writing the abs idiom inline (ternary,
// fabsf, copysignf) makes clang fold it back to the llvm.fabs.f32 intrinsic,
// which the translator can't lower. An opaque extern call defeats that fold.
float CVM_FAbsF(float v);
// [cronopio] Round-half-away-from-zero (replaces lroundf, which clang expands
// with an unordered float compare the translator rejects).
int CVM_LRoundF(float v);

#endif
