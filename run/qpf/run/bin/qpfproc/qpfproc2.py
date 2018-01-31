#!/usr/bin/python
__author__ = 'jcgonzalez'

'''
    QPF Processor main class
    Simple implementation of this class will drive the execution of the processors
    launched by the QLA Processing Framework
'''

import os
import sys
import glob
import argparse
import re
import json
import subprocess
import logging

logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)

def get_args():
    '''
    Function for parsing command line arguments
    '''
    parser = argparse.ArgumentParser(description='Euclid QLA Processor',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog, max_help_position=70))
    parser.add_argument('-t', '--taskpath', help='Full task directory', dest='taskdir', required=True)
    parser.add_argument('-c', '--cfg', help='Full configuration file name', dest='cfgfile', required=True)

    return parser.parse_args()


#----------------------------------------------------------------------------
# Class: Processor
#----------------------------------------------------------------------------
class Processor(object):
    '''
    Processor is the base class for all the processors to be executed from
    the Euclid QLA Processing Framework, independent if they are run inside or
    outside Docker containers.
    '''

    # Framework environment related variables
    Home = os.environ['HOME']

    QPFWorkArea = "/Users/jcgonzalez/ws/QPF/processors/WA"  # Home + "/qpf"

    QPFRunPath      = QPFWorkArea + "/run"
    QPFRunToolsPath = QPFWorkArea + "/run/bin"
    QPFProcsPath    = QPFWorkArea + "/bin"

    QPFDckImageDefault  = "qpf-debian:a"
    QPFDckImageRunPath  = "/qpf/run"
    QPFDckImageProcPath = "/qlabin"

    @staticmethod
    def call(taskDir, cmdLine, outFile):
        logging.debug(">>> Trying to run\n>>> '{}'\n>>> in\n>>> '{}'\n".format(cmdLine, taskDir))
        os.chdir(taskDir)
        p = subprocess.Popen(cmdLine, stdout=subprocess.PIPE, shell=True)
        out, err = p.communicate()
        with open(outFile, 'w') as fd:
            fd.write(out)

    @staticmethod
    def substitute(inVar, rule):
        strFrom, strTo = rule.split('=>')
        logging.debug("Trying the substitution in {}: {} => {}".format(inVar, strFrom, strTo))
        return re.sub(strFrom, strTo, inVar)

    @staticmethod
    def get_substitution_rules(item):
        fromVar, rules = item[1:-1].split(':')
        logging.debug("Idenfied subs. rules: {} : {}".format(fromVar, rules))
        return [fromVar, rules.split(',')]

    @staticmethod
    def is_substitution_rules(item):
        return (item[0] == '{') and (item[-1] == '}')

    @staticmethod
    def do_rules(obj, item):
        fromVar, rules = Processor.get_substitution_rules(item)
        value = ' '.join(obj.__dict__[fromVar])
        logging.debug("Original value: {}".format(value))
        for rule in rules:
            logging.debug("{} : {}".format(value, rule))
            value = Processor.substitute(value, rule)
        return value.split(' ')

    def __init__(self, taskDir, cfgFile):
        # Get arguments, and incorporate to object
        self.taskDir  = taskDir
        self.taskId   = os.path.basename(taskDir)

        self.cfgFile  = cfgFile

        #dckOpts = "-i -t"  # Interactive
        dckOpts = "-d -P"  # Daemon mode

        # Load configuration
        with open(self.cfgFile) as json_data:
            self.cfg = json.load(json_data)

        # Evaluate configuration entries
        # 1. Input file(s), Outputs and Log
        self.input     = glob.glob(self.cfg["input"])
        self.output    = self.cfg["output"]
        self.log       = self.cfg["log"]
        # 2. Processor subfolder name (folder under QPF_WA/bin/"
        self.processor = self.cfg["processor"]
        # 3. Processor entire subfolder name
        self.procDir   = Processor.QPFProcsPath + "/" + self.processor
        # 4. Main script to invoke processor (something like driver.py)
        self.script    = self.cfg["script"]
        self.args      = self.cfg["args"]

        # Expand/process output and log (if needed)
        if Processor.is_substitution_rules(self.output):
            self.output = Processor.do_rules(self, self.output)
        if Processor.is_substitution_rules(self.log):
            self.log = Processor.do_rules(self, self.log)

        # Expand arguments
        # Valid placeholders are:
        # - {input}  : Content of the input variable, comma separated if multiple
        # - {output} : Content of the output variable, comma separated if multiple
        # - {log}    : Content of the log variable
        self.args = re.sub('{input}',
                           ','.join(self.input),
                           re.sub('{output}',
                                  ','.join(self.output),
                                  re.sub('{log}',
                                         ','.join(self.log),
                                         self.args)))
        logging.debug("Arguments: {}".format(self.args))

        for v in ['input', 'output', 'log']:
            logging.debug("{}: {}".format(v, self.__dict__[v]))

        # Create additional folders for processor execution
        self.inDir  = self.taskDir + "/in"
        self.logDir = self.taskDir + "/log"
        self.outDir = self.taskDir + "/out"
        for aDir in [self.inDir, self.logDir, self.outDir]:
            if not os.path.exists(aDir):
                os.makedirs(aDir)

        # Prepare Docker folder mapping, just in case...
        self.taskDirImg = Processor.QPFDckImageRunPath + "/" + self.taskId
        self.procDirImg = Processor.QPFDckImageProcPath + "/" + self.processor

        maps = {'taskDir': [self.taskDir, self.taskDirImg],
                'procDir': [self.procDir, self.procDirImg]}
        self.mapping = ""
        for k,v in maps.iteritems():
            self.mapping += " -v {}:{}".format(v[0], v[1])

        self.dckOpts  = "{} -w={} --privileged=true".format(dckOpts, self.taskDirImg)
        self.dckImage = Processor.QPFDckImageDefault

    def run(self, arguments=None, additional="", container=None):
        if arguments is None:
            scriptArgs = self.args
        else:
            scriptArgs = self.args.format(*arguments)

        #os.symlink(self.procDir + "/" + self.script, self.taskDir + "/" + self.script)

        cmdLine = "python {0}/{1} {2} {3}".format(self.procDirImg, self.script,
                                                  scriptArgs, additional)
        if container is None:
            useContainers = self.cfg['container']
        else:
            useContainers = container

        if not useContainers:
            outFile = self.taskDir + "/" + self.processor + ".nfo"
            callCmdLine = cmdLine
        else:
            outFile = self.taskDir + "/docker.id"
            callCmdLine = "docker run {0} {1} {2} {3} 2>&1 "\
                .format(self.dckOpts, self.mapping, self.dckImage, cmdLine)

        Processor.call(self.taskDir, callCmdLine, outFile)


