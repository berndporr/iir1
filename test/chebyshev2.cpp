#include "Iir.h"

#include <stdio.h>

#include "assert_print.h"

int main (int,char**)
{
	// setting up non-optimal order to test how an
	// underutilised filter deals with it.
	const int order = 3;
	
	Iir::ChebyshevII::BandPass<order> f;
	
	const float samplingrate = 1000; // Hz
	f.setup (samplingrate, 100, 10, 20);
	f.reset ();
	double b;
	for(int i=0;i<10000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		b = f.filter(a);
		assert_print(!isnan(b),"Lowpass output is NAN\n");
	}
	//fprintf(stderr,"%e\n",b);
	assert_print(fabs(b) < 1E-15,"Lowpass value for t->inf to high!");

	Iir::ChebyshevII::BandStop<4,Iir::DirectFormI> bs;
	const float center_frequency = 50;
	const float frequency_width = 5;
	bs.setup (samplingrate, center_frequency, frequency_width, 20);
	bs.reset ();
	for(int i=0;i<10000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		b = bs.filter(a);
		assert_print(!isnan(b),"Bandstop output is NAN\n");
		//fprintf(stderr,"%e\n",b);
	}
	assert_print(fabs(b) < 1E-15,"Bandstop value for t->inf to high!");
	//fprintf(stderr,"%e\n",b);
	return 0;
}
