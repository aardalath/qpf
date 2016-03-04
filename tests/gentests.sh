#!/bin/bash

cat <<EOF>list
src QPF
qpfhmi QPF
sdc SDC
infix InFix
EOF

while read d ns; do
    rm -rf $d
    mkdir $d
    cd $d
    bash -x ../cls2test.sh \
         -n Test${ns} \
         -N ${ns} \
         -x /home/jcgonzalez/ws/personal/QPF/build/debug/$d/*.o
    cd ..
done < list > ~/ws/personal/QPF/tests/out 2>&1

rm list
