# (C) 2023 Andy Leuckert
# (C) 2026 Bernd Porr
# Generates the filter outputs
import numpy as np
from scipy.signal import butter, lfilter, bessel, cheby1, cheby2, sosfilt

def export_to_csv(time, filtered_signal, path):
    with open(path, 'w') as file:
        # Write the header row
        file.write('time;signal\n')
        # Write the data rows
        for t, s in zip(time, filtered_signal):
            file.write(f'{t:.4f};{s:.5f}\n')


def readData(filename):
    # Read the data
    data = np.loadtxt(fname=filename, delimiter=';',skiprows=1)
    time = data[:, 0]
    signal = data[:, 1]
    return time, signal


def filterAndExport(sos, signal, exportName):
    # Apply the filter to the signal (using lfilter for one-pass filtering)
    filtered_signal = sosfilt(sos, signal)
    # Export the result to a CSV file
    export_to_csv(time, filtered_signal, exportName)
    print(f"Filtered signal has been saved to {exportName}")

def createFileNameFromFilter(filtertype, btype, lc, hc, order):
    return str(filtertype + "-" + btype + "-LC_" + str(lc) + "-HC_" + str(hc) + "-Order_" + str(order) + ".csv")

time, signal = readData('signal.txt')

# Calculate the sample rate
sample_rate = 1 / (time[1] - time[0])
lc = 200
hc = 300

filtertypes = ['Butterworth', 'Bessel', 'Chebychev1', 'Chebychev2']
btypes = ['lowpass', 'highpass', 'bandpass', 'bandstop']
orders = [1,2,4,5,10]

###########################################################################

for filtertype in filtertypes:
    for btype in btypes:
        for order in orders:
            # Handle different critical frequencies for lowpass/highpass vs bandpass/bandstop
            if btype in ['lowpass', 'highpass']:
                Wn = hc if btype == 'lowpass' else lc
            else:
                Wn = [lc, hc]

            if filtertype == "Butterworth":
                sos = butter(N=order, Wn=Wn, fs=sample_rate, btype=btype, output='sos')
            elif filtertype == "Bessel":
                sos = bessel(N=order, Wn=Wn, fs=sample_rate, btype=btype, output='sos')
            elif filtertype == "Chebychev1":
                sos = cheby1(N=order, rp=1, Wn=Wn, fs=sample_rate, btype=btype, output='sos')
            elif filtertype == "Chebychev2":
                sos = cheby2(N=order, rs=1, Wn=Wn, fs=sample_rate, btype=btype, output='sos')

            filename = createFileNameFromFilter(filtertype.lower(), btype, lc, hc, order)
            filterAndExport(sos, signal, filename)
