/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 02:56:33 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 04:01:01 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/*
** ============================================================================
** STANDARD LIBRARY INCLUDES
** ============================================================================
** These headers provide the fundamental functionality needed for the project
*/

# include <stdio.h>        /* printf() - for console output */
# include <stdlib.h>       /* malloc(), free() - for dynamic memory allocation */
# include <unistd.h>       /* usleep(), write() - for system calls and delays */
# include <pthread.h>      /* pthread functions - for thread creation/management */
# include <sys/time.h>     /* gettimeofday() - for precise time measurement */

/* ============================================================================
** DATA STRUCTURES
** ============================================================================
** These structures represent the core entities in the simulation:
** - t_data: Shared global data accessible to all threads
** - t_philo: Individual philosopher data (one per thread)
*/

/*
** t_data - Main data structure (shared across all threads)
** --------------------------------------------------------
** This structure contains all global state and resources for the simulation.
** It's shared between all philosopher threads and the monitor thread.
** 
** THREAD SAFETY:
** - Most fields are read-only after initialization
** - someone_died: protected by death_mutex (read/write)
** - print_mutex: used to protect console output
** - death_mutex: used to protect someone_died flag
** - forks: array of mutexes (each fork is a mutex)
** 
** MEMORY LAYOUT:
** - Basic config: nb_philos, timing parameters
** - Simulation state: start_time, someone_died
** - Synchronization: print_mutex, death_mutex
** - Resources: forks array (mutexes)
** - Philosophers: philos array (structures)
*/
typedef struct s_data
{
	/* ===== CONFIGURATION (read-only after initialization) ===== */
	
	int				nb_philos;        /* Number of philosophers (and forks)
	                                    ** Determines size of arrays
	                                    ** Valid range: typically 1-200
	                                    ** Edge case: 1 philosopher can't eat */
	
	int				time_to_die;      /* Max milliseconds without eating
	                                    ** If (current_time - last_meal_time) > this
	                                    ** philosopher dies from starvation
	                                    ** Monitor checks this constantly */
	
	int				time_to_eat;      /* Milliseconds to eat
	                                    ** How long philosopher holds forks
	                                    ** Affects how long others wait */
	
	int				time_to_sleep;    /* Milliseconds to sleep
	                                    ** Rest period after eating
	                                    ** Part of eat->sleep->think cycle */
	
	int				must_eat_count;   /* Required meals per philosopher
	                                    ** -1 = unlimited (no meal requirement)
	                                    ** >= 0 = stop after everyone eats this many times
	                                    ** Alternative end condition to death */
	
	/* ===== SIMULATION STATE ===== */
	
	long			start_time;       /* Simulation start timestamp (milliseconds)
	                                    ** Set by run_simulation() before threads start
	                                    ** Used to calculate all timestamps:
	                                    ** timestamp = get_time() - start_time
	                                    ** All printed times are relative to this */
	
	int				someone_died;     /* Simulation end flag
	                                    ** 0 = simulation running
	                                    ** 1 = simulation should end (death or completion)
	                                    ** CRITICAL: Must be protected by death_mutex!
	                                    ** Read by: all philosopher threads
	                                    ** Written by: monitor thread only */
	
	/* ===== SYNCHRONIZATION PRIMITIVES ===== */
	
	pthread_mutex_t	print_mutex;      /* Protects console output
	                                    ** Prevents interleaved/garbled prints
	                                    ** Used in: print_status()
	                                    ** Locks when: any thread prints
	                                    ** Why needed: printf() is not atomic */
	
	pthread_mutex_t	death_mutex;      /* Protects someone_died flag
	                                    ** Prevents race conditions on simulation state
	                                    ** Used in: is_simulation_over(), death detection
	                                    ** Critical for: coordinating thread termination */
	
	pthread_mutex_t	*forks;           /* Array of fork mutexes
	                                    ** Size: nb_philos mutexes
	                                    ** Each fork is a shared resource
	                                    ** fork[i] shared by philosopher i and (i+1)%N
	                                    ** Why mutex: only one philosopher can hold fork
	                                    ** Deadlock prevention: global ordering strategy */
	
	/* ===== PHILOSOPHERS ===== */
	
	struct s_philo	*philos;          /* Array of philosopher structures
	                                    ** Size: nb_philos structures
	                                    ** Each philosopher has own data + thread
	                                    ** Forward declaration needed (circular reference) */
}	t_data;

/*
** t_philo - Individual philosopher structure (one per thread)
** ------------------------------------------------------------
** Each philosopher thread has its own instance of this structure.
** Contains philosopher-specific data and links to shared resources.
** 
** THREAD OWNERSHIP:
** - Each philosopher thread "owns" one t_philo structure
** - Some fields accessed by monitor thread (protected by meal_mutex)
** - Links to shared resources (data, forks) for coordination
** 
** LIFECYCLE:
** 1. Created in init_philosophers()
** 2. Thread created in create_threads()
** 3. Thread runs philosopher_routine()
** 4. Thread exits when simulation ends
** 5. Thread joined in join_threads()
** 6. Structure freed in cleanup_data()
*/
typedef struct s_philo
{
	/* ===== IDENTITY ===== */
	
	int				id;               /* Philosopher ID (1-based for display)
	                                    ** Range: 1 to nb_philos
	                                    ** Used in: output messages
	                                    ** Used for: fork ordering (last philo special) */
	
	/* ===== MEAL TRACKING (protected by meal_mutex) ===== */
	
	int				meals_eaten;      /* Number of completed meals
	                                    ** Initialized: 0
	                                    ** Incremented: after each successful eat
	                                    ** Used by: monitor to check if everyone ate enough
	                                    ** Thread safety: protected by meal_mutex */
	
	long			last_meal_time;   /* Timestamp of last meal (milliseconds)
	                                    ** CRITICAL for death detection!
	                                    ** Updated: immediately after acquiring forks
	                                    ** Checked by: monitor to detect starvation
	                                    ** Formula: time_since_meal = current - last_meal_time
	                                    ** Death if: time_since_meal > time_to_die
	                                    ** Thread safety: protected by meal_mutex */
	
	pthread_mutex_t	meal_mutex;       /* Protects meal data
	                                    ** Protects: meals_eaten, last_meal_time
	                                    ** Why needed: monitor reads while philo writes
	                                    ** Locked when: reading/writing meal data
	                                    ** Prevents: race conditions in death detection */
	
	/* ===== THREAD MANAGEMENT ===== */
	
	pthread_t		thread;           /* Thread handle
	                                    ** Created by: pthread_create() in create_threads()
	                                    ** Runs: philosopher_routine()
	                                    ** Joined by: pthread_join() in join_threads()
	                                    ** Purpose: identifies this philosopher's thread */
	
	/* ===== RESOURCES (pointers to shared mutexes) ===== */
	
	pthread_mutex_t	*left_fork;       /* Pointer to left fork mutex
	                                    ** Points to: data->forks[id-1]
	                                    ** Shared with: philosopher (id-1)
	                                    ** Used in: take_forks(), drop_forks()
	                                    ** Deadlock prevention: global ordering */
	
	pthread_mutex_t	*right_fork;      /* Pointer to right fork mutex
	                                    ** Points to: data->forks[id % nb_philos]
	                                    ** Shared with: philosopher (id+1) or philo 1 if last
	                                    ** Used in: take_forks(), drop_forks()
	                                    ** Note: modulo creates circular arrangement */
	
	/* ===== BACK-REFERENCE ===== */
	
	t_data			*data;            /* Pointer back to shared data
	                                    ** Access to: global config, mutexes, other philos
	                                    ** Used for: checking simulation state, timing
	                                    ** Enables: communication with other threads */
}	t_philo;

/* ============================================================================
** FUNCTION PROTOTYPES - Organized by functional area
** ============================================================================
** Each section contains related functions that work together to implement
** a specific aspect of the simulation.
*/

/* =============== INITIALIZATION ===============
** These functions set up all data structures before the simulation starts.
** Call order: init_data() → init_mutexes() → init_forks() → init_philosophers()
** Each function must succeed for simulation to start safely.
*/

/*
** init_data - Master initialization function
** 
** Purpose: Coordinates all initialization steps
** Called by: initialize_simulation() in main.c
** Calls: init_mutexes(), init_forks(), init_philosophers()
** Returns: 1 on success, 0 on failure
** On failure: Caller must cleanup any partial initialization
** 
** What it initializes:
** - Basic flags (someone_died, start_time)
** - Global mutexes (print_mutex, death_mutex)
** - Fork mutexes (one per philosopher)
** - Philosopher structures (with meal mutexes)
*/
int		init_data(t_data *data);

/*
** init_mutexes - Initialize global synchronization primitives
** 
** Creates:
** - print_mutex: for thread-safe console output
** - death_mutex: for thread-safe simulation state
** 
** Returns: 1 on success, 0 on failure
** On failure: Cleans up any successfully initialized mutexes
*/
int		init_mutexes(t_data *data);

/*
** init_forks - Initialize fork mutexes
** 
** Allocates and initializes: Array of nb_philos mutexes
** Each fork is shared between: Adjacent philosophers
** Returns: 1 on success, 0 on failure
** Memory: Must be freed later in cleanup_data()
*/
int		init_forks(t_data *data);

/*
** init_philosophers - Initialize philosopher structures
** 
** Allocates: Array of nb_philos philosopher structures
** Initializes: IDs, meal mutexes, fork pointers, back-reference
** Calls: setup_philosopher() for each philosopher
** Returns: 1 on success, 0 on failure
*/
int		init_philosophers(t_data *data);

/*
** setup_philosopher - Configure individual philosopher
** 
** Sets up: ID, fork pointers (based on position), data pointer
** Fork assignment: left_fork = forks[i], right_fork = forks[(i+1)%N]
** Called by: init_philosophers() for each philosopher
** Returns: void (always succeeds)
*/
void	setup_philosopher(t_data *data, int i);

/* =============== PARSING ===============
** These functions validate and parse command line arguments.
** Must be called before initialization to fill config values.
*/

/*
** parse_arguments - Main argument parsing function
** 
** Validates:
** - Argument count (must be 5 or 6 including program name)
** - Each argument is a valid positive integer
** 
** Fills: All config fields in data structure
** Prints: Configuration summary
** Returns: 1 on success, 0 on invalid arguments
*/
int		parse_arguments(int argc, char **argv, t_data *data);

/*
** validate_argument_count - Check argc is 5 or 6
** 
** Valid: 5 (4 required params) or 6 (4 required + 1 optional)
** Prints: Usage message on failure
** Returns: 1 if valid, 0 if invalid
*/
int		validate_argument_count(int argc);

/*
** parse_and_validate - Parse and validate all arguments
** 
** Checks: Each argument is valid number (digits only) and positive
** Fills: nb_philos, time_to_die, time_to_eat, time_to_sleep, must_eat_count
** Returns: 1 on success, 0 on invalid argument
*/
int		parse_and_validate(char **argv, t_data *data);

/* =============== PARSING HELPERS ===============
** Utility functions for string/number manipulation.
*/

/*
** ft_atoi - Convert string to integer (ASCII to Integer)
** 
** Handles: Leading whitespace, optional sign, digits
** Example: "  -42" → -42
** Returns: Integer value of string
*/
int		ft_atoi(const char *str);

/*
** is_valid_number - Check if string is valid positive number
** 
** Valid: Non-empty string with only digits (0-9)
** Invalid: Signs, decimals, letters, whitespace
** Returns: 1 if valid, 0 if invalid
*/
int		is_valid_number(char *str);

/* =============== UTILS ===============
** Utility functions for timing and output.
*/

/*
** get_time - Get current time in milliseconds
** 
** Uses: gettimeofday() for microsecond precision
** Returns: Milliseconds since Unix epoch (Jan 1, 1970)
** On failure: Returns 0 (rare - system call error)
** Used for: Timestamps, timing calculations, death detection
*/
long	get_time(void);

/*
** ft_usleep - Precise sleep function
** 
** Why: Standard usleep() can overshoot significantly
** How: Active polling with get_time(), brief sleeps to reduce CPU usage
** Parameter: milliseconds to sleep
** Returns: void
** More accurate than: usleep() for small values
*/
void	ft_usleep(long milliseconds);

/*
** print_status - Thread-safe status printing
** 
** Format: "timestamp_ms philosopher_id status\n"
** Example: "150 3 is eating"
** Protected by: print_mutex (prevents interleaved output)
** Checks: Only prints if simulation still running
** Returns: void
*/
void	print_status(t_philo *philo, char *status);

/* =============== ROUTINE ===============
** Thread lifecycle functions - creation, execution, joining.
*/

/*
** philosopher_routine - Main philosopher thread function
** 
** This is what each philosopher thread executes!
** Lifecycle: EAT → SLEEP → THINK → (repeat until simulation ends)
** Checks: is_simulation_over() before/after each action
** Parameter: void pointer to t_philo (cast from void*)
** Returns: NULL when exits (standard pthread return)
*/
void	*philosopher_routine(void *arg);

/*
** create_threads - Create all philosopher threads
** 
** Creates: nb_philos threads, each running philosopher_routine()
** Each thread: Receives pointer to its t_philo structure
** Returns: 1 on success, 0 if any thread creation fails
** After this: All philosophers running concurrently
*/
int		create_threads(t_data *data);

/*
** join_threads - Wait for all philosopher threads to complete
** 
** Waits for: Each philosopher thread to exit
** Called after: Monitor detects simulation end
** Purpose: Clean thread termination, resource cleanup
** Returns: 1 on success, 0 if any join fails
*/
int		join_threads(t_data *data);

/* =============== ACTIONS ===============
** Philosopher actions - the eat/sleep/think cycle.
*/

/*
** take_forks - Acquire both forks using global ordering strategy
** 
** CRITICAL: This implements deadlock prevention!
** Strategy: Most philos take left→right, last philo takes right→left
** Edge case: Single philosopher (takes one fork, waits forever)
** Returns: 1 if both forks acquired, 0 if simulation ended
*/
int		take_forks(t_philo *philo);

/*
** drop_forks - Release both forks
** 
** Order: Doesn't matter for unlocking (unlike locking)
** Purpose: Make forks available for other philosophers
** Returns: void
*/
void	drop_forks(t_philo *philo);

/*
** philo_eat - Philosopher eating routine
** 
** Steps:
** 1. Take forks (deadlock prevention)
** 2. Update last_meal_time (CRITICAL for death detection!)
** 3. Print "is eating"
** 4. Sleep for time_to_eat
** 5. Increment meals_eaten
** 6. Drop forks
** 
** Thread safety: last_meal_time and meals_eaten protected by meal_mutex
** Returns: void
*/
void	philo_eat(t_philo *philo);

/*
** philo_sleep - Philosopher sleeping routine
** 
** Simple: Print status, sleep for time_to_sleep
** Purpose: Rest after eating, give others chance to eat
** Returns: void
*/
void	philo_sleep(t_philo *philo);

/*
** philo_think - Philosopher thinking routine
** 
** Thinking time:
** - Odd number of philos: 1ms (helps prevent starvation)
** - Even number: 0ms (no delay needed)
** 
** Purpose: Buffer between actions, prevent starvation
** Returns: void
*/
void	philo_think(t_philo *philo);

/* =============== FORK HELPERS ===============
** Helper functions for fork acquisition (deadlock prevention).
*/

/*
** select_fork_order - Determine fork acquisition order
** 
** DEADLOCK PREVENTION: This is the key!
** Last philosopher: Takes right fork first
** All others: Take left fork first
** This breaks circular wait condition
** 
** Parameters:
** - philo: philosopher attempting to take forks
** - first: output pointer to first fork to lock
** - second: output pointer to second fork to lock
*/
void	select_fork_order(t_philo *philo, pthread_mutex_t **first,
			pthread_mutex_t **second);

/*
** lock_first_fork - Lock first fork with safety checks
** 
** Flow:
** 1. Lock the fork (blocks if busy)
** 2. Print "has taken a fork"
** 3. Check if simulation ended while waiting
** 4. If ended: unlock and return 0
** 5. If not ended: return 1 (keep fork locked)
** 
** Returns: 1 if successful, 0 if simulation ended
*/
int		lock_first_fork(t_philo *philo, pthread_mutex_t *first_fork);

/* =============== MONITOR ===============
** Death detection and simulation monitoring.
*/

/*
** monitor_routine - Monitor thread main function
** 
** Continuously checks:
** 1. Has any philosopher died? (check_death)
** 2. Have all philosophers eaten enough? (check_all_ate)
** 
** Exits when: Either condition is true
** Sleep: 1ms between checks (prevents CPU hogging)
** Returns: NULL when simulation ends
*/
void	*monitor_routine(void *arg);

/*
** check_death - Check if any philosopher died
** 
** Iterates: Through all philosophers
** Calls: check_philosopher_death() for each
** Returns: 1 if any died, 0 if all alive
** Early exit: Returns immediately when first death found
*/
int		check_death(t_data *data);

/*
** check_philosopher_death - Check individual philosopher for death
** 
** Death condition: (current_time - last_meal_time) > time_to_die
** On death: Prints death message, sets someone_died = 1
** Returns: 1 if died, 0 if alive
** Thread safety: Uses meal_mutex, print_mutex, death_mutex
*/
int		check_philosopher_death(t_data *data, int i);

/*
** is_simulation_over - Check if simulation should end
** 
** Thread-safe check: Reads someone_died flag with death_mutex protection
** Called by: All threads to decide if they should continue
** Returns: 1 if simulation ended, 0 if still running
*/
int		is_simulation_over(t_data *data);

/*
** check_all_ate - Check if all philosophers ate enough
** 
** Only relevant: When must_eat_count != -1
** Checks: Each philosopher's meals_eaten >= must_eat_count
** On success: Sets someone_died = 1 (ends simulation)
** Returns: 1 if all finished, 0 if not
** Early exit: Stops checking when finds one who hasn't eaten enough
*/
int		check_all_ate(t_data *data);

/* =============== MAIN ===============
** High-level simulation control functions.
*/

/*
** initialize_simulation - Set up simulation
** 
** Steps:
** 1. Parse and validate arguments
** 2. Initialize all data structures
** 
** Returns: 1 on success, 0 on failure
** On failure: Cleans up partial initialization
*/
int		initialize_simulation(int argc, char **argv, t_data *data);

/*
** run_simulation - Execute the simulation
** 
** Steps:
** 1. Set start_time
** 2. Initialize last_meal_time for all philosophers
** 3. Create philosopher threads
** 4. Create monitor thread
** 5. Wait for monitor (simulation runs until end detected)
** 6. Join all philosopher threads
** 
** Returns: 1 on success, 0 on thread failure
*/
int		run_simulation(t_data *data);

/* =============== CLEANUP ===============
** Resource cleanup to prevent leaks.
*/

/*
** cleanup_mutexes - Destroy all mutexes
** 
** Destroys:
** - Fork mutexes (all)
** - Meal mutexes (all)
** - Global print_mutex
** - Global death_mutex
** 
** Must be called: Before cleanup_data() (before freeing memory)
** Returns: void
*/
void	cleanup_mutexes(t_data *data);

/*
** cleanup_data - Free all allocated memory
** 
** Frees:
** - Fork array
** - Philosopher array
** 
** Must be called: After cleanup_mutexes()
** Returns: void
*/
void	cleanup_data(t_data *data);

#endif

/*
** ============================================================================
** END OF HEADER FILE
** ============================================================================
** 
** This header provides the complete interface for the philosophers simulation.
** All functions are organized by purpose for easy navigation.
** All structures are documented with thread safety notes.
** 
** Key concepts:
** - DEADLOCK PREVENTION: Global ordering strategy (last philo takes forks reversed)
** - THREAD SAFETY: All shared data protected by mutexes
** - DEATH DETECTION: Monitor checks last_meal_time vs time_to_die
** - CLEAN TERMINATION: someone_died flag coordinated with mutexes
** 
** Usage:
** 1. Parse arguments
** 2. Initialize data structures
** 3. Create threads (philosophers + monitor)
** 4. Wait for simulation to end
** 5. Cleanup resources
*/
