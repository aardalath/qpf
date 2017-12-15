#!/usr/bin/python
__author__ = 'jcgonzalez'

'''
    Sample DummyProcessor
'''

from time import sleep

import random
import argparse


def get_args():
    '''
    Function for parsing command line arguments
    '''
    parser = argparse.ArgumentParser(description='Simple DummyProcessor',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog, max_help_position=70))
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

    print "Starting processing . . ."

    step = 0
    while step < numSteps:

        # Do something
        sleep(500.0 / 1000.0)

        # Report progress
        step += 1
        progress += deltaProgress
        print "Done step {0} of {1} [P] {2:.1f}% . . .".format(step, numSteps, progress)

        # At some point, number of steps could change
        if random.randint(0, 1000) < 50:
            numSteps += moreSteps
            deltaProgress = (endProgress - progress) / (numSteps - step)

    print "Processing finished."


if __name__ == "__main__":
    main()
