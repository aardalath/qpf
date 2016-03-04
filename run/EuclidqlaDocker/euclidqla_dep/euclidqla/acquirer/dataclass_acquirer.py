from dataclass_creator import get_dataclass_list as _get_dataclass_list
from ..datamodel.dataclass_image import VISSimCCDDataElement as _VISSimCCDDataElement
import logging as _logging
from os import path as _path

_logger = _logging.getLogger('acquirer')


class DataElementAcquirer(object):
    """
    This class get all files provided by supplier and yields a data element.
    """
    def __init__(self, supplier):
        self.supplier = supplier

    def __iter__(self):
        """
        generator that yields the data element object
        """
        for file_name in self.supplier:
            for dataclass in _get_dataclass_list(file_name):
                yield _build_data_element(dataclass)

    def __len__(self):
        _logger.debug('Acquirer: number of data elements')
        return len(self.supplier)


build_function = {'VISCCDSim': _VISSimCCDDataElement}


def _build_data_element(dataclass):
    return build_function[dataclass['type']](dataclass)

