#!/bin/bash

make

echo

./build/stage0 test/002.dat my.lst my.obj 


echo -e "\n-- Begin listing file output -- \n~"
cat my.lst
echo -e "~\n-- End of listing File --\n"
