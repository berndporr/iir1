/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vinnie Falco and adapted by Bernd Porr for Linux

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

#ifndef DSPFILTERS_BUTTERWORTH_H
#define DSPFILTERS_BUTTERWORTH_H

#include "Common.h"
#include "Cascade.h"
#include "PoleFilter.h"
#include "State.h"

namespace Iir {

/**
 * Filters with Butterworth response characteristics
 **/
namespace Butterworth {

// Half-band analog prototypes (s-plane)

class DllExport AnalogLowPass : public LayoutBase
{
public:
  AnalogLowPass ();

  void design (const int numPoles);

private:
  int m_numPoles;
};

//------------------------------------------------------------------------------

class DllExport AnalogLowShelf : public LayoutBase
{
public:
  AnalogLowShelf ();

  void design (int numPoles, double gainDb);

private:
  int m_numPoles;
  double m_gainDb;
};

//------------------------------------------------------------------------------

// Factored implementations to reduce template instantiations

struct DllExport LowPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency);
};

struct DllExport HighPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency);
};

struct DllExport BandPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency);
};

struct DllExport BandStopBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency);
};

struct DllExport LowShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double gainDb);
};

struct DllExport HighShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double gainDb);
};

struct DllExport BandShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              double gainDb);
};

//------------------------------------------------------------------------------

//
// Filters for the user
//

/** 
 * Butterworth Lowpass filter. Call the method "setup(int order,
 *             double sampleRate,
 *             double cutoffFrequency);".
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport LowPass : PoleFilter <LowPassBase, StateType, MaxOrder>
{
};

/** 
 * Butterworth Highpass filter. 
 * Call the method "void setup (int order,
 *             double sampleRate,
 *             double cutoffFrequency);".
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport HighPass : PoleFilter <HighPassBase, StateType, MaxOrder>
{
};

/**
 * Butterworth  Bandpass filter.
 * Call the method "setup (int order,
 *             double sampleRate,
 *             double centerFrequency,
 *             double widthFrequency);".
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport BandPass : PoleFilter <BandPassBase, StateType, MaxOrder, MaxOrder*2>
{
};


/**
 * Butterworth Bandstop filter. Call the method "setup (int order,
 *             double sampleRate,
 *             double cutoffFrequency);".
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport BandStop : PoleFilter <BandStopBase, StateType, MaxOrder, MaxOrder*2>
{
};

/**
 * Butterworth low shelf filter. Call the method "setup(int order,
 *             double sampleRate,
 *             double cutoffFrequency,
 *             double gainDb);
 **/
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport LowShelf : PoleFilter <LowShelfBase, StateType, MaxOrder>
{
};

/**
 * Butterhworth high shelf filter. Call the method "setup (int order,
 *             double sampleRate,
 *             double cutoffFrequency,
 *             double gainDb);"
 **/
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport HighShelf : PoleFilter <HighShelfBase, StateType, MaxOrder>
{
};

/**
 * Butterworth band shelf filter. Call the method "setup (int order,
 *             double sampleRate,
 *             double centerFrequency,
 *             double widthFrequency,
 *             double gainDb);"
 */
template <int MaxOrder, class StateType = DEFAULT_STATE>
struct DllExport BandShelf : PoleFilter <BandShelfBase, StateType, MaxOrder, MaxOrder*2>
{
};

}

}

#endif

