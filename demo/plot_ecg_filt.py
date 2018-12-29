#!/usr/bin/python3

import matplotlib.pyplot as plt
import scipy
import numpy as np

# Plots the impulse response of the Bandstop and its frequency response

def plot_if(figno,name,figtitle):
    plt.figure(figno)
    plt.suptitle(figtitle)
    fs = 1000
    y = np.loadtxt(name);
    plt.subplot(211)
    plt.title("Signal")
    plt.plot(y);
    #
    # Fourier Transform
    yf = scipy.fft(y)
    plt.subplot(212)
    plt.plot(scipy.linspace(0,fs,len(yf)),20*np.log10(abs(yf)))
    plt.xlim(0,fs/2)
    plt.title("Frequency spectrum")
    plt.xlabel("f/Hz")
    plt.ylabel("gain/dB")

plot_if(1,"ecg50hz.dat","ECG with 50Hz noise")

plot_if(2,"ecg_filtered.dat","ECG after filtering")

plt.show()
