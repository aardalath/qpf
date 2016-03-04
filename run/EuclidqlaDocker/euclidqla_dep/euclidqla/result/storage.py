
class Storage(object):
    """
    Class to hold intermediate results and reports under a name
    """
    def __init__(self, image_name):
        self.image_name = image_name
        self.result = {}
        self.report = {}
