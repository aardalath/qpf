from __future__ import division

from euclidqla.checks.factory import theFactory as _theFactory

from abc import ABCMeta, abstractmethod
from numpy import arange as _np_arange, any as _np_any
import logging as _logging

__author__ = 'vvillar'

_logger = _logging.getLogger('report')


class Report(object):
    """
    Base abstract class for reports.
    """
    __metaclass__ = ABCMeta

    dependencies = None
    check_dependencies = None

    @classmethod
    def report(cls, image, storage):
        _logger.info("Report {0} on {1} starts".format(cls.NAME, image.name))
        cls.run_report(image, storage)
        _logger.info("Report {0} on {1} finished successfully".format(cls.NAME, image.name))
        return storage

    @classmethod
    @abstractmethod
    def run_report(cls, image, storage):
        pass


class PostScanReport(Report):
    """
    Reports post scan checks results, detailing the fraction of problematic lines
    """
    NAME = "POSTSCAN_REP"
    check_dependencies = 'OVERSCAN'

    @classmethod
    def run_report(cls, meta_image, storage):
        _logger.info("{0}: Over scan thresholds checks for image {1} started".format(cls.NAME, meta_image.name))
        high_threshold = \
            _theFactory.build('VIS').detector[meta_image.detector].quadrant[meta_image.quadrant].overscan_threshold
        post_scan_average = storage.result['Over-Scan']['average']
        if _np_any(post_scan_average > high_threshold):
            _logger.warning("{0}: Over scan thresholds exceeded for image {1}".format(cls.NAME, meta_image.name))
            cls.report_warning(post_scan_average, high_threshold, storage)
        else:
            _logger.info("{0}: Over scan thresholds are OK for image {1}".format(cls.NAME, meta_image.name))
            storage.report['Over-Scan'] = {'Result': 'Nominal'}
        _logger.info("{0}: Over scan thresholds checks for image {1} finished".format(cls.NAME, meta_image.name))

    @classmethod
    def report_warning(cls, post_scan_average, high_threshold, storage):
        post_scan_wrong = post_scan_average[post_scan_average > high_threshold]
        lines_wrong = len(post_scan_wrong)
        storage.report['Over-Scan'] = {'Result': 'WARNING: Exceeding value in %d(%.2f%%) of lines'
                                       % (lines_wrong, (100 * lines_wrong/len(post_scan_average)))}


class PostScanFullReport(PostScanReport):
    """
    Reports post scan checks results, detailing which lines are problematic
    """

    NAME = "FULL_POSTSCAN_REP"

    @classmethod
    def report_warning(cls, post_scan_average, high_threshold, storage):
        storage.report['Over-Scan'] = {'Result': 'WARNING: Exceeding value in lines: '}
        storage.report['Over-Scan']['Result'] += \
            ' '.join([str(i) for i in _np_arange(len(post_scan_average))[post_scan_average > high_threshold]])


class PreScanReport(Report):
    """
    Reports pre scan checks results
    """
    NAME = "PRESCAN_REP"
    check_dependencies = 'OVERSCAN'

    @classmethod
    def run_report(cls, meta_image, storage):
        storage.report['Pre-Scan'] = storage.result['Pre-Scan']
        storage.report['Pre-Scan']['Result'] = 'Nominal'


class ScienceStatsReport(Report):
    """
    Reports science frame statistic checks results
    """

    NAME = "SCIENCES_STATS_REP"
    check_dependencies = 'SCIENCE STATS'

    @classmethod
    def run_report(cls, meta_image, storage):
        storage.report['ScienceStats'] = storage.result['ScienceStats']
        storage.report['ScienceStats']['Result'] = 'Nominal'


class CompositeReporter(object):
    """
    Reporter composite to group several reporters
    """
    def __init__(self, list_of_reports):
        self.reports = list_of_reports

    def report(self, meta_image, storage):
        for report in self.reports:
            report.report(meta_image, storage)
