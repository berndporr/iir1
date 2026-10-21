#include "Iir.h"

#include "scipy.h"
#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// ------------------------------------------------------------
// Filter configuration
// ------------------------------------------------------------

std::string filterBType2String (FilterBType type)
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
// Read a semicolon-separated CSV signal
// ------------------------------------------------------------

Signal readSignalFromCSV (const std::string filename)
{
    std::ifstream file (filename);

    if (!file)
    {
        throw std::runtime_error ("Cannot open " + filename);
    }

    Signal signal;
    std::string line;

    // Skip the header.
    if (!std::getline (file, line))
    {
        throw std::runtime_error ("Empty CSV: " + filename);
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
            throw std::runtime_error ("Malformed CSV row in " + filename);
        }

        signal.time.push_back (std::stod (timeString));
        signal.values.push_back (std::stod (valueString));
    }
    signal.calcSamplerate ();
    return signal;
}

// ------------------------------------------------------------
// Generate reference filename
// ------------------------------------------------------------

std::string generateFileName (const std::string filterName, FilterBType type,
                              int order)
{
    return filterName + "-" + filterBType2String (type) + "-Order_"
           + std::to_string (order) + ".csv";
}

// ------------------------------------------------------------
// Compare reference and actual signals
// ------------------------------------------------------------

void compareSignals (const Signal &reference, const Signal &actual)
{
    if (reference.size () != actual.size ())
    {
        const std::string msg
            = "FAIL: signal sizes differ: reference="
              + std::to_string (reference.size ())
              + ", actual=" + std::to_string (actual.size ());
        throw std::runtime_error (msg);
    }

    double maxError = 0;
    for (std::size_t i = 0; i < reference.size (); ++i)
    {
        const double expected = reference.values[i];
        const double actualValue = actual.values[i];
        const double e = std::abs (expected - actualValue);
        if (e > maxError)
            maxError = e;

        if (!std::isfinite (expected) || !std::isfinite (actualValue)
            || (e > tolerance))
        {
            const std::string msg
                = "FAIL at index " + std::to_string (i)
                  + ": reference=" + std::to_string (expected)
                  + ", actual=" + std::to_string (actualValue)
                  + ", difference=" + std::to_string (e);
            std::cerr << std::endl << msg << std::endl;
            throw std::runtime_error (msg);
        }
    }

    std::cerr << "max error=" << maxError << " -- PASS." << std::endl;
}

// ------------------------------------------------------------
// Run one test
// ------------------------------------------------------------

void runTest (const std::string filtername, const Signal &input,
              std::shared_ptr<IIRFilter> iirFilter, int order)
{
    if (input.size () < 2)
    {
        throw std::runtime_error (
            "Input signal must contain at least two samples");
    }

    const std::string filename
        = generateFileName (filtername, iirFilter->filterBtype, order);

    const Signal reference = readSignalFromCSV (filename);

    Signal actual;
    actual.time = input.time;
    actual.values.reserve (input.size ());

    for (double sample : input.values)
    {
        actual.values.push_back (iirFilter->filter (sample));
    }

    std::cerr << filtername << ", "
              << filterBType2String (iirFilter->filterBtype)
              << ", order=" << order << ": ";

    compareSignals (reference, actual);
}
