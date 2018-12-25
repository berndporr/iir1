/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vinnie Falco and adapted by Bernd Porr

Official project location:
https://github.com/vinniefalco/DSPFilters

See Documentation.cpp for contact information, notes, and bibliography.

--------------------------------------------------------------------------------

License: MIT License (http://www.opensource.org/licenses/mit-license.php)
Copyright (c) 2009 by Vinnie Falco
Copyrgith (c) 2011 by Bernd Porr

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

#ifndef DSPFILTERS_CHEBYSHEVII_H
#define DSPFILTERS_CHEBYSHEVII_H

#include "Common.h"
#include "Cascade.h"
#include "PoleFilter.h"
#include "State.h"

namespace Iir {

/**
 * Filters with ChebyshevII response characteristics. The last parameter "minimumStopBandRejectionInDB"
 * defines the minimal stopband rejection requested. Generally there will be frequencies where
 * the rejection is much better but this parameter guarantees that the rejection is at least
 * "minimumStopBandRejectionInDB".
 *
 **/
namespace ChebyshevII {

// Half-band analog prototypes (s-plane)

class DllExport AnalogLowPass : public LayoutBase
{
public:
  AnalogLowPass ();

  void design (const int numPoles,
               double stopBandDb);

private:
  int m_numPoles;
  double m_stopBandDb;
};

//------------------------------------------------------------------------------

class DllExport AnalogLowShelf : public LayoutBase
{
public:
  AnalogLowShelf ();

  void design (int numPoles,
               double gainDb,
               double stopBandDb);

private:
  int m_numPoles;
  double m_stopBandDb;
  double m_gainDb;
};

//------------------------------------------------------------------------------

// Factored implementations to reduce template instantiations

struct DllExport LowPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double stopBandDb);
};

struct DllExport HighPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double stopBandDb);
};

struct DllExport BandPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              double stopBandDb);
};

struct DllExport BandStopBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              double stopBandDb);
};

struct DllExport LowShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double gainDb,
              double stopBandDb);
};

struct DllExport HighShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double gainDb,
              double stopBandDb);
};

struct DllExport BandShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              double gainDb,
              double stopBandDb);
};

//------------------------------------------------------------------------------

//
// Userland filters
//

/**
 * ChebyshevII lowpass filter. Init with "setup (
 * double sampleRate,
 * double cutoffFrequency,
 * double stopbandDb);"
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport LowPass : PoleFilter <LowPassBase, StateType, MaxOrder>
{

	void setup (int order,
		    double sampleRate,
		    double cutoffFrequency,
		    double stopBandDb) {
		LowPassBase::setup (order,
				    sampleRate,
				    cutoffFrequency,
				    stopBandDb);
	}

	void setup (double sampleRate,
		    double cutoffFrequency,
		    double stopBandDb) {
		LowPassBase::setup (MaxOrder,
				    sampleRate,
				    cutoffFrequency,
				    stopBandDb);
	}
};

/**
 * ChebyshevI highpass filter. Init with "setup(
 *             double sampleRate,
 *             double cutoffFrequency,
 *             double minimumStopBandRejectionInDB);"
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport HighPass : PoleFilter <HighPassBase, StateType, MaxOrder>
{
	void setup (int order,
		    double sampleRate,
		    double cutoffFrequency,
		    double stopBandDb) {
		HighPassBase::setup (order,
				     sampleRate,
				     cutoffFrequency,
				     stopBandDb);
	}

	void setup (double sampleRate,
		    double cutoffFrequency,
		    double stopBandDb) {
		HighPassBase::setup (MaxOrder,
				     sampleRate,
				     cutoffFrequency,
				     stopBandDb);
	}
};

/**
 * ChebyshevII bandpass filter. Init with "setup (
 *             double sampleRate,
 *             double centerFrequency,
 *             double widthFrequency,
 *             double minimumStopBandRejectionInDB);"
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport BandPass : PoleFilter <BandPassBase, StateType, MaxOrder, MaxOrder*2>
{
	void setup (int order,
		    double sampleRate,
		    double centerFrequency,
		    double widthFrequency,
		    double stopBandDb) {
		BandPassBase::setup (order,
				     sampleRate,
				     centerFrequency,
				     widthFrequency,
				     stopBandDb);
	}
	void setup (double sampleRate,
		    double centerFrequency,
		    double widthFrequency,
		    double stopBandDb) {
		BandPassBase::setup (MaxOrder,
				     sampleRate,
				     centerFrequency,
				     widthFrequency,
				     stopBandDb);
	}
};

/**
 * ChebyshevII bandstop filter. Init with "setup (
 *             double sampleRate,
 *             double centerFrequency,
 *             double widthFrequency,
 *             double minimumStopBandRejectionInDB);"
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport BandStop : PoleFilter <BandStopBase, StateType, MaxOrder, MaxOrder*2>
{
	void setup (int order,
		    double sampleRate,
		    double centerFrequency,
		    double widthFrequency,
		    double stopBandDb) {
		BandStopBase::setup (order,
				     sampleRate,
				     centerFrequency,
				     widthFrequency,
				     stopBandDb);
	}
	void setup (double sampleRate,
		    double centerFrequency,
		    double widthFrequency,
		    double stopBandDb) {
		BandStopBase::setup (MaxOrder,
				     sampleRate,
				     centerFrequency,
				     widthFrequency,
				     stopBandDb);
	}
};

/**
 * ChebyshevII low shelf filter. Init with "setup (
 *             double sampleRate,
 *             double cutoffFrequency,
 *             double gainDb,
 *             double minimumStopBandRejectionInDB);"
 **/
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport LowShelf : PoleFilter <LowShelfBase, StateType, MaxOrder>
{
	void setup (int order,
		    double sampleRate,
		    double cutoffFrequency,
		    double gainDb,
		    double stopBandDb) {
		LowShelfBase::setup (order,
				     sampleRate,
				     cutoffFrequency,
				     gainDb,
				     stopBandDb);
	}
	
	void setup (double sampleRate,
		    double cutoffFrequency,
		    double gainDb,
		    double stopBandDb) {
		LowShelfBase::setup (MaxOrder,
				     sampleRate,
				     cutoffFrequency,
				     gainDb,
				     stopBandDb);
	}
	
};

/**
 * ChebyshevII high shelf filter. Init with "setup (int order,
 *             double sampleRate,
 *             double cutoffFrequency,
 *             double gainDb,
 *             double minimumStopBandRejectionInDB);"
 **/
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport HighShelf : PoleFilter <HighShelfBase, StateType, MaxOrder>
{
	void setup (int order,
		    double sampleRate,
		    double cutoffFrequency,
		    double gainDb,
		    double stopBandDb) {
		HighShelfBase::setup (order,
				      sampleRate,
				      cutoffFrequency,
				      gainDb,
				      stopBandDb);
	}
	
	void setup (double sampleRate,
		    double cutoffFrequency,
		    double gainDb,
		    double stopBandDb) {
		HighShelfBase::setup (MaxOrder,
				      sampleRate,
				      cutoffFrequency,
				      gainDb,
				      stopBandDb);
	}
	
};

/**
 * ChebyshevII bandshelf filter. Init with "setup (
 *             double sampleRate,
 *             double centerFrequency,
 *             double widthFrequency,
 *             double gainDb,
 *             double minimumStopBandRejectionInDB);
 **/
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport BandShelf : PoleFilter <BandShelfBase, StateType, MaxOrder, MaxOrder*2>
{
	void setup (int order,
		    double sampleRate,
		    double centerFrequency,
		    double widthFrequency,
		    double gainDb,
		    double stopBandDb) {
		BandShelfBase::setup (order,
				      sampleRate,
				      centerFrequency,
				      widthFrequency,
				      gainDb,
				      stopBandDb);
	}
	  

	void setup (double sampleRate,
		    double centerFrequency,
		    double widthFrequency,
		    double gainDb,
		    double stopBandDb) {
		BandShelfBase::setup (MaxOrder,
				      sampleRate,
				      centerFrequency,
				      widthFrequency,
				      gainDb,
				      stopBandDb);
	}
	  

};

}

}

#endif

