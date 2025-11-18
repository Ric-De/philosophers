#!/bin/bash

echo "🧪 PHILOSOPHERS TEST SUITE"
echo "=========================="

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

pass=0
fail=0

# Test function
run_test() {
    test_name=$1
    shift
    command="$@"
    
    echo -n "Testing: $test_name... "
    
    if $command 2>&1 | grep -q "ERROR SUMMARY: 0 errors"; then
        echo -e "${GREEN}PASS${NC}"
        ((pass++))
    else
        echo -e "${RED}FAIL${NC}"
        ((fail++))
    fi
}

# Helgrind tests
echo ""
echo "📊 Helgrind Tests (Data Races)"
echo "------------------------------"
run_test "1 philo" valgrind --tool=helgrind ./philo 1 800 200 200
run_test "2 philos" valgrind --tool=helgrind ./philo 2 600 200 200 2
run_test "5 philos" valgrind --tool=helgrind ./philo 5 800 200 200 5
run_test "200 philos" timeout 10 valgrind --tool=helgrind ./philo 200 800 200 200

# DRD tests
echo ""
echo "📊 DRD Tests (Thread Errors)"
echo "----------------------------"
run_test "2 philos" valgrind --tool=drd ./philo 2 600 200 200 2
run_test "4 philos" valgrind --tool=drd ./philo 4 800 200 200 5
run_test "Death case" valgrind --tool=drd ./philo 4 310 200 100

# Memory leak tests
echo ""
echo "📊 Memory Leak Tests"
echo "--------------------"
run_test "Normal case" valgrind --leak-check=full --error-exitcode=1 ./philo 3 800 200 200 3
run_test "Death case" valgrind --leak-check=full --error-exitcode=1 ./philo 4 310 200 100

# Summary
echo ""
echo "=========================="
echo -e "Results: ${GREEN}$pass PASS${NC} | ${RED}$fail FAIL${NC}"
echo "=========================="

if [ $fail -eq 0 ]; then
    echo -e "${GREEN}✅ ALL TESTS PASSED!${NC}"
    exit 0
else
    echo -e "${RED}❌ SOME TESTS FAILED${NC}"
    exit 1
fi
