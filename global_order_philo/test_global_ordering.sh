#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  PHILOSOPHERS - GLOBAL ORDERING TEST  ${NC}"
echo -e "${BLUE}========================================${NC}\n"

# Test counter
PASSED=0
FAILED=0

# Function to run test
run_test() {
    local test_name=$1
    local cmd=$2
    local expected=$3
    
    echo -e "${YELLOW}Testing: ${test_name}${NC}"
    output=$($cmd 2>&1)
    
    if echo "$output" | grep -q "$expected"; then
        echo -e "${GREEN}✅ PASSED${NC}\n"
        ((PASSED++))
    else
        echo -e "${RED}❌ FAILED${NC}"
        echo "Expected: $expected"
        echo "Got: $output"
        echo ""
        ((FAILED++))
    fi
}

# Function to check for death
check_no_death() {
    local test_name=$1
    local philos=$2
    local die=$3
    local eat=$4
    local sleep=$5
    local meals=$6
    local runs=${7:-10}
    
    echo -e "${YELLOW}Testing: ${test_name} (${runs} runs)${NC}"
    
    local deaths=0
    for i in $(seq 1 $runs); do
        if [ -n "$meals" ]; then
            output=$(timeout 5 ./philo $philos $die $eat $sleep $meals 2>&1)
        else
            output=$(timeout 2 ./philo $philos $die $eat $sleep 2>&1)
        fi
        
        if echo "$output" | grep -q "died"; then
            ((deaths++))
        fi
    done
    
    if [ $deaths -eq 0 ]; then
        echo -e "${GREEN}✅ PASSED - No deaths in ${runs} runs${NC}\n"
        ((PASSED++))
    else
        echo -e "${RED}❌ FAILED - ${deaths}/${runs} runs had deaths${NC}\n"
        ((FAILED++))
    fi
}

# Function to check variance
check_variance() {
    local test_name=$1
    local philos=$2
    local die=$3
    local eat=$4
    local sleep=$5
    local meals=$6
    local expected=$7
    local max_variance=${8:-15}
    
    echo -e "${YELLOW}Testing: ${test_name} (variance check)${NC}"
    
    local counts=()
    for i in $(seq 1 10); do
        count=$(timeout 5 ./philo $philos $die $eat $sleep $meals 2>&1 | grep -c "is eating")
        counts+=($count)
    done
    
    # Find min and max
    IFS=$'\n' sorted=($(sort -n <<<"${counts[*]}"))
    min=${sorted[0]}
    max=${sorted[-1]}
    variance=$((max - min))
    
    echo "Meal counts: ${counts[@]}"
    echo "Expected: ~$expected (variance: ±$variance, max allowed: ±$max_variance)"
    
    if [ $variance -le $max_variance ]; then
        echo -e "${GREEN}✅ PASSED - Variance within acceptable range${NC}\n"
        ((PASSED++))
    else
        echo -e "${RED}❌ FAILED - Variance too high: ±$variance (max: ±$max_variance)${NC}\n"
        ((FAILED++))
    fi
}

echo -e "${BLUE}=== BASIC VALIDATION ===${NC}\n"

# Test 1: Single philosopher (should die)
run_test "1 philo dies" "./philo 1 800 200 200" "died"

# Test 2: 2 philosophers should not die
check_no_death "2 philos - no death" 2 800 200 200 "" 5

# Test 3: 4 philosophers (even) should be perfect
check_no_death "4 philos (even) - no death" 4 410 200 200 "" 5

echo -e "${BLUE}=== CRITICAL: ODD PHILOSOPHER TESTS ===${NC}\n"

# Test 4: 5 philosophers - THE BIG ONE
check_no_death "5 philos - no death" 5 800 200 200 7 20

# Test 5: 9 philosophers
check_no_death "9 philos - no death" 9 600 200 200 7 20

# Test 6: 3 philosophers
check_no_death "3 philos - no death" 3 800 200 200 5 10

echo -e "${BLUE}=== VARIANCE TESTS ===${NC}\n"

# Test 7: Even count variance (should be perfect)
check_variance "4 philos variance" 4 410 200 200 10 40 2

# Test 8: Odd count variance (should be better than before)
check_variance "5 philos variance" 5 800 200 200 7 35 10

# Test 9: 9 philosophers variance
check_variance "9 philos variance" 9 600 200 200 7 63 10

echo -e "${BLUE}=== STRESS TESTS ===${NC}\n"

# Test 10: Large number of philosophers
check_no_death "200 philos - no death" 200 800 200 200 "" 3

# Test 11: Tight timing
check_no_death "Tight timing test" 4 310 200 100 "" 5

echo -e "${BLUE}=== DEATH DETECTION TESTS ===${NC}\n"

# Test 12: Should die quickly
run_test "Quick death detection" "timeout 1 ./philo 4 200 210 100" "died"

# Test 13: Death at exactly time_to_die
run_test "Death timing accuracy" "timeout 1 ./philo 2 400 210 100" "died"

echo -e "\n${BLUE}========================================${NC}"
echo -e "${BLUE}           TEST SUMMARY                 ${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
    echo -e "${GREEN}Global ordering implementation is working correctly!${NC}\n"
    exit 0
else
    echo -e "\n${RED}⚠️  SOME TESTS FAILED ⚠️${NC}"
    echo -e "${YELLOW}Review the failed tests above${NC}\n"
    exit 1
fi
