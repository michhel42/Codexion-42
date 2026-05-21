*This project has been created as part of the 42 curriculum by vihardy.*

---

# Codexion

## Description

**Codexion** is a multithreaded concurrency simulation project inspired by the classic Dining Philosophers problem.
The project models a set of coders competing for shared resources called **dongles** in order to compile code, debug, and refactor without causing race conditions, deadlocks, starvation, or inconsistent states.

The primary goal of the project is to explore:

* POSIX threads (`pthread`)
* Thread synchronization
* Shared resource management
* Scheduling strategies
* Deadlock prevention
* Starvation handling
* Precise timing and monitoring systems

Each coder runs in its own thread and repeatedly performs the following cycle:

1. Acquire two dongles
2. Compile
3. Debug
4. Refactor

A dedicated monitor thread supervises the simulation and detects burnout conditions in real time.

The project also introduces advanced synchronization concepts such as:

* Priority queues for dongle access
* Cooldown systems on shared resources
* Serialized logging
* Custom scheduling policies:

  * FIFO Scheduler
  * EDF (Earliest Deadline First)

---

# Project Structure

```bash
.
├── headers
│   └── codexion.h
├── Makefile
└── src
    ├── checker.c
    ├── cleaner.c
    ├── dongle_manager.c
    ├── heap.c
    ├── init.c
    ├── main.c
    ├── monitor.c
    ├── routine.c
    ├── timer.c
    └── utils.c
```

---

# Features

* Multithreaded coder simulation
* Precise burnout monitoring
* FIFO and EDF scheduling support
* Mutex-protected shared resources
* Condition variable synchronization
* Priority queue implementation for fair dongle access
* Serialized logging system
* Cooldown management for dongles
* Deadlock prevention mechanisms
* Starvation reduction strategies
* Custom sleep/timing utilities for accurate synchronization

---

# Instructions

## Compilation

Compile the project using:

```bash
make
```

Available Makefile rules:

```bash
make        # Compile the project
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Recompile everything
```

---

## Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile \
time_to_debug time_to_refactor \
[number_of_required_compiles] scheduler
```

### Parameters

| Argument                      | Description                               |
| ----------------------------- | ----------------------------------------- |
| `number_of_coders`            | Number of coder threads                   |
| `time_to_burnout`             | Maximum idle time before burnout          |
| `time_to_compile`             | Time spent compiling                      |
| `time_to_debug`               | Time spent debugging                      |
| `time_to_refactor`            | Time spent refactoring                    |
| `number_of_required_compiles` | Optional simulation stop condition        |
| `scheduler`                   | scheduler mode (`fifo` or `edf`) |

---

## Example

```bash
./codexion 5 800 200 200 200 7 edf
```

This launches:

* 5 coders
* burnout after 800 ms
* compile/debug/refactor cycles of 200 ms each
* simulation stops after 7 compiles per coder
* EDF scheduling enabled

---

# Blocking Cases Handled

## Deadlock Prevention

The project prevents deadlocks by carefully controlling access ordering and synchronization between coder threads.

### Coffman Conditions Addressed

| Coffman Condition | Handling Strategy                            |
| ----------------- | -------------------------------------------- |
| Mutual exclusion  | Necessary for dongle ownership               |
| Hold and wait     | Reduced through controlled acquisition logic |
| No preemption     | Managed through scheduling and release logic |
| Circular wait     | Prevented through ordered dongle acquisition |

Dongles are never accessed without synchronization, and acquisition order avoids circular dependency chains.

---

## Starvation Prevention

Starvation is minimized using:

* Priority-based waiting queues
* EDF scheduling support
* Fair wake-up handling with condition variables
* Queue management through custom heap structures

Each dongle maintains a waiting queue to ensure fair access among competing coders.

---

## Dongle Cooldown Handling

Each dongle tracks a cooldown timestamp:

```c
long long last_released_time;
```

A coder cannot immediately reuse a dongle until the cooldown duration has expired.

This mechanism:

* avoids resource monopolization
* improves fairness
* simulates hardware cooldown constraints

---

## Precise Burnout Detection

A dedicated monitor thread continuously checks:

```c
last_compile_start
```

If a coder exceeds:

```text
time_to_burnout
```

without compiling, the simulation stops immediately.

This ensures:

* deterministic burnout detection
* low-latency monitoring
* accurate simulation timing

---

## Serialized Logging

All console output is protected using:

```c
pthread_mutex_t log_mutex;
```

This guarantees:

* ordered log messages
* no interleaved thread output
* deterministic debugging traces

Example logs:

```text
12 3 has taken a dongle
15 3 is compiling
217 2 burned out
```

---

# Thread Synchronization Mechanisms

## Mutexes (`pthread_mutex_t`)

Mutexes are heavily used to protect shared state and prevent race conditions.

### Dongle Protection

Each dongle owns its own mutex:

```c
pthread_mutex_t mutex;
```

This ensures only one coder can manipulate a dongle state at a time.

Protected resources include:

* dongle ownership
* cooldown timestamps
* waiting queues

---

## Condition Variables (`pthread_cond_t`)

Dongles also use condition variables:

```c
pthread_cond_t cond;
```

Threads waiting for a dongle sleep efficiently instead of busy-waiting.

Condition variables are used for:

* wake-up notifications
* synchronization between waiting coders
* efficient scheduler coordination

---

## Monitor Synchronization

Global simulation state is protected using:

```c
pthread_mutex_t state_mutex;
```

This mutex synchronizes:

* simulation shutdown
* burnout detection
* shared status flags

The monitor thread communicates safely with coder threads using this shared protected state.

---

## Thread-Safe Logging

Logging is synchronized through:

```c
pthread_mutex_t log_mutex;
```

Without this mutex, simultaneous `printf` calls from multiple threads would produce corrupted output.

---

## Custom Event Queue / Heap Scheduling

The project implements a custom heap structure:

```c
typedef struct s_heap
```

used to manage scheduling priorities.

The heap allows:

* ordered access management
* fair queue progression
* EDF/FIFO scheduling policies

Operations include:

* insertion
* removal
* priority extraction

This prevents uncontrolled contention between coder threads.

---

## Race Condition Prevention Examples

### Shared Dongle Access

Without synchronization:

```text
Coder A and Coder B acquire the same dongle simultaneously
```

Prevented by:

```c
pthread_mutex_lock(&dongle->mutex);
```

---

### Logging Race Conditions

Without serialization:

```text
12 2 is comp15 3 has taken
iling
```

Prevented by:

```c
pthread_mutex_lock(&data->log_mutex);
```

---

### Monitor/Coder State Access

The monitor and coder threads both access:

```c
simulation_running
```

This shared variable is protected through:

```c
pthread_mutex_lock(&data->state_mutex);
```

ensuring consistent shutdown behavior.

---

# Technical Choices

## Why POSIX Threads?

POSIX threads provide:

* lightweight concurrency
* fine-grained synchronization primitives
* realistic systems programming experience

This project focuses on low-level thread coordination rather than high-level abstractions.

---

## Why Condition Variables Instead of Busy Waiting?

Busy waiting wastes CPU cycles.

Condition variables allow:

* efficient sleeping
* lower CPU usage
* cleaner synchronization patterns

---

## Why a Heap Scheduler?

A heap structure provides:

* efficient priority management
* scalable scheduling
* fair ordering for EDF implementation

Complexity remains efficient even with many coders.

---

# Resources

## Documentation

* POSIX Threads Documentation
  [https://man7.org/linux/man-pages/man7/pthreads.7.html](https://man7.org/linux/man-pages/man7/pthreads.7.html)

* Mutex Documentation
  [https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)

* Condition Variables
  [https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html)

* Dining Philosophers Problem
  [https://en.wikipedia.org/wiki/Dining_philosophers_problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)

* EDF Scheduling Overview
  [https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)

---

## Tutorials & Articles

* Beej’s Guide to C Programming
  [https://beej.us/guide/](https://beej.us/guide/)

* Advanced Pthreads Programming
  [https://computing.llnl.gov/tutorials/pthreads/](https://computing.llnl.gov/tutorials/pthreads/)

* Concurrency Patterns in C
  [https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html](https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html)

---

## AI Usage Disclosure

AI tools were used as assistants during the project for:

* README structuring
* Documentation wording improvements
* Concurrency theory clarification
* Explanations of synchronization concepts
* Reviewing deadlock prevention strategies
* Improving technical explanations

AI was **not** used to automatically generate the entire project implementation.
The core architecture, synchronization logic, thread management, and debugging were designed and implemented manually.

---

# Author

* vihardy — 42 Student
