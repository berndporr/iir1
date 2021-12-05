#include "Iir.h"

#include <stdio.h>

#include "assert_print.h"

void assert_double(double a, double b) {
	const double d = fabs(a-b);
	if (d < 1E-5) {
		fprintf(stderr,"%e == %e, err = %e\n",a,b,d);		
		return;
	}
	char tmp[1024];
	sprintf(tmp,"Mismatch: %e != %e\n",a,b);
	fprintf(stderr,"%s",tmp);
	throw tmp;
}

void test1 () {
        const int nSOS = 1;
        const double coeff[nSOS][6] =
		{{ 0.02008337,  0.04016673,  0.02008337,
		   1.,         -1.56101808,  0.64135154}};
        Iir::Custom::SOSCascade<nSOS,Iir::DirectFormI> cust(coeff);
	assert_double(cust.filter(-1.0f),-2.00833656e-02);
	assert_double(cust.filter(0.5f),-6.14755450e-02);
	assert_double(cust.filter(1.0f),-6.30005740e-02);
}

void test2 () {
        const int nSOS = 1;
        const double coeff[nSOS][6] =
		{{ 0.02008337,  0.04016673,  0.02008337,
		   1.,         -1.56101808,  0.64135154}};
        Iir::Custom::SOSCascade<nSOS,Iir::DirectFormII> cust(coeff);
	assert_double(cust.filter(-1.0f),-2.00833656e-02);
	assert_double(cust.filter(0.5f),-6.14755450e-02);
	assert_double(cust.filter(1.0f),-6.30005740e-02);
}

void test3() {
        const int nSOS = 2;
        const double coeff[nSOS][6] =
		{{1.78260999e-03,  3.56521998e-03,  1.78260999e-03,
		  1.00000000e+00,  -1.25544047e+00, 4.09013783e-01},
		 {1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
		  1.00000000e+00,  -1.51824184e+00, 7.03962657e-01}};
        Iir::Custom::SOSCascade<nSOS,Iir::DirectFormI> cust(coeff);
	assert_double(cust.filter(-1.0f),-1.782610e-03);
	assert_double(cust.filter(0.5f),-1.118353e-02);
	assert_double(cust.filter(-1.0f),-3.455084e-02);
	assert_double(cust.filter(0.5f),-7.277369e-02);
	assert_double(cust.filter(-0.3f),-1.197387e-01);
	assert_double(cust.filter(3.0f),-1.588640e-01);
	assert_double(cust.filter(-1E-5f),-1.587363e-01);
}

void test4() {
        const int nSOS = 2;
        const double coeff[nSOS][6] =
		{{1.78260999e-03,  3.56521998e-03,  1.78260999e-03,
		  1.00000000e+00,  -1.25544047e+00, 4.09013783e-01},
		 {1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
		  1.00000000e+00,  -1.51824184e+00, 7.03962657e-01}};
        Iir::Custom::SOSCascade<nSOS,Iir::DirectFormII> cust(coeff);
	assert_double(cust.filter(-1.0f),-1.782610e-03);
	assert_double(cust.filter(0.5f),-1.118353e-02);
	assert_double(cust.filter(-1.0f),-3.455084e-02);
	assert_double(cust.filter(0.5f),-7.277369e-02);
	assert_double(cust.filter(-0.3f),-1.197387e-01);
	assert_double(cust.filter(3.0f),-1.588640e-01);
	assert_double(cust.filter(-1E-5f),-1.587363e-01);
}

int main (int,char**)
{
	test1();
	test2();
	test3();
	test4();

	return 0;
}
