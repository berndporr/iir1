#include "Iir.h"

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>

#include "assert_print.h"
#include "scipy.h"


// ------------------------------------------------------------
// Chebyshev1
// ------------------------------------------------------------

template <int Order>
std::shared_ptr<IIRFilter>
createChebychev1 (FilterBType type, double sampleRate)
{
    switch (type)
    {
    case FilterBType::Lowpass:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevI::LowPass<Order> > > (type, sampleRate,
                                                             hc, rippledB);

    case FilterBType::Highpass:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevI::HighPass<Order> > > (type, sampleRate,
                                                              lc, rippledB);

    case FilterBType::Bandpass:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevI::BandPass<Order> > > (
            type, sampleRate, center, width, rippledB);

    case FilterBType::Bandstop:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevI::BandStop<Order> > > (
            type, sampleRate, center, width, rippledB);

    case FilterBType::NONE:
        throw std::runtime_error ("Filter type not set.");
    }

    throw std::runtime_error ("Unknown filter configuration");
}

std::shared_ptr<IIRFilter> createFixedOrderChebychev1 (FilterBType type,
                                                       int order,
                                                       double sampleRate)
{
    switch (order)
    {
    case 1:
        return createChebychev1<1> (type, sampleRate);

    case 2:
        return createChebychev1<2> (type, sampleRate);

    case 4:
        return createChebychev1<4> (type, sampleRate);

    case 5:
        return createChebychev1<5> (type, sampleRate);

    case 10:
        return createChebychev1<10> (type, sampleRate);

    default:
        throw std::invalid_argument ("Unsupported filter order: "
                                     + std::to_string (order));
    }
}

int compare2scipy()
{
    const std::string filtername{ "chebychev1" };
    try
    {
        const Signal input = readSignalFromCSV ("scipy/signal.txt");
        for (FilterBType type : types)
        {
            for (int order : orders)
            {
                auto filter = createFixedOrderChebychev1 (
                    type, order, input.sampleRate);
                runTest (filtername, input, filter, order, lc, hc);
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what () << std::endl;
        return 1;
    }
    return 0;
}




int main(int, char**)
{
	const int order = 3;
	Iir::ChebyshevI::HighPass<order> f;
	const float samplingrate = 1000; // Hz
	const float cutoff_frequency = 5; // Hz
	f.setup(samplingrate, cutoff_frequency, 1);
	double b;
	for (int i = 0; i < 100000; i++)
	{
		float a = 0;
		if (i == 10) a = 1;
		b = f.filter(a);
		assert_print(!isnan(b), "Highpass output is NAN\n");
	}
	//fprintf(stderr,"%e\n",b);
	assert_print(fabs(b) < 1E-15, "Highpass value for t->inf to high!");

	Iir::ChebyshevI::BandStop<4, Iir::DirectFormI> bs;
	const float center_frequency = 50;
	const float frequency_width = 5;
	bs.setup(samplingrate, center_frequency, frequency_width, 1);
	bs.reset();
	for (int i = 0; i < 100000; i++)
	{
		float a = 0;
		if (i == 10) a = 1;
		b = bs.filter(a);
		assert_print(!isnan(b), "Bandstop output is NAN\n");
		//fprintf(stderr,"%e\n",b);
	}
	//fprintf(stderr,"%e\n",b);
	assert_print(fabs(b) < 1E-15, "Bandstop value for t->inf to high!");

	Iir::ChebyshevI::LowPass<8> lp_cheby1;
	const float pass_ripple_db2 = 1; // dB
	double norm_cutoff_frequency = 0.01;
	lp_cheby1.setupN(norm_cutoff_frequency,
		pass_ripple_db2);
	double norm_signal_frequency = 0.097;
	for (int i = 0; i < 10000; i++)
	{
		b = fabs(lp_cheby1.filter(sin(2 * M_PI*norm_signal_frequency*i)));
		if (i > 5000) {
			assert_print(b < 1E-5, "Lowpass not removing a sine.\n");
		}
	}

	return compare2scipy();
}
