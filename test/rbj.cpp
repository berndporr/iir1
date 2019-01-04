#include "Iir.h"

#include <stdio.h>

#include "assert_print.h"

int main (int,char**)
{
	Iir::RBJ::LowPass f;
	const float samplingrate = 1000; // Hz
	const float cutoff_frequency = 5; // Hz
	const float qfactor = 1;
	f.setup (samplingrate, cutoff_frequency,qfactor);
	double b;
	for(int i=0;i<10000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		b = f.filter(a);
		//fprintf(stdout,"%e\n",b);
		assert_print(!isnan(b),"Lowpass output is NAN\n");
	}
	fprintf(stderr,"%e\n",b);
	assert_print(fabs(b) < 1E-15,"Lowpass value for t->inf to high!");

	Iir::RBJ::BandStop bs;
	const float center_frequency = 50;
	const float frequency_width = 5;
	bs.setup (samplingrate, center_frequency, frequency_width);
	for(int i=0;i<10000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		//b = bs.filter1(a);
		assert_print(!isnan(b),"Bandstop output is NAN\n");
		//fprintf(stderr,"%e\n",b);
	}
	fprintf(stderr,"%e\n",b);
	assert_print(fabs(b) < 1E-15,"Bandstop value for t->inf to high!");
	return 0;
}
