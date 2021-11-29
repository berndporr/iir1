#include "Iir.h"

#include <stdio.h>

#include "assert_print.h"

void assert_float(float a, float b) {
	if (fabs(a-b) < 1E-30) {
		return;
	}
	fprintf(stderr,"Mismatch: %f != %f\n",a,b);
	assert(false);
}

void test1 () {
        const int nSOS = 1;
        const double coeff[nSOS][6] = {
                {1,
                 -1,
                 0,
                 1,
                 -2,
                 1}
        };
        Iir::Custom::SOSCascade<nSOS> cust(coeff);
	assert_float(cust.filter(5),5);
	assert_float(cust.filter(3),8);
	assert_float(cust.filter(-2),6);
}

void test2() {
        const int nSOS = 2;
        const double coeff[nSOS][6] = {
                {1,
                 -1,
                 1,
                 1,
                 0.5,
                 0.25},
                {-1,
                 1,
                 1,
                 1,
                 0.1,
                 0.1},
        };
        Iir::Custom::SOSCascade<nSOS> cust(coeff);
	assert_float(cust.filter(-1),1);
	assert_float(cust.filter(0.5),-3.1);
	assert_float(cust.filter(1),2);
	assert_float(cust.filter(0.5),0.689);
	assert_float(cust.filter(0.3),-2.4899);
	assert_float(cust.filter(-77),78);
	assert_float(cust.filter(1E-5),-199.64402900000002);
}

int main (int,char**)
{
	test1();
	test2();

	return 0;
}
