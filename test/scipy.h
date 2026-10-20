#include "Iir.h"

#include <array>
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

#pragma once

namespace fs = std::filesystem;

constexpr double lc = 200.0;
constexpr double hc = 300.0;
const double rippledB = 1;
const double center = (lc + hc) / 2.0;
const double width = hc - lc;
const std::array<int, 5> orders = { 1, 2, 4, 5, 10 };

enum class FilterBType
{
    Lowpass,
    Highpass,
    Bandpass,
    Bandstop,
    NONE
};

const std::array<FilterBType, 4> types
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

std::string toString (FilterBType type);

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

Signal readSignalFromCSV (const fs::path &filename);

// ------------------------------------------------------------
// Generate reference filename
// ------------------------------------------------------------

std::string generateFileName (std::string filterName, FilterBType type,
                              int order, double lc, double hc);

// ------------------------------------------------------------
// Compare reference and actual signals
// ------------------------------------------------------------

void compareSignals (const Signal &reference, const Signal &actual,
                     double tolerance);

// ------------------------------------------------------------
// Run one test
// ------------------------------------------------------------

void runTest (std::string filtername, const Signal &input,
              std::shared_ptr<IIRFilter> iirFilter, int order, double lc,
              double hc);
