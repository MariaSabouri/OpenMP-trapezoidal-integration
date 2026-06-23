# OpenMP Trapezoidal Integration

A parallel implementation of the Trapezoidal Rule using OpenMP, developed to study workload distribution strategies, race conditions, numerical accuracy, and parallel performance.

## Overview

The Trapezoidal Rule is a numerical integration technique that approximates the area under a curve by dividing the integration interval into smaller subintervals and approximating each section with a trapezoid.

Since the contribution of each trapezoid can be computed independently, the algorithm is naturally suitable for parallelization using OpenMP.

This project investigates:

- Different OpenMP scheduling policies
- Load balancing techniques
- Race conditions and synchronization
- Numerical accuracy
- Parallel performance metrics such as Speedup and Efficiency

---

## Repository Branches

### `main`

Contains the primary implementation of the parallel trapezoidal integration algorithm.

Features:

- OpenMP parallelization
- Configurable thread count
- Configurable scheduling policy
- Configurable chunk size
- Reduction-based accumulation of partial results

### `race-condition-existence`

A modified version of the program where the reduction clause is intentionally removed.

Purpose:

- Demonstrate race conditions
- Observe incorrect and non-deterministic outputs
- Analyze the effect of unsynchronized access to shared variables

### `race-condition-fix`

A corrected version of the race-condition branch.

Fix:

- Uses a critical section to protect updates to the shared result variable
- Produces consistent and correct results

---

## Scheduling Policies

### Static Schedule (Default Chunk Size)

Uses block partitioning.

Each thread receives a contiguous block of iterations.

Advantages:

- Low scheduling overhead
- Good cache locality
- Efficient when all iterations have similar computational cost

---

### Static Schedule with Chunk Size = 1

Uses cyclic partitioning.

Example with 4 threads:

```text
Thread 0: 0, 4, 8, 12, ...
Thread 1: 1, 5, 9, 13, ...
Thread 2: 2, 6, 10, 14, ...
Thread 3: 3, 7, 11, 15, ...
```

Advantages:

- Better load balancing when iteration costs are irregular

Disadvantages:

- Reduced memory locality
- Increased cache misses

---

### Dynamic Schedule

Iterations are assigned during execution.

When a thread finishes its current chunk, it requests another chunk from the OpenMP runtime.

Advantages:

- Excellent load balancing
- Reduces thread idle time

Disadvantages:

- Higher scheduling overhead

---

### Guided Schedule

Similar to dynamic scheduling.

The runtime initially assigns large chunks and gradually decreases chunk sizes as execution progresses.

Advantages:

- Reduced scheduling overhead
- Good load balancing

Combines many advantages of both static and dynamic scheduling.

---

## Accuracy Analysis

The numerical approximation is evaluated using different numbers of slices:

```text
10³
10⁴
10⁵
10⁶
```

The error is computed as:

```text
Error = |I_approx - I_exact|
```

As the number of slices increases, the approximation converges to the exact solution and the error decreases.

---

## Parallel Performance Analysis

Experiments are performed using:

```text
1 Thread
2 Threads
4 Threads
8 Threads
```

### Speedup

```text
Speedup(p) = T₁ / Tₚ
```

Where:

- `T₁` = execution time using one thread
- `Tₚ` = execution time using `p` threads

### Efficiency

```text
Efficiency(p) = Speedup(p) / p
```

These metrics are used to evaluate the scalability and effectiveness of the parallel implementation.

---

## Compilation

Compile using GCC with OpenMP support:

```bash
gcc -fopenmp trapezoidal.c -o trapezoidal
```

---

## Usage

```bash
./trapezoidal <thread_count> <schedule_type> <chunk_size> <num_slices>
```

Example:

```bash
./trapezoidal 4 static 100 1000000
```

---

## Requirements

- GCC with OpenMP support
- Linux environment
- OpenMP runtime library

---

## Learning Objectives

This project demonstrates:

- OpenMP parallel programming
- Work-sharing constructs
- Reduction operations
- Synchronization techniques
- Race condition analysis
- Scheduling strategies
- Numerical integration
- Speedup and efficiency evaluation
