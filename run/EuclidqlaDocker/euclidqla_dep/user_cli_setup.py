from euclidqla.checks.overscan import OverscanCheck, ImageStatistics
from euclidqla.report.report import PostScanFullReport, PostScanReport, PreScanReport, ScienceStatsReport
from euclidqla.checks.factory import theFactory as _theFactory
from euclidqla.result.storage import Storage as _Storage
import numpy as np
import logging

logging.basicConfig(level=logging.ERROR, format='%(asctime)s:%(levelname)s:%(name)s:%(message)s')


def verbose(level_b=True):
    level = logging.INFO if level_b else logging.ERROR
    logging.getLogger().setLevel(level=level)


def run(image, check, storage=None):
    if not storage:
        storage = _Storage(image.name)
    check().run(image, storage)
    print storage.result
    return storage


def report(image, report, storage):
    report().report(image, storage)
    print storage.report
