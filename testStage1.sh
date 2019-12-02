#!/bin/bash

make

echo
lab=118;

./build/stage1 test/$lab.dat my.lst my.obj 

tail -n +2 test/$lab.obj | cut -c1-16 > his_output.txt
tail -n +2 my.obj | cut -c1-16 > my_output.txt

echo -e "\n-- Begin listing file output -- \n~"
cat my.lst
echo -e "~\n-- End of listing File --\n"

echo -e "\n-- diff my_output.txt his_output.txt -- \n~"
diff my_output.txt his_output.txt
echo -e "~\n-- End Differences -- \n"