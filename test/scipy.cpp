#include "Iir.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// ------------------------------------------------------------
// Signal
// ------------------------------------------------------------

struct Signal
{
    std::vector<double> time;
    std::vector<double> values;

    std::size_t size () const { return values.size (); }
};

// ------------------------------------------------------------
// Filter configuration
// ------------------------------------------------------------

enum class FilterBType
{
    Lowpass,
    Highpass,
    Bandpass,
    Bandstop,
    NONE
};

std::string toString (FilterBType type)
{
    switch (type)
    {
    case FilterBType::Lowpass:
        return "lowpass";
    case FilterBType::Highpass:
        return "highpass";
    case FilterBType::Bandpass:
        return "bandpass";
    case FilterBType::Bandstop:
        return "bandstop";
    case FilterBType::NONE:
        throw std::runtime_error ("BUG: filter type not init'd.");
    }

    throw std::runtime_error ("Unknown filter type");
}

// ------------------------------------------------------------
// Common filter interface
// ------------------------------------------------------------

struct IIRFilter
{
    virtual ~IIRFilter () = default;
    virtual double filter (double input) = 0;
    FilterBType filterBtype = FilterBType::NONE;
};

// ------------------------------------------------------------
// Adapter for an IIR1 filter
// ------------------------------------------------------------

template <typename Filter> struct IIR1Adapter final : IIRFilter
{
    template <typename... Args>
    explicit IIR1Adapter (FilterBType ft, Args... args)
    {
        filterBtype = ft;
        filter_.setup (args...);
    }

    double filter (double input) override { return filter_.filter (input); }

    Filter filter_;
};

// ------------------------------------------------------------
// Read a semicolon-separated CSV signal
// ------------------------------------------------------------

Signal readSignalFromCSV (const fs::path &filename)
{
    std::ifstream file (filename);

    if (!file)
    {
        throw std::runtime_error ("Cannot open " + filename.string ());
    }

    Signal signal;
    std::string line;

    // Skip the header.
    if (!std::getline (file, line))
    {
        throw std::runtime_error ("Empty CSV: " + filename.string ());
    }

    while (std::getline (file, line))
    {
        if (line.empty ())
        {
            continue;
        }

        std::istringstream row (line);
        std::string timeString;
        std::string valueString;

        if (!std::getline (row, timeString, ';')
            || !std::getline (row, valueString, ';'))
        {
            throw std::runtime_error ("Malformed CSV row in "
                                      + filename.string ());
        }

        signal.time.push_back (std::stod (timeString));
        signal.values.push_back (std::stod (valueString));
    }

    return signal;
}

// ------------------------------------------------------------
// Generate reference filename
// ------------------------------------------------------------

std::string generateFileName (std::string filterName, FilterBType type, int order, double lc,
                              double hc)
{
    return filterName + "-" + toString (type) + "-LC_"
           + std::to_string (static_cast<int> (lc)) + "-HC_"
           + std::to_string (static_cast<int> (hc)) + "-Order_"
           + std::to_string (order) + ".csv";
}

// ------------------------------------------------------------
// Compare reference and actual signals
// ------------------------------------------------------------

bool compareSignals (const Signal &reference, const Signal &actual,
                     double tolerance)
{
    if (reference.size () != actual.size ())
    {
        std::cerr << "FAIL: signal sizes differ: reference="
                  << reference.size () << ", actual=" << actual.size ()
                  << '\n';

        return false;
    }

    for (std::size_t i = 0; i < reference.size (); ++i)
    {
        const double expected = reference.values[i];
        const double actualValue = actual.values[i];

        if (!std::isfinite (expected) || !std::isfinite (actualValue)
            || std::abs (expected - actualValue) > tolerance)
        {

            std::cerr << std::setprecision (12) << "FAIL at index " << i
                      << ": reference=" << expected
                      << ", actual=" << actualValue
                      << ", difference=" << std::abs (expected - actualValue)
                      << '\n';

            return false;
        }
    }

    return true;
}

// ------------------------------------------------------------
// Run one test
// ------------------------------------------------------------

bool runTest (std::string filtername, const Signal &input, std::shared_ptr<IIRFilter> iirFilter,
              int order, double lc, double hc)
{
    constexpr double tolerance = 1e-5;

    if (input.size () < 2)
    {
        throw std::runtime_error (
            "Input signal must contain at least two samples");
    }

    const std::string filename
        = generateFileName (filtername, iirFilter->filterBtype, order, lc, hc);

    const Signal reference = readSignalFromCSV (fs::path ("scipy") / filename);

    Signal actual;
    actual.time = input.time;
    actual.values.reserve (input.size ());

    for (double sample : input.values)
    {
        actual.values.push_back (iirFilter->filter (sample));
    }

    std::cout << filename << ", " << toString (iirFilter->filterBtype)
              << ", order=" << order << ": ";

    const bool passed = compareSignals (reference, actual, tolerance);

    std::cout << (passed ? "PASS" : "FAIL") << '\n';

    return passed;
}

// ------------------------------------------------------------
// Butterworth
// ------------------------------------------------------------

template <int Order>
std::shared_ptr<IIRFilter>
createButterworth (FilterBType type, double sampleRate, double lc, double hc)
{
    const double center = (lc + hc) / 2.0;
    const double width = hc - lc;

    switch (type)
    {
    case FilterBType::Lowpass:
        return std::make_shared<
            IIR1Adapter<Iir::Butterworth::LowPass<Order> > > (type, sampleRate,
                                                              hc);

    case FilterBType::Highpass:
        return std::make_shared<
            IIR1Adapter<Iir::Butterworth::HighPass<Order> > > (type,
                                                               sampleRate, lc);

    case FilterBType::Bandpass:
        return std::make_shared<
            IIR1Adapter<Iir::Butterworth::BandPass<Order> > > (
            type, sampleRate, center, width);

    case FilterBType::Bandstop:
        return std::make_shared<
            IIR1Adapter<Iir::Butterworth::BandStop<Order> > > (
            type, sampleRate, center, width);

    case FilterBType::NONE:
        throw std::runtime_error ("Filter type not set.");
    }

    throw std::runtime_error ("Unknown filter configuration");
}

std::shared_ptr<IIRFilter> createFixedOrderButterworth (FilterBType type,
                                                        int order,
                                                        double sampleRate,
                                                        double lc, double hc)
{
    switch (order)
    {
    case 1:
        return createButterworth<1> (type, sampleRate, lc, hc);

    case 2:
        return createButterworth<2> (type, sampleRate, lc, hc);

    case 4:
        return createButterworth<4> (type, sampleRate, lc, hc);

    case 5:
        return createButterworth<5> (type, sampleRate, lc, hc);

    case 10:
        return createButterworth<10> (type, sampleRate, lc, hc);

    default:
        throw std::invalid_argument ("Unsupported filter order: "
                                     + std::to_string (order));
    }
}

int testButterworth ()
{
    constexpr double lc = 200.0;
    constexpr double hc = 300.0;
    const std::string filtername {"butterworth"};

    const std::vector<int> orders = { 1, 2, 4, 5, 10 };

    const std::vector<FilterBType> types
        = { FilterBType::Lowpass, FilterBType::Highpass, FilterBType::Bandpass,
            FilterBType::Bandstop };

    int passed = 0;
    int failed = 0;
    int errors = 0;

    try
    {
        const Signal input = readSignalFromCSV ("scipy/signal.txt");
        const double dt = input.time[1] - input.time[0];

        if (dt <= 0.0)
        {
            throw std::runtime_error ("Invalid sampling interval");
        }

        const double sampleRate = 1.0 / dt;

        for (FilterBType type : types)
        {
            for (int order : orders)
            {
                try
                {
                    auto filter = createFixedOrderButterworth (
                        type, order, sampleRate, lc, hc);

                    if (runTest (filtername, input, filter, order, lc, hc))
                    {
                        ++passed;
                    }
                    else
                    {
                        ++failed;
                    }
                }
                catch (const std::exception &e)
                {
                    ++errors;

                    std::cerr << "ERROR: " << toString (type)
                              << ", order=" << order << ": " << e.what ()
                              << '\n';
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what () << '\n';
        return 1;
    }

    std::cout << "Passed: " << passed << '\n'
              << "Failed: " << failed << '\n'
              << "Errors: " << errors << '\n'
              << "Total:  " << passed + failed + errors << '\n';

    return (failed == 0 && errors == 0) ? 0 : 1;
}






// ------------------------------------------------------------
// Chebyshev1
// ------------------------------------------------------------

template <int Order>
std::shared_ptr<IIRFilter>
createChebychev1 (FilterBType type, double sampleRate, double lc, double hc)
{
    const double center = (lc + hc) / 2.0;
    const double width = hc - lc;
    const double rippledB = 1;

    switch (type)
    {
    case FilterBType::Lowpass:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevI::LowPass<Order> > > (type, sampleRate,
                                                              hc, rippledB);

    case FilterBType::Highpass:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevI::HighPass<Order> > > (type,
                                                               sampleRate, lc, rippledB);

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
                                                        double sampleRate,
                                                        double lc, double hc)
{
    switch (order)
    {
    case 1:
        return createChebychev1<1> (type, sampleRate, lc, hc);

    case 2:
        return createChebychev1<2> (type, sampleRate, lc, hc);

    case 4:
        return createChebychev1<4> (type, sampleRate, lc, hc);

    case 5:
        return createChebychev1<5> (type, sampleRate, lc, hc);

    case 10:
        return createChebychev1<10> (type, sampleRate, lc, hc);

    default:
        throw std::invalid_argument ("Unsupported filter order: "
                                     + std::to_string (order));
    }
}

int testChebychev1 ()
{
    constexpr double lc = 200.0;
    constexpr double hc = 300.0;
    const std::string filtername {"chebychev1"};

    const std::vector<int> orders = { 1, 2, 4, 5, 10 };

    const std::vector<FilterBType> types
        = { FilterBType::Lowpass, FilterBType::Highpass, FilterBType::Bandpass,
            FilterBType::Bandstop };

    int passed = 0;
    int failed = 0;
    int errors = 0;

    try
    {
        const Signal input = readSignalFromCSV ("scipy/signal.txt");
        const double dt = input.time[1] - input.time[0];

        if (dt <= 0.0)
        {
            throw std::runtime_error ("Invalid sampling interval");
        }

        const double sampleRate = 1.0 / dt;

        for (FilterBType type : types)
        {
            for (int order : orders)
            {
                try
                {
                    auto filter = createFixedOrderChebychev1 (
                        type, order, sampleRate, lc, hc);

                    if (runTest (filtername, input, filter, order, lc, hc))
                    {
                        ++passed;
                    }
                    else
                    {
                        ++failed;
                    }
                }
                catch (const std::exception &e)
                {
                    ++errors;

                    std::cerr << "ERROR: " << toString (type)
                              << ", order=" << order << ": " << e.what ()
                              << '\n';
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what () << '\n';
        return 1;
    }

    std::cout << "Passed: " << passed << '\n'
              << "Failed: " << failed << '\n'
              << "Errors: " << errors << '\n'
              << "Total:  " << passed + failed + errors << '\n';

    return (failed == 0 && errors == 0) ? 0 : 1;
}







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
            IIR1Adapter<Iir::ChebyshevII::HighPass<Order> > > (type,
                                                               sampleRate, lc, rippledB);

    case FilterBType::Bandpass:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevII::BandPass<Order> > > (
            type, sampleRate, center, width, rippledB);

    case FilterBType::Bandstop:
        return std::make_shared<
            IIR1Adapter<Iir::ChebyshevII::BandStop<Order> > > (
            type, sampleRate, center, width, rippledB);

    case FilterBType::NONE:
        throw std::runtime_error ("Filter type not set.");
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

int testChebychev2 ()
{
    constexpr double lc = 200.0;
    constexpr double hc = 300.0;
    const std::string filtername {"chebychev2"};

    const std::vector<int> orders = { 1, 2, 4, 5, 10 };

    const std::vector<FilterBType> types
        = { FilterBType::Lowpass, FilterBType::Highpass, FilterBType::Bandpass,
            FilterBType::Bandstop };

    int passed = 0;
    int failed = 0;
    int errors = 0;

    try
    {
        const Signal input = readSignalFromCSV ("scipy/signal.txt");
        const double dt = input.time[1] - input.time[0];

        if (dt <= 0.0)
        {
            throw std::runtime_error ("Invalid sampling interval");
        }

        const double sampleRate = 1.0 / dt;

        for (FilterBType type : types)
        {
            for (int order : orders)
            {
                try
                {
                    auto filter = createFixedOrderChebychev2 (
                        type, order, sampleRate, lc, hc);

                    if (runTest (filtername, input, filter, order, lc, hc))
                    {
                        ++passed;
                    }
                    else
                    {
                        ++failed;
                    }
                }
                catch (const std::exception &e)
                {
                    ++errors;

                    std::cerr << "ERROR: " << toString (type)
                              << ", order=" << order << ": " << e.what ()
                              << '\n';
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what () << '\n';
        return 1;
    }

    std::cout << "Passed: " << passed << '\n'
              << "Failed: " << failed << '\n'
              << "Errors: " << errors << '\n'
              << "Total:  " << passed + failed + errors << '\n';

    return (failed == 0 && errors == 0) ? 0 : 1;
}









int main ()
{
    int fails = 0;
    fails += testButterworth ();
    fails += testChebychev1();
    fails += testChebychev2();
    if (fails > 0)
        throw;
    return 0;
}