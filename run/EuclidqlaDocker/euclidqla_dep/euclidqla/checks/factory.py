import logging

_logger = logging.getLogger('checks')


class NotFoundError(Exception):
    pass


class TheFactory(object):
    """
    Factory to build the required objects. It may also hold already built objects.
    """
    CONSTRUCTOR = "constructor"
    CACHED = "cached"

    def __init__(self):
        self._constructors_dict = {}
        self.cache = {}

    def subscribe(self, name, constructor, cached=False):
        """
        Subscribe constructor of object with a name and specify if it must be cached within the factory.
        """
        if name in self._constructors_dict:
            raise Exception("Constructor already exists")
        self._constructors_dict[name] = {self.CONSTRUCTOR: constructor, self.CACHED: cached}

    def build(self, name, *args, **kwargs):
        """
        Subscribe constructor of object with a name and specify if it must be cached within the factory.
        """
        _logger.debug("Building {0}".format(name))
        if name not in self._constructors_dict:
            _logger.error("{0} not found in factory".format(name))
            raise NotFoundError("Constructor not found in factory")

        if self._constructors_dict[name][self.CACHED]:
            the_object = self._build_cached(name, *args, **kwargs)
        else:
            the_object = self._build_non_cached(name, *args, **kwargs)
        return the_object

    def _build_non_cached(self, name, *args, **kwargs):
        built_instance = self._constructors_dict[name][self.CONSTRUCTOR](*args, **kwargs)
        _logger.debug("{0} built".format(name))
        return built_instance

    def _build_cached(self, name, *args, **kwargs):
        if name in self.cache:
            _logger.debug("{0} was already built. Retrieving instance.".format(name))
            return self.cache[name]
        else:
            self.cache[name] = self._build_non_cached(name, *args, **kwargs)
            return self.cache[name]

theFactory = TheFactory()

TheFactory = None
