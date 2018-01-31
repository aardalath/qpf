#!/usr/bin/env python2
# -*- coding: utf-8 -*-

from time import time,sleep

import os, sys
import requests
import json
import subprocess
import logging

BaseURL = "http://0.0.0.0:8080"


class Task(object):
    '''
    Main class to encapsulate task operations and data
    '''
    GetTaskURL = ''
    EndTaskURL = ''
    DownloadInputURL = ''
    UploadOutputURL = ''

    RunExternalProcessor = True

    def __init__(self):
        self.task_data = None
        self.GetTaskURL = '{}/get_task'.format(BaseURL)
        self.EndTaskURL = '{}/end_task?task_id={}'.format(BaseURL, '{}')
        self.DownloadInputURL = '{}/input/{}'.format(BaseURL, '{}')
        self.UploadOutputURL = '{}/output/{}'.format(BaseURL, '{}')

        logging.basicConfig(format='%(asctime)s [%(levelname)s] %(message)s',
                            level=logging.DEBUG)

        try:
            os.stat('./tasks')
        except:
            os.mkdir('./tasks')

    def get_task_params(self):
        r = requests.get(url=self.GetTaskURL)
        if not r.status_code == requests.codes.ok:
            logging.error("Couldn't get task parameters")
            exit(-1)
        self.task_data = r.json()
        self.task_id = self.task_data['task_id']
        self.in_file = self.task_data['in_file']
        self.out_file = self.task_data['out_file']
        self.log_file = self.task_data['log_file']
        logging.info("TASK {}".format(self.task_id))
        logging.debug(json.dumps(self.task_data))

    def setup_env(self):
        try:
            logging.info("- Setting up task environment")
            self.task_folder = './tasks/{}'.format(self.task_id)
            for fld in ['{}/{}'.format(self.task_folder,x) for x in ['', 'in', 'out', 'log']]:
                os.mkdir(fld)
        except:
            logging.info('Cannot create folders for task {}'.format(self.task_id))

    def download_inputs(self):
        logging.info("- Downloading input file {}".format(self.in_file))
        input_file_url = self.DownloadInputURL.format(self.in_file)
        self.local_input_file = '{}/in/{}'.format(self.task_folder, self.in_file)
        self.local_output_file = '{}/out/{}'.format(self.task_folder, self.out_file)
        self.local_log_file = '{}/log/{}'.format(self.task_folder, self.log_file)
        r = requests.get(input_file_url)
        with open(self.local_input_file, 'wb') as fd:
            fd.write(r.content)

    def build_cmd_line(self):
        script = 'python2 ./QDT/driver.py'
        local_input_dir = '{}/in'.format(self.task_folder)
        argmts = "-v --input {}/in --result {} --log {}".format(self.task_folder,
                                                                self.local_output_file,
                                                                self.local_log_file)

        cmdline = '{} {}'.format(script, argmts)
        logging.debug('Executing: $ {}'.format(cmdline))
        return cmdline

    def run_external_processor(self):
        cmd = self.build_cmd_line()
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True)
        out, err = p.communicate()
        if out:
            with open('{}.out'.format(self.task_id), 'w') as fd:
                fd.write(out)
        if err:
            with open('{}.err'.format(self.task_id), 'w') as fd:
                fd.write(err)

    def process(self):
        '''
        Method to run the actual processing.  In case the class variable RunExternalProcessor is False,
        just a dummy processing is done.
        :return:
        '''
        logging.info("- Start processing . . .")
        start_time = time()

        if self.RunExternalProcessor:
            self.run_external_processor()
        else:
            os.system('od -xao {} > {}'.format(self.local_input_file, self.local_output_file))
            for x in range(100):
                sleep(.05)
                sys.stdout.write('. ')
                sys.stdout.flush()
            sys.stdout.write('\n')
            sys.stdout.flush()

        elapsed_time = time() - start_time
        logging.info("- Done.\n- Execution time: {0:.2f} s".format(elapsed_time))
        with open(self.local_log_file, 'w') as flog:
            flog.write('Execution time: {0:.2f} s'.format(elapsed_time))

    def upload_file(self, url, local_file):
        logging.info("- Uploading output file {}".format(local_file))
        files = {'file': open(local_file, 'rb')}
        r = requests.post(url, files=files)
        if not r.status_code == requests.codes.ok:
            logging.warning("  !!! Couldn't upload file ".format(local_file))

    def upload_outputs(self):
        self.upload_file(self.UploadOutputURL.format(''), self.local_output_file)
        self.upload_file(self.UploadOutputURL.format(''), self.local_log_file)

    def notify_end_of_task(self):
        logging.info("- Notifying end of task")
        end_task_url = self.EndTaskURL.format(self.task_id)
        r = requests.get(url=end_task_url)
        if not r.status_code == requests.codes.ok:
            logging.error("  !!! Couldn't get task parameters")
            exit(-1)

    def run(self):
        task_data = self.get_task_params()
        self.setup_env()
        self.download_inputs()
        self.process()
        self.upload_outputs()
        self.notify_end_of_task()


def main():
    while True:
        task = Task()
        task.run()


if __name__ == '__main__':
    main()
