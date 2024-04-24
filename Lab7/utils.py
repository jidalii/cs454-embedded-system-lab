import signal
import time
import numpy as np
from typing import Callable, Union

start_time = 0
i = 0
def loop_every(interval: float, callback: Callable[[int, float], None]):
    """Call the callback every interval seconds.
    The callback will be called with two arguments:
    1. The number of times the callback has been called.
    2. The time elsapsed since the first call.
    and the time since the first call.
    """
    def handler(signum, frame):
        global i, start_time
        if i == 0:
            start_time = time.time()
        elapsed_time = time.time() - start_time
        callback(i, elapsed_time)
        i += 1
    signal.signal(signal.SIGALRM, handler)
    signal.setitimer(signal.ITIMER_REAL, interval, interval)


def noise( t: Union[float, np.ndarray], min_freq: float = 30.0, max_freq: float = 50.0, num_waves: int = 30, seed=42):
    """ Generate a random noise signal, and evaluate it at time t in seconds. t can be a float or a numpy array
        The noise signal is a sum of sinusoids with random frequencies, amplitudes and phases.
    """
    rng = np.random.default_rng(seed)
    freqs = rng.uniform(min_freq, max_freq, size=num_waves)
    amplitudes = rng.uniform(0.0, 0.01, size=num_waves)
    phases = rng.uniform(0, 2*np.pi, size=num_waves)
    angles = np.multiply.outer(freqs*2*np.pi, t).T + phases
    return np.sum(amplitudes*np.sin(angles), axis=-1)
