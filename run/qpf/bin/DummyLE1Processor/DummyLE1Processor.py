#!/usr/bin/python
__author__ = 'jcgonzalez'

'''
    Sample DummyProcessor
'''

from time import sleep
from shutil import copyfile

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
    parser.add_argument('-i', '--input', help='Input file', dest='input', required=True)
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
            
    copyfile(args.input, args.resfile)
    with open(args.resfile, 'r') as fj:
        correctOwnership(fj)
            
    if writeLog:
        fh.write("Processing finished.\n")
        fh.close()
    else:
        print "Processing finished."


if __name__ == "__main__":
    main()
