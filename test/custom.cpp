#include "Iir.h"

#include <stdio.h>

#include "assert_print.h"

int main (int,char**)
{
	// generated with: "elliptic_design.py"
	const double coeff[][6] = {
		{1.665623674062209972e-02,
		 -3.924801366970616552e-03,
		 1.665623674062210319e-02,
		 1.000000000000000000e+00,
		 -1.715403014004022175e+00,
		 8.100474793174089472e-01},
		{1.000000000000000000e+00,
		 -1.369778997100624895e+00,
		 1.000000000000000222e+00,
		 1.000000000000000000e+00,
		 -1.605878925999785656e+00,
		 9.538657786383895054e-01}};

	Iir::Custom::SOSCascade<2> f(coeff);

	auto coeffRet = f.getCascadeStorage();
	for(int i = 0; i < 2; i++) {
	    assert_print(coeffRet.stageArray[i].m_a0 == coeff[i][3],"m_a0 mismatch.\n");
	    assert_print(coeffRet.stageArray[i].m_a1 == coeff[i][4],"m_a1 mismatch.\n");
	    assert_print(coeffRet.stageArray[i].m_a2 == coeff[i][5],"m_a2 mismatch.\n");
	    assert_print(coeffRet.stageArray[i].m_b0 == coeff[i][0],"m_b0 mismatch.\n");
	    assert_print(coeffRet.stageArray[i].m_b1 == coeff[i][1],"m_b1 mismatch.\n");
	    assert_print(coeffRet.stageArray[i].m_b2 == coeff[i][2],"m_b2 mismatch.\n");
	}

	double b = 0;
	double b2 = 0;
	for(int i=0;i<10000;i++) 
	{
		double a=0;
		if (i==10) a = 1;
		b2 = b;
		b = f.filter(a);
		assert_print(!isnan(b),"Custom filter output is NAN\n");
		if ((i>20) && (i<100)) {
			assert_print((b != 0) || (b2 != 0),
				     "Custom output is zero\n");
		}
	}
	fprintf(stderr,"Final filter value: %e\n",b);
	assert_print(fabs(b) < 1E-25,"Custom filter value for t->inf to high!");

	return 0;
}
