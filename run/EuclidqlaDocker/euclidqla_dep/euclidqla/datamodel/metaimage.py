from astropy.io import fits
from os.path import split as _split
import re

import numpy as np


class MetaImage(object):
    """
    Base class for image classes
    """
    def __init__(self, image_name, image, extension):
        self.name = _split(image_name)[1]
        if extension:
            self.name += '[' + str(extension) + ']'
        self.data = image[extension].data.astype(dtype=np.float32)
        self.header = image[extension].header

    @property
    def instrument(self):
        # TODO
        instrument = self.header.get('INSTRUME').upper()
        if 'VIS' in instrument:
            return 'VIS'
        return instrument


class VISSimImage(MetaImage):
    """
    Class for VIS simulated images (one image is one quadrant)
    """
    def __init__(self, image_name, image, extension):
        MetaImage.__init__(self, image_name, image, extension)
        if 'CCDX' in self.header:
            self.detector = self.header['CCDX'] + self.header['CCDY'] * 6
        if 'QUADRANT' in self.header:
            self.quadrant = self.header['QUADRANT']
        # FIXME
        self.observing_mode = "SCIENCE"


class UknownImageFormatError(Exception):
    pass


def build_image(image_name, extension=1):
    """
    Build an image object of the appropriate class according to its header
    """
    reg_exp = re.compile(r'(...+)[\[]([0-9]+)[\]]')

    res = reg_exp.search(image_name)

    if res:
        image_name = res.group(1)
        extension = int(res.group(2))

    with build_image.fitsController.get_fits(image_name) as image:
        if 'INSTRUME' in image[extension].header:
            instrument = image[extension].header.get('INSTRUME').lower()
            if len(image) > 1:
                if 'vissim' in instrument:
                    return VISSimImage(image_name, image, extension)
                else:
                    raise UknownImageFormatError('Unknown image format: {0}'.format(image_name))
        else:
            raise UknownImageFormatError('Unknown image format: {0}'.format(image_name))


class FitsController(object):
    def __init__(self):
        self.name = None
        self.cached = None

    def get_fits(self, image_name):
        if image_name != self.name:
            self.cached = fits.open(image_name)
            self.name = image_name
        return self

    def __enter__(self):
        return self.cached

    def __exit__(self, type, value, traceback):
        pass

build_image.fitsController = FitsController()


