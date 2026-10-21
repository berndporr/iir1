#include "Iir.h"

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

#pragma once

constexpr double lc = 200.0;
constexpr double hc = 300.0;
constexpr double rippledB = 1;
constexpr double center = (lc + hc) / 2.0;
constexpr double width = hc - lc;
constexpr std::array<int, 5> orders = { 1, 2, 4, 5, 10 };
constexpr double tolerance = 1e-5;
const std::string inputSignalFilename = "signal.csv";

enum class FilterBType
{
    Lowpass,
    Highpass,
    Bandpass,
    Bandstop,
    NONE
};

constexpr std::array<FilterBType, 4> types
    = { FilterBType::Lowpass, FilterBType::Highpass, FilterBType::Bandpass,
        FilterBType::Bandstop };

// ------------------------------------------------------------
// Signal
// ------------------------------------------------------------
struct Signal
{
    std::vector<double> time;
    std::vector<double> values;

    std::size_t size () const { return values.size (); }
    double sampleRate = 0;
    void calcSamplerate ()
    {
        const double dt = time[1] - time[0];

        if (dt <= 0.0)
        {
            throw std::runtime_error ("Invalid sampling interval");
        }

        sampleRate = 1.0 / dt;
    }
};

// ------------------------------------------------------------
// Filter configuration
// ------------------------------------------------------------
std::string filterBType2String (FilterBType type);

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
Signal readSignalFromCSV (const std::string filename);

// ------------------------------------------------------------
// Generate reference filename
// ------------------------------------------------------------
std::string generateFileName (const std::string filterName, FilterBType type,
                              int order);

// ------------------------------------------------------------
// Compare reference and actual signals
// ------------------------------------------------------------
void compareSignals (const Signal &reference, const Signal &actual);

// ------------------------------------------------------------
// Run one test
// ------------------------------------------------------------
void runTest (const std::string filtername, const Signal &input,
              std::shared_ptr<IIRFilter> iirFilter, int order);
