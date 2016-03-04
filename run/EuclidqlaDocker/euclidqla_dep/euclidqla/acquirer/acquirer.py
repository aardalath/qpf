from ..datamodel.metaimage import build_image as _build_image

from astropy.io import fits
from glob import iglob as _iglob
import logging
from multiprocessing import Process
from os import path as _path
import zmq

# Family of classes to retrieve the images and metadata to work on

_logger = logging.getLogger('acquirer')


class Acquirer(object):
    """
    This class get all *fits file in a directory.
    Specify input folder and (optional) the file matching expression (e.g. *d3*.fits, *.fits by default)
    """
    def __init__(self, supplier):
        self.supplier = supplier

    def __iter__(self):
        """
        generator that yields the image Metaimage object
        """
        for image_name in self.supplier:
            yield _build_image(image_name)

    def __len__(self):
        _logger.debug('Acquirer: number of images')
        return len(self.supplier)


class ImageSupplier(object):
    def __init__(self, input_dir, files=None):
        if files is None:
            files = "*.fits"
        self.search_dir = _path.join(input_dir, files)
        self._cached_list = []

    def __len__(self):
        return len(self.image_list)

    def __iter__(self):
        return self.image_list.__iter__()

    @property
    def image_list(self):
        """
        return the image names list
        """
        if not self._cached_list:
            self._cached_list = sorted(_iglob(self.search_dir))
        return self._cached_list


class MultiExtensionImageSupplier(ImageSupplier):
    def __init__(self, input_dir, files=None):
        self.len = 0
        ImageSupplier.__init__(self, input_dir, files=None)

    def __iter__(self):
        for name in self.image_list:
            image_len = len(fits.open(name))
            if image_len - 2:  # FIXME hack to get the images
                for i in range(1, image_len):
                    yield name + '[' + str(i) + ']'
            else:
                yield name

    def __len__(self):
        if not self.len:
            for name in self.image_list:
                self.len += len(fits.open(name)) - 1
        return self.len


class ImageRemoteSupplierClient(object):
    def __init__(self):
        self.socket = None

    def __iter__(self):
        if not self.socket:
            self.initialize_socket()
        return self

    def __next__(self):
        return self.next()

    def next(self):
        self.socket.send(b'GET')
        name = self.socket.recv()
        if name == 'END':
            raise StopIteration
        else:
            return name

    def __len__(self):
        _logger.debug('Client: number of images')
        if not self.socket:
            self.initialize_socket()
        self.socket.send(b'NUMBER')
        return int(self.socket.recv())

    def initialize_socket(self):
        _logger.debug('Client: Initializing socket')
        context = zmq.Context()
        self.socket = context.socket(zmq.REQ)
        self.socket.connect("tcp://localhost:5559")


class ImageRemoteSupplierServer(Process):
    def __init__(self, supplier):
        Process.__init__(self)
        self.supplier = supplier

    def run(self):
        context = zmq.Context()
        socket = context.socket(zmq.REP)
        socket.bind("tcp://*:5559")

        iterator = iter(self.supplier)
        while True:
            msg = socket.recv()
            _logger.debug('Server: Received message {0}'.format(msg))
            if msg == b'NUMBER':
                _logger.info('Server: Received petition for number of images')
                socket.send(str(len(self.supplier)))
            else:
                _logger.info('Server: Received petition for next image')
                socket.send(next(iterator, 'END'))

