# DSP IIR Realtime C++ filter library

![alt tag](title.png)

 - High performance
 - Realtime sample in - sample out processing
 - Butterworth, RBJ, Chebychev filters
 - Lowpass, highpass, bandpass and bandstop filters
 - Template based header-only filter functions
 - Cross platform: Linux, Windows and Mac

An infinite impulse response (IIR) filter library for
Linux, Mac OSX and Windows
which implements Butterworth, RBJ, Chebychev filters
and can easily import coefficients generated by Python (scipy).

The filter processes the data sample by sample for realtime
processing.

It uses templates to allocate the required memory so that
it can run without any malloc / new commands.
Memory is allocated at compile time
so that there is never the risk of memory leaks.

All realtime filter code is in the header files which guarantees
efficient integration into the main program and the compiler
can optimise both filter code and main program at the same time.

## C++ code
Add the following include statement to your code:
```
#include "Iir.h"
```

The general coding approach is that first the filter is
instantiated specifying its order, then the
parameters are set with the function `setup` and
then it's ready to be used for sample by sample realtime filtering.

### Instantiating the filter
The idea is to allocate the memory of the
filter at compile time with a template argument to avoid any new
commands. This prevents memory leaks and can be optimised at compile
time. The `order` provided to the template (for example here for a
lowpass filter):
```
Iir::Butterworth::LowPass<order> f;
```
is used as the default order by the `setup` command below
but can be overridden by a lower order if required.

### Setting the filter parameters: `setup`
All filters are available as lowpass, highpass, bandpass and bandstop/notch
filters. Butterworth / Chebyshev offer also low/high/band-shelves with
specified passband gain and 0dB gain in the stopband.

The frequencies can either be analogue ones against the sampling rate
or normalised ones between 0..1/2 where 1/2 is the Nyquist frequency. Note
that normalised frequencies are simply f = F/Fs and are in units of 1/samples.
Internally the library uses normalised frequencies and the setup commands
simply divide by the sampling rate if given. Choose between:
 1. `setup`: sampling rate and the analogue cutoff frequencies
 2. `setupN`: normalised frequencies in 1/samples between f = 0..1/2 where 1/2 = Nyquist.

By default `setup` uses the order supplied by the template argument but
can be overridden by lower filter orders.

See the header files in `\iir` or the documentation for the arguments
of the `setup` commands.

The examples below are for lowpass filters:

1. Butterworth -- `Butterworth.h`
Standard filter suitable for most applications. Monotonic response.
```
const int order = 4; // 4th order (=2 biquads)
Iir::Butterworth::LowPass<order> f;
const float samplingrate = 1000; // Hz
const float cutoff_frequency = 5; // Hz
f.setup (samplingrate, cutoff_frequency);
```
or specify a normalised frequency between 0..1/2:
```
f.setupN(norm_cutoff_frequency);
```

2. Chebyshev Type I -- `ChebyshevI.h`
With permissible passband ripple in dB.
```
Iir::ChebyshevI::LowPass<order> f;
const float passband_ripple_in_db = 5;
f.setup (samplingrate,
         cutoff_frequency,
         passband_ripple_in_dB);
```
or specify a normalised frequency between 0..1/2:
```
f.setupN(norm_cutoff_frequency,passband_ripple_in_dB);
```


3. Chebyshev Type II -- `ChebyshevII.h`
With worst permissible stopband rejection in dB.
```
Iir::ChebyshevII::LowPass<order> f;
double stopband_ripple_in_dB = 20;
f.setup (samplingrate,
         cutoff_frequency,
         stopband_ripple_in_dB);
```
or specify a normalised frequency between 0..1/2:
```
f.setupN(norm_cutoff_frequency,stopband_ripple_in_dB);
```

4. RBJ -- `RBJ.h`
2nd order filters with cutoff and Q factor.
```
Iir::RBJ::LowPass f;
const float cutoff_frequency = 100;
const float Q_factor = 5;
f.setup (samplingrate, cutoff_frequency, Q_factor);
```
or specify a normalised frequency between 0..1/2:
```
f.setupN(norm_cutoff_frequency, Q_factor);
```

5. Designing filters with Python's scipy.signal -- `Custom.h`
```
########
# Python
# See "elliptic_design.py" for the complete code.
from scipy import signal
order = 4
sos = signal.ellip(order, 5, 40, 0.2, 'low', output='sos')
print(sos) # copy/paste the coefficients over & replace [] with {}

///////
// C++
// part of "iirdemo.cpp"
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
		 9.538657786383895054e-01}
	};
const int nSOS = sizeof(coeff) / sizeof(coeff[0]); // here: nSOS = 2 = order / 2
Iir::Custom::SOSCascade<nSOS> cust(coeff);
```

### Realtime filtering sample by sample
Samples are processed one by one. In the example below
a sample `x` is processed with the `filter`
command and then saved in `y`. The types of `x` and `y` can either be
float or double
(integer is also allowed but is still processed internally as floating point):
```
y = f.filter(x);
```
This is then repeated for every incoming sample in a
loop or event handler.


### Error handling
Invalid values provided to `setup()` will throw
an exception. Parameters provided to `setup()` which
result in coefficients being NAN will also
throw an exception.

You can switch off exeption handling by defining
`IIR1_NO_EXCEPTIONS` via cmake or in your program.


## Linking

### CMake setup
If you use cmake as your build system then just add
to your `CMakeLists.txt` the following lines for the dynamic library:
```
find_package(iir)
target_link_libraries(... iir::iir)
```
or for the static one:
```
find_package(iir)
target_link_libraries(... iir::iir_static)
```

### Generic linker setup
Link it against the dynamic library
(Unix/Mac: `-liir`, Windows: `iir.lib`)
or the static library (Unix/Mac: `libiir_static.a`,
Windows: `libiir_static.lib`).



## Pre compiled packages for Ubuntu LTS (PPA):

If you are using Ubuntu LTS then you can
install this library as a pre-compiled package.

Add this repository to your system:
```
sudo add-apt-repository ppa:berndporr/dsp
```

Then install the packages:

  - Library files: `sudo apt install iir1`
  - Development files: `sudo apt install iir1-dev`

It's available for 64 bit Intel and 32,64 bit ARM (Raspberry PI etc).
The documentation of the development package and the example programs are in:
```
/usr/share/doc/iir1-dev/
```


## Compilation from source

The build tool is `cmake` which generates the make- or project
files for the different platforms. `cmake` is available for
Linux, Windows and Mac. It also compiles directly on a
Raspberry PI.

### Linux / Mac

Run
```
cmake .
```
which generates the Makefile. Then run:
```
make
sudo make install
```
which installs it under `/usr/local/lib` and `/usr/local/include`.

Both gcc and clang have been tested.

### Windows

```
cmake -G "Visual Studio 16 2019" -A x64 .
```

See `cmake` for the different build-options. Above is for a 64 bit build.
Then start Visual C++ and open the solution. This will create
the DLL and the LIB files. Under Windows it's highly recommended
to use the static library and link it into the application program.

### Unit tests

Run unit tests by typing `make test` or just `ctest`.
These test if after a delta pulse all filters relax to zero,
that their outputs never become NaN and if the Direct Form I&II filters calculate
expected sequences by comparing them from results created
by the output of scipy's `sosfilt`.

You can disable the generation of tests by setting `IIR1_BUILD_TESTING` to off.

## Documentation

### Learn from the demos
The easiest way to learn is from the examples which are in the `demo`
directory. A delta pulse as a test signal is sent into the different
filters and saved in a file. With the Python script
`plot_impulse_fresponse.py` you can then plot the frequency responses.

You can disable the compilation of the demos by setting `IIR1_BUILD_DEMO` to off.

Also the directory containing the unit tests provides examples for
every filter type.

### Detailed documentation
A PDF of all classes, methods and in particular `setup` functions
is in the `docs/pdf` directory.

The online documentation is here: http://berndporr.github.io/iir1

## Example filter responses

These responses have been generated by `iirdemo.cpp`
in the `/demo/` directory and then plotted with `plot_impulse_fresponse.py`.

![alt tag](demo/1.png)
![alt tag](demo/2.png)
![alt tag](demo/3.png)
![alt tag](demo/4.png)
![alt tag](demo/5.png)
![alt tag](demo/7.png)
![alt tag](demo/8.png)

## Credits

This library has been further developed from Vinnie Falco's
great original work which can be found here:

https://github.com/vinniefalco/DSPFilters

While the original library processes audio arrays this
library has been adapted to do fast realtime processing sample
by sample. The `setup`
command won't require the filter order and instead remembers
it from the template argument. The class structure has
been simplified and all functions documented for doxygen.
Instead of having assert() statements this libary throws
exceptions in case a parameter is wrong. Any filter design
requiring optimisation (for example Ellipic filters) has
been removed and instead a function has been added which can import easily
coefficients from scipy.

Bernd Porr -- http://www.berndporr.me.uk
