from scipy import signal

fs = 20.0

# Set the filter order
N_ord = 3

# Set the filter cutoff in Hz
f_cutoff = 5


# Compute the normalization factor as a fraction of the Nyquist frequency
wn = f_cutoff / (0.5 * fs)

# Get the filter coefficients
b, a = signal.butter(N_ord, wn, "low", analog=False)
print("a:", a)
print("b:", b)