#!/usr/bin/python
# -*- coding: utf-8 -*-
'''Processor driver

Usage:

  python <path>/RunProcessor.py --taskpath <taskPath> --cfg <jsonCfgFile>

where:

  <path>           Directory where the RunProcessor.py script is located.
  <tashPath>       Directory where the task is to be executed (where the
                   actual processor should be run.
  <jsonCfgFile>    Configuration file for the execution of the processor.

Alternatively, you can activate the execution permissions of this script, and
call it directly.

Note that these scripts expect that you have defined the env. variable
QPFWA as the QPF Working Area path (usually /home/eucops/qpf). Otherwise
they try to pressume its value from the location of the scripts
themselves.

Usage example:

  python ../WA/run/bin/RunProcessor.py --taskpath $(pwd) --cfg ./task.cfg.json

'''

from qpfproc.qpfproc import Processor,get_args


VERSION = '0.1.0'

__author__ = "jcgonzalez"
__version__ = VERSION
__email__ = "jcgonzalez@sciops.esa.int"
__status__ = "Prototype" # Prototype | Development | Production


def main():
    '''
    Main processor program
    '''
    args = get_args()
    theProc = Processor(args.taskdir, args.cfgfile)
    theProc.run()


if __name__ == "__main__":
    main()
