# PHILOSOPHERS - Dining Philosophers Problem Solution

## 📚 PROJECT OVERVIEW

This project is a solution to the classic **Dining Philosophers Problem** using threads and mutexes in C. It's a synchronization problem that demonstrates the challenges of concurrent programming, particularly **deadlock prevention** and **resource sharing**.

---

## 🧠 THE PROBLEM

### The Scenario
- N philosophers sit at a round table
- There is a fork between each pair of adjacent philosophers (N forks total)
- Each philosopher alternates between: **EATING → SLEEPING → THINKING**
- To eat, a philosopher needs **TWO forks** (the one on their left AND right)
- Philosophers don't communicate with each other
- A philosopher dies if they don't start eating within `time_to_die` milliseconds

### The Challenge
How do we prevent:
1. **Deadlock** - All philosophers grab their left fork and wait forever for their right fork
2. **Starvation** - Some philosophers never get to eat
3. **Race conditions** - Multiple threads accessing shared data incorrectly

---

## 🔑 THE SOLUTION: GLOBAL ORDERING STRATEGY

### Deadlock Prevention
The key innovation is the **global ordering of fork acquisition**:

- **Most philosophers (1 to N-1)**: Take LEFT fork first, then RIGHT fork
- **Last philosopher (N)**: Take RIGHT fork first, then LEFT fork

### Why This Works
```
Without ordering (DEADLOCK):
    Philo 1: Grabs fork 1 → waits for fork 2
    Philo 2: Grabs fork 2 → waits for fork 3
    Philo 3: Grabs fork 3 → waits for fork 4
    Philo 4: Grabs fork 4 → waits for fork 5
    Philo 5: Grabs fork 5 → waits for fork 1
    → CIRCULAR WAIT! Everyone stuck!

With ordering (NO DEADLOCK):
    Philo 1: Grabs fork 1 → waits for fork 2
    Philo 2: Grabs fork 2 → waits for fork 3
    Philo 3: Grabs fork 3 → waits for fork 4
    Philo 4: Grabs fork 4 → waits for fork 5
    Philo 5: Grabs fork 1 first! (NOT fork 5)
    → Philo 5 competes with Philo 1 for fork 1
    → Circular wait broken! Progress possible!
```

---

## 🏗️ PROJECT ARCHITECTURE

### File Structure
```
philosophers/
├── includes/
│   └── philo.h              # All structure definitions and function prototypes
├── main.c                   # Entry point and simulation orchestration
├── parsing.c                # Command line argument parsing
├── helpers_parsing.c        # ft_atoi and input validation
├── init.c                   # Initialization of all data structures
├── utils.c                  # Utility functions (timing, printing)
├── routine.c                # Thread creation and philosopher lifecycle
├── actions.c                # Philosopher actions (eat, sleep, think)
├── helpers_forks.c          # Fork acquisition logic (deadlock prevention)
├── monitor.c                # Death detection and simulation monitoring
├── cleanup.c                # Resource cleanup (mutexes and memory)
└── Makefile                 # Build system
```

---

## 📊 DATA STRUCTURES

### Main Data Structure (t_data)
```c
typedef struct s_data
{
    int             nb_philos;        // Number of philosophers
    int             time_to_die;      // Max time without eating (ms)
    int             time_to_eat;      // Time to eat (ms)
    int             time_to_sleep;    // Time to sleep (ms)
    int             must_eat_count;   // Required meals per philo (-1 = unlimited)
    long            start_time;       // Simulation start timestamp
    int             someone_died;     // Flag: 1 if simulation should end
    pthread_mutex_t print_mutex;      // Protects console output
    pthread_mutex_t death_mutex;      // Protects someone_died flag
    pthread_mutex_t *forks;           // Array of fork mutexes
    struct s_philo  *philos;          // Array of philosopher structures
}   t_data;
```

### Philosopher Structure (t_philo)
```c
typedef struct s_philo
{
    int             id;               // Philosopher ID (1-based)
    int             meals_eaten;      // Number of completed meals
    long            last_meal_time;   // Timestamp of last meal
    pthread_mutex_t meal_mutex;       // Protects meals_eaten and last_meal_time
    pthread_t       thread;           // Thread handle
    pthread_mutex_t *left_fork;       // Pointer to left fork mutex
    pthread_mutex_t *right_fork;      // Pointer to right fork mutex
    t_data          *data;            // Pointer back to shared data
}   t_philo;
```

---

## 🔄 PROGRAM FLOW

### 1. Initialization Phase
```
main()
  ↓
initialize_simulation()
  ↓
parse_arguments() → Validate and parse command line args
  ↓
init_data() → Initialize all data structures
  ↓
  ├─ init_mutexes() → Create print_mutex and death_mutex
  ├─ init_forks() → Create fork mutexes
  └─ init_philosophers() → Create philosopher structures
```

### 2. Execution Phase
```
run_simulation()
  ↓
Set start_time and last_meal_time for all philosophers
  ↓
create_threads() → Create all philosopher threads
  ↓                 (Each runs philosopher_routine)
  ↓
Create monitor thread → Watches for deaths/completion
  ↓
pthread_join(monitor) → Wait for simulation to end
  ↓
join_threads() → Wait for all philosopher threads to finish
```

### 3. Philosopher Lifecycle (Each Thread)
```
philosopher_routine()
  ↓
┌─────────────────┐
│  while (!dead)  │
│      ↓          │
│   philo_eat()   │ → Take forks, eat, drop forks
│      ↓          │
│  philo_sleep()  │ → Sleep
│      ↓          │
│  philo_think()  │ → Think (brief delay for odd numbers)
│      ↓          │
└─────────────────┘
  ↓
Exit when is_simulation_over() returns true
```

### 4. Monitor Thread
```
monitor_routine()
  ↓
┌──────────────────────┐
│  while (true)        │
│      ↓               │
│  check_death()       │ → Check all philosophers for starvation
│      ↓               │
│  If death: break     │
│      ↓               │
│  check_all_ate()     │ → Check if everyone finished eating
│      ↓               │
│  If all done: break  │
│      ↓               │
│  usleep(1ms)         │ → Brief sleep to prevent CPU hogging
└──────────────────────┘
  ↓
Exit (simulation ends)
```

### 5. Cleanup Phase
```
cleanup_mutexes() → Destroy all mutexes
  ↓
cleanup_data() → Free all allocated memory
```

---

## 🔐 SYNCHRONIZATION MECHANISMS

### Mutexes Used

1. **Fork Mutexes** (N mutexes, one per fork)
   - Purpose: Ensure only one philosopher can hold a fork at a time
   - Prevents: Race conditions when grabbing forks

2. **Print Mutex** (1 global mutex)
   - Purpose: Ensure only one thread prints at a time
   - Prevents: Interleaved/garbled console output

3. **Death Mutex** (1 global mutex)
   - Purpose: Protect access to `someone_died` flag
   - Prevents: Race conditions when checking/setting simulation end

4. **Meal Mutexes** (N mutexes, one per philosopher)
   - Purpose: Protect each philosopher's `meals_eaten` and `last_meal_time`
   - Prevents: Race conditions between philosopher and monitor threads

---

## 🎯 CRITICAL SECTIONS

### Taking Forks (Deadlock Prevention)
```c
// The HEART of deadlock prevention
select_fork_order(philo, &first_fork, &second_fork);
    // Last philosopher takes right fork first
    // All others take left fork first

lock_first_fork(philo, first_fork);
    // Lock first fork and check if simulation ended

pthread_mutex_lock(second_fork);
    // Lock second fork
```

### Eating (Death Prevention)
```c
pthread_mutex_lock(&philo->meal_mutex);
philo->last_meal_time = get_time();  // CRITICAL!
pthread_mutex_unlock(&philo->meal_mutex);
    // Update timestamp IMMEDIATELY after getting forks
    // If we don't, monitor will think we died!
```

### Death Detection
```c
time_since_meal = current_time - last_meal_time;
if (time_since_meal > time_to_die)
{
    // DEATH DETECTED!
    printf("%ld %d died\n", timestamp, philo_id);
    someone_died = 1;  // End simulation
}
```

---

## 📝 USAGE

### Compilation
```bash
make
```

### Execution
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Parameters
- `number_of_philosophers`: Number of philosophers (and forks)
- `time_to_die`: Milliseconds until death from starvation
- `time_to_eat`: Milliseconds to eat
- `time_to_sleep`: Milliseconds to sleep
- `[number_of_times_each_philosopher_must_eat]`: (Optional) Stop after everyone eats this many times

### Examples
```bash
# 5 philosophers, die after 800ms, eat for 200ms, sleep for 200ms
./philo 5 800 200 200

# Same as above, but stop after each philosopher eats 7 times
./philo 5 800 200 200 7

# Edge case: 1 philosopher (will die - can't eat with one fork)
./philo 1 800 200 200

# Stress test: 200 philosophers
./philo 200 800 200 200
```

---

## 🐛 EDGE CASES HANDLED

1. **Single Philosopher**
   - Only has one fork (needs two to eat)
   - Takes fork, prints status, waits for death

2. **Very Short time_to_die**
   - Monitor checks frequently (every 1ms)
   - Death detected quickly

3. **Odd Number of Philosophers**
   - Added 1ms thinking time to prevent starvation
   - Helps rebalance fork access patterns

4. **Large Number of Philosophers**
   - Global ordering prevents deadlock regardless of N
   - All mutexes properly initialized and cleaned up

---

## 🎓 KEY LEARNINGS

### Deadlock Prevention Techniques
1. **Global Ordering** - Order resource acquisition to break circular wait
2. **Timeout** - Not used here, but valid approach
3. **Resource Hierarchy** - Assign priority to resources

### Thread Synchronization
1. **Mutexes** - Mutual exclusion for shared resources
2. **Atomic Operations** - Critical sections must be uninterruptible
3. **Thread Communication** - Flags and shared state

### Race Condition Prevention
1. **Mutex Protection** - All shared data access must be protected
2. **Consistent Ordering** - Lock/unlock in consistent order
3. **Minimal Critical Sections** - Hold locks briefly

### Performance Optimization
1. **Precise Timing** - Custom ft_usleep for accuracy
2. **Efficient Monitoring** - Monitor sleeps between checks
3. **Early Exit** - Stop checking when condition met

---

## ⚠️ COMMON PITFALLS AVOIDED

1. **Forgetting to update last_meal_time**
   - Would cause false death detection
   - Fixed by updating immediately after getting forks

2. **Not checking simulation end between forks**
   - Could hold mutex forever
   - Fixed by checking in lock_first_fork()

3. **Unprotected access to shared data**
   - Race conditions
   - Fixed by protecting ALL shared data with mutexes

4. **Inconsistent mutex ordering**
   - Potential deadlock
   - Fixed by global ordering strategy

5. **Memory leaks**
   - Fixed by proper cleanup_mutexes() and cleanup_data()

---

## 🚀 PERFORMANCE CHARACTERISTICS

- **Time Complexity**: O(N) for death checking (N = number of philosophers)
- **Space Complexity**: O(N) for philosophers and forks
- **Deadlock**: **IMPOSSIBLE** due to global ordering
- **Starvation**: **MINIMIZED** by thinking delay for odd numbers
- **Response Time**: Deaths detected within ~1-2ms

---

## 🔬 TESTING

### Test Cases
1. No one should die: `./philo 5 800 200 200`
2. Someone should die: `./philo 4 310 200 100`
3. Everyone should eat enough: `./philo 5 800 200 200 7`
4. Single philosopher: `./philo 1 800 200 200`
5. Large scale: `./philo 200 800 200 200`

---

## 📖 ADDITIONAL RESOURCES

- [Dining Philosophers Problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
- [Mutex Documentation](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html)

---

**Author**: rdellaza  
**Date**: November 2025  
**Project**: 42 School - Philosophers
