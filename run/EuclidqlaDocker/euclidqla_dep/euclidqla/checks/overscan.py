from .check import Check
from .factory import theFactory as _theFactory
from ..algorithm.statistics import statistics as _statistics
from numpy import any as _np_any
from numpy import s_ as _slice_region
import logging as _logging

_logger = _logging.getLogger('checks')


def from_square_to_slice(region):
    """
    This function expects a list [[x0,y0],[x1,y1]] defining a square and returns
    a tuple with both slice_object for a 2D ndarray
    """
    return _slice_region[region[0][1] - 1: region[1][1]], _slice_region[region[0][0] - 1: region[1][0]]


def get_science_region(image):
    # get the science region for the kind of image
    instrument = _theFactory.build(image.instrument)
    science_region = instrument.detector[image.detector].quadrant[image.quadrant].science_region
    return image.data[from_square_to_slice(science_region)]


def get_prescan_region(image):
    # get the prescan region for the kind of image
    instrument = _theFactory.build(image.instrument)
    prescan_region = instrument.detector[image.detector].quadrant[image.quadrant].prescan_region
    return image.data[from_square_to_slice(prescan_region)]


def get_postcan_region(image):
    # get the postcan region for the kind of image
    instrument = _theFactory.build(image.instrument)
    overscan_region = instrument.detector[image.detector].quadrant[image.quadrant].overscan_region
    return image.data[from_square_to_slice(overscan_region)]


class OverscanCheck(Check):
    """
    Computes statistics for the pre and post scan regions of a image
    Computes Pre-scan statistics over the whole prescan region.
    Computes Post-scan statistics for each row of the pstscan region.
    """
    PRE_SCAN = 'Pre-Scan'
    POST_SCAN = 'Over-Scan'
    NAME = 'OVERSCAN'

    @classmethod
    def run_check(cls, image, storage):
        _logger.info("{0}: Measuring pre scan".format(cls.NAME))
        pre_scan = get_prescan_region(image)
        prescan_average, prescan_median, prescan_var = _statistics(pre_scan)
        storage.result[cls.PRE_SCAN] = {'average': prescan_average, 'median': prescan_median, 'var': prescan_var}
        _logger.info("{0}: Measuring over scan".format(cls.NAME))
        post_scan = get_postcan_region(image)
        postscan_averages, postscan_medians, postscan_vars = _statistics(post_scan, 1)
        storage.result[cls.POST_SCAN] = \
            {'average': postscan_averages, 'median': postscan_medians, 'var': postscan_vars}


class ImageStatistics(Check):
    """
    Computes statistics for the science region of a image
    """
    NAME = 'SCIENCE STATS'
    KEY = 'ScienceStats'

    @classmethod
    def run_check(cls, image, storage):
        sci_image = get_science_region(image)
        _logger.info("{0}: Measuring statistics".format(cls.NAME))
        science_average, science_median, science_var = _statistics(sci_image)
        storage.result[cls.KEY] = {'average': science_average, 'median': science_median,
                                   'var': science_var, 'zeros': _np_any(sci_image == 0)}
