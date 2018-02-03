#!/usr/bin/python
'''
    QDT_fake driver.py
'''

#from common.controller.controller import Controller
#from common.controller.multi_processor_controller import MultiProcessorController

from time import time,sleep
from astropy.io import fits

import random
import argparse
import json
import logging
import glob


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


# def start_controller(args):
#     controller = Controller(args) if args.processors < 2 else MultiProcessorController(args)
#     controller.run()


def main():
    start_time = time()

    args = get_args()

    format_string = '%(asctime)s:%(levelname)s:%(name)s:%(message)s'
    logging.basicConfig(level=logging.DEBUG, format=format_string, filename=args.log_file)
    logger = logging.getLogger(__name__)

    fres = open(args.result_file, 'w')

    logger.debug("Calling QDT_fake with:")
    logger.debug(json.dumps(args.__dict__))

    input_files = glob.glob(args.input_dir + "/*.fits")
    logger.debug("Input files: %s", ''.join(input_files))

    numSteps  = 40
    moreSteps = 10

    progress      = 0.0
    endProgress   = 100.0
    deltaProgress = endProgress / numSteps

    fres.write("Starting processing . . .\n")

    logger.info("Starting processing . . .")

    step = 0
    while step < numSteps:

        # Do something
        sleep(50.0 / 1000.0)

        # Report progress
        step += 1
        progress += deltaProgress

        logger.info("Processing step {0} of {1}.".format(step, numSteps))
        logger.info("Processing executed: {0:.2f}%".format(progress))

        # At some point, number of steps could change
        if random.randint(0, 1000) < 50:
            numSteps += moreSteps
            deltaProgress = (endProgress - progress) / (numSteps - step)

    for fits_file in input_files:
        hdulist = fits.open(fits_file)
        hdulist.info(output=fres)
        hdu_no = 0
        for hdu in hdulist:
            fres.write("HDU # {}:\n".format(hdu_no))
            for hkey, hval in hdu.header.items():
                fres.write("{:10s} : {}\n".format(hkey, hval))
            hdu_no = hdu_no + 1
        hdulist.close()

    fres.write("Processing finished.\n")
    fres.close()

    elapsed_time = time() - start_time

    logger.info("Processing finished.")
    logger.info("Execution time: {0:.2f} s".format(elapsed_time))



if __name__ == "__main__":
    main()
