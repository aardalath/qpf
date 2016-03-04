
from astropy.io import fits as _fits


class FileType(object):
    FITS = 'fits'


class DataType(object):
    VISCCDSIM = 'VISCCDSIM'
    VISCCD = 'VISCCD'
    VISQUAD = 'VISQUAD'


def get_dataclass_list(file_name):
    file_type = file_type_from_name(file_name)
    with open_file_dict[file_type](file_name) as data_file:
        data_type = check_datatype_dict[file_type](data_file)
        dataclasses = build_dataclass_dict[data_type](data_file, file_name)
    return dataclasses


def file_type_from_name(file_name):
    if 'fit' in file_name:
        return FileType.FITS


open_file_dict = {FileType.FITS: _fits.open}


class UknownDataTypeError(Exception):
    pass


def check_type_of_fits(data_file):
    # First we look for the number of HDUs. If it is 145 we know it is VIS
    if len(data_file) > 2:
        science_header = None
        instrument = None
        if 'INSTRUME' in data_file[0].header:
            science_header = 0
        elif 'INSTRUME' in data_file[1].header:
            science_header = 1

        if science_header is not None:
            instrument = data_file[science_header].header['INSTRUME'].upper()

        if 'VISSIM' in instrument:
            # FIXME check if we need the CCD or only the Quadrant
            return DataType.VISCCDSIM
    else:
        raise UknownDataTypeError('Unknown data type')

check_datatype_dict = {FileType.FITS: check_type_of_fits}


def build_visccd_dataclass(input_file, file_name):
    dataclass_list = []
    for i in range(1,37):
        dataclass = {}
        dataclass['parent'] = file_name
        dataclass['name'] = 'CCD' + str(i)
        dataclass['type'] = 'VISCCD'
        base_quad = (i - 1) * 4
        dataclass['components'] = [base_quad + 1, base_quad + 2, base_quad + 3, base_quad + 4]
        dataclass_list.append(dataclass)
    return dataclass_list


def build_visccdsim_dataclass(input_file, file_name):
    dataclass_list = [None]*36
    for n_frame, frame in enumerate(file[1:]):
        ccdx = frame.header['CCDX']
        ccdy = frame.header['CCDY']
        CCD_n = ccdx + ccdy * 6
        quadrant = frame.header['QUADRANT']
        if dataclass_list[CCD_n] is None:
            dataclass = {}
            dataclass['parent'] = file_name
            dataclass['name'] = file_name + '_CCD_' + str(ccdy) + '-' + str(ccdx)
            dataclass['type'] = 'VISCCDSim'
            dataclass['components'] = [None, None, None, None]
            dataclass_list[CCD_n] = dataclass
        dataclass_list[CCD_n]['components'][quadrant] = n_frame
    return dataclass_list


def build_visccdsim_dataclass(input_file, file_name):
    dataclass_list = []
    for j_y in range(1,7):
        for i_x in range(1,7):
            dataclass = {}
            dataclass['parent'] = file_name
            dataclass['name'] = file_name + '_CCD_' + str(j_y) + '-' + str(i_x)
            dataclass['type'] = 'VISCCDSim'
            dataclass['components'] = [None, None, None, None]
            for k_q in range(4):
                n_frame = (j_y - 1) * 24 + (6 - i_x) * 4 + (k_q + 1)
                dataclass['components'][k_q] = n_frame
            dataclass_list.append(dataclass)
    return dataclass_list


def build_visquad_dataclass(input_file):
    pass


build_dataclass_dict = {DataType.VISCCD: build_visccd_dataclass,
                        DataType.VISCCDSIM: build_visccdsim_dataclass,
                        DataType.VISQUAD: build_visquad_dataclass}



if __name__ == '__main__':
    print get_dataclass_list('/home/vvillar/Euclid/Data/SimChallenge/EUC-TEST-FINAL-2015-06-25T135146.900Z.fits')
