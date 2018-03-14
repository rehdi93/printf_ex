./build.sh

exe_name="sample.exe"
mode="$1"

echo Running $mode

if [ -z $mode ]; then 
	mode="test"
fi

if [[ $mode = "test" ]]; then
	./$exe_name
	echo $?
elif [[ $mode = "debug" ]]; then 
	gdb $exe_name
fi

