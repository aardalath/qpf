from euclidqla.datamodel.metaimage import build_image as _build_image
from euclidqla.result.storage import Storage

class DataElement(object):
    pass


class ImageDataElement(DataElement):
    pass


class VISSimCCDDataElement(ImageDataElement):
    def __init__(self, dataclass):
        self.parent = dataclass['parent']
        self.name = dataclass['name']
        self.type = self.instrument = 'VISCCD'
        self.observing_mode = 'SCIENCE'
        self.quadrants = []
        for i in range(4):
            quadrant_dataclass = {'parent': dataclass['parent'], 'name': dataclass['name'],
                                  'type': 'VISQuadrant', 'component': dataclass['components'][i]}
            self.quadrants.append(VISSimQuadrantDataElement(quadrant_dataclass))

    def set_configuration(self, configurator):
        self.diagnostics, self.reporters = configurator.get_configuration(self)
        for quadrant in self.quadrants:
            quadrant.set_configuration(configurator)

    def process(self, results_container):
        for quadrant in self.quadrants:
            quadrant.process(results_container)
        results_container.dump_storage(self.name)


class VISSimQuadrantDataElement(ImageDataElement):
    def __init__(self, dataclass):
        self.parent = dataclass['parent']
        self.name = dataclass['name']
        self.type = self.instrument = 'VISQUADRANT'
        self.observing_mode = 'SCIENCE'
        quadrant_name = dataclass['parent'] + '[' + str(dataclass['component']) + ']'
        self.meta_image = _build_image(quadrant_name)
        self.meta_image.name = 'QUAD' + '[' + str(dataclass['component']) + ']'

    def set_configuration(self, configurator):
        self.diagnostics, self.reporters = configurator.get_configuration(self)

    def process(self, results_container):
        storage_image = results_container.get_storage(self.name)
        storage_quad = results_container.get_storage(self.meta_image.name)
        for diagnostic in self.diagnostics:
            diagnostic.run(self.meta_image, storage_quad)
        for reporter in self.reporters:
            reporter.report(self.meta_image, storage_quad)
        storage_image.result[self.meta_image.name] = storage_quad.result
        storage_image.report[self.meta_image.name] = storage_quad.report

if __name__ == '__main__':
    dataclass_vis = {'components': [21, 22, 23, 24], 'type': 'VISCCDSim', 'name': 'CCD_1-1',
                     'parent': '/home/vvillar/Euclid/Data/SimChallenge/EUC-TEST-FINAL-2015-06-25T135146.900Z.fits'}
    print dataclass_vis
    image = VISSimCCDDataElement(dataclass_vis)
    print image.quadrants