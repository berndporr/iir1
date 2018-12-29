/**
 *
 * "A Collection of Useful C++ Classes for Digital Signal Processing"
 * By Vinnie Falco and Bernd Porr
 *
 * Official project location:
 * https://github.com/berndporr/iir1
 *
 * See Documentation.cpp for contact information, notes, and bibliography.
 * 
 * -----------------------------------------------------------------
 *
 * License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 * Copyright (c) 2009 by Vinnie Falco
 * Copyright (c) 2011 by Bernd Porr
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **/

#ifndef IIR1_CUSTOM_H
#define IIR1_CUSTOM_H

#include "Common.h"
#include "Biquad.h"
#include "Cascade.h"
#include "PoleFilter.h"
#include "State.h"


namespace Iir {

/*
 * Single pole and Biquad with parameters allowing
 * for directly setting the poles and zeros
 *
 */

namespace Custom {

//
// Raw filters
//

struct OnePole : Biquad
{
	void setup (double scale,
		    double pole,
		    double zero);
};

struct TwoPole : Biquad
{
	void setup (double scale,
		    double poleRho,
		    double poleTheta,
		    double zeroRho,
		    double zeroTheta);
};

/**
 * A custom cascade of 2nd order (SOS) filters.
 **/
template <int NSOS, class StateType = DEFAULT_STATE>
struct DllExport SOSCascade : CascadeStages<NSOS,StateType>
{
	/**
         * Python scipy.signal-friendly setting of coefficients.
	 * Sets the coefficients of the whole chain of
	 * biquads. The argument is a 2D array where the 1st
         * dimension holds an array of 2nd order biquad coefficients.
         * The sos coefficients are ordered "Python" style with first
         * the FIR coefficients (B) and then the IIR coefficients (A).
	 * \param sosCoefficients 2D array in Python style sos[][6] indexing: 0-2: FIR, 3-5: IIR coefficients.
	 **/
	void setup (const double sosCoefficients[NSOS][6]) {
		CascadeStages<NSOS,StateType>::setup(sosCoefficients);
	}
};


}

}

#endif
