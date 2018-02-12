#!/bin/bash

packages=$*

SCRIPTDIR=$(pwd)

TMP=${SCRIPTDIR}/tmp
mkdir ${TMP}

for Pkg in ${packages}; do

    pkg=$(echo $Pkg | tr 'A-Z' 'a-z')
    PKG=$(echo $Pkg | tr 'a-z' 'A-Z')

    mkdir $pkg

    echo "# Procesing pkg ${Pkg} . . . "

    for hdrfile in $(ls ../$pkg/*.h); do

        hdrf=$(basename $hdrfile)
        hdr=$(basename $hdrf .h)
        
        classes=$(egrep "^class " ${hdrfile} | cut -d' ' -f2 | grep -v ';')

        echo "# - - Procesing header ${hdrf} . . . "

        for Cls in ${classes}; do
            
            cls=$(echo $Cls | tr 'A-Z' 'a-z')
            CLS=$(echo $Cls | tr 'a-z' 'A-Z')

            echo "# - - - - Generating test file for class ${Cls} . . . "

            # Get public methods
            objdump -g ../build/${pkg}/CMakeFiles/${pkg}.dir/${hdr}.cpp.o | \
                c++filt | \
                awk '
(/DW_AT_linkage_name/&&/: '${Cls}'::/) { 
  name=$7;
  gsub(/\(.*/,"",name);
  mth=1;
} 
(/DW_AT_accessibility/&&(mth>0)) {
  accss=$4;
  if (accss == "(public)") { print name; }
  mth=0;
}' > ${TMP}/${Cls}.pubmth
            pubmth=$(sed -e "s#$Cls::##g" ${TMP}/${Cls}.pubmth)            
            
            # Generaete header file for tests
            cat <<EOF>$pkg/test_${Cls}.h
#ifndef TEST_${CLS}_H
#define TEST_${CLS}_H

#include "${hdrf}"
#include "gtest/gtest.h"

//using namespace ${Cls};

namespace Test${Cls} {

class Test${Cls} : public ::testing::Test {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    Test${Cls}() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~Test${Cls}() {}

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp() {}

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
    // ${Cls}::obj ev;
};

class Test${Cls}Exit : public Test${Cls} {

protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    // You can do set-up work for each test here.
    Test${Cls}Exit() {}

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~Test${Cls}Exit() {}

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

}

#endif // TEST_${CLS}_H
EOF

            # Generaete header file for tests
            cat <<EOF>$pkg/test_${Cls}.cpp
#include "test_${Cls}.h"

//#define CheckResultOf(s,r) do {                                         \\
//    ev.clear();                                                         \\
//    ev.set(std::string( #s ));                                          \\
//    EXPECT_EQ(ev.getValue(), r);                                        \\
//    } while (0)

namespace Test${Cls} {

EOF
            prevmth=""
            for mth in ${pubmth}; do
                if [ "$prevmth" != "$mth" ]; then
                    rmth=$(echo "$mth"|tr -d '[]&*:')
                    if [ "$mth" == "$rmth" ]; then
                       mmth=$(echo $mth | sed -e 's#operator=#operatorEq#g')
                       echo "# - - - - - - Testing method ${Cls}::${mmth} "
                       cat <<EOF>>$pkg/test_${Cls}.cpp
TEST_F(Test${Cls}, Test_${mmth}) {
    
}

EOF
                    fi
                fi
                prevmth=$mth
            done
            
            cat <<EOF>>$pkg/test_${Cls}.cpp
}           
EOF

        done
                        
    done

done
