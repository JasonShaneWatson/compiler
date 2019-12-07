#!/bin/bash

make

echo
lab=201;
testDir="test2";

./build/stage2 $testDir/$lab.dat my.lst my.obj 

tail -n +2 $testDir/$lab.obj | cut -c1-16 > his_trimmed.obj
tail -n +2 my.obj | cut -c1-16 > my_trimmed.obj
echo -e "Testing: $testDir/$lab.dat\n"
echo -e "-- Diff my.lst $testDir/$lab.obj -- \n~"
diff <(tail -n +2 "my.lst") <(tail -n +2 "$testDir/$lab.lst") 
echo -e "~\n-- End of listing File --\n\n"
echo -e "\n-- Diff my_trimmed.obj his_trimmed.obj -- \n~"
diff my_trimmed.obj his_trimmed.obj | cat -te
echo -e "~\n-- End Differences -- \n"
