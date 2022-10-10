// Usage Examples
//
// This updates the filter parameters while the
// filter is operating simulating a variable
// framerate for example.
//

// This is the only include you need
#include "Iir.h"

#include <stdio.h>

int main(int, char**)
{
	float samplingrate = 75;

	FILE *fimpulse = NULL;

	// RBJ lowpass filter with sampling rate updated
	// every second to see how the filter reacts.
	// The Q factor determines the resonance at the
	// cutoff frequency. If Q=1/sqrt(2) then the transition
	// has no resonance. The higher the Q-factor the higher
	// the resonance.
	Iir::RBJ::LowPass lp;
	const double lp_cutoff_frequency = 1;
	const double lp_qfactor = 5;
	lp.setup(samplingrate, lp_cutoff_frequency, lp_qfactor);
	fimpulse = fopen("rbj_update.dat", "wt");
	for (int i = 0; i < 10000; i++)
	{
		double a = 0;
		if (i > 10) a = 1;
		double b = lp.filter(a);
		fprintf(fimpulse, "%e\n", b);
		if ((i%75) == 0) {
			samplingrate = samplingrate + ((float)random()/(float)RAND_MAX)*10 - 5;
			printf("n=%d,fs=%f\n",i,samplingrate);
			lp.setup(samplingrate, lp_cutoff_frequency, lp_qfactor);
		}
	}
	fclose(fimpulse);
}
