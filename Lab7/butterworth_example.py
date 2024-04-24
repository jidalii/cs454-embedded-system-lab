#####################################################
###                                               ###
### BU CAS CS454/654 Embedded Systems Development ###
###                                               ###
### Author: Renato Mancuso (BU)                   ###
###                                               ###
### File: Butterworth Filters Demo                ###
###                                               ###
#####################################################

from scipy.fftpack import fft, fftshift, fftfreq
from scipy import signal
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.colors import to_rgba
import random
import utils

## Where to save generated figures
savepath="/home/renato/BU/Classes/CS454_Sp23/Python/";

## Generic function to compute the FFT.
## @s: sampled signal for which to compute the FFT
## @fc: frequency of sampling in Hz
## returns: xf and yf ready for plotting

def getFFT(s, fc):
    # SAmpling period
    Tc = 1/fc
    # Number of samples
    N = len(s)

    # Compute y-axis values 
    yf = fftshift(fft(s)/(N/2))

    # Shift and normalize x-axis values
    xf = fftshift(fftfreq(N, Tc))

    return xf, yf

def __finalizePlot():
    plt.grid()
    plt.show()


def plotSignal(x, y, title):
    plt.title(title)
    plt.xlabel("time (s)")
    plt.ylabel("$s(t)$ value")
    plt.plot(x, y)
    __finalizePlot()
    
def plotMultipleSignals(x, y1, color1, y2, color2, title):
    plt.title(title)
    plt.xlabel("time (s)")
    plt.ylabel("signal value")

    plt.plot(x, y1, color=color1, label="Original Signal")
    plt.plot(x, y2, color=color2, label="Filtered Signal")
    plt.legend(loc="lower right")
    
    __finalizePlot()
    

def plotFFT(xf, yf, signal_band=None, noise_band=None):
    plt.title("Fast Fuorier Transform (FFT)")
    plt.xlabel("frenqeucy (Hz)")
    plt.ylabel("magnitude of harmonic")
    plt.plot(xf, np.abs(yf))

    ## If band definitios have been provided, annotate plot accordingly
    ax = plt.gca()
    bottom, top = ax.get_ylim()
    
    if signal_band != None:
        signal_bandwidth = signal_band[1] - signal_band[0]
        rect_signal = patches.Rectangle((signal_band[0], bottom), signal_bandwidth, top,
                                        linewidth=0, facecolor=to_rgba('green', 0.3))
        rect_signal_neg = patches.Rectangle((-signal_band[1], bottom), signal_bandwidth, top,
                                        linewidth=0, facecolor=to_rgba('green', 0.3))

        ax.add_patch(rect_signal)
        ax.add_patch(rect_signal_neg)
    
    if noise_band != None:
        noise_bandwidth = noise_band[1] - noise_band[0]
        rect_noise = patches.Rectangle((noise_band[0], bottom), noise_bandwidth, top,
                                        linewidth=0, facecolor=to_rgba('red', 0.3))
        rect_noise_neg = patches.Rectangle((-noise_band[1], bottom), noise_bandwidth, top,
                                        linewidth=0, facecolor=to_rgba('red', 0.3))

        ax.add_patch(rect_noise)
        ax.add_patch(rect_noise_neg)
        
    
    plt.grid()
    plt.show()


### PART 1: Simple signal at 500 Hz and random noise between 10 and 123 Hz ###
# Generate time axis for a 500 Hz sampled signal between -2 and 2 seconds
fc = 500.0
Tc = 1/fc
time_start = -2.0
time_end = 2.0
N = (time_end - time_start) / Tc
t = np.linspace(time_start, time_end, num=int(N), endpoint=False)

# Generate sampled signal: a sinusoid with main component at 5 Hz
s_hz = 5
s = 1.5 + 10*np.sin(2 * np.pi * s_hz * t)

# Now generate very messy noise with 100 components in the band 20 Hz to 123 Hz
n = 0 
num_noise_comp = 100
noise_lo_hz = 20.0
noise_hi_hz = 123.0
n = utils.noise(t, noise_lo_hz, noise_hi_hz, num_noise_comp)
noise_band = np.linspace(noise_lo_hz, noise_hi_hz, num_noise_comp)

xf, yf = getFFT(s+n, fc)
plotSignal(t, s+n, "Simple signal: $s(t) = 1 + 0.8\\sin(2\\pi5t)$")
plotFFT(xf, yf, [0, s_hz], [noise_lo_hz, noise_hi_hz])


### PART 2: Butterworth Filter Design ###

# Set the filter order
N_ord = 5

# Set the filter cutoff in Hz
f_cutoff = 6

# Compute percentage w.r.t. Nyquist frequency
wn = f_cutoff * 2/fc

# Get the signal coefficients
b, a = signal.butter(N_ord, wn, 'low', analog=False)

s_filt = signal.lfilter(b, a, s+n)

plotSignal(t, s_filt, "Filtered signal.")

# Now plot the FFT of the filtered signal
xf, yf = getFFT(s_filt, fc)
plotFFT(xf, yf, [0, s_hz], [noise_lo_hz, noise_hi_hz])

# Now let's compare the original signal and the filtered time signal
plotMultipleSignals(t, s, 'blue', s_filt, 'red', "Comparison btw original and filtered signal.")
