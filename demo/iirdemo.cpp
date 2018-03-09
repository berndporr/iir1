// Usage Examples
//
// This shows you how to operate the filters
//

// This is the only include you need
#include "Iir.h"

#include <stdio.h>

int main (int,char**)
{
	const int order = 4;
	Iir::Butterworth::LowPass<order> f;
	const float samplingrate = 1000; // Hz
	const float cutoff_frequency = 100; // Hz
	f.setup (order, samplingrate, cutoff_frequency);
	FILE *fimpulse = fopen("lp.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		double a=0;
		if (i==10) a = 1;
		double b = f.filter(a);
		fprintf(fimpulse,"%e\n",b);
	}
	fclose(fimpulse);
	
	// bandstop filter
	// here the "direct form I" is chosen for the number crunching
	Iir::Butterworth::BandStop<order,Iir::DirectFormI> bs;
	const float center_frequency = 50;
	const float frequency_width = 5;
	bs.setup (order, samplingrate, center_frequency, frequency_width);
	fimpulse = fopen("bs.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		double a=0;
		if (i==10) a = 1;
		double b = bs.filter(a);
		fprintf(fimpulse,"%e\n",b);
	}
	fclose(fimpulse);
	
	// RBJ highpass filter
	// The Q factor determines the resonance at the
	// cutoff frequency. If Q=1/sqrt(2) then the transition
	// has no resonance. The higher the Q-factor the higher
	// the resonance.
	Iir::RBJ::HighPass hp;
	const float hp_cutoff_frequency = 100;
	const float hp_qfactor = 5;
	hp.setup (samplingrate, hp_cutoff_frequency, hp_qfactor);
	fimpulse = fopen("hp_rbj.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		double a=0;
		if (i==10) a = 1;
		double b = hp.filter(a);
		fprintf(fimpulse,"%e\n",b);
	}
	fclose(fimpulse);

	Iir::Elliptic::LowPass<order> lp_elliptic;
	const float pass_ripple_db = 5; // dB
	const float rolloff = 0.1;
	lp_elliptic.setup (order,
			   samplingrate,
			   cutoff_frequency,
			   pass_ripple_db,
			   rolloff);
	fimpulse = fopen("lp_elliptic.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		double a=0;
		if (i==10) a = 1;
		double b = lp_elliptic.filter(a);
		fprintf(fimpulse,"%e\n",b);
	}
	fclose(fimpulse);

	Iir::ChebyshevI::LowPass<order> lp_cheby1;
	lp_cheby1.setup (order,
			 samplingrate,
			 cutoff_frequency,
			 pass_ripple_db);
	fimpulse = fopen("lp_cheby1.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		double a=0;
		if (i==10) a = 1;
		double b = lp_cheby1.filter(a);
		fprintf(fimpulse,"%e\n",b);
	}
	fclose(fimpulse);

	Iir::ChebyshevII::LowPass<order> lp_cheby2;
	double stop_ripple_dB = 20;
	lp_cheby2.setup (order,
			 samplingrate,
			 cutoff_frequency,
			 stop_ripple_dB);
	fimpulse = fopen("lp_cheby2.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		double a=0;
		if (i==10) a = 1;
		double b = lp_cheby2.filter(a);
		fprintf(fimpulse,"%e\n",b);
	}
	fclose(fimpulse);

}
