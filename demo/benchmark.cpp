// Benchmark test
// Feel free to experiment around with -O2,-O3,-march=native
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include "Iir.h"

#include <stdio.h>

const int n = 10000;
double ecg[n];

long us() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main (int,char**)
{
	const float fs = 1000;
        const float mains = 50;
	Iir::RBJ::IIRNotch iirnotch;
	iirnotch.setup(fs,mains);

        const float fhp = 0.5;
	Iir::Butterworth::HighPass<8> hp;
	hp.setup(fs,fhp);

	FILE *finput = fopen("ecg50hz.dat","rt");
	for(int i = 0; i < n; i++) {
		int r = fscanf(finput,"%lf\n",ecg+i);
		if (r < 0) {
			throw "Could not read ECG file";
		}
	}
	fclose(finput);

	long t1 = us();
	for(int j = 0; j < 10000; j++) {
		for(int i = 0; i < n; i++) {
			double a = hp.filter(ecg[i]);
			ecg[i] = iirnotch.filter(a);
		}
	}
	long t2 = us();
	printf("Duration in us: %ld\n",t2-t1);
}
