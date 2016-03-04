__author__ = 'vvillar'


def check_proc_in_seq(proc, ops_level):
    if ops_level[0] == proc:
        return True, ops_level
    num_op_seqs = sum([isinstance(a,list) for a in ops_level])
    index = len(ops_level) - num_op_seqs
    for ops_seq in ops_level[index:]:
        res, seq = check_proc_in_seq(proc, ops_seq)
        if res:
            return res, seq
    return False, None


def check_proc(proc, ops_def):
    if proc.LEVEL not in ops_def:
        return False, None

    ops_level = ops_def[proc.LEVEL]
    return check_proc_in_seq(proc, ops_level)


def insert_proc_in_seq(ops_def, proc):
    if proc.LEVEL in ops_def:
        ops_def[proc.LEVEL].append([proc])
        return ops_def[proc.LEVEL][-1]
    else:
        ops_def[proc.LEVEL] = [proc]
        return ops_def[proc.LEVEL]


def insert_proc(proc, ops_def):
    res, seq = check_proc(proc, ops_def)
    if res:
        return seq

    if proc.PROC:
        seq = insert_proc(proc.PROC, ops_def)
        if proc.LEVEL == proc.PROC.LEVEL:
            seq.append([proc])
            return seq[-1]
    #     else:
    #         return insert_proc_in_seq(ops_def, proc)
    # else:
    return insert_proc_in_seq(ops_def, proc)


def insert_diagnostic(diagnostic, ops_def):
    for sub_diag in diagnostic.DIAG:
        insert_diagnostic(sub_diag, ops_def)

    ops_seq = insert_proc(diagnostic.PROC, ops_def)
    if diagnostic in ops_seq:
        return
    num_op_seqs = sum([isinstance(a,list) for a in ops_seq])
    index = len(ops_seq) - num_op_seqs
    ops_seq.insert(index, diagnostic)


def build_operation_def(reports):
    ops_def = {}
    for report in reports:
        for diag in report.DIAG:
            insert_diagnostic(diag, ops_def)
    return ops_def


class DataElementConfiguratorFromCheck(object):
    def __init__(self):


    def get_opseq_and_reports(self, data_element):
        reports = self.configurator.get_configuration(data_element)

        return [], []
