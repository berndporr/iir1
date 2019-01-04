#include "Iir.h"

#include <stdio.h>

#include "assert_print.h"

int main (int,char**)
{
	const int order = 3;
	Iir::ChebyshevI::HighPass<order> f;
	const float samplingrate = 1000; // Hz
	const float cutoff_frequency = 5; // Hz
	f.setup (samplingrate, cutoff_frequency, 1);
	double b;
	for(int i=0;i<100000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		b = f.filter(a);
		assert_print(!isnan(b),"Lowpass output is NAN\n");
	}
	//fprintf(stderr,"%e\n",b);
	assert_print(fabs(b) < 1E-15,"Lowpass value for t->inf to high!");

	Iir::ChebyshevI::BandStop<4,Iir::DirectFormI> bs;
	const float center_frequency = 50;
	const float frequency_width = 5;
	bs.setup (samplingrate, center_frequency, frequency_width, 1);
	bs.reset ();
	for(int i=0;i<100000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		b = bs.filter(a);
		assert_print(!isnan(b),"Bandstop output is NAN\n");
		//fprintf(stderr,"%e\n",b);
	}
	//fprintf(stderr,"%e\n",b);
	assert_print(fabs(b) < 1E-15,"Bandstop value for t->inf to high!");
	return 0;
}
