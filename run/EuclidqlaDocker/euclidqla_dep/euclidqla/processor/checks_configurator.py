__author__ = 'vvillar'

from .. import __top_dir__
from ..checks.factory import theFactory, NotFoundError
from abc import ABCMeta, abstractmethod
import logging
import json
from os.path import join as _join


_logger = logging.getLogger('checks_configurator')


class BaseSchema(object):
    """
    Base abstract class for Schema classes
    """
    __metaclass__ = ABCMeta

    def __init__(self, kind):
        self.kind = kind

    def get_names(self, image, param):
        """
        Get the names of the schema items for a given image and some parameter
        """
        instrument = image.instrument
        observing_mode = image.observing_mode
        return self.get_names_for_inst_and_obsmode(instrument, observing_mode, param)

    @abstractmethod
    def get_names_for_inst_and_obsmode(self, instrument, observing_mode, param):
        pass


class BaseDictionarySchema(BaseSchema):
    """
    Schema class where data is stored as a dictionary
    """
    def __init__(self, dictionary, kind):
        if not isinstance(dictionary, dict):
            raise TypeError
        BaseSchema.__init__(self, kind)
        self.dictionary = dictionary

    def get_names_for_inst_and_obsmode(self, instrument, observing_mode, param):
        """
        Get the names of the items for the instrument, observing mode and param
        """
        _logger.info("Getting {0}(s) for instrument {1}, observing mode {2} and mode {3}"
                     .format(self.kind, instrument, observing_mode, param))
        return self.dictionary[instrument][observing_mode][param]


class DataElementDictionarySchema(BaseDictionarySchema):
    def get_names_for_inst_and_obsmode(self, instrument, observing_mode, _):
        names = []
        names.extend(self.dictionary[instrument]['COMMON'])
        names.extend(self.dictionary[instrument][observing_mode])
        return names


def read_json_from_file(config_file):
    """
    Auxiliary function to read a json from a file
    """
    with file(config_file) as file_data:
        checks_dict = json.load(file_data)
    return checks_dict


_dict_kinds = ["check", "report"]


def schema_from_file(config_file, kind, schema=BaseDictionarySchema):
    """
    Create a schema of a specfic kind from a given config file and using class schema
    """
    if kind not in _dict_kinds:
        raise TypeError
    _logger.info("Importing {0}s schema from {1}".format(kind, config_file))
    data_dict = read_json_from_file(config_file)
    return schema(data_dict, kind)

# Default check and reports config files
config_check_default = _join(__top_dir__, 'processor', 'instrument_checks.txt')
config_report_default = _join(__top_dir__, 'processor', 'instrument_reports.txt')
#config_report_default = _join(__top_dir__, 'processor', 'dataelement_checks.txt')


def checks_configurator_from_file(config_file=config_check_default, mode="NOMINAL", schema=BaseDictionarySchema):
    """
    Build the checks configurator from json file
    """
    return BaseConfigurator(schema_from_file(config_file, "check", schema), mode)


def reports_configurator_from_file(config_file=config_report_default, mode="NOMINAL", schema=BaseDictionarySchema):
    """
    Build the reports configurator from json file
    """
    return BaseConfigurator(schema_from_file(config_file, "report", schema), mode)


class BaseConfigurator(object):
    """
    Class to get the items from a schema given an image
    """
    def __init__(self, schema, mode="NOMINAL"):
        self.schema = schema
        self.mode = mode

    def get_configuration(self, image):
        """
        Get the configuration items from the schema according to the image characteristics
        """
        _logger.info("Configuring {0}s for image {1}".format(self.schema.kind, image.name))
        names = self.schema.get_names(image, self.mode)
        elements = []
        if not names:
            _logger.warning("No {0}s configured for image {1}".format(self.schema.kind, image.name))
            return elements
        _logger.info("{0}s to configure: {1}".format(self.schema.kind, ", ".join(names)))
        self.build_elements(elements, names)
        return elements

    def build_elements(self, elements, names):
        for name in names:
            try:
                new_element = theFactory.build(name)
                if new_element.dependencies:
                    self.build_elements(elements, new_element.dependencies)
                if new_element not in elements:
                    elements.append(new_element)
            except NotFoundError:
                _logger.warning("Unable to configure {0} {1}".format(names, self.schema.kind))


class ConfiguratorFromCheck(BaseConfigurator):
    def __init__(self, config_file=config_report_default, schema=BaseDictionarySchema):
        self.configurator = reports_configurator_from_file(config_file=config_file, schema=schema)
        self.schema = self.configurator.schema

    def get_configuration(self, image):
        reports = self.configurator.get_configuration(image)
        check_names = []
        for report in reports:
            if report.check_dependencies:
                check_names.append(report.check_dependencies)
        checks = []
        if check_names:
            self.build_elements(checks, check_names)
        return checks, reports


class CompositeConfigurator(object):
    """
    Composite of coonfigurators
    """

    def __init__(self, *args):
        self.configurators = list(args)

    def get_configuration(self, image):
        """
        Get the configuration items from the schemas according to the image characteristics
        """
        return tuple(configurator.get_configuration(image) for configurator in self.configurators)
