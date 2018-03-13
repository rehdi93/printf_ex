
SolutionDir="/f/DEV/RedHands/printf_ex"
PF_Path="${SolutionDir}/printf_ex"
exe_name="sample.exe"

# copy souces to this dir
cp `find $PF_Path -iname '*.h'` .
cp `find $PF_Path -iname '*.cpp'` .
cp "${SolutionDir}/SampleDemo/sample.cpp" .

# remove exe
rm $exe_name

g++ *.cpp -Wall -g -D _DEBUG -o $exe_name

# remove local copies
rm *.cpp *.h

find $exe_name