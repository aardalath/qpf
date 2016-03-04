''' QLA processing pipeline '''

from .version import version
import os

__version__ = version
__top_dir__ = os.path.dirname(os.path.realpath(__file__))

from euclidqla.checks.factory import theFactory
from euclidqla.instrument.instrument import build_vis_instrument

theFactory.subscribe('VIS', build_vis_instrument, cached=True)
