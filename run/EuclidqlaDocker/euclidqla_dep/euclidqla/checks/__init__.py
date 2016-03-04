__author__ = 'vvillar'

from euclidqla.checks.overscan import OverscanCheck, ImageStatistics
from euclidqla.checks.badpixels import PixelsBelowADC
from euclidqla.report.report import PostScanReport, PostScanFullReport
from euclidqla.report.report import PreScanReport, ScienceStatsReport
from euclidqla.checks.factory import theFactory

# We subscribe here the check name and constructor to the factory
theFactory.subscribe(OverscanCheck.NAME, OverscanCheck, cached=True)
theFactory.subscribe(ImageStatistics.NAME, ImageStatistics, cached=True)
theFactory.subscribe(PixelsBelowADC.NAME, PixelsBelowADC, cached=True)

# We subscribe here the reporter name and constructor to the factory
theFactory.subscribe(PostScanReport.NAME, PostScanReport, cached=True)
theFactory.subscribe(PostScanFullReport.NAME, PostScanFullReport, cached=True)
theFactory.subscribe(PreScanReport.NAME, PreScanReport, cached=True)
theFactory.subscribe(ScienceStatsReport.NAME, ScienceStatsReport, cached=True)
