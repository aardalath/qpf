#!/usr/bin/python
'''
    QDT_fake driver.py
'''

from time import time,sleep
from astropy.io import fits

import random
import argparse
import json
import logging
import glob
import os


__author__ = 'jcgonzalez' # QDT fake


def get_args():
    parser = argparse.ArgumentParser(description='QLA Diagnostics Processing Tool',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog, max_help_position=60))

    parser.add_argument('-i', '--input_dir',   help='input directory',    dest='input_dir', default='./')
    parser.add_argument('-r', '--result_file', help='results file',       dest='result_file', default='results.out')
    parser.add_argument('-l', '--log_file',    help='execution log file', dest='log_file', default=None)
    parser.add_argument('-p', '--processors',  help='number of processors', default='1', type=int)
    parser.add_argument('-v', '--verbose',     help='show logging', action='store_true')
    parser.add_argument('-d', '--debugging',   help='show debug', required=False, action='store_true')


    # conf parameters
    parser.add_argument('--vis_bpm', type=str)
    parser.add_argument('--vis_sat_map_fits', type=str)
    parser.add_argument('--intermediate_products', action='store_true')
    parser.add_argument('--checks_file', type=str, default=None)
    parser.add_argument('--det_gain', action='store_true')

    return parser.parse_args()


def main():
    start_time = time()

    args = get_args()

    format_string = '%(asctime)s:%(levelname)s:%(name)s:%(message)s'
    logging_level = logging.INFO
    if args.debugging:
        logging_level = logging.DEBUG
    logging.basicConfig(level=logging_level, format=format_string, filename=args.log_file)
    logger = logging.getLogger(__name__)

    fres = open(args.result_file, 'w')

    logger.debug("Calling QDT_fake with:")
    logger.debug(json.dumps(args.__dict__))

    input_files = glob.glob(args.input_dir + "/*.fits")
    logger.info("Input files: %s", ''.join(input_files))

    for fits_file in input_files:
        hdulist = fits.open(fits_file)
        hnfo = hdulist.info(output=False)
        hnfo.insert(0,('----','-------','-------','------',
                       '-------------','-------'))
        hnfo.insert(0,('No.','Name','Type','Cards','Dimensions','Format'))
        for t in hnfo:
            tt = map(str, t)
            logger.info('{0:4s} {1:10s} {2:16s} {3:8s} {4:16s} {5:8s}'.format(*tt))

        hdu_no = 0
        for hdu in hdulist:
            logger.info("HDU # {0}:".format(hdu_no))
            for hkey, hval in hdu.header.items():
                logger.info("{0:10s} : {1}".format(hkey, hval))
            hdu_no = hdu_no + 1
        hdulist.close()

    numSteps  = 40
    moreSteps = 10

    progress      = 0.0
    endProgress   = 100.0
    deltaProgress = endProgress / numSteps

    logger.info("Starting processing . . .")

    step = 0
    while step < numSteps:

        # Do something
        sleep(50.0 / 1000.0)

        # Report progress
        step += 1
        progress += deltaProgress

        if args.verbose:
            logger.info("Processing step {0} of {1}.".format(step, numSteps))
            logger.info("Processing executed: {0:.2f}%".format(progress))

        # At some point, number of steps could change
        if random.randint(0, 1000) < 50:
            numSteps += moreSteps
            deltaProgress = (endProgress - progress) / (numSteps - step)

    resFile = os.path.basename(input_files[0]) # args.result_file
    report = {
        resFile: {
            "CCD_1-6": {
                "Q-1": {
                    "diagnostics": {
                        "Electronic_Offset": {
                            "name": "Electronic_Offset",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "value": 200.02508798391153
                            }
                        },
                        "Overflow_Pixels": {
                            "name": "Overflow_Pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        },
                        "Readout_Noise": {
                            "name": "Readout_Noise",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "value": 1.4694961041027172
                            }
                        },
                        "Saturated_pixels": {
                            "name": "Saturated_pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        },
                        "Saturation_Level": {
                            "name": "Saturation_Level",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "map_ADU": [
                                    [ 1297.0, 464.0, 202.0, 160536.59375, 173045.09375 ],
                                    [ 1296.0, 464.5, 61.0, 159436.09375, 172468.5 ],
                                    [ 1298.0, 464.5, 7.0, 172322.796875, 172759.90625 ],
                                    [ 1605.0, 1661.5, 35.0, 156940.59375, 171814.40625 ],
                                    [ 1606.0, 1661.5, 7.0, 171898.09375, 172970.703125 ]
                                ]
                            }
                        },
                        "Statistics": {
                            "name": "Statistics",
                            "result": {
                                "messages": [ "Std.Dev. threshold (150) exceeded" ],
                                "outcome": "Warning"
                            },
                            "values": {
                                "average": -2332.303377663727,
                                "median": -2418.0,
                                "std": 1641.5975057454245
                            }
                        },
                        "Underflow_Pixels": {
                            "name": "Underflow_Pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        }
                    },
                    "processing": {
                        "BIAS_COR": {
                            "value": 1000.0
                        },
                        "GAIN_COR": {
                            "value": 3.1
                        }
                    }
                },
                "Q-2": {
                    "diagnostics": {
                        "Electronic_Offset": {
                            "name": "Electronic_Offset",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "value": 200.01892911010557
                            }
                        },
                        "Overflow_Pixels": {
                            "name": "Overflow_Pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        },
                        "Readout_Noise": {
                            "name": "Readout_Noise",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "value": 1.478162114445415
                            }
                        },
                        "Saturated_pixels": {
                            "name": "Saturated_pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        },
                        "Saturation_Level": {
                            "name": "Saturation_Level",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "map_ADU": [
                                    [ 1148.0, 27.0, 8.0, 161461.953125, 165933.703125 ]
                                ]
                            }
                        },
                        "Statistics": {
                            "name": "Statistics",
                            "result": {
                                "messages": [ "Std.Dev. threshold (150) exceeded" ],
                                "outcome": "Warning"
                            },
                            "values": {
                                "average": -2347.976286844222,
                                "median": -2418.0,
                                "std": 792.1386082283798
                            }
                        },
                        "Underflow_Pixels": {
                            "name": "Underflow_Pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        }
                    },
                    "processing": {
                        "BIAS_COR": {
                            "value": 1000.0
                        },
                        "GAIN_COR": {
                            "value": 3.1
                        }
                    }
                },
                "Q-3": {
                    "diagnostics": {
                        "Electronic_Offset": {
                            "name": "Electronic_Offset",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "value": 200.02735042735043
                            }
                        },
                        "Overflow_Pixels": {
                            "name": "Overflow_Pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        },
                        "Readout_Noise": {
                            "name": "Readout_Noise",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "value": 1.480563748079826
                            }
                        },
                        "Saturated_pixels": {
                            "name": "Saturated_pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        },
                        "Saturation_Level": {
                            "name": "Saturation_Level",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "map_ADU": [
                                    [ 262.0, 1177.5, 59.0, 169914.09375, 173082.296875 ],
                                    [ 261.0, 1177.0, 6.0, 170227.203125, 170689.09375 ]
                                ]
                            }
                        },
                        "Statistics": {
                            "name": "Statistics",
                            "result": {
                                "messages": [ "Std.Dev. threshold (150) exceeded" ],
                                "outcome": "Warning"
                            },
                            "values": {
                                "average": -2337.071306827887,
                                "median": -2418.0,
                                "std": 1113.4418961469928
                            }
                        },
                        "Underflow_Pixels": {
                            "name": "Underflow_Pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        }
                    },
                    "processing": {
                        "BIAS_COR": {
                            "value": 1000.0
                        },
                        "GAIN_COR": {
                            "value": 3.1
                        }
                    }
                },
                "Q-4": {
                    "diagnostics": {
                        "Electronic_Offset": {
                            "name": "Electronic_Offset",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "value": 200.03288084464555
                            }
                        },
                        "Overflow_Pixels": {
                            "name": "Overflow_Pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        },
                        "Readout_Noise": {
                            "name": "Readout_Noise",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "value": 1.4760630140943263
                            }
                        },
                        "Saturated_pixels": {
                            "name": "Saturated_pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        },
                        "Saturation_Level": {
                            "name": "Saturation_Level",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "map_ADU": [
                                    [ 603.0, 97.0, 14.0, 166784.65625, 173321.0 ],
                                    [ 1551.0, 464.0, 54.0, 157662.90625, 171767.90625 ],
                                    [ 1552.0, 464.0, 8.0, 171560.203125, 173001.703125 ],
                                    [ 1975.0, 2011.0, 6.0, 153977.0, 157563.703125 ]
                                ]
                            }
                        },
                        "Statistics": {
                            "name": "Statistics",
                            "result": {
                                "messages": [ "Std.Dev. threshold (150) exceeded" ],
                                "outcome": "Warning"
                            },
                            "values": {
                                "average": -2344.572935556028,
                                "median": -2418.0,
                                "std": 1085.328809959921
                            }
                        },
                        "Underflow_Pixels": {
                            "name": "Underflow_Pixels",
                            "result": { "outcome": "Nominal" },
                            "values": {
                                "number": 0
                            }
                        }
                    },
                    "processing": {
                        "BIAS_COR": {
                            "value": 1000.0
                        },
                        "GAIN_COR": {
                            "value": 3.1
                        }
                    }
                },
                "diagnostics": {
                }
            }
        }
    }

    with open(args.result_file, 'w') as fj:
        json.dump(report, fj)
            
    elapsed_time = time() - start_time

    logger.info("Processing finished.")
    if args.verbose:
        logger.info("Execution time: {0:.2f} s".format(elapsed_time))


if __name__ == "__main__":
    main()
