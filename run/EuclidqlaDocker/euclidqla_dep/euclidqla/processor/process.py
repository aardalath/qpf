import logging

_logger = logging.getLogger('process')

# Classes to handle the processing of the images


class Processor(object):
    """
    Simple processor that handles one data elements at a time,
    retrieves the checks to be done
    and apply the checks to the image.
    """
    def __init__(self, acquirer, configurator, results_container):
        self.acquirer = acquirer
        self.configurator = configurator
        self.results_container = results_container

    def process(self):
        """
        Process all dataclasses supplied by Acquirer.
        Checks and reports are configured through checks and report configurators.
        """
        _logger.info("Processing starts.")
        n_images = len(self.acquirer)
        if n_images:
            with self.results_container:
                _logger.info("{0} data elements to process".format(n_images))
                for i, meta_image in enumerate(self.acquirer):
                    _logger.info("Processing data element {0}/{1}: {2}".format(i + 1, n_images, meta_image.name))
                    self.process_image(meta_image)
        else:
            _logger.warning("No data elements found for processing!")
        _logger.info("Processing ends.")

    def process_image(self, meta_image):
        """
        get the checks and reports for a data element and process it
        """
        _logger.info("Processing of data element {0} starts".format(meta_image.name))
        checks, reporters = self.configurator.get_configuration(meta_image)
        try:
            self.run(meta_image, checks, reporters)
        except NoChecksError:
            _logger.info("No processing as no checks were configured")
        finally:
            _logger.info("Processing of data element {0} ends".format(meta_image.name))

    def run(self, meta_image, checks, reporters):
        """
        run every check and report on a single data element
        """
        if not checks:
            raise NoChecksError()

        storage = self.results_container.get_storage(meta_image.name)
        num_checks = len(checks)
        for i, checker in enumerate(checks):
            _logger.info("Running check {0} ({1}/{2})".format(checker.NAME, i + 1, num_checks))
            checker.run(meta_image, storage)

        num_reporters = len(reporters)
        for i, reporter in enumerate(reporters):
            _logger.info("Running report {0} ({1}/{2})".format(reporter.NAME, i + 1, num_reporters))
            reporter.report(meta_image, storage)
        self.results_container.dump_storage(meta_image.name)


class DataElementsProcessor(Processor):
    def process_image(self, data_element):
        """
        get the checks and reports for a data element and process it
        """
        _logger.info("Processing of data element {0} starts".format(data_element.name))
        _logger.info("Doing some processing here!!".format(data_element.name))
        data_element.set_configuration(self.configurator)
        data_element.process(self.results_container)


class NoChecksError(Exception):
    pass
