// Usage Examples
//
// This shows you how to operate the filters
//

// This is the only include you need
#include "Iir.h"

#include <stdio.h>

int main (int,char**)
{
	// 4th order for all filters shown here
	const int order = 4;

	// sampling rate here 1kHz as an example
	const float samplingrate = 1000;

	FILE *fimpulse = NULL;

	// Butterworth lowpass
	Iir::Butterworth::LowPass<order> f;
	const float cutoff_frequency = 100; // Hz
	const float passband_gain = 10; // db
	f.setup (samplingrate, cutoff_frequency);
	fimpulse = fopen("lp.dat","wt");
	// let's simulated date streaming in
	for(int i=0;i<1000;i++) 
	{
		double a=0;
		if (i==10) a = 1;
		double b = f.filter(a);
		fprintf(fimpulse,"%e\n",b);
	}
	fclose(fimpulse);
	
	// Bandpass shelf filter (default 0dB execpt 10 dB in the passband)
	// Here the "direct form I" is chosen for the number crunching
	Iir::Butterworth::BandShelf<order,Iir::DirectFormI> bp;
	const float center_frequency = 100;
	const float frequency_width = 20;
	const float gain_in_passband = 10;
	bp.setup (samplingrate,
		  center_frequency,
		  frequency_width,
		  gain_in_passband);
	fimpulse = fopen("bp.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		double a=0;
		if (i==10) a = 1;
		double b = bp.filter(a);
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
	const float rolloff = 0.1F;
	lp_elliptic.setup (samplingrate,
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
	lp_cheby1.setup (samplingrate,
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
	lp_cheby2.setup (samplingrate,
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

	Iir::Bessel::BandPass<order> bp_bessel;
	bp_bessel.setup (samplingrate,
			 center_frequency, frequency_width);
	fimpulse = fopen("bp_bessel.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		double a=0;
		if (i==10) a = 1;
		double b = bp_bessel.filter(a);
		fprintf(fimpulse,"%e\n",b);
	}
	fclose(fimpulse);

	// Digital bandstop filter
        // This is a filter which as infinite damping
	// at the notch frequency because the zero of the
	// filter is placed there. A resonance at the notch
	// then tunes the width of the filter.
	// The higher the Q factor the narrow the notch
	// but the longer its impulse response (=ringing).
	Iir::RBJ::IIRNotch bsn;
	const float bs_frequency = 50;
	const float bs_q = 10;
	bsn.setup (samplingrate, bs_frequency, bs_q);
	fimpulse = fopen("bs_rbj.dat","wt");
	for(int i=0;i<1000;i++) 
	{
		double a=0;
		if (i==10) a = 1;
		double b = bsn.filter(a);
		fprintf(fimpulse,"%e\n",b);
	}
	fclose(fimpulse);

	printf("finished!\n");
	printf("Now run `plot_impulse_fresponse.py` to display the impulse/frequency responses.\n");
}
