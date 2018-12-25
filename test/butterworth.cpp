#include "Iir.h"

#include <stdio.h>

#include "assert_print.h"

int main (int,char**)
{
	// setting up non-optimal order to test how an
	// underutilised filter deals with it.
	const int order = 3;

	// create the filter structure for 3rd order
	Iir::Butterworth::LowPass<order> f;

	// filter parameters
	const float samplingrate = 1000; // Hz
	const float cutoff_frequency = 5; // Hz

	// calc the coefficients
	f.setup (samplingrate, cutoff_frequency);
	
	double b;
	for(int i=0;i<10000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		b = f.filter(a);
		assert_print(!isnan(b),"Lowpass output is NAN\n");
	}
	//fprintf(stderr,"%e\n",b);
	assert_print(fabs(b) < 1E-30,"Lowpass value for t->inf to high!");

	Iir::Butterworth::BandStop<order,Iir::DirectFormI> bs;
	const float center_frequency = 50;
	const float frequency_width = 5;
	bs.setup (samplingrate, center_frequency, frequency_width);
	bs.reset ();
	for(int i=0;i<10000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		b = bs.filter(a);
		assert_print(!isnan(b),"Bandstop output is NAN\n");
		//fprintf(stderr,"%e\n",b);
	}
	assert_print(fabs(b) < 1E-30,"Bandstop value for t->inf to high!");
	//fprintf(stderr,"%e\n",b);
	return 0;
}
