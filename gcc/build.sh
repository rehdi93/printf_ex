
SolutionDir=".."
PF_Path="${SolutionDir}/printf_ex"
Sample_Path="${SolutionDir}/SampleDemo"
exe_name="sample.exe"

echo Executing build script

# copy souces to this dir
cp `find $PF_Path -iname '*.h'` .
cp `find $PF_Path -iname '*.cpp'` .
cp `find $Sample_Path -iname '*.h'` .
cp `find $Sample_Path -iname '*.cpp'` .

# remove old exe
rm $exe_name

g++ *.cpp -Wall -g -D _DEBUG -o $exe_name

find $exe_name