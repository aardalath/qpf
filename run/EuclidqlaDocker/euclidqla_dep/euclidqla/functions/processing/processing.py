__author__ = 'vvillar'

from ...result.storage import Storage

from abc import ABCMeta, abstractmethod
import logging

_logger = logging.getLogger('processing')


class Processing(object):
    """
    Base abstract class for checks.
    """
    __metaclass__ = ABCMeta

    dependencies = None

    @classmethod
    def run(cls, data_element, storage=None):
        if storage is None:
            storage = Storage(data_element.name)
        _logger.info("Processing function {0} on {1} starts".format(cls.NAME, data_element.name))
        cls.run_check(data_element, storage)
        _logger.info("Processing function {0} on {1} finished successfully".format(cls.NAME, data_element.name))
        return storage

    @classmethod
    @abstractmethod
    def run_check(cls, data_element, storage):
        pass
