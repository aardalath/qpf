#!/bin/bash

testsNamespace="tests"
inputNamespace=""
hasExitTests="no"

#-- Parse command line
while getopts :N:n:x OPT; do
    case $OPT in
        N|+N)
            inputNamespace="$OPTARG"
            ;;
        n|+n)
            testsNamespace="$OPTARG"
            ;;
        x|+x)
            hasExitTests="yes"
            ;;
        *)
            echo "usage: `basename $0` [+-N ARG] [+-n ARG] [+-x} [--] header_files ..."
            exit 2
    esac
done
shift `expr $OPTIND - 1`
OPTIND=1

files=$*
usens="using namespace ${inputNamespace};"

#-- Loop over input files
for f in $files ; do

    base=$(basename $f .o)

    testf=test_${base}

    testh=${testf}.h
    testcpp=${testf}.cpp

    testlbl=$(echo $testf | tr '[a-z]' '[A-Z]')_H

    cat <<EOF>${testh}
#ifndef ${testlbl}
#define ${testlbl}

#include "${base}.h"
#include "gtest/gtest.h"

${usens}

namespace ${testsNamespace} {
EOF

    nm $f | \
        c++filt | \
        egrep "T ${inputNamespace}::" | grep -v "~" | \
        tee ${base}.lst | \
        sed -e "s#${inputNamespace}::##g" | \
        awk -v mthout="${base}.mth" '
BEGIN { print "" > mthout; }
($2 ~ /T|W/) {
    a = $3;
    p = index(a,"::");
    cls = substr(a, 0, p - 1);
    if (cls !~ /\(/) {
        clss = cls"::";
        sub(clss, a, "");
    }
    q = index(a,"(");
    if (q < p) { next; }
    mth = substr(a, p + 2, q - p - 2)
    if (mth == "operator\<\<") { mth = "opInStream"; }
    if (mth == "operator\[\]") { mth = "opSubscript"; }
    if (mth == "operator\=")   { mth = "opAssign"; }
    if (mth == "operator")     { mth = "opFunCall"; }
    printf "%s %s\n", cls, mth >> mthout;
}
'
    classes=$(cat ${base}.mth | cut -d" " -f1 | sort -u)

    for cls in $classes ; do
        testcls=Test${cls}

        usens=""
        if [ -n "$inputNamespace" ]; then
            usens="using namespace ${inputNamespace};"
        fi

        #-- Create header file for test class

        cat <<EOF>>${testh}

class ${testcls} : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    ${testcls}() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~${testcls}() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    ${cls} o;
};
EOF

        if [ -n "${hasExitTests}" ]; then
            cat <<EOF>>${testf}.h

class ${testcls}Exit : public ${testcls} {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    ${testcls}Exit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~${testcls}Exit() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
};
EOF
        fi

    done

    cat <<EOF>>${testf}.h

}

#endif // ${testlbl}
EOF

    #-- Create implementation file for test class

    cat <<EOF>${testcpp}
#include "${testf}.h"

namespace ${testsNamespace} {

EOF

    tac ${base}.mth | sort -k 2,1 | uniq | \
        while read cls mth ; do
            if [ -n "$mth" ]; then
                printf "TEST_F(%s, %s) {\n    EXPECT_EQ(1, 1);\n}\n\n" \
                       "Test${cls}" "${mth}" >>${testcpp}
            fi
        done

    if [ "$hasExitTests" == "yes" ]; then
        for cls in $classes ; do
            testcls=Test${cls}
            cat <<EOF>>${testcpp}

TEST_F(${testcls}Exit, ${testcls}_Exit) {
    EXPECT_EXIT({ exit(1); },
                ::testing::ExitedWithCode(1),
                "");
}
EOF
        done
    fi

    cat <<EOF>>${testf}.cpp

}
EOF

    ls -l ${testf}.h ${testf}.cpp

done
