__author__ = 'vvillar'
class PROC1(object):
    NAME = 'PROC1'
    PROC = None
    DIAG = None
    LEVEL = 'QUADRANT'


class PROC2(object):
    NAME = 'PROC2'
    PROC = PROC1
    DIAG = None
    LEVEL = 'QUADRANT'


class PROC3(object):
    NAME = 'PROC3'
    PROC = PROC2
    DIAG = None
    LEVEL = 'QUADRANT'

class PROC3B(object):
    NAME = 'PROC3B'
    PROC = PROC1
    DIAG = None
    LEVEL = 'QUADRANT'


class PROC4(object):
    NAME = 'PROC4'
    PROC = PROC3
    DIAG = None
    LEVEL = 'CCD'

procdict = { PROC1.NAME: PROC1, PROC2.NAME: PROC2, PROC3.NAME: PROC3, PROC4.NAME: PROC4}


class DIAG1(object):
    NAME = 'DIAG1'
    PROC = PROC1
    DIAG = None
    LEVEL = 'QUADRANT'


class DIAG3B(object):
    NAME = 'DIAG3B'
    PROC = PROC3B
    DIAG = None
    LEVEL = 'QUADRANT'

class DIAG3(object):
    NAME = 'DIAG3'
    PROC = PROC3
    DIAG = None
    LEVEL = 'QUADRANT'


class DIAGCCD(object):
    NAME = 'DIAGCCD'
    PROC = PROC4
    DIAG = DIAG1
    LEVEL = 'CCD'

class DIAGCCD2(object):
    NAME = 'DIAGCCD2'
    PROC = PROC4
    DIAG = DIAG3B
    LEVEL = 'CCD'


diagdict =  { DIAG1.NAME: DIAG1, DIAGCCD.NAME: DIAGCCD}

class REPORT1(object):
    NAME = 'REPORT1'
    DIAG = DIAG3

class REPORT1B(object):
    NAME = 'REPORT1B'
    DIAG = DIAG3B


class REPORTCCD(object):
    NAME = 'REPORTCCD'
    DIAG = DIAGCCD

class REPORTCCD2(object):
    NAME = 'REPORTCCD2'
    DIAG = DIAGCCD2



reportdict = { REPORT1.NAME: REPORT1, REPORTCCD.NAME: REPORTCCD}


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
    if diagnostic.DIAG:
        insert_diagnostic(diagnostic.DIAG, ops_def)
    #
    ops_seq = insert_proc(diagnostic.PROC, ops_def)
    if diagnostic in ops_seq:
        return
    num_op_seqs = sum([isinstance(a,list) for a in ops_seq])
    index = len(ops_seq) - num_op_seqs
    ops_seq.insert(index, diagnostic)

def build_operation_def(reports):
    ops_def = {}
    for report in reports:
        diag = report.DIAG
        insert_diagnostic(diag, ops_def)
    return ops_def

if __name__ == '__main__':
    reports = [REPORTCCD, REPORT1, REPORTCCD, REPORTCCD, REPORT1, REPORTCCD2]
    ops_def = build_operation_def(reports)
    print 'Lamer'
