/*
 * Copyright (c) 1997-1999 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Sun Nov  7 20:43:47 EST 1999 */

#include "fftw-int.h"
#include "fftw.h"

/* Generated by: ./genfft -magic-alignment-check -magic-twiddle-load-all -magic-variables 4 -magic-loopi -notwiddle 3 */

/*
 * This function contains 12 FP additions, 4 FP multiplications,
 * (or, 10 additions, 2 multiplications, 2 fused multiply/add),
 * 12 stack variables, and 12 memory accesses
 */
static const fftw_real K500000000 = FFTW_KONST(+0.500000000000000000000000000000000000000000000);
static const fftw_real K866025403 = FFTW_KONST(+0.866025403784438646763723170752936183471402627);

/*
 * Generator Id's : 
 * $Id: exprdag.ml,v 1.41 1999/05/26 15:44:14 fftw Exp $
 * $Id: fft.ml,v 1.43 1999/05/17 19:44:18 fftw Exp $
 * $Id: to_c.ml,v 1.25 1999/10/26 21:41:32 stevenj Exp $
 */

void fftw_no_twiddle_3(const fftw_complex *input, fftw_complex *output, int istride, int ostride)
{
     fftw_real tmp1;
     fftw_real tmp10;
     fftw_real tmp4;
     fftw_real tmp9;
     fftw_real tmp8;
     fftw_real tmp11;
     fftw_real tmp5;
     fftw_real tmp12;
     ASSERT_ALIGNED_DOUBLE;
     tmp1 = c_re(input[0]);
     tmp10 = c_im(input[0]);
     {
	  fftw_real tmp2;
	  fftw_real tmp3;
	  fftw_real tmp6;
	  fftw_real tmp7;
	  ASSERT_ALIGNED_DOUBLE;
	  tmp2 = c_re(input[istride]);
	  tmp3 = c_re(input[2 * istride]);
	  tmp4 = tmp2 + tmp3;
	  tmp9 = K866025403 * (tmp3 - tmp2);
	  tmp6 = c_im(input[istride]);
	  tmp7 = c_im(input[2 * istride]);
	  tmp8 = K866025403 * (tmp6 - tmp7);
	  tmp11 = tmp6 + tmp7;
     }
     c_re(output[0]) = tmp1 + tmp4;
     tmp5 = tmp1 - (K500000000 * tmp4);
     c_re(output[2 * ostride]) = tmp5 - tmp8;
     c_re(output[ostride]) = tmp5 + tmp8;
     c_im(output[0]) = tmp10 + tmp11;
     tmp12 = tmp10 - (K500000000 * tmp11);
     c_im(output[ostride]) = tmp9 + tmp12;
     c_im(output[2 * ostride]) = tmp12 - tmp9;
}

fftw_codelet_desc fftw_no_twiddle_3_desc =
{
     "fftw_no_twiddle_3",
     (void (*)()) fftw_no_twiddle_3,
     3,
     FFTW_FORWARD,
     FFTW_NOTW,
     67,
     0,
     (const int *) 0,
};
