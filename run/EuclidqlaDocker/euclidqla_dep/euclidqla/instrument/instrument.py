__author__ = 'vvillar'

from .. import __top_dir__

import json


class Quadrant(object):
    """
    Basic quadrant class to hold quadrant data
    """
    common_data = {}

    def __init__(self, i, common_data, quadrant_data):
        if i not in self.common_data:
            self.common_data[i] = common_data
        self.overscan_threshold = quadrant_data["overscan_threshold"]
        self.overscan_region = self.common_data[i]["overscan"]
        self.prescan_region = self.common_data[i]["prescan"]
        self.science_region = self.common_data[i]["science"]


class Detector(object):
    """
    Basic detector class to hold detector data and quadrant list
    """
    def __init__(self, quadrants_array):
        self.quadrant = quadrants_array


class Instrument(object):
    """
    Basic instrument class to hold detector list
    """
    def __init__(self, name, detectors_array):
        self.name = name
        self.detector = detectors_array


def build_quadrant(i, common_data, quadrant_data):
    """
    Build quadrant based on common quadrant data and specific quadrant data.
    """
    return Quadrant(i, common_data, quadrant_data)


def build_vis_detector(common_data, detector_data):
    """
    Build detector based on common detector data and specific detector data.
    """
    quadrants = \
        [build_quadrant(i, common_data["quadrant"+str(i)], detector_data["quadrant"+str(i)]) for i in range(1, 5)]
    return Detector(quadrants)


def build_vis_instrument(config_file=None):
    """
    Build VIS instrument from default config file vis.txt
    Optionally pass a custom config file
    """
    if config_file is None:
        config_file = __top_dir__ + '/instrument/vis.txt'

    with file(config_file) as file_data:
        vis_dict = json.load(file_data)

    detectors_def = vis_dict["detectors"]
    detectors = \
        [build_vis_detector(vis_dict["common"], detectors_def[str(i + 1)]) for i in range(len(detectors_def))]
    return Instrument("VIS", detectors)


def merge(a, b):
    """
    Merge two dictionaries adding data from both in case they share a key
    """
    for key, value in b.iteritems():
        if key in a:
            if isinstance(a[key], dict) and isinstance(value, dict):
                merge(a[key], value)
            else:
                raise Exception("Keys with same value and not dictionaries")
        else:
            a[key] = value
    return a
