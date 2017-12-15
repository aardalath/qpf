#!/usr/bin/python
'''
    Processor driver:
    This is executed with:
      python ../WA/run/bin/RunProcessor.py --taskpath $(pwd) --cfg ./task.cfg.json
'''

from qpfproc.qpfproc import Processor,get_args


__author__ = 'jcgonzalez'


def main():
    '''
    Main processor program
    '''
    args = get_args()
    theProc = Processor(args.taskdir, args.cfgfile)
    theProc.run()


if __name__ == "__main__":
    main()
