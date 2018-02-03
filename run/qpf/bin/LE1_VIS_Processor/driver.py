#!/usr/bin/python
'''
    Fake LE1 NISP Processor
'''

from time import sleep

import random
import argparse


__author__ = 'jcgonzalez'


def get_args():
    '''
    Function for parsing command line arguments
    '''
    parser = argparse.ArgumentParser(description='Fake LE1 NISP Processor',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog, max_help_position=70))
    parser.add_argument('-i', '--input_dir',   help='input directory',    dest='input_dir', default='./')
    parser.add_argument('-r', '--output', help='output file',       dest='output', default='output.dat')
    parser.add_argument('-l', '--log',    help='execution log file', dest='logfile', default=None)
    parser.add_argument('-s', '--steps', help='Initial number of steps', dest='steps', required=True)
    parser.add_argument('-a', '--add', help='Number of additional steps', dest='additional', required=True)

    return parser.parse_args()

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
        fh.write("Starting processing . . .\n")
    else:
        print "Starting processing . . ."

    step = 0
    while step < numSteps:

        # Do something
        sleep(100.0 / 1000.0)

        # Report progress
        step += 1
        progress += deltaProgress
        if writeLog:
            fh.write("Done step {0} of {1} - Processing executed: {2:.1f}%\n".format(step, numSteps, progress))
        else:
            print "Done step {0} of {1} - Processing executed: {2:.1f}%".format(step, numSteps, progress)

        # At some point, number of steps could change
        if random.randint(0, 1000) < 50:
            numSteps += moreSteps
            deltaProgress = (endProgress - progress) / (numSteps - step)

    if writeLog:
        fh.write("Processing finished.\n")
        fh.close()
    else:
        print "Processing finished."


if __name__ == "__main__":
    main()
