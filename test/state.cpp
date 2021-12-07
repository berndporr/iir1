#include "Iir.h"

#include <stdio.h>
#include<iostream>

#include "assert_print.h"

void assert_double(const double a, const double b) {
	const double d = fabs(a-b);
	const double epsilon = 1E-5;
	const double err1 = d/fabs(a);
	const double err2 = d/fabs(b);
	if ((err1 < epsilon) && (err2 < epsilon)) {
		fprintf(stderr,"%e == %e, err = %e,%e\n",a,b,err1,err2);
		return;
	}
	const std::string s = "Mismatch: " + std::to_string(a) + "!=" +
		std::to_string(b) + "\n";
	std::cerr << s;
	throw s;
}

void test1 () {
        const int nSOS = 1;
        const double coeff[nSOS][6] =
		{{ 0.02008337,  0.04016673,  0.02008337,
		   1.,         -1.56101808,  0.64135154}};
        Iir::Custom::SOSCascade<nSOS,Iir::DirectFormI> cust(coeff);
	assert_double(cust.filter(-1.0),-2.00833656e-02);
	assert_double(cust.filter(0.5),-6.14755450e-02);
	assert_double(cust.filter(1.0),-6.30005740e-02);
}

void test2 () {
        const int nSOS = 1;
        const double coeff[nSOS][6] =
		{{ 0.02008337,  0.04016673,  0.02008337,
		   1.,         -1.56101808,  0.64135154}};
        Iir::Custom::SOSCascade<nSOS,Iir::DirectFormII> cust(coeff);
	assert_double(cust.filter(-1.0),-2.00833656e-02);
	assert_double(cust.filter(0.5),-6.14755450e-02);
	assert_double(cust.filter(1.0),-6.30005740e-02);
}

void test3() {
        const int nSOS = 2;
        const double coeff[nSOS][6] =
		{{1.78260999e-03,  3.56521998e-03,  1.78260999e-03,
		  1.00000000e+00,  -1.25544047e+00, 4.09013783e-01},
		 {1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
		  1.00000000e+00,  -1.51824184e+00, 7.03962657e-01}};
        Iir::Custom::SOSCascade<nSOS,Iir::DirectFormI> cust(coeff);
	assert_double(cust.filter(-1.0),-1.782610e-03);
	assert_double(cust.filter(0.5),-1.118353e-02);
	assert_double(cust.filter(-1.0),-3.455084e-02);
	assert_double(cust.filter(0.5),-7.277369e-02);
	assert_double(cust.filter(-0.3),-1.197387e-01);
	assert_double(cust.filter(3.0),-1.588640e-01);
	assert_double(cust.filter(-1.0E-5),-1.587363e-01);
}

void test4() {
        const int nSOS = 2;
        const double coeff[nSOS][6] =
		{{1.78260999e-03,  3.56521998e-03,  1.78260999e-03,
		  1.00000000e+00,  -1.25544047e+00, 4.09013783e-01},
		 {1.00000000e+00,  2.00000000e+00,  1.00000000e+00,
		  1.00000000e+00,  -1.51824184e+00, 7.03962657e-01}};
        Iir::Custom::SOSCascade<nSOS,Iir::DirectFormII> cust(coeff);
	assert_double(cust.filter(-1.0),-1.782610e-03);
	assert_double(cust.filter(0.5),-1.118353e-02);
	assert_double(cust.filter(-1.0),-3.455084e-02);
	assert_double(cust.filter(0.5),-7.277369e-02);
	assert_double(cust.filter(-0.3),-1.197387e-01);
	assert_double(cust.filter(3.0),-1.588640e-01);
	assert_double(cust.filter(-1.0E-5),-1.587363e-01);
}

int main (int,char**)
{
	test1();
	test2();
	test3();
	test4();

	return 0;
}
