__author__ = 'vvillar'

from processing import Processing

from numpy import float32 as _float32


class ImageCastProcessingFromUint16toFloat32(Processing):
    """
    Cast image from unsigned 16 bits int to 32 bits float image.
    """
    @classmethod
    def run_check(cls, data_element, _):
        data_element.data.astype(dtype=_float32)
