from .. import __top_dir__
from ..acquirer.acquirer import Acquirer, MultiExtensionImageSupplier
from ..processor.process import Processor, DataElementsProcessor
from ..processor.checks_configurator import ConfiguratorFromCheck, DataElementDictionarySchema
from ..result.results_container import ResultsContainer, FileOutput, ReportSelector, ResultSelector, OutputComposite
from ..result.fomatters import JsonFormatter

from ..acquirer.acquirer import ImageSupplier
from ..acquirer.dataclass_acquirer import DataElementAcquirer


import logging
from os.path import join as _join


class Controller(object):
    def __init__(self, args):
        log_level = logging.INFO if args.verbose else logging.ERROR
        logging.basicConfig(level=log_level, format='%(asctime)s:%(levelname)s:%(name)s:%(message)s')

        acquirer = Acquirer(MultiExtensionImageSupplier(args.input_dir))
        output = OutputComposite(FileOutput(args.report_file, JsonFormatter(prettyprint=True), ReportSelector),
                                 FileOutput(args.result_file, JsonFormatter(), ResultSelector))

        results_container = ResultsContainer(output)
        configurator = ConfiguratorFromCheck()
        self.processor = Processor(acquirer, configurator, results_container)

    def run(self):
        self.processor.process()


class NewController(object):
    def __init__(self, args):
        log_level = logging.INFO if args.verbose else logging.ERROR
        logging.basicConfig(level=log_level, format='%(asctime)s:%(levelname)s:%(name)s:%(message)s')

        acquirer = DataElementAcquirer(ImageSupplier(args.input_dir))
        output = OutputComposite(FileOutput(args.report_file, JsonFormatter(prettyprint=True), ReportSelector),
                                 FileOutput(args.result_file, JsonFormatter(), ResultSelector))

        results_container = ResultsContainer(output)
        configurator = ConfiguratorFromCheck(config_file=_join(__top_dir__, 'processor', 'dataelement_checks.txt'),
                                             schema=DataElementDictionarySchema)
        #configurator = DataElementConfiguratorFromCheck(config_file=_join(__top_dir__, 'processor', 'dataelement_checks.txt'),
        #                                                schema=DataElementDictionarySchema)
        self.processor = DataElementsProcessor(acquirer, configurator, results_container)

    def run(self):
        self.processor.process()