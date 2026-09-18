*This project has been created as part of the 42 curriculum by <aben-sab>.*

# Codexion

## Description

**Codexion** is a multithreaded simulation in C based on the classic Dining Philosophers problem.

A group of developers (coders) sit around a table in a circle. In the center is a shared compiler. To compile, each coder needs two hardware USB dongles: the one on their left and the one on their right. Because adjacent coders share dongles, neighbors cannot compile at the same time.

Coders repeat three activities in a loop:
1. **Compile**: Take both dongles and compile code.
2. **Debug**: Put down both dongles and debug.
3. **Refactor**: Clean up code before trying to compile again.

If a coder does not start compiling within `time_to_burnout` milliseconds, they burn out, and the simulation immediately ends. The goal is to coordinate threads safely without deadlocks, race conditions, or starvation.

---

## Instructions

### Compilation

Compile the project using `make`:

```bash
make        # Compile the executable 'codexion'
make clean  # Remove object files
make fclean # Remove object files and the executable
make re     # Recompile the project
```

The code compiles with `cc` and the flags `-Wall -Wextra -Werror -pthread`.

### Execution

Run the program with 8 arguments:

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

#### Arguments
1. `number_of_coders`: Total number of coders and dongles.
2. `time_to_burnout`: Time in ms before a coder burns out if they do not start compiling.
3. `time_to_compile`: Time in ms spent compiling.
4. `time_to_debug`: Time in ms spent debugging.
5. `time_to_refactor`: Time in ms spent refactoring.
6. `number_of_compiles_required`: Number of compiles each coder must finish (simulation stops when everyone reaches this quota).
7. `dongle_cooldown`: Cooldown time in ms after a dongle is released before it can be used again.
8. `scheduler`: Dongle queue policy, either `fifo` (First In, First Out) or `edf` (Earliest Deadline First).

#### Example Commands

- **Single coder (immediate burnout)**:
  ```bash
  ./codexion 1 800 200 200 200 10 0 fifo
  ```
  *(Coder 1 only has 1 dongle, cannot compile, and burns out at 800 ms.)*

- **Standard run with FIFO**:
  ```bash
  ./codexion 5 2000 200 200 200 5 0 fifo
  ```

- **Run with cooldown and EDF**:
  ```bash
  ./codexion 5 3000 200 200 200 5 50 edf
  ```

---

## Blocking Cases Handled

1. **Deadlock Prevention (Coffman's Conditions)**:
   - **Circular Wait**: Prevented by strict lock ordering. Coders always lock the lower dongle ID first, then the higher dongle ID (`min(left, right)` then `max(left, right)`). Because every thread locks resources in the same order, a circular deadlock cannot form.
  - **Hold and Wait**: Coders lock both required dongles in a consistent order before checking availability. If they cannot compile, they release both locks, poll briefly, and retry.

2. **Starvation Prevention**:
   - Each dongle has a priority queue (min-heap).
   - Under `fifo`, requests are served in the exact order they arrive.
   - Under `edf`, priority goes to the coder closest to their burnout deadline, saving coders at risk.

3. **Cooldown Handling**:
   - Each dongle tracks when it was last released (`last_released_ms`).
  - A coder cannot take a dongle until `current_time >= last_released_ms + dongle_cooldown`. If cooldown is active, coders briefly sleep with `usleep` before retrying.

4. **Precise Burnout Detection**:
   - A dedicated monitor thread checks all coders every 1 ms.
   - If `current_time - last_compile_start >= time_to_burnout`, the monitor immediately sets the stop flag and prints the burnout message within 10 ms.

5. **Log Serialization**:
   - All console outputs are protected by a shared `log_mutex` to prevent messages from different threads from overlapping.

---

## Thread Synchronization Mechanisms

The project uses POSIX threading primitives to coordinate threads and shared data:

- `pthread_mutex_t`:
  - **Dongle Mutexes**: Each dongle has a mutex to protect its state (`in_use`, `last_released_ms`) and its request queue.
  - **State Mutex**: Protects shared simulation flags (`stop_simulation`), coder compile counters, and compile timestamps.
  - **Log Mutex**: Protects `stdout` to ensure clean, one-at-a-time log messages.

- `pthread_cond_t`:
  - Dongle condition variables are broadcast when dongles are released or the simulation stops. Acquisition currently uses short `usleep` polling while retrying availability and cooldown checks.

- **Custom Event & Priority Queue (Min-Heap)**:
  - Each dongle has a custom binary min-heap to order waiting requests based on FIFO arrival time or EDF deadline.

### Race Condition Prevention & Thread Communication

- **Safe State Updates**:
  When a coder starts compiling, it locks `state_mutex` before updating `last_compile_start` and `compiles_count`. This prevents the monitor thread from reading values while they are being written.

- **Safe Dongle Acquisition**:
  Checking if both dongles are free and claiming them happens while holding both dongle mutexes in sorted order. No two coders can claim the same dongle at the same time.

- **Coder-Monitor Communication**:
  The monitor thread reads coder deadlines under `state_mutex`. When a burnout occurs or all coders reach their quota, the monitor sets `stop_simulation = 1`, broadcasts on all dongle condition variables, and wakes up all waiting coders so every thread exits cleanly.

---

## Resources

### References
- [POSIX Threads (pthreads) Manual](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Pthread Mutex and Condition Variables](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_wait.html)
- [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Binary Min-Heap](https://en.wikipedia.org/wiki/Binary_heap)

### AI Usage Declaration
AI (Google DeepMind Antigravity) was used in this project for:
1. Designing the ordered locking strategy to avoid deadlocks.
2. Developing the min-heap logic for FIFO and EDF dongle scheduling.
3. Reviewing edge cases for cooldown timing and thread-safe monitor shutdown.
4. Writing clear project documentation in simple English.
# Codexion2
