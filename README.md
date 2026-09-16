*This project has been created as part of the 42 curriculum by cpietrza.*

# Codexion

## Description

Codexion is a concurrency simulation written in C. Several coders share a
limited set of USB dongles. A coder needs the two adjacent dongles to compile,
then alternates between debugging and refactoring.

The goal is to coordinate all threads without deadlocks, mixed log messages or
unfair access to resources. A monitor stops the simulation when a coder burns
out or when every coder has completed the requested number of compilations.

## Instructions

Build the program with:

```sh
make
```

Run it with the eight mandatory arguments:

```sh
./codexion number_of_coders time_to_burnout time_to_compile \
time_to_debug time_to_refactor number_of_compiles_required \
dongle_cooldown scheduler
```

All times are expressed in milliseconds. The scheduler must be `fifo` or
`edf`.

Example:

```sh
./codexion 5 800 200 100 100 5 20 fifo
./codexion 5 800 200 100 100 5 20 edf
```

The available Makefile rules are `all`, `clean`, `fclean` and `re`.

## Scheduling policies

Each dongle owns a priority heap containing the pending requests.

- FIFO gives priority to the request that arrived first.
- EDF gives priority to the coder with the nearest burnout deadline.
- Equal EDF deadlines are resolved by arrival order, then by coder ID.

## Blocking cases handled

### Deadlock prevention

Coders request their two dongles in increasing dongle ID order. Every coder
therefore follows the same locking order, which removes the circular-wait
condition required for a deadlock.

### Starvation prevention

Requests cannot bypass the priority heap. FIFO preserves arrival order, while
EDF prioritizes the coder whose burnout deadline is closest.

### Dongle cooldown

When a dongle is released, its next availability time is recorded. The first
request waits with `pthread_cond_timedwait` until the cooldown expires. The
condition is checked again after every wake-up.

### Precise burnout detection

A separate monitor thread checks the last compile start of every coder. It
polls at short intervals and prints the burnout while holding the output and
stop mutexes.

### Serialized logs

Every message is printed while the output mutex is locked. The stop state is
checked before printing, so regular messages cannot appear after the final
burnout message.

### Single coder

With one coder, only one dongle exists. The coder takes it, cannot obtain a
second one and eventually burns out.

## Thread synchronization mechanisms

The project uses the following POSIX synchronization primitives:

- one mutex per dongle protects its owner, cooldown and request heap;
- one condition variable per dongle puts waiting coders to sleep;
- one mutex per coder protects the last compile time and compile counter;
- one output mutex prevents log lines from being mixed;
- one stop mutex protects the global end state;
- condition broadcasts wake waiting threads when a dongle is released or the
  simulation stops.

For example, a coder inserts a request while the dongle mutex is locked. It
then waits on the dongle condition. After waking up, it checks the heap, owner
and cooldown again before taking the dongle. The monitor reads coder state only
while the coder state mutex is locked.

## Resources

- [POSIX Threads Programming documentation](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [`pthread_create(3)`](https://man7.org/linux/man-pages/man3/pthread_create.3.html) and [`pthread_join(3)`](https://man7.org/linux/man-pages/man3/pthread_join.3.html) manual pages
- [`pthread_mutex_lock(3)`](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html) manual page
- [`pthread_cond_wait(3)`](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html) and [`pthread_cond_timedwait(3)`](https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html) manual pages
- [`gettimeofday(2)`](https://man7.org/linux/man-pages/man2/gettimeofday.2.html) manual page
- [Coffman conditions for deadlocks](https://en.wikipedia.org/wiki/Deadlock#Necessary_conditions)
- [Binary heap and priority queue documentation](https://en.wikipedia.org/wiki/Binary_heap)

AI was used to translate the project subject, generate concurrency test cases, and write this README.
The generated suggestions and tests were reviewed against the original subject requirements, compiled with the
mandatory flags, and tested with ThreadSanitizer. The final implementation and its design choices must be 
understood and defended by the author.