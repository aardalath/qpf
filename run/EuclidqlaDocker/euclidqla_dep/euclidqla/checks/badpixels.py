__author__ = 'vvillar'

from .check import Check
from .overscan import get_science_region
from .overscan import OverscanCheck

from numpy import median as _np_median
import logging as _logging


_logger = _logging.getLogger('checks')


class PixelsBelowADC(Check):
    """
    Compute number of pixels below postscan value.
    """
    NAME = 'PIXELS BELOW ADC'
    KEY = 'Pixels_below_ADC'
    dependencies = ['OVERSCAN']

    @classmethod
    def run_check(cls, image, storage):
        overscan_value = _np_median(storage.result[OverscanCheck.POST_SCAN]['median'])
        _logger.info("{0}: Getting overscan value".format(cls.NAME))
        _logger.info("{0}: Measuring pixels below overscan value {1}".format(cls.NAME, overscan_value))
        science_frame = get_science_region(image)
        num_pixels = len(science_frame[science_frame < overscan_value])
        storage.result[cls.KEY] = {'Number': num_pixels}
