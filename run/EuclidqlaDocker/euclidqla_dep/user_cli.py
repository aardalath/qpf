#!/usr/bin/python
#from __future__ import print_function

from euclidqla.datamodel.metaimage import build_image

import argparse


def get_args():
    parser = argparse.ArgumentParser(description='QLA command line.')
    parser.add_argument('-i', '--input_files', nargs='+', dest='input_files')
    return parser.parse_args()

if __name__ == "__main__":
    args = get_args()
    images = []
    if args.input_files:
        for image_name in args.input_files:
            try:
                images.append(build_image(image_name))
            except IOError as io_error:
                print io_error

    try:
        from IPython import start_ipython
        print "\nIPython available. Using IPython shell...\n"
        start_ipython(['-i', 'user_cli_setup.py'], user_ns={'images': images})
    except ImportError:
        print "\nIPython not available. Using Python shell...\n"
        from user_cli_setup import *
        import code
        code.interact(local=locals())
