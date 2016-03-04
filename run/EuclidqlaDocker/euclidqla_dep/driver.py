#!/usr/bin/python
__author__ = 'vvillar'

from euclidqla.acquirer.acquirer import Acquirer, ImageSupplier, MultiExtensionImageSupplier
from euclidqla.processor.process import Processor
from euclidqla.processor.checks_configurator import ConfiguratorFromCheck
from euclidqla.result.results_container import ResultsContainer, FileOutput, ReportSelector, ResultSelector, OutputComposite
from euclidqla.result.fomatters import JsonFormatter

from euclidqla.controller.controller import Controller, NewController

import argparse
import logging
from time import time


def get_args():
    parser = argparse.ArgumentParser(description='QLA processing',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog, max_help_position=60))
    parser.add_argument('-i', '--input_dir', help='input directory', dest='input_dir', default='./')
    parser.add_argument('-o', '--report_file', help='report file',
                        dest='report_file', default='report.out')
    parser.add_argument('-r', '--result_file', help='intermediate results file',
                        dest='result_file', default='results.out')
    parser.add_argument('-v', '--verbose', help='show logging', action='store_true')
    return parser.parse_args()


def main():
    start_time = time()

    args = get_args()
    controller = Controller(args)
    controller.run()

    elapsed_time = time() - start_time
    print "Execution time: {0:.2f} s".format(elapsed_time)


if __name__ == "__main__":
    main()

