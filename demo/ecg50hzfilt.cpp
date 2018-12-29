// Usage Examples
//
#include "Iir.h"

#include <stdio.h>

int main (int,char**)
{
	Iir::RBJ::IIRNotch iirnotch;
	iirnotch.setup(1000,50,10);

	FILE *finput = fopen("ecg50hz.dat","rt");
	FILE *foutput = fopen("ecg_filtered.dat","wt");
	for(;;) 
	{
		float a;
		if (fscanf(finput,"%f\n",&a)<1) break;
		a = iirnotch.filter(a);
		fprintf(foutput,"%f\n",a);
	}
	fclose(finput);
	fclose(foutput);
	fprintf(stderr,"Written the filtered ECG to 'ecg_filtered.dat'\n");
}
