#include "Iir.h"

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>

#include "assert_print.h"
#include "scipy.h"


// ------------------------------------------------------------
// Chebyshev2
// ------------------------------------------------------------

template <int Order>
std::shared_ptr<IIRFilter>
createChebychev2 (FilterBType type, double sampleRate, double lc, double hc)
{
    const double center = (lc + hc) / 2.0;
    const double width = hc - lc;
    const double rippledB = 1;

    switch (type)
    {
    case FilterBType::Lowpass:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevII::LowPass<Order> > > (type, sampleRate,
                                                              hc, rippledB);

    case FilterBType::Highpass:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevII::HighPass<Order> > > (
            type, sampleRate, lc, rippledB);

    case FilterBType::Bandpass:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevII::BandPass<Order> > > (
            type, sampleRate, center, width, rippledB);

    case FilterBType::Bandstop:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevII::BandStop<Order> > > (
            type, sampleRate, center, width, rippledB);

    case FilterBType::NONE:
        throw std::runtime_error ("BUG: Filter type not set.");
    }

    throw std::runtime_error ("Unknown filter configuration");
}

std::shared_ptr<IIRFilter> createFixedOrderChebychev2 (FilterBType type,
                                                       int order,
                                                       double sampleRate,
                                                       double lc, double hc)
{
    switch (order)
    {
    case 1:
        return createChebychev2<1> (type, sampleRate, lc, hc);

    case 2:
        return createChebychev2<2> (type, sampleRate, lc, hc);

    case 4:
        return createChebychev2<4> (type, sampleRate, lc, hc);

    case 5:
        return createChebychev2<5> (type, sampleRate, lc, hc);

    case 10:
        return createChebychev2<10> (type, sampleRate, lc, hc);

    default:
        throw std::invalid_argument ("Unsupported filter order: "
                                     + std::to_string (order));
    }
}

int compare2scipy ()
{
    const std::string filtername{ "chebychev2" };
    try
    {
        const Signal input = readSignalFromCSV ("scipy/signal.txt");
        for (FilterBType type : types)
        {
            for (int order : orders)
            {
                auto filter = createFixedOrderChebychev2 (
                    type, order, input.sampleRate, lc, hc);
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
	// setting up non-optimal order to test how an
	// underutilised filter deals with it.
	const int order = 3;

	Iir::ChebyshevII::BandPass<order> f;

	const float samplingrate = 1000; // Hz
	f.setup(samplingrate, 100, 10, 20);
	f.reset();
	double b;
	for (int i = 0; i < 10000; i++)
	{
		float a = 0;
		if (i == 10) a = 1;
		b = f.filter(a);
		assert_print(!isnan(b), "Lowpass output is NAN\n");
	}
	//fprintf(stderr,"%e\n",b);
	assert_print(fabs(b) < 1E-15, "Lowpass value for t->inf to high!");

	Iir::ChebyshevII::BandStop<4, Iir::DirectFormI> bs;
	const float center_frequency = 50;
	const float frequency_width = 5;
	bs.setup(samplingrate, center_frequency, frequency_width, 20);
	bs.reset();
	for (int i = 0; i < 10000; i++)
	{
		float a = 0;
		if (i == 10) a = 1;
		b = bs.filter(a);
		assert_print(!isnan(b), "Bandstop output is NAN\n");
		//fprintf(stderr,"%e\n",b);
	}
	assert_print(fabs(b) < 1E-15, "Bandstop value for t->inf to high!");
	//fprintf(stderr,"%e\n",b);


	Iir::ChebyshevII::HighPass<8> hp_cheby2;
	double stop_ripple_dB = 80;
	// Setting cutoff to normalised f=0.1
	double normalised_cutoff_freq = 0.1;
	hp_cheby2.setupN(normalised_cutoff_freq,
		stop_ripple_dB);
	double norm_signal_frequency = 0.0507;
	for (int i = 0; i < 10000; i++)
	{
		b = fabs(hp_cheby2.filter(sin(2 * M_PI*norm_signal_frequency*i)));
		if (i > 5000) {
			assert_print(b < 1E-3, "Highpass not removing a sine.\n");
		}
	}

	return compare2scipy();
}
