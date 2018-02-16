#!/usr/bin/python
__author__ = 'jcgonzalez'

'''
    Sample DummyProcessor
'''

from time import sleep

import random
import argparse

import pwd
import grp
import os

import json

def get_args():
    '''
    Function for parsing command line arguments
    '''
    parser = argparse.ArgumentParser(description='Simple DummyProcessor',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog, max_help_position=70))
    parser.add_argument('-s', '--steps', help='Initial number of steps', dest='steps', required=True)
    parser.add_argument('-a', '--add', help='Number of additional steps', dest='additional', required=True)
    parser.add_argument('-l', '--log', help='Name of the log file', dest='logfile', default="")
    parser.add_argument('-r', '--results', help='Name of the results file', dest='resfile', default="")

    return parser.parse_args()


def correctOwnership(fh):
    stat_this = os.stat(__file__)   
    uid = stat_this.st_uid #pwd.getpwnam("eucops").pw_uid
    gid = stat_this.st_gid #grp.getgrnam("eucops").gr_gid
    os.fchown(fh.fileno(), uid, gid)
    

def main():
    '''
    Main processor program
    '''

    args = get_args()

    numSteps  = int(args.steps)
    moreSteps = int(args.additional)

    progress      = 0.0
    endProgress   = 100.0
    deltaProgress = endProgress / numSteps

    writeLog = len(args.logfile) > 0
    if writeLog:
        fh = open(args.logfile, 'w')
        correctOwnership(fh)
        fh.write("Starting processing . . .\n")
    else:
        print "Starting processing . . ."

    step = 0
    while step < numSteps:

        # Do something
        sleep(500.0 / 1000.0)

        # Report progress
        step += 1
        progress += deltaProgress
        if writeLog:
            fh.write("Done step {0} of {1} - Processing executed: {2:.1f}%\n".format(step, numSteps, progress))
            fh.flush()
        else:
            print "Done step {0} of {1} - Processing executed: {2:.1f}%".format(step, numSteps, progress)

        # At some point, number of steps could change
        if random.randint(0, 1000) < 50:
            numSteps += moreSteps
            deltaProgress = (endProgress - progress) / (numSteps - step)

    resFile = os.path.basename(args.resfile)
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

    with open(args.resfile, 'w') as fj:
        correctOwnership(fj)
        json.dump(report, fj)
            
    if writeLog:
        fh.write("Processing finished.\n")
        fh.close()
    else:
        print "Processing finished."


if __name__ == "__main__":
    main()
