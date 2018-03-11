/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vinnie Falco adapted for Linux by Bernd Porr

Official project location:
https://github.com/vinniefalco/DSPFilters

See Documentation.cpp for contact information, notes, and bibliography.

--------------------------------------------------------------------------------

License: MIT License (http://www.opensource.org/licenses/mit-license.php)
Copyright (c) 2009 by Vinnie Falco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*******************************************************************************/

#ifndef DSPFILTERS_RBJ_H
#define DSPFILTERS_RBJ_H

#include "Common.h"
#include "Biquad.h"
#include "State.h"

namespace Iir {

/**
 * Filter realizations based on Robert Bristol-Johnson formulae:
 *
 * http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
 *
 * These are all 2nd order filters which are tuned with the Q (or Quality factor).
 * The Q factor causes a resonance at the cutoff frequency. The higher the Q
 * factor the higher the responance. If 0.5 < Q < 1/sqrt(2) then there is no resonance peak.
 * Above 1/sqrt(2) the peak becomes more and more pronounced. For bandpass and stopband
 * the Q factor is replaced by the width of the filter. The higher Q the more narrow
 * the bandwidth of the notch or bandpass.
 *
 **/
namespace RBJ {

	/// The base class of all RBJ filters
	struct DllExport RBJbase : BiquadBase
	{
	public:
		/// filter operation
		template <typename Sample>
			inline Sample filter(Sample s) {
			return state.filter(s,*this);
		}
		/// resets the delay lines to zero
		void reset() {
			state.reset();
		}
		/// gets the delay lines (=state) of the filter
		const DirectFormI& getState() {
			return state;
		}
	private:
		DirectFormI state;
	};

	// The Q factor determines the resonance peak at the cutoff
	struct DllExport LowPass : RBJbase
	{
		void setup(double sampleRate,
			   double cutoffFrequency,
			   double q);
	};

	// The Q factor determines the resonance peak at the cutoff
	struct DllExport HighPass : RBJbase
	{
		void setup (double sampleRate,
			    double cutoffFrequency,
			    double q);
	};

	/// (constant skirt gain, peak gain = Q)
	struct DllExport BandPass1 : RBJbase
	{
		void setup (double sampleRate,
			    double centerFrequency,
			    double bandWidth);
	};

	/// (constant 0 dB peak gain)
	struct DllExport BandPass2 : RBJbase
	{
		void setup (double sampleRate,
			    double centerFrequency,
			    double bandWidth);
	};

	struct DllExport BandStop : RBJbase
	{
		void setup (double sampleRate,
			    double centerFrequency,
			    double bandWidth);
	};

	struct DllExport LowShelf : RBJbase
	{
		void setup (double sampleRate,
			    double cutoffFrequency,
			    double gainDb,
			    double shelfSlope);
	};

	struct DllExport HighShelf : RBJbase
	{
		void setup (double sampleRate,
			    double cutoffFrequency,
			    double gainDb,
			    double shelfSlope);
	};

	struct DllExport BandShelf : RBJbase
	{
		void setup (double sampleRate,
			    double centerFrequency,
			    double gainDb,
			    double bandWidth);
	};

	struct DllExport AllPass : RBJbase
	{
		void setup (double sampleRate,
			    double phaseFrequency,
			    double q);
	};
	
}

}


#endif
