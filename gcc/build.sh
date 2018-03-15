
SolutionDir=".."
PF_Path="${SolutionDir}/printf_ex"
Sample_Path="${SolutionDir}/SampleDemo"
exe_name="sample.exe"

[[ "$1" = "/?" ]] && echo build.sh [additional g++ args...] && exit 0

CC="g++ *.cpp -Wall -o $exe_name $*"

echo Executing build script

# copy souces to this dir
cp `find $PF_Path -iname '*.h'` . &> /dev/null
cp `find $PF_Path -iname '*.cpp'` . &> /dev/null
cp `find $Sample_Path -iname '*.h'` . &> /dev/null
cp `find $Sample_Path -iname '*.cpp'` . &> /dev/null

# echo `find $PF_Path/*.{h,cpp}`
# echo `find $Sample_Path/*.{h,cpp}`

#pfcp="${PF_Path}/*.{h,cpp}"
#scp="${Sample_Path}/*.{h,cpp}"

# remove old exe
rm $exe_name &> /dev/null

echo $CC
$CC

find $exe_name