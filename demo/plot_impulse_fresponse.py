import matplotlib.pyplot as plt
import scipy
import numpy as np

# Plots the impulse response of the Bandstop and its frequency response

def plot_if(figno,name):
    plt.figure(figno)
    fs = 1000
    y = np.loadtxt(name);
    plt.subplot(211)
    plt.title("Impulse response")
    plt.plot(y);
    #
    # Fourier Transform
    yf = scipy.fft(y)
    plt.subplot(212)
    plt.plot(scipy.linspace(0,fs,len(yf)),abs(yf))
    plt.title("Frequency response")

plot_if(1,"bs.dat");

plot_if(2,"lp.dat");

plot_if(3,"hp.dat");

plt.show()
