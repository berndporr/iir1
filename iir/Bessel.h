/**
 *
 * "A Collection of Useful C++ Classes for Digital Signal Processing"
 * By Vinnie Falco and Bernd Porr
 *
 * Official project location:
 * https://github.com/berndporr/iir1
 *
 * See Documentation.txt for contact information, notes, and bibliography.
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

#ifndef IIR1_BESSEL_H
#define IIR1_BESSEL_H

#include "Common.h"
#include "Cascade.h"
#include "PoleFilter.h"
#include "RootFinder.h"
#include "State.h"

namespace Iir {

/**
 * Filters with Bessel response characteristics
 *
 **/
namespace Bessel {

// A Workspace is necessary to find roots

struct WorkspaceBase
{
  WorkspaceBase (RootFinderBase* rootsBase)
    : roots (*rootsBase)
  {
  }

  RootFinderBase& roots;

private:
  WorkspaceBase (WorkspaceBase&);
  WorkspaceBase& operator= (WorkspaceBase&);
};

template <int FilterOrder>
struct Workspace : WorkspaceBase
{
  Workspace ()
    : WorkspaceBase (&m_roots)
  {
  }

private:
  RootFinder <FilterOrder> m_roots;
};

//------------------------------------------------------------------------------

// Half-band analog prototypes (s-plane)

class DllExport AnalogLowPass : public LayoutBase
{
public:
  AnalogLowPass ();

  void design (const int numPoles,
               WorkspaceBase* w);

private:
  int m_numPoles;
};

//------------------------------------------------------------------------------

class DllExport AnalogLowShelf : public LayoutBase
{
public:
  AnalogLowShelf ();

  void design (int numPoles,
               double gainDb,
               WorkspaceBase* w);

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
              double cutoffFrequency,
              WorkspaceBase* w);
};

struct DllExport HighPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              WorkspaceBase* w);
};

struct DllExport BandPassBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              WorkspaceBase* w);
};

struct DllExport BandStopBase : PoleFilterBase <AnalogLowPass>
{
  void setup (int order,
              double sampleRate,
              double centerFrequency,
              double widthFrequency,
              WorkspaceBase* w);
};

struct DllExport LowShelfBase : PoleFilterBase <AnalogLowShelf>
{
  void setup (int order,
              double sampleRate,
              double cutoffFrequency,
              double gainDb,
              WorkspaceBase* w);
};

//------------------------------------------------------------------------------

//
// Userland filters
//
/**
 * Bessel Lowpass
 * \param FilterOrder The order of the filter.
 * \param StateType The filter topology: DirectFormI, DirectFormII, ...
 **/
template <int FilterOrder, class StateType = DEFAULT_STATE>
struct DllExport LowPass : PoleFilter <LowPassBase, StateType, FilterOrder>
{
	/**
         * Calculate the coefficients
         * \param sampleRate Sampling rate
         * \param cutoffFrequency Cutoff frequency
         **/
	void setup (double sampleRate,
		    double cutoffFrequency)
	{
		Workspace <FilterOrder> w;
		LowPassBase::setup (FilterOrder,
				    sampleRate,
				    cutoffFrequency,
				    &w);
	}
	
	void setup (int order,
		    double sampleRate,
		    double cutoffFrequency)
	{
		Workspace <FilterOrder> w;
		LowPassBase::setup (order,
				    sampleRate,
				    cutoffFrequency,
				    &w);
	}
	
};

/**
 * Bessel Highpass.
 * \param FilterOrder The order of the filter.
 * \param StateType The filter topology: DirectFormI, DirectFormII, ...
 **/
template <int FilterOrder, class StateType = DEFAULT_STATE>
	struct DllExport HighPass : PoleFilter <HighPassBase, StateType, FilterOrder>
{
	/**
         * Calculate the coefficients
         * \param sampleRate Sampling rate
         * \param cutoffFrequency Cutoff frequency
         **/
	void setup (double sampleRate,
		    double cutoffFrequency)
	{
		Workspace <FilterOrder> w;
		HighPassBase::setup (FilterOrder,
				     sampleRate,
				     cutoffFrequency,
				     &w);
	}

	void setup (int order,
		    double sampleRate,
		    double cutoffFrequency)
	{
		Workspace <FilterOrder> w;
		HighPassBase::setup (order,
				     sampleRate,
				     cutoffFrequency,
				     &w);
	}

};

/**
 * Bessel bandpass.
 * \param FilterOrder The order of the filter.
 * \param StateType The filter topology: DirectFormI, DirectFormII, ...
 **/
template <int FilterOrder, class StateType = DEFAULT_STATE>
	struct DllExport BandPass : PoleFilter <BandPassBase, StateType, FilterOrder, FilterOrder*2>
{
	/**
	 * Calculate the coefficients
	 * \param sampleRate Sampling rate
	 * \param centerFrequency Center frequency of the bandpass in Hz
         * \param widthFrequency Width of the bandpass in Hz
	 **/
	void setup (double sampleRate,
		    double centerFrequency,
		    double widthFrequency) {
		BandPass::setup (FilterOrder,
				 sampleRate,
				 centerFrequency,
				 widthFrequency);
	}
	
	void setup (int order,
		    double sampleRate,
		    double centerFrequency,
		    double widthFrequency)
	{
		Workspace <FilterOrder> w;
		BandPassBase::setup (order,
				     sampleRate,
				     centerFrequency,
				     widthFrequency,
				     &w);
	}
	
};


/**
 * Bessel bandstop.
 * \param FilterOrder The order of the filter.
 * \param StateType The filter topology: DirectFormI, DirectFormII, ...
 **/
template <int FilterOrder, class StateType = DEFAULT_STATE>
	struct DllExport BandStop : PoleFilter <BandStopBase, StateType, FilterOrder, FilterOrder*2>
{
	/**
	 * Calculate the coefficients
	 * \param sampleRate Sampling rate
	 * \param centerFrequency Center frequency of the bandpass in Hz
         * \param widthFrequency Width of the bandpass in Hz
	 **/
	void setup (double sampleRate,
		    double centerFrequency,
		    double widthFrequency)
	{
		Workspace <FilterOrder> w;
		BandStopBase::setup (FilterOrder,
				     sampleRate,
				     centerFrequency,
				     widthFrequency,
				     &w);
	}

	void setup (int order,
		    double sampleRate,
		    double centerFrequency,
		    double widthFrequency)
	{
		Workspace <FilterOrder> w;
		BandStopBase::setup (order,
				     sampleRate,
				     centerFrequency,
				     widthFrequency,
				     &w);
	}
	
};

}

}

#endif
