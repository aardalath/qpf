__author__ = 'vvillar'


class Operation(object):
    def __init__(self, operation):
        self.operation = operation

    def run(self, dataclass):
        self.operation.run(dataclass.meta_image)


class ZeroDescendantOperationSequence(object):
    def __init__(self, processing, diagnostics):
        self.processing = processing
        self.diagnostics = diagnostics  # List of operations

    def run(self, dataclass):
        self.processing.run(dataclass)
        for operation in self.operation_sequence:
            operation.run(dataclass)


class SingleDescendantOperationSequence(ZeroDescendantOperationSequence):
    def __init__(self, processing, diagnostics, operation_seq):
        ZeroDescendantOperationSequence.__init__(self, processing, diagnostics)
        self.operation_sequence = operation_seq  # next operation sequence

    def run(self, dataclass):
        ZeroDescendantOperationSequence.run(self, dataclass)
        self.operation_sequence.run(dataclass)


class MultiDescendantOperationSequence(ZeroDescendantOperationSequence):
    def __init__(self, processing, diagnostics, operation_seq):
        ZeroDescendantOperationSequence.__init__(self, processing, diagnostics)
        self.operation_sequence = operation_seq
        self.cache = None  # Cache needed to hold the data if more than one operation sequence

    def run(self, dataclass):
        ZeroDescendantOperationSequence.run(self, dataclass)
        self.cache = dataclass.copy()
        for op_seq in self.operation_sequence:
            op_seq.run(dataclass)
            dataclass = self.cache.copy()


def build_operation_sequence(operation_list):
    num_op_seqs = sum([isinstance(a,list) for a in operation_list])
    reverse_index = -num_op_seqs if num_op_seqs else None
    processing_op = Operation(operation_list[0])
    diagnostics_ops = [Operation(operation) for operation in operation_list[1: reverse_index]]
    if num_op_seqs > 1:
        print 'Creating multi descendant processing sequence'
        op_seq_list = []
        for proc in operation_list[reverse_index:]:
            op_seq_list.append(build_operation_sequence(proc))
        return MultiDescendantOperationSequence(processing_op, diagnostics_ops, op_seq_list)
    elif num_op_seqs == 1:
        print 'Creating single descendant processing sequence'
        return SingleDescendantOperationSequence(processing_op, diagnostics_ops,
                                                 build_operation_sequence(operation_list[-1]))
    elif num_op_seqs == 0:
        print 'Creating zero descendant processing sequence'
        return ZeroDescendantOperationSequence(processing_op, diagnostics_ops)
    else:
        print 'Error'


if __name__ == '__main__':
    import euclidqla.checks.overscan as _os
    import euclidqla.checks.badpixels as _bp
    operations = [_os.OverscanCheck(), _os.OverscanCheck(), _os.ImageStatistics(),
                  [_os.OverscanCheck(), _os.ImageStatistics(), [_bp.PixelsBelowADC, _bp.OverscanCheck]],
                  [_os.ImageStatistics, _os.OverscanCheck]]
    op_seq = build_operation_sequence(operations)

    def print_op_seq(op_seq):
        print type(op_seq)
        print op_seq.processing.operation.NAME
        for oper in op_seq.diagnostics:
            print oper.operation.NAME
        if isinstance(op_seq, MultiDescendantOperationSequence):
            for oper_seq in op_seq.operation_sequence:
                print_op_seq(oper_seq)
        elif isinstance(op_seq, SingleDescendantOperationSequence):
            print_op_seq(op_seq.operation_sequence)

    print_op_seq(op_seq)
