__author__ = 'vvillar'

from numpy import mean as _np_mean, median as _np_median, var as _np_var


def statistics(input_array, axis=None):
    """
    Compute mean, median and variance for an input array. Optional: specify axis
    """
    return _np_mean(input_array, axis), _np_median(input_array, axis), _np_var(input_array, axis)

