#!/usr/bin/python
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


__author__ = 'jcgonzalez' # QDT fake


# Change INFO for DEBUG to get debug messages
Process_log_level = logging.DEBUG

# Set up logging information
format_string = '%(asctime)s:%(levelname)s:%(name)s:%(message)s'
logging.basicConfig(level=Process_log_level, format=format_string, stream=sys.stderr)


def get_args():
    '''
    Function for parsing command line arguments
    '''
    parser = argparse.ArgumentParser(description='Euclid QLA Processor',
                                     formatter_class=lambda prog:
                                     argparse.HelpFormatter(prog,
                                                            max_help_position=70))
    parser.add_argument('-t', '--taskpath',
                        help='Full task directory',
                        dest='taskdir', required=True)
    parser.add_argument('-c', '--cfg',
                        help='Full configuration file name',
                        dest='cfgfile', required=True)

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

    QPFRunPath = QPFWorkArea + "/run"
    QPFRunToolsPath = QPFWorkArea + "/run/bin"
    QPFProcsPath = QPFWorkArea + "/bin"

    QPFDckImageDefault = "qpf-debian:a"
    QPFDckImageRunPath = "/qpf/run"
    QPFDckImageProcPath = "/qlabin"

    @staticmethod
    def call(task_dir, cmd_line, out_file):
        '''
        Actual method calling the process
        '''
        logging.debug(">>> Trying to run\n>>> '%s'\n>>> in\n>>> '%s'\n",
                      cmd_line, task_dir)
        os.chdir(task_dir)
        proc = subprocess.Popen(cmd_line, stdout=subprocess.PIPE, shell=True)
        out_str, err_str = proc.communicate()
        with open(out_file, 'w') as f_handler:
            f_handler.write(out_str)

    @staticmethod
    def substitute(in_var, rule):
        '''
        Apply a single substitution rule
        '''
        str_from, str_to = rule.split('=>')
        logging.debug("Trying the substitution in %s: %s => %s",
                      in_var, str_from, str_to)
        return re.sub(str_from, str_to, in_var)

    @staticmethod
    def get_substitution_rules(item):
        '''
        Get the list of substitution rules
        '''
        from_var, rules = item[1:-1].split(':')
        logging.debug("Idenfied subs. rules: %s : %s", from_var, rules)
        return [from_var, rules.split(',')]

    @staticmethod
    def is_substitution_rules(item):
        '''
        Determine if a certain item has rules inside
        '''
        if len(item) < 1:
            return False
        return (item[0] == '{') and (item[-1] == '}')

    @staticmethod
    def do_rules(obj, item):
        '''
        Class method to apply a set of rules from config file
        '''
        from_var, rules = Processor.get_substitution_rules(item)
        value = ' '.join(obj.__dict__[from_var])
        logging.debug("Original value: %s", value)
        for rule in rules:
            logging.debug("%s : %s", value, rule)
            value = Processor.substitute(value, rule)
        return value.split(' ')

    def __init__(self, task_dir, cfg_file):
        '''
        Instance initialization method
        '''
        # Get arguments, and incorporate to object
        self.task_dir = task_dir
        self.task_id = os.path.basename(task_dir)

        self.cfg_file = cfg_file

        #dck_opts = "-i -t"  # Interactive
        dck_opts = "-d -P"  # Daemon mode

        # Load configuration
        with open(self.cfg_file) as json_data:
            self.cfg = json.load(json_data)

        # Evaluate configuration entries
        # 1. Input file(s), Outputs and Log
        self.input = glob.glob(self.cfg["input"])
        self.output = self.cfg["output"]
        self.log = self.cfg["log"]
        # 2. Processor subfolder name (folder under QPF_WA/bin/"
        self.processor = self.cfg["processor"]
        # 3. Processor entire subfolder name
        self.proc_dir = Processor.QPFProcsPath + "/" + self.processor
        # 4. Main script to invoke processor (something like driver.py)
        self.script = self.cfg["script"]
        self.args = self.cfg["args"]

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
        # self.args = re.sub('{input}',
        #                    ','.join(self.input),
        #                    re.sub('{output}',
        #                           ','.join(self.output),
        #                           re.sub('{log}',
        #                                  ','.join(self.log),
        #                                  self.args)))
        entries = self.cfg.copy()
        entries.update({'input': ','.join(self.input),
                        'output': ','.join(self.output),
                        'log': ','.join(self.log)})
        for kitem, vitem in entries.iteritems():
            self.args = re.sub('{' + kitem + '}', str(vitem), self.args)

        logging.debug("Arguments: %s", self.args)

        for cfg_item in ['input', 'output', 'log']:
            logging.debug("%s: %s", cfg_item, self.__dict__[cfg_item])

        # Create additional folders for processor execution
        self.in_dir = self.task_dir + "/in"
        self.log_dir = self.task_dir + "/log"
        self.out_dir = self.task_dir + "/out"
        for a_dir in [self.in_dir, self.log_dir, self.out_dir]:
            if not os.path.exists(a_dir):
                os.makedirs(a_dir)

        # Prepare Docker folder mapping, just in case...
        self.task_dir_img = Processor.QPFDckImageRunPath + "/" + self.task_id
        self.proc_dir_img = Processor.QPFDckImageProcPath + "/" + self.processor

        maps = {'task_dir': [self.task_dir, self.task_dir_img],
                'proc_dir': [self.proc_dir, self.proc_dir_img]}
        self.mapping = ""
        for mkey, mval in maps.iteritems():
            self.mapping += " -v {}:{}".format(mval[0], mval[1])

        self.dck_opts = "{} -w={} --privileged=true".format(dck_opts, self.task_dir_img)
        self.dck_image = Processor.QPFDckImageDefault

    def run(self, arguments=None, additional="", container=None):
        '''
        Public method to execute the configured processor
        :param arguments: list of values for the argument placeholders in cfg.
        :param additional: additional string to add to command line
        :param container: TRUE: launch processor inside a Docker container
        :return: Nothing
        '''
        if arguments is None:
            script_args = self.args
        else:
            script_args = self.args.format(*arguments)

        #os.symlink(self.proc_dir + "/" + self.script,
        #           self.task_dir + "/" + self.script)

        cmd_line = "python {0}/{1} {2} {3}".format(self.proc_dir_img, self.script,
                                                   script_args, additional)
        if container is None:
            use_containers = self.cfg['container']
        else:
            use_containers = container

        if not use_containers:
            out_file = self.task_dir + "/" + self.processor + ".nfo"
            call_cmd_line = cmd_line
        else:
            out_file = self.task_dir + "/docker.id"
            call_cmd_line = "docker run {0} {1} {2} {3} 2>&1 "\
                .format(self.dck_opts, self.mapping, self.dck_image, cmd_line)

        Processor.call(self.task_dir, call_cmd_line, out_file)
