/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vinnie Falco and adapted by Bernd Porr

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
 * ChebyshevII lowpass filter. Init with "setup (int order,
 * double sampleRate,
 * double cutoffFrequency,
 * double stopbandDb);"
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport LowPass : PoleFilter <LowPassBase, StateType, MaxOrder>
{
};

/**
 * ChebyshevI highpass filter. Init with "setup(int order,
 *             double sampleRate,
 *             double cutoffFrequency,
 *             double minimumStopBandRejectionInDB);"
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport HighPass : PoleFilter <HighPassBase, StateType, MaxOrder>
{
};

/**
 * ChebyshevII bandpass filter. Init with "setup (int order,
 *             double sampleRate,
 *             double centerFrequency,
 *             double widthFrequency,
 *             double minimumStopBandRejectionInDB);"
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport BandPass : PoleFilter <BandPassBase, StateType, MaxOrder, MaxOrder*2>
{
};

/**
 * ChebyshevII bandstop filter. Init with "setup (int order,
 *             double sampleRate,
 *             double centerFrequency,
 *             double widthFrequency,
 *             double minimumStopBandRejectionInDB);"
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport BandStop : PoleFilter <BandStopBase, StateType, MaxOrder, MaxOrder*2>
{
};

/**
 * ChebyshevII low shelf filter. Init with "setup (int order,
 *             double sampleRate,
 *             double cutoffFrequency,
 *             double gainDb,
 *             double minimumStopBandRejectionInDB);"
 **/
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport LowShelf : PoleFilter <LowShelfBase, StateType, MaxOrder>
{
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
};

/**
 * ChebyshevII bandshelf filter. Init with "setup (int order,
 *             double sampleRate,
 *             double centerFrequency,
 *             double widthFrequency,
 *             double gainDb,
 *             double minimumStopBandRejectionInDB);
 **/
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport BandShelf : PoleFilter <BandShelfBase, StateType, MaxOrder, MaxOrder*2>
{
};

}

}

#endif

