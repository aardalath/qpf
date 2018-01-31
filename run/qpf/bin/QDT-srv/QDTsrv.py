#!/usr/bin/python
'''
    QDTsrv main script
'''

#from common.controller.controller import Controller
#from common.controller.multi_processor_controller import MultiProcessorController

import qdttask.qdttask as qdtsk

import argparse


VERSION = '0.1'

__author__ = 'jcgonzalez' # QDT service mode


def get_args():
    parser = argparse.ArgumentParser(description='QLA Diagnostics Processing Tool - Service v {}'.format(VERSION),
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog, max_help_position=60))

    parser.add_argument('-s', '--server',
                        help='URL of the data server',
                        dest='server_url',
                        default=qdtsk.BaseURL)
    parser.add_argument('-v', '--verbose',
                        help='Verbose mode, logging info',
                        action='store_true')
    # parser.add_argument('-d', '--debugging',   help='Show debug info', required=False, action='store_true')
    # # conf parameters
    # parser.add_argument('--vis_bpm', type=str)
    # parser.add_argument('--vis_sat_map_fits', type=str)
    # parser.add_argument('--intermediate_products', action='store_true')
    # parser.add_argument('--checks_file', type=str, default=None)
    # parser.add_argument('--det_gain', action='store_true')

    return parser.parse_args()


def main():
    args = get_args()
    print 'Base URL is: {}'.format(qdtsk.BaseURL)
    qdtsk.BaseURL = args.server_url
    print 'Base URL is: {}'.format(qdtsk.BaseURL)

    # Create Taslk entity, and launch it
    task = qdtsk.Task()
    task.run()


if __name__ == '__main__':
    main()
