import matplotlib.pyplot as plt
import scipy
import numpy as np

# Plots the impulse response of the Bandstop and its frequency response

fs = 1000
y = np.loadtxt('bs.dat');
plt.figure(1)
plt.plot(y);
#
# Fourier Transform
yf = scipy.fft(y)
plt.figure(2)
plt.plot(scipy.linspace(0,fs,len(yf)),abs(yf))
plt.show()
