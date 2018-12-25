/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vinnie Falco adapted for Linux by Bernd Porr

Official project location:
https://github.com/vinniefalco/DSPFilters

See Documentation.cpp for contact information, notes, and bibliography.

--------------------------------------------------------------------------------

License: MIT License (http://www.opensource.org/licenses/mit-license.php)
Copyright (c) 2009 by Vinnie Falco
Copyright (c) 2011 by Bernd Porr

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

	/**
         * Lowpass.
         **/
	struct DllExport LowPass : RBJbase
	{
		/**
                 * Calculates the coefficients
                 * \param sampleRate Sampling rate
                 * \param cutoffFrequency Cutoff frequency
                 * \param q Q factor determines the resonance peak at the cutoff.
                 **/
		void setup(double sampleRate,
			   double cutoffFrequency,
			   double q);
	};

	/**
         * Highpass.
         **/
	struct DllExport HighPass : RBJbase
	{
		/**
                 * Calculates the coefficients
                 * \param sampleRate Sampling rate
                 * \param cutoffFrequency Cutoff frequency
                 * \param q Q factor determines the resonance peak at the cutoff.
                 **/
		void setup (double sampleRate,
			    double cutoffFrequency,
			    double q);
	};

	/**
         * Bandpass with constant skirt gain
         **/
	struct DllExport BandPass1 : RBJbase
	{
		/**
                 * Calculates the coefficients
                 * \param sampleRate Sampling rate
                 * \param centerFrequency Center frequency of the bandpass
                 * \param bandWidth Bandwidth of the bandpass
                 **/
		void setup (double sampleRate,
			    double centerFrequency,
			    double bandWidth);
	};

	/**
         * Bandpass with constant 0 dB peak gain
         **/
	struct DllExport BandPass2 : RBJbase
	{
		/**
                 * Calculates the coefficients
                 * \param sampleRate Sampling rate
                 * \param centerFrequency Center frequency of the bandpass
                 * \param bandWidth Bandwidth of the bandpass
                 **/
		void setup (double sampleRate,
			    double centerFrequency,
			    double bandWidth);
	};

	/**
         * Bandstop
         **/
	struct DllExport BandStop : RBJbase
	{
		/**
                 * Calculates the coefficients
                 * \param sampleRate Sampling rate
                 * \param centerFrequency Center frequency of the bandpass
                 * \param bandWidth Bandwidth of the bandpass
                 **/
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
