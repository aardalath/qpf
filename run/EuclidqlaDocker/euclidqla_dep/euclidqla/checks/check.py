__author__ = 'vvillar'

from ..result.storage import Storage

from abc import ABCMeta, abstractmethod
import logging

_logger = logging.getLogger('checks')


class Check(object):
    """
    Base abstract class for checks.
    """
    __metaclass__ = ABCMeta

    dependencies = None

    @classmethod
    def run(cls, image, storage=None):
        if storage is None:
            storage = Storage(image.name)
        _logger.info("Check {0} on {1} starts".format(cls.NAME, image.name))
        cls.run_check(image, storage)
        _logger.info("Check {0} on {1} finished successfully".format(cls.NAME, image.name))
        return storage

    @classmethod
    @abstractmethod
    def run_check(cls, image, storage):
        pass
