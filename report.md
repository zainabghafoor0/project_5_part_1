
# 1. Overview
This project benchmarks four priority-queue implementations—**Binary Heap**, **Binomial Queue**, **Linear Baseline**, and a **Quadratic-Time Oracle**—under the **Batch-Then-Drain** workload profile. The goal is to understand how different priority-queue designs behave when a queue is built in one batch and then drained entirely.

We measure:
- Relative performance across implementations  
- Constant factor differences  
- Whether empirical behavior aligns with theoretical expectations  
- Crossover points where one design becomes faster or slower  

This analysis aims to reveal which data structures perform best in deletion-heavy workloads and how fast each grows as N increases.

# 2. Experimental Setup

## Hardware & Software
**Compiler:** g++ (GNU C++ Compiler)  
**Version:** C++20  
**Compiler Flags:**  
```
-std=c++20
-O2
```

## Experiment Configuration
**Seeds per configuration:** 1 seed per value of N  
**Trials per seed:** 1 warm-up run + 7 timing trials  
**Aggregation:** Median of 7 runs  
**Warm-up:** Yes  
**Timing Precision:** nanoseconds (converted to ms)

## Parameter Choices
**Problem sizes N:**  
13, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576  

**Profiles used:** Batch-Then-Drain  
**Key distribution:** Uniform in [1, N]  

# 3. Implementations

## 3.1 Baselines

### Linear Baseline (Unsorted Vector PQ)
- **Description:** Stores items in an unsorted vector.  
- **Complexity:** insert → O(1), deleteMin → O(N)  
- **Purpose:** Slow baseline for deletion-heavy workloads.

## 3.2 Main Implementations

### Binary Heap
- **Description:** Classic binary heap in an array.  
- **Complexity:** insert → O(log N), deleteMin → O(log N)  

### Binomial Queue
- **Description:** Forest of binomial trees.  
- **Complexity:** insert → O(1) amortized, deleteMin → O(log N)  

### Quadratic Oracle
- **Description:** O(N²) reference implementation.  
- **Purpose:** Correctness verification only.

# 4. Workload Profiles

## Batch-Then-Drain
- **Models:** Bulk-build scenarios like heapsort.  
- **Operation Mix:** N inserts → N extractMin operations.  
- **Expected Performance:**  
  - Heaps → O(N log N)  
  - Linear baseline → O(N²)  
  - Oracle → unusable for large N  

# 5. Reproducibility & Data Organization

## File Naming Scheme
```
<profile>_N_<N>_S_<seed>.trace
```
Example: `batch_then_drain_N_1024_S_23.trace`

## Trace Files
Contain profile name, seed, parameters, and operation list.

## CSV Output Columns
- implementation  
- trace_path  
- profile  
- N  
- seed  
- elapsed_ns  
- inserts  
- findmins  
- deletemins  
- extractmins  

# 6. Results

## Batch-Then-Drain
**Interpretation:**  
- Binary heap & binomial queue follow expected **N log N** scaling.  
- Linear baseline grows faster because deleteMin is **O(N)**.  
- Quadratic oracle diverges after ~2¹⁴.  
- Curves match theoretical predictions.

# 7. Baseline Sanity Checks
- Linear baseline behaves poorly on deletion-heavy workloads → expected.  
- Heaps dominate baseline → correct.  
- Oracle collapses early → confirms correctness.  
- No anomalies observed.

# 8. Limitations
- Shared compute environment → noise possible.  
- Only one workload implemented.  
- Implementations not heavily optimized.  
- One seed per N → limited variance analysis.

# 9. Reproducing Results

## Build
```
g++ -std=c++20 src/trace-generators/batch_then_drain/main.cpp utils/TraceConfig.cpp -o batch_then_drain_trace

g++ -std=c++20 \
  src/harness/main.cpp \
  src/implementations/BinaryHeapInVector/BinaryHeapInVector.cpp \
  src/implementations/BinomialQueues/BinomialQueue.cpp \
  src/implementations/BinomialQueues/BQnode.cpp \
  src/implementations/LinearBaseLine/LinearBaseLine.cpp \
  src/implementations/Oracle/QuadraticOracle.cpp \
  utils/comparator.cpp \
  utils/TraceConfig.cpp \
  -o harness
```

## Run Experiments
```
./batch_then_drain_trace
./harness batch_then_drain > csvs/batch_then_drain_results.csv
```

## Plot
Open:
```
charts/pq_multi_impl_anchor_heap_tooltips.html
```
Upload:
```
csvs/batch_then_drain_results.csv
```

# 10. Conclusion
Binary heap and binomial queue scale best under batch-then-drain. Linear baseline becomes inefficient for large inputs. The quadratic oracle is only useful for correctness checks. Heap-based designs are the recommended choice for deletion-heavy workloads.
