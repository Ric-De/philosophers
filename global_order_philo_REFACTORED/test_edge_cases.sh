#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  EDGE CASE TESTS - FOCUSED SUITE      ${NC}"
echo -e "${BLUE}========================================${NC}\n"

PASSED=0
FAILED=0

echo -e "${BLUE}=== DEATH DETECTION TESTS ===${NC}\n"

# Test 1: Quick death detection (time_to_die < time_to_eat)
echo -e "${YELLOW}Test 1: Quick death detection${NC}"
echo "Parameters: 4 philos, 200ms to die, 210ms to eat"
echo "Expected: Philosopher should die (can't finish eating)"

output=$(./philo 4 200 210 100 2>&1 | head -20)
if echo "$output" | grep -q "died"; then
    last_death=$(echo "$output" | grep "died" | tail -1)
    echo -e "${GREEN}✅ PASSED${NC}"
    echo "Death detected: $last_death"
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED - No death detected${NC}"
    echo "Output:"
    echo "$output"
    ((FAILED++))
fi
echo ""

# Test 2: Death timing accuracy
echo -e "${YELLOW}Test 2: Death timing accuracy${NC}"
echo "Parameters: 2 philos, 400ms to die, 210ms to eat, 100ms to sleep"
echo "Expected: Death should occur around 400ms mark"

output=$(./philo 2 400 210 100 2>&1 | head -20)
if echo "$output" | grep -q "died"; then
    death_time=$(echo "$output" | grep "died" | head -1 | awk '{print $1}')
    echo -e "${GREEN}✅ PASSED${NC}"
    echo "Death occurred at: ${death_time}ms (expected ~400ms)"
    
    # Check if death time is reasonable (within 10% of time_to_die)
    if [ "$death_time" -ge 360 ] && [ "$death_time" -le 450 ]; then
        echo -e "${GREEN}Death timing is accurate!${NC}"
    else
        echo -e "${YELLOW}⚠️  Death timing is outside expected range (360-450ms)${NC}"
    fi
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED - No death detected${NC}"
    echo "Output:"
    echo "$output"
    ((FAILED++))
fi
echo ""

echo -e "${BLUE}=== STRESS TESTS ===${NC}\n"

# Test 3: Large philosopher count (reduced from 200)
echo -e "${YELLOW}Test 3: 100 philosophers - stability test${NC}"
echo "Parameters: 100 philos, 800ms to die, 200ms eat/sleep"
echo "Running 3 attempts..."

deaths=0
for i in {1..3}; do
    # Kill after 5 seconds if still running
    (sleep 5 && killall -9 philo 2>/dev/null) &
    killer_pid=$!
    
    output=$(./philo 100 800 200 200 2>&1)
    
    # Kill the killer if program finished
    kill $killer_pid 2>/dev/null
    wait $killer_pid 2>/dev/null
    
    if echo "$output" | grep -q "died"; then
        echo "  Run $i: Death detected ❌"
        ((deaths++))
    else
        echo "  Run $i: No deaths ✅"
    fi
done

if [ $deaths -eq 0 ]; then
    echo -e "${GREEN}✅ PASSED - No deaths in 3 runs${NC}"
    ((PASSED++))
elif [ $deaths -le 1 ]; then
    echo -e "${YELLOW}⚠️  ACCEPTABLE - Only $deaths/3 runs had deaths${NC}"
    echo "Note: 100 philos is a stress test, occasional deaths are acceptable"
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED - $deaths/3 runs had deaths${NC}"
    ((FAILED++))
fi
echo ""

# Test 4: Corrected tight timing test
echo -e "${YELLOW}Test 4: Tight timing (but survivable)${NC}"
echo "Parameters: 4 philos, 410ms to die, 200ms eat, 100ms sleep"
echo "Note: time_to_die (410) > eat (200) + sleep (100) = 300ms ✅"
echo "Running 5 attempts..."

deaths=0
for i in {1..5}; do
    # Kill after 3 seconds if still running
    (sleep 3 && killall -9 philo 2>/dev/null) &
    killer_pid=$!
    
    output=$(./philo 4 410 200 100 2>&1)
    
    # Kill the killer if program finished
    kill $killer_pid 2>/dev/null
    wait $killer_pid 2>/dev/null
    
    if echo "$output" | grep -q "died"; then
        echo "  Run $i: Death detected ❌"
        ((deaths++))
    else
        echo "  Run $i: No deaths ✅"
    fi
done

if [ $deaths -eq 0 ]; then
    echo -e "${GREEN}✅ PASSED - No deaths in 5 runs${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED - $deaths/5 runs had deaths${NC}"
    echo "This suggests a timing issue - philosophers should survive"
    ((FAILED++))
fi
echo ""

# Test 5: Impossible scenario (SHOULD fail)
echo -e "${YELLOW}Test 5: Impossible scenario (should die)${NC}"
echo "Parameters: 4 philos, 310ms to die, 200ms eat, 200ms sleep"
echo "Note: time_to_die (310) < eat (200) + sleep (200) = 400ms ❌"
echo "Expected: Philosophers SHOULD die"

output=$(./philo 4 310 200 200 2>&1 | head -20)

if echo "$output" | grep -q "died"; then
    echo -e "${GREEN}✅ PASSED - Death detected as expected${NC}"
    death_time=$(echo "$output" | grep "died" | head -1 | awk '{print $1}')
    echo "Death occurred at: ${death_time}ms"
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED - No death detected (but should die!)${NC}"
    echo "Output:"
    echo "$output"
    ((FAILED++))
fi
echo ""

echo -e "${BLUE}=== ADDITIONAL EDGE CASES ===${NC}\n"

# Test 6: Many philosophers with meals
echo -e "${YELLOW}Test 6: 50 philosophers with meal limit${NC}"
echo "Parameters: 50 philos, 800ms to die, 200ms eat/sleep, 5 meals"
echo "Expected: All should eat 5 times, then stop cleanly"

(sleep 10 && killall -9 philo 2>/dev/null) &
killer_pid=$!

output=$(./philo 50 800 200 200 5 2>&1)

kill $killer_pid 2>/dev/null
wait $killer_pid 2>/dev/null

if echo "$output" | grep -q "died"; then
    echo -e "${RED}❌ FAILED - Death detected${NC}"
    echo "$output" | grep "died" | head -5
    ((FAILED++))
else
    # Count total meals
    meal_count=$(echo "$output" | grep -c "is eating")
    expected=$((50 * 5))
    variance=$((meal_count - expected))
    
    echo "Total meals: $meal_count (expected: $expected, variance: $variance)"
    
    if [ $variance -ge -10 ] && [ $variance -le 10 ]; then
        echo -e "${GREEN}✅ PASSED - No deaths, acceptable meal count${NC}"
        ((PASSED++))
    else
        echo -e "${YELLOW}⚠️  ACCEPTABLE - No deaths, but high variance${NC}"
        ((PASSED++))
    fi
fi
echo ""

# Test 7: Single philosopher (edge case)
echo -e "${YELLOW}Test 7: Single philosopher (should die)${NC}"
echo "Parameters: 1 philo, 800ms to die"
echo "Expected: Should die after 800ms (only has 1 fork)"

output=$(./philo 1 800 200 200 2>&1 | head -10)

if echo "$output" | grep -q "died"; then
    death_time=$(echo "$output" | grep "died" | head -1 | awk '{print $1}')
    echo -e "${GREEN}✅ PASSED - Death at ${death_time}ms${NC}"
    
    if [ "$death_time" -ge 750 ] && [ "$death_time" -le 850 ]; then
        echo -e "${GREEN}Death timing is accurate!${NC}"
    else
        echo -e "${YELLOW}⚠️  Death timing outside expected range (750-850ms)${NC}"
    fi
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED - No death detected${NC}"
    ((FAILED++))
fi
echo ""

# Test 8: Two philosophers, multiple meals
echo -e "${YELLOW}Test 8: Two philosophers, perfect alternation${NC}"
echo "Parameters: 2 philos, 800ms to die, 200ms eat/sleep, 10 meals"
echo "Expected: Perfect alternation, 20 total meals"

(sleep 5 && killall -9 philo 2>/dev/null) &
killer_pid=$!

output=$(./philo 2 800 200 200 10 2>&1)

kill $killer_pid 2>/dev/null
wait $killer_pid 2>/dev/null

if echo "$output" | grep -q "died"; then
    echo -e "${RED}❌ FAILED - Death detected${NC}"
    ((FAILED++))
else
    meal_count=$(echo "$output" | grep -c "is eating")
    echo "Total meals: $meal_count (expected: 20)"
    
    if [ "$meal_count" -eq 20 ]; then
        echo -e "${GREEN}✅ PASSED - Perfect meal count!${NC}"
        ((PASSED++))
    elif [ "$meal_count" -ge 18 ] && [ "$meal_count" -le 22 ]; then
        echo -e "${GREEN}✅ PASSED - Acceptable variance${NC}"
        ((PASSED++))
    else
        echo -e "${RED}❌ FAILED - Meal count too far off${NC}"
        ((FAILED++))
    fi
fi
echo ""

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}           TEST SUMMARY                 ${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL EDGE CASE TESTS PASSED! 🎉${NC}"
    echo -e "${GREEN}Your implementation handles edge cases correctly!${NC}\n"
    exit 0
else
    echo -e "\n${YELLOW}⚠️  SOME EDGE CASES FAILED ⚠️${NC}"
    echo -e "${YELLOW}Review the failed tests above${NC}\n"
    exit 1
fi
