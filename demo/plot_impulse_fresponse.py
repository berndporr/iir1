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
    plt.title("Impulse response")
    plt.plot(y);
    #
    # Fourier Transform
    yf = scipy.fft(y)
    plt.subplot(212)
    plt.plot(scipy.linspace(0,fs,len(yf)),20*np.log10(abs(yf)))
    plt.xlim(0,fs/2)
    plt.title("Frequency response")
    plt.xlabel("f/Hz")
    plt.ylabel("gain/dB")

plot_if(1,"bs.dat","Butterworth bandstop")

plot_if(2,"lp.dat","Butterworth lowpass")

plot_if(3,"hp_rbj.dat","RBJ highpass")

plot_if(4,"lp_elliptic.dat","Elliptic lowpass")

plot_if(5,"lp_cheby1.dat","ChebyshevI lowpass")

plot_if(6,"lp_cheby2.dat","ChebyshevII lowpass")

plt.show()
