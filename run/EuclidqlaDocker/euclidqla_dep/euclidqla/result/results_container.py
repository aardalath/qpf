
from .storage import Storage

from abc import ABCMeta, abstractmethod
import zmq


class ResultsContainer(object):
    """
    Class in charge of put results in the output
    """
    def __init__(self, output):
        self.output = output
        self.results = {}

    def __enter__(self):
        self.output.open()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.output.close()

    def get_storage(self, image_name):
        """
        Get a storage object for the image name.
        If it does not exist, create a new one.
        """
        if image_name not in self.results:
            self.results[image_name] = Storage(image_name)
        return self.results[image_name]

    def dump_storage(self, image_name):
        """
        Write storage data to the output and detach storage object from itself
        """
        self.output.write_formatted(self.get_storage(image_name))
        self.results.pop(image_name)

    def close(self):
        """
        Close output
        """
        self.output.close()


class BaseSelector(object):
    """
    Base abstract class for selector
    """
    __metaclass__ = ABCMeta

    @classmethod
    def get_data(cls, storage):
        """
        Get relevant data: image name and data
        """
        return storage.image_name, cls.get_specific_data(storage)

    @staticmethod
    @abstractmethod
    def get_specific_data(storage):
        pass


class ResultSelector(BaseSelector):
    """
    Class to select the intermediate results data
    """
    @staticmethod
    def get_specific_data(storage):
        return storage.result


class ReportSelector(BaseSelector):
    """
    Class to select the report data
    """
    @staticmethod
    def get_specific_data(storage):
        return storage.report


class BaseOutput(object):
    """
    Base abstract class for output classes
    """
    __metaclass__ = ABCMeta

    def __init__(self, formatter, selector):
        self.formatter = formatter
        self.selector = selector

    def open(self):
        """
        Initial write when output opened
        """
        self.write(self.formatter.format_start())

    def write_formatted(self, storage):
        """
        Format data selected by selector from storage with formatter and write to output
        """
        if not isinstance(storage, Storage):
            raise TypeError("WriteOutput needs a Storage")
        formatted_data = self.formatter.format_data(*(self.selector.get_data(storage)))
        self.write(formatted_data)

    @abstractmethod
    def write(self, data):
        pass

    def close(self):
        """
        Final write when output closed
        """
        self.write(self.formatter.format_close())


class SocketOutput(BaseOutput):
    """
    Handles output to ZMQSocket
    """
    def __init__(self, port, formatter, selector):
        BaseOutput.__init__(self, formatter, selector)
        self.port = port
        self.socket = None

    def open(self):
        # Socket to send messages to
        context = zmq.Context()
        self.socket = context.socket(zmq.PUSH)
        self.socket.connect('tcp://localhost:{0}'.format(self.port))

    def write(self, data):
        self.socket.send(data)

    def close(self):
        self.socket.close()


class FileOutput(BaseOutput):
    """
    Handles output to file
    """
    def __init__(self, filename, formatter, selector):
        self.file = open(filename, 'w')
        BaseOutput.__init__(self, formatter, selector)

    def write(self, data):
        self.file.write(data)

    def close(self):
        BaseOutput.close(self)
        self.file.close()


class ConsoleOutput(BaseOutput):
    """
    Handles output to console
    """
    def __init__(self, formatter, selector):
        BaseOutput.__init__(self, formatter, selector)

    def write(self, data):
        print("%s" % data),  # needed to avoid newline


class NullOutput(object):
    """
    Produces no output
    """
    def open(self):
        pass

    def close(self):
        pass

    def write_formatted(self, storage):
        pass

    def write(self, data):
        pass


class OutputComposite(object):
    """
    Composite of outputs
    """

    def __init__(self, *args):
        self.outputs = list(args)

    def open(self):
        for output in self.outputs:
            output.open()

    def add(self, output):
        self.outputs.append(output)

    def write_formatted(self, storage):
        for output in self.outputs:
            output.write_formatted(storage)

    def close(self):
        for output in self.outputs:
            output.close()
