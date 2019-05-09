#!/bin/bash

file=rpmult.cpp

if [ ! -f "$file" ]; then
    echo -e "Error: File '$file' not found.\nTest failed."
    exit 1
fi

num_right=0
total=0
line="________________________________________________________________________"
compiler=
interpreter=
language=
extension=${file##*.}
if [ "$extension" = "py" ]; then
    if [ ! -z "$PYTHON_PATH" ]; then
        interpreter=$(which python.exe)
    else
        interpreter=$(which python3.2)
    fi
    command="$interpreter $file"
    echo -e "Testing $file\n"
elif [ "$extension" = "java" ]; then
    language="java"
    command="java ${file%.java}"
    echo -n "Compiling $file..."
    javac $file
    echo -e "done\n"
elif [ "$extension" = "c" ] || [ "$extension" = "cpp" ]; then
    language="c"
    command="./${file%.*}"
    echo -n "Compiling $file..."
    results=$(make 2>&1)
    if [ $? -ne 0 ]; then
        echo -e "\n$results"
        exit 1
    fi
    echo -e "done\n"
fi

run_test_args() {
    (( ++total ))
    echo -n "Running test $total..."
    expected=$2
    received=$( $command $1 2>&1 | tr -d '\r' )
    if [ "$expected" = "$received" ]; then
        echo "success"
        (( ++num_right ))
    else
        echo -e "failure\n\nExpected$line\n$expected\nReceived$line\n$received\n"
    fi
}

run_test_args "" "Usage: ./rpmult <integer m> <integer n>"
run_test_args "42 16 8" "Usage: ./rpmult <integer m> <integer n>"
run_test_args "cat 16" "Error: The first argument is not a valid nonnegative integer."
run_test_args "42 dog" "Error: The second argument is not a valid nonnegative integer."
run_test_args "-142 23" "Error: The first argument is not a valid nonnegative integer."
run_test_args "121 2147483648" "Error: The second argument is not a valid nonnegative integer."
run_test_args "0 0" "0 x 0 = 0"
run_test_args "1 0" "1 x 0 = 0"
run_test_args "0 123" "0 x 123 = 0"
run_test_args "1 1" "1 x 1 = 1"
run_test_args "9 9" "9 x 9 = 81"
run_test_args "1781 10" "1781 x 10 = 17810"
run_test_args "148212 56431" "148212 x 56431 = 8363751372"
run_test_args "134214781 58191121" "134214781 x 58191121 = 7810108561159501"
run_test_args "2147483647 2147483647" "2147483647 x 2147483647 = 4611686014132420609"

echo -e "\nTotal tests run: $total"
echo -e "Number correct : $num_right"
echo -n "Percent correct: "
echo "scale=2; 100 * $num_right / $total" | bc

if [ "$language" = "java" ]; then
    echo -e -n "\nRemoving class files..."
    rm -f *.class
    echo "done"
elif [ "$language" = "c" ]; then
    echo -e -n "\nCleaning project..."
    make clean > /dev/null 2>&1
    echo "done"
fi
