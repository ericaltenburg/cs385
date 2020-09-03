#!/bin/bash

file=quicksort.cpp
MAXTIME="2.0"

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

timeout() {
    time=$1

    # start the command in a subshell to avoid problem with pipes
    # (spawn accepts one command)
    cmd="/bin/bash -c \"$2\""

    expect -c "set echo \"-noecho\"; set timeout $time; spawn -noecho $cmd; expect timeout { exit 1 } eof { exit 0 }"    

    if [ $? -eq 1 ]; then
        return 0
    fi
    return 1
}

run_test() {
    (( ++total ))
    echo -n "Running test $total..."
    expected=$2
    local ismac=0
    date --version >/dev/null 2>&1
    if [ $? -ne 0 ]; then
       ismac=1
    fi
    local start=0
    if (( ismac )); then
        start=$(python -c 'import time; print time.time()')
    else
        start=$(date +%s.%N)
    fi
(cat << ENDOFTEXT
$1
ENDOFTEXT
) > input.txt
    if timeout $MAXTIME "cat input.txt | $command 2>&1 | tr -d '\r' > tmp.txt"; then
        echo "failure [timed out after $MAXTIME seconds]"
    else
        received=$(cat tmp.txt)
        local end=$(date +%s.%N)
        if (( ismac )); then
            end=$(python -c 'import time; print time.time()')
        else
            end=$(date +%s.%N)
        fi
        local elapsed=$(echo "scale=3; $end - $start" | bc | awk '{printf "%.3f", $0}') 
        if [ "$expected" != "$received" ]; then
            echo -e "failure\n\nExpected$line\n$expected\n"
            echo -e "Received$line\n$received\n"
        else
            echo "success [$elapsed seconds]"
            (( ++num_right ))
        fi
    fi
    rm -f tmp.txt input.txt
}

run_test "x 1 2 3" "Enter sequence of integers, each followed by a space: Error: Non-integer value 'x' received at index 0."
run_test "1 2 x 3" "Enter sequence of integers, each followed by a space: Error: Non-integer value 'x' received at index 2."
run_test "" "Enter sequence of integers, each followed by a space: Error: Sequence of integers not received."
run_test "  " "Enter sequence of integers, each followed by a space: Error: Sequence of integers not received."
run_test "124" "Enter sequence of integers, each followed by a space: [124]"
run_test "10 10 10 10" "Enter sequence of integers, each followed by a space: [10, 10, 10, 10]"
run_test "124 34 9 -1 89" "Enter sequence of integers, each followed by a space: [-1, 9, 34, 89, 124]"
run_test "124 34 9 -1 89 3 65 8881 12" "Enter sequence of integers, each followed by a space: [-1, 3, 9, 12, 34, 65, 89, 124, 8881]"
run_test "124 34 9 -1 89 3 65 8881 12 342 94 29 394 9 100 -13 489" "Enter sequence of integers, each followed by a space: [-13, -1, 3, 9, 9, 12, 29, 34, 65, 89, 94, 100, 124, 342, 394, 489, 8881]"
run_test "124 34 9 -1 89 3 65 8881 12 342 94 29 394 9 100 -13 489 392 991 123 84 237 340 2398" "Enter sequence of integers, each followed by a space: [-13, -1, 3, 9, 9, 12, 29, 34, 65, 84, 89, 94, 100, 123, 124, 237, 340, 342, 392, 394, 489, 991, 2398, 8881]"

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
