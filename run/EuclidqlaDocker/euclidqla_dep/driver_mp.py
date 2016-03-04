#!/usr/bin/python
__author__ = 'vvillar'

from euclidqla.acquirer.acquirer import Acquirer, ImageSupplier, ImageRemoteSupplierServer, MultiExtensionImageSupplier, ImageRemoteSupplierClient
from euclidqla.processor.process import Processor
from euclidqla.processor.checks_configurator import checks_configurator_from_file, reports_configurator_from_file, ConfiguratorFromCheck
from euclidqla.result.results_container import ResultsContainer, SocketOutput, FileOutput, ReportSelector, ResultSelector, OutputComposite
from euclidqla.result.fomatters import XMLFormatter, JsonFormatter, NullFormatter
from euclidqla.result.storage import Storage


import argparse
import logging
import json
from multiprocessing import Process
from time import time
import zmq


def get_args():
    parser = argparse.ArgumentParser(description='QLA processing',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog, max_help_position=60))
    parser.add_argument('-i', '--input_dir', help='input directory', dest='input_dir', default='./')
    parser.add_argument('-o', '--report_file', help='report file',
                        dest='report_file', default='report.out')
    parser.add_argument('-r', '--result_file', help='intermediate results file',
                        dest='result_file', default='results.out')
    parser.add_argument('-v', '--verbose', help='show logging', action='store_true')
    parser.add_argument('-p', '--processors', help='number of processors', default='2', type=int)
    return parser.parse_args()


def run_process(number, args):
    log_level = logging.INFO if args.verbose else logging.ERROR
    logging.basicConfig(level=log_level, format='%(asctime)s:%(levelname)s:PROCESS_{0}:%(name)s:%(message)s'.format(number))
    acquirer = Acquirer(ImageRemoteSupplierClient())
    output1 = SocketOutput('5569', NullFormatter(), ReportSelector)
    output2 = SocketOutput('5570', NullFormatter(), ResultSelector)
    output = OutputComposite(output1, output2)
    results_container = ResultsContainer(output)
    configurator = ConfiguratorFromCheck()
    processor = Processor(acquirer, configurator, results_container)
    processor.process()


class Sink(Process):
    def __init__(self, port, output):
        Process.__init__(self)
        self.port = port
        self.output = output

    def run(self):
        context = zmq.Context()
        receiver = context.socket(zmq.PULL)
        receiver.bind('tcp://*:' + str(self.port))

        self.output.open()
        while True:
            data = receiver.recv()
            if data == 'END':
                break
            image_name, image_data = data.split('%::%')
            storage = Storage(image_name)
            storage.report = json.loads(image_data)
            self.output.write_formatted(storage)
        self.output.close()


def send_end_signal(port):
    context = zmq.Context()
    socket = context.socket(zmq.PUSH)
    socket.connect('tcp://localhost:{0}'.format(port))
    socket.send('END')


def main():
    start_time = time()
    args = get_args()
   # log_level = logging.INFO if args.verbose else logging.ERROR
   # logging.basicConfig(level=log_level, format='%(asctime)s:%(levelname)s:%(name)s:%(message)s')

    remote_server = \
        ImageRemoteSupplierServer(MultiExtensionImageSupplier(args.input_dir))
    remote_server.start()

    sink = Sink(5570, FileOutput(args.result_file, JsonFormatter(), ReportSelector)) # TODO Using reportSelector instead of ResultSelector
    sink.start()

    sink_report = Sink(5569, FileOutput(args.report_file, JsonFormatter(prettyprint=True), ReportSelector))
    sink_report.start()

    procs = [Process(target=run_process, args=(i, args)) for i in range(args.processors)]
    [proc.start() for proc in procs]
    [proc.join() for proc in procs]

    send_end_signal(5569)
    send_end_signal(5570)
    remote_server.terminate()
    elapsed_time = time() - start_time
    print "Execution time: {0:.2f} s".format(elapsed_time)


if __name__ == "__main__":
    main()

