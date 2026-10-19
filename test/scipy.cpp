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

struct Signal {
    std::vector<double> time;
    std::vector<double> values;

    std::size_t size() const {
        return values.size();
    }
};

// ------------------------------------------------------------
// Filter configuration
// ------------------------------------------------------------

enum class FilterBType {
    Lowpass,
    Highpass,
    Bandpass,
    Bandstop
};

std::string toString(FilterBType type) {
    switch (type) {
        case FilterBType::Lowpass:  return "lowpass";
        case FilterBType::Highpass: return "highpass";
        case FilterBType::Bandpass: return "bandpass";
        case FilterBType::Bandstop: return "bandstop";
    }

    throw std::runtime_error("Unknown filter type");
}

// ------------------------------------------------------------
// Common filter interface
// ------------------------------------------------------------

class IIRFilter {
public:
    virtual ~IIRFilter() = default;

    virtual double filter(double input) = 0;
};

// ------------------------------------------------------------
// Adapter for an IIR1 filter
// ------------------------------------------------------------

template<typename Filter>
class IIR1Adapter final : public IIRFilter {
public:
    template<typename... Args>
    explicit IIR1Adapter(Args... args) {
        filter_.setup(args...);
    }

    double filter(double input) override {
        return filter_.filter(input);
    }

private:
    Filter filter_;
};

// ------------------------------------------------------------
// Construct a filter with a compile-time order
// ------------------------------------------------------------

template<int Order>
std::unique_ptr<IIRFilter> createButterworth(
    FilterBType type,
    double sampleRate,
    double lc,
    double hc
) {
    const double center = (lc + hc) / 2.0;
    const double width  = hc - lc;

    switch (type) {
        case FilterBType::Lowpass:
            return std::make_unique<
                IIR1Adapter<Iir::Butterworth::LowPass<Order>>
            >(sampleRate, hc);

        case FilterBType::Highpass:
            return std::make_unique<
                IIR1Adapter<Iir::Butterworth::HighPass<Order>>
            >(sampleRate, lc);

        case FilterBType::Bandpass:
            return std::make_unique<
                IIR1Adapter<Iir::Butterworth::BandPass<Order>>
            >(sampleRate, center, width);

        case FilterBType::Bandstop:
            return std::make_unique<
                IIR1Adapter<Iir::Butterworth::BandStop<Order>>
            >(sampleRate, center, width);
    }

    throw std::runtime_error("Unknown filter configuration");
}

// ------------------------------------------------------------
// Runtime order selection
// ------------------------------------------------------------

std::unique_ptr<IIRFilter> createIIRFilter(
    FilterBType type,
    int order,
    double sampleRate,
    double lc,
    double hc
) {
    switch (order) {
        case 1:
            return createButterworth<1>(
                type, sampleRate, lc, hc
            );

        case 2:
            return createButterworth<2>(
                type, sampleRate, lc, hc
            );

        case 4:
            return createButterworth<4>(
                type, sampleRate, lc, hc
            );

        case 5:
            return createButterworth<5>(
                type, sampleRate, lc, hc
            );

        case 10:
            return createButterworth<10>(
                type, sampleRate, lc, hc
            );

        default:
            throw std::invalid_argument(
                "Unsupported filter order: " +
                std::to_string(order)
            );
    }
}

// ------------------------------------------------------------
// Read a semicolon-separated CSV signal
// ------------------------------------------------------------

Signal readSignalFromCSV(const fs::path& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error(
            "Cannot open " + filename.string()
        );
    }

    Signal signal;
    std::string line;

    // Skip the header.
    if (!std::getline(file, line)) {
        throw std::runtime_error(
            "Empty CSV: " + filename.string()
        );
    }

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream row(line);
        std::string timeString;
        std::string valueString;

        if (!std::getline(row, timeString, ';') ||
            !std::getline(row, valueString, ';')) {
            throw std::runtime_error(
                "Malformed CSV row in " + filename.string()
            );
        }

        signal.time.push_back(std::stod(timeString));
        signal.values.push_back(std::stod(valueString));
    }

    return signal;
}

// ------------------------------------------------------------
// Generate reference filename
// ------------------------------------------------------------

std::string generateFileName(
    FilterBType type,
    int order,
    double lc,
    double hc
) {
    return "butterworth-" + toString(type) +
           "-LC_" + std::to_string(static_cast<int>(lc)) +
           "-HC_" + std::to_string(static_cast<int>(hc)) +
           "-Order_" + std::to_string(order) +
           ".csv";
}

// ------------------------------------------------------------
// Compare reference and actual signals
// ------------------------------------------------------------

bool compareSignals(
    const Signal& reference,
    const Signal& actual,
    double tolerance
) {
    if (reference.size() != actual.size()) {
        std::cerr
            << "FAIL: signal sizes differ: reference="
            << reference.size()
            << ", actual=" << actual.size()
            << '\n';

        return false;
    }

    for (std::size_t i = 0; i < reference.size(); ++i) {
        const double expected = reference.values[i];
        const double actualValue = actual.values[i];

        if (!std::isfinite(expected) ||
            !std::isfinite(actualValue) ||
            std::abs(expected - actualValue) > tolerance) {

            std::cerr
                << std::setprecision(12)
                << "FAIL at index " << i
                << ": reference=" << expected
                << ", actual=" << actualValue
                << ", difference="
                << std::abs(expected - actualValue)
                << '\n';

            return false;
        }
    }

    return true;
}

// ------------------------------------------------------------
// Run one test
// ------------------------------------------------------------

bool runTest(
    const Signal& input,
    FilterBType type,
    int order,
    double lc,
    double hc
) {
    constexpr double tolerance = 1e-5;

    if (input.size() < 2) {
        throw std::runtime_error(
            "Input signal must contain at least two samples"
        );
    }

    const double dt = input.time[1] - input.time[0];

    if (dt <= 0.0) {
        throw std::runtime_error(
            "Invalid sampling interval"
        );
    }

    const double sampleRate = 1.0 / dt;

    const std::string filename =
        generateFileName(type, order, lc, hc);

    const Signal reference =
        readSignalFromCSV(fs::path("scipy") / filename);

    auto filter = createIIRFilter(
        type, order, sampleRate, lc, hc
    );

    Signal actual;
    actual.time = input.time;
    actual.values.reserve(input.size());

    for (double sample : input.values) {
        actual.values.push_back(filter->filter(sample));
    }

    std::cout
        << "Butterworth, "
        << toString(type)
        << ", order=" << order
        << ", fs=" << sampleRate
        << ": ";

    const bool passed = compareSignals(
        reference, actual, tolerance
    );

    std::cout << (passed ? "PASS" : "FAIL")
              << '\n';

    return passed;
}

// ------------------------------------------------------------
// Main test runner
// ------------------------------------------------------------

int main() {
    constexpr double lc = 200.0;
    constexpr double hc = 300.0;

    const std::vector<int> orders = {
        1, 2, 4, 5, 10
    };

    const std::vector<FilterBType> types = {
        FilterBType::Lowpass,
        FilterBType::Highpass,
        FilterBType::Bandpass,
        FilterBType::Bandstop
    };

    int passed = 0;
    int failed = 0;
    int errors = 0;

    try {
        const Signal input =
            readSignalFromCSV("scipy/signal.txt");

        for (FilterBType type : types) {
            for (int order : orders) {
                try {
                    if (runTest(
                            input, type, order, lc, hc
                        )) {
                        ++passed;
                    } else {
                        ++failed;
                    }
                } catch (const std::exception& e) {
                    ++errors;

                    std::cerr
                        << "ERROR: "
                        << toString(type)
                        << ", order=" << order
                        << ": " << e.what()
                        << '\n';
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: "
                  << e.what() << '\n';

        return 1;
    }

    std::cout
        << "\n========== SUMMARY ==========\n"
        << "Passed: " << passed << '\n'
        << "Failed: " << failed << '\n'
        << "Errors: " << errors << '\n'
        << "Total:  " << passed + failed + errors << '\n';

    return (failed == 0 && errors == 0) ? 0 : 1;
}
