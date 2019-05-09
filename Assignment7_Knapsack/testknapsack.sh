#!/bin/bash

file=knapsack.cpp

if [ ! -f "$file" ]; then
echo -e "Error: File '$file' not found.\nTest failed."
exit 1
fi

MAXTIME="0.200"
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
received=$($command $1 2>&1 | tr -d '\r')
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
result=$(echo $elapsed $MAXTIME | awk '{if ($1 > $2) print 1; else print 0}')
if [ "$result" -eq 1 ]; then
echo -e "failure\nTest timed out at $elapsed seconds. Maximum time allowed is $MAXTIME seconds.\n"
else
echo "success [$elapsed seconds]"
(( ++num_right ))
fi
fi
}

run_test_args "" "Usage: ./knapsack <capacity> <filename>"
run_test_args "10 items.txt 200" "Usage: ./knapsack <capacity> <filename>"
run_test_args "10 notfound.txt" "Error: Cannot open file 'notfound.txt'."
run_test_args "xxx notfound.txt" "Error: Bad value 'xxx' for capacity."
run_test_args "-10 notfound.txt" "Error: Bad value '-10' for capacity."

(cat << ENDOFTEXT
lamp
ENDOFTEXT
) > items.txt
run_test_args "10 items.txt" "Error: Line number 1 does not contain 3 fields."

(cat << ENDOFTEXT
lamp,2,12
desk,10,20,30,40
ENDOFTEXT
) > items.txt
run_test_args "10 items.txt" "Error: Line number 2 does not contain 3 fields."

(cat << ENDOFTEXT
lamp,x,3
ENDOFTEXT
) > items.txt
run_test_args "10 items.txt" "Error: Invalid weight 'x' on line number 1."

(cat << ENDOFTEXT
lamp,2,12
towel,1,-3
ENDOFTEXT
) > items.txt
run_test_args "10 items.txt" "Error: Invalid value '-3' on line number 2."

(cat << ENDOFTEXT
lamp,2,12
ENDOFTEXT
) > items.txt
run_test_args "1 items.txt" "Candidate items to place in knapsack:"$'\n'"   Item 1: lamp (2 pounds, \$12)"$'\n'"Capacity: 1 pound"$'\n'"Items to place in knapsack: None"$'\n'"Total weight: 0 pounds"$'\n'"Total value : \$0"

(cat << ENDOFTEXT
lamp,2,12
towel,1,10
C++ textbook,3,20
clock,2,15
frozen lunches,10,50
ENDOFTEXT
) > items.txt
run_test_args "0 items.txt" "Candidate items to place in knapsack:"$'\n'"   Item 1: lamp (2 pounds, \$12)"$'\n'"   Item 2: towel (1 pound, \$10)"$'\n'"   Item 3: C++ textbook (3 pounds, \$20)"$'\n'"   Item 4: clock (2 pounds, \$15)"$'\n'"   Item 5: frozen lunches (10 pounds, \$50)"$'\n'"Capacity: 0 pounds"$'\n'"Items to place in knapsack: None"$'\n'"Total weight: 0 pounds"$'\n'"Total value : \$0"

(cat << ENDOFTEXT
lamp,2,12
towel,1,10
C++ textbook,3,20
clock,2,15
frozen lunches,10,50
laptop,7,1400
Samsung Galaxy phone,1,200
1 ream of paper,2,5
color printer,25,400
sneakers,1,79
vitamins,1,17
pillow,1,11
diamond watch,1,2500
blender,5,129
ENDOFTEXT
) > items.txt

run_test_args "1 items.txt" "Candidate items to place in knapsack:"$'\n'"   Item 1: lamp (2 pounds, \$12)"$'\n'"   Item 2: towel (1 pound, \$10)"$'\n'"   Item 3: C++ textbook (3 pounds, \$20)"$'\n'"   Item 4: clock (2 pounds, \$15)"$'\n'"   Item 5: frozen lunches (10 pounds, \$50)"$'\n'"   Item 6: laptop (7 pounds, \$1400)"$'\n'"   Item 7: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 8: 1 ream of paper (2 pounds, \$5)"$'\n'"   Item 9: color printer (25 pounds, \$400)"$'\n'"   Item 10: sneakers (1 pound, \$79)"$'\n'"   Item 11: vitamins (1 pound, \$17)"$'\n'"   Item 12: pillow (1 pound, \$11)"$'\n'"   Item 13: diamond watch (1 pound, \$2500)"$'\n'"   Item 14: blender (5 pounds, \$129)"$'\n'"Capacity: 1 pound"$'\n'"Items to place in knapsack:"$'\n'"   Item 13: diamond watch (1 pound, \$2500)"$'\n'"Total weight: 1 pound"$'\n'"Total value : \$2500"
run_test_args "2 items.txt" "Candidate items to place in knapsack:"$'\n'"   Item 1: lamp (2 pounds, \$12)"$'\n'"   Item 2: towel (1 pound, \$10)"$'\n'"   Item 3: C++ textbook (3 pounds, \$20)"$'\n'"   Item 4: clock (2 pounds, \$15)"$'\n'"   Item 5: frozen lunches (10 pounds, \$50)"$'\n'"   Item 6: laptop (7 pounds, \$1400)"$'\n'"   Item 7: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 8: 1 ream of paper (2 pounds, \$5)"$'\n'"   Item 9: color printer (25 pounds, \$400)"$'\n'"   Item 10: sneakers (1 pound, \$79)"$'\n'"   Item 11: vitamins (1 pound, \$17)"$'\n'"   Item 12: pillow (1 pound, \$11)"$'\n'"   Item 13: diamond watch (1 pound, \$2500)"$'\n'"   Item 14: blender (5 pounds, \$129)"$'\n'"Capacity: 2 pounds"$'\n'"Items to place in knapsack:"$'\n'"   Item 7: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 13: diamond watch (1 pound, \$2500)"$'\n'"Total weight: 2 pounds"$'\n'"Total value : \$2700"
run_test_args "5 items.txt" "Candidate items to place in knapsack:"$'\n'"   Item 1: lamp (2 pounds, \$12)"$'\n'"   Item 2: towel (1 pound, \$10)"$'\n'"   Item 3: C++ textbook (3 pounds, \$20)"$'\n'"   Item 4: clock (2 pounds, \$15)"$'\n'"   Item 5: frozen lunches (10 pounds, \$50)"$'\n'"   Item 6: laptop (7 pounds, \$1400)"$'\n'"   Item 7: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 8: 1 ream of paper (2 pounds, \$5)"$'\n'"   Item 9: color printer (25 pounds, \$400)"$'\n'"   Item 10: sneakers (1 pound, \$79)"$'\n'"   Item 11: vitamins (1 pound, \$17)"$'\n'"   Item 12: pillow (1 pound, \$11)"$'\n'"   Item 13: diamond watch (1 pound, \$2500)"$'\n'"   Item 14: blender (5 pounds, \$129)"$'\n'"Capacity: 5 pounds"$'\n'"Items to place in knapsack:"$'\n'"   Item 7: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 10: sneakers (1 pound, \$79)"$'\n'"   Item 11: vitamins (1 pound, \$17)"$'\n'"   Item 12: pillow (1 pound, \$11)"$'\n'"   Item 13: diamond watch (1 pound, \$2500)"$'\n'"Total weight: 5 pounds"$'\n'"Total value : \$2807"
run_test_args "14 items.txt" "Candidate items to place in knapsack:"$'\n'"   Item 1: lamp (2 pounds, \$12)"$'\n'"   Item 2: towel (1 pound, \$10)"$'\n'"   Item 3: C++ textbook (3 pounds, \$20)"$'\n'"   Item 4: clock (2 pounds, \$15)"$'\n'"   Item 5: frozen lunches (10 pounds, \$50)"$'\n'"   Item 6: laptop (7 pounds, \$1400)"$'\n'"   Item 7: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 8: 1 ream of paper (2 pounds, \$5)"$'\n'"   Item 9: color printer (25 pounds, \$400)"$'\n'"   Item 10: sneakers (1 pound, \$79)"$'\n'"   Item 11: vitamins (1 pound, \$17)"$'\n'"   Item 12: pillow (1 pound, \$11)"$'\n'"   Item 13: diamond watch (1 pound, \$2500)"$'\n'"   Item 14: blender (5 pounds, \$129)"$'\n'"Capacity: 14 pounds"$'\n'"Items to place in knapsack:"$'\n'"   Item 6: laptop (7 pounds, \$1400)"$'\n'"   Item 7: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 13: diamond watch (1 pound, \$2500)"$'\n'"   Item 14: blender (5 pounds, \$129)"$'\n'"Total weight: 14 pounds"$'\n'"Total value : \$4229"

(cat << ENDOFTEXT
lamp,2,12
C++ textbook,3,20
clock,2,15
frozen lunches,10,50
laptop,7,1400
Samsung Galaxy phone,1,200
1 ream of paper,2,5
color printer,25,400
sneakers,1,79
vitamins,1,17
blender,5,129
ENDOFTEXT
) > items.txt

run_test_args "31 items.txt" "Candidate items to place in knapsack:"$'\n'"   Item 1: lamp (2 pounds, \$12)"$'\n'"   Item 2: C++ textbook (3 pounds, \$20)"$'\n'"   Item 3: clock (2 pounds, \$15)"$'\n'"   Item 4: frozen lunches (10 pounds, \$50)"$'\n'"   Item 5: laptop (7 pounds, \$1400)"$'\n'"   Item 6: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 7: 1 ream of paper (2 pounds, \$5)"$'\n'"   Item 8: color printer (25 pounds, \$400)"$'\n'"   Item 9: sneakers (1 pound, \$79)"$'\n'"   Item 10: vitamins (1 pound, \$17)"$'\n'"   Item 11: blender (5 pounds, \$129)"$'\n'"Capacity: 31 pounds"$'\n'"Items to place in knapsack:"$'\n'"   Item 2: C++ textbook (3 pounds, \$20)"$'\n'"   Item 3: clock (2 pounds, \$15)"$'\n'"   Item 4: frozen lunches (10 pounds, \$50)"$'\n'"   Item 5: laptop (7 pounds, \$1400)"$'\n'"   Item 6: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 9: sneakers (1 pound, \$79)"$'\n'"   Item 10: vitamins (1 pound, \$17)"$'\n'"   Item 11: blender (5 pounds, \$129)"$'\n'"Total weight: 30 pounds"$'\n'"Total value : \$1910"
run_test_args "36 items.txt" "Candidate items to place in knapsack:"$'\n'"   Item 1: lamp (2 pounds, \$12)"$'\n'"   Item 2: C++ textbook (3 pounds, \$20)"$'\n'"   Item 3: clock (2 pounds, \$15)"$'\n'"   Item 4: frozen lunches (10 pounds, \$50)"$'\n'"   Item 5: laptop (7 pounds, \$1400)"$'\n'"   Item 6: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 7: 1 ream of paper (2 pounds, \$5)"$'\n'"   Item 8: color printer (25 pounds, \$400)"$'\n'"   Item 9: sneakers (1 pound, \$79)"$'\n'"   Item 10: vitamins (1 pound, \$17)"$'\n'"   Item 11: blender (5 pounds, \$129)"$'\n'"Capacity: 36 pounds"$'\n'"Items to place in knapsack:"$'\n'"   Item 5: laptop (7 pounds, \$1400)"$'\n'"   Item 6: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 8: color printer (25 pounds, \$400)"$'\n'"   Item 9: sneakers (1 pound, \$79)"$'\n'"   Item 10: vitamins (1 pound, \$17)"$'\n'"Total weight: 35 pounds"$'\n'"Total value : \$2096"
run_test_args "100 items.txt" "Candidate items to place in knapsack:"$'\n'"   Item 1: lamp (2 pounds, \$12)"$'\n'"   Item 2: C++ textbook (3 pounds, \$20)"$'\n'"   Item 3: clock (2 pounds, \$15)"$'\n'"   Item 4: frozen lunches (10 pounds, \$50)"$'\n'"   Item 5: laptop (7 pounds, \$1400)"$'\n'"   Item 6: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 7: 1 ream of paper (2 pounds, \$5)"$'\n'"   Item 8: color printer (25 pounds, \$400)"$'\n'"   Item 9: sneakers (1 pound, \$79)"$'\n'"   Item 10: vitamins (1 pound, \$17)"$'\n'"   Item 11: blender (5 pounds, \$129)"$'\n'"Capacity: 100 pounds"$'\n'"Items to place in knapsack:"$'\n'"   Item 1: lamp (2 pounds, \$12)"$'\n'"   Item 2: C++ textbook (3 pounds, \$20)"$'\n'"   Item 3: clock (2 pounds, \$15)"$'\n'"   Item 4: frozen lunches (10 pounds, \$50)"$'\n'"   Item 5: laptop (7 pounds, \$1400)"$'\n'"   Item 6: Samsung Galaxy phone (1 pound, \$200)"$'\n'"   Item 7: 1 ream of paper (2 pounds, \$5)"$'\n'"   Item 8: color printer (25 pounds, \$400)"$'\n'"   Item 9: sneakers (1 pound, \$79)"$'\n'"   Item 10: vitamins (1 pound, \$17)"$'\n'"   Item 11: blender (5 pounds, \$129)"$'\n'"Total weight: 59 pounds"$'\n'"Total value : \$2327"

rm -rf items.txt

echo -e "\nTotal tests run: $total"
echo -e "Number correct : $num_right"
echo -n "Percent correct: "
echo "scale=2; 100 * $num_right / $total" | bc

if [ "$language" = "java" ]; then
echo -e -n "\nRemoving class files..."
rm -f *.class
echo "done"
elif [ $language = "c" ]; then
echo -e -n "\nCleaning project..."
make clean > /dev/null 2>&1
echo "done"
fi
