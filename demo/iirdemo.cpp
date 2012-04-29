// Usage Examples
//
// This shows you how to operate the filters
//

// This is the only include you need
#include "Iir.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>

int main (int,char**)
{
	const int order = 3;
	Iir::Butterworth::LowPass<order> f;
	const float samplingrate = 1000; // Hz
	const float cutoff_frequency = 5; // Hz
	f.setup (order, samplingrate, cutoff_frequency);
	f.reset ();
	FILE *fimpulse = fopen("lp.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		float b = f.filter(a);
		fprintf(fimpulse,"%f\n",b);
	}
	fclose(fimpulse);
	
	// calculate response at frequency 440 Hz
	Iir::complex_t response = f.response (440./44100);
	
	std::ostringstream os;

	// coefficients
	int n = f.getNumStages();
	os << "numStages = " << n << "\n";
	for(int i=0; i<n; i++)
	{
		os << "a0[" << i << "] = " << f[i].getA0 () << "\n"
		   << "a1[" << i << "] = " << f[i].getA1 () << "\n"
		   << "a2[" << i << "] = " << f[i].getA2 () << "\n"
		   << "b0[" << i << "] = " << f[i].getB0 () << "\n"
		   << "b1[" << i << "] = " << f[i].getB1 () << "\n"
		   << "b2[" << i << "] = " << f[i].getB2 () << "\n";
	}
	
	std::cout << os.str();


	// bandstop filter
	// here the "direct form I" is chosen for the number crunching
	Iir::Butterworth::BandStop<order,Iir::DirectFormI> bs;
	const float center_frequency = 50;
	const float frequency_width = 5;
	bs.setup (order, samplingrate, center_frequency, frequency_width);
	bs.reset ();
	FILE *fbs = fopen("bs.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		float b = bs.filter(a);
		fprintf(fbs,"%f\n",b);
	}
	fclose(fbs);
	


}
