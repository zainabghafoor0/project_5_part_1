### Student Info:
**Name:** Zainab Ghafoor  
**ID:** 008259427  
**GitHub repository link:** [[https://github.com/zainabghafoor0/project_5_part_1.git]((https://github.com/zainabghafoor0/project_5_part_1.git))]

# 📦 Batch-then-Drain Priority Queue Profile

### *Empirical Analysis Project — Priority Queue Study*

This project extends the existing Huffman profile analysis by adding a new workload called **Batch-then-Drain**. The goal is to:

- Generate traces for a new profile  
- Replay them using the existing harness  
- Time four different priority queue implementations  
- Produce CSV results  
- Visualize performance using the provided HTML tool  
- Write a short analytical report  

This README guides you through **every step** required to reproduce the full pipeline.

---

# 🏗 0. Directory Setup (Required by Assignment)

A **new directory** was created for this profile:

```
src/trace-generators/batch_then_drain/
```

A new generator file was added by following the given main.cpp for huffman_coding:

```
src/trace-generators/batch_then_drain/main.cpp
```

The generator creates:

- **N insert operations**, followed by  
- **N extract operations**  

Keys are sampled from a large uniform range.  
Generated traces appear under:

```
traces/batch_then_drain/
```

---

# ⚙️ 1. Building the Project

## 1.1 Build the Batch-then-Drain Trace Generator

```bash
g++ -std=c++20 src/trace-generators/batch_then_drain/main.cpp utils/TraceConfig.cpp -o batch_then_drain_trace

```

---

## 1.2 Build the Harness

```bash
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

---

# 📜 2. Generate Batch-then-Drain Traces

```bash
./batch_then_drain_trace
```

Outputs traces such as:

```
traces/batch_then_drain/batch_then_drain_N_1024_S_23.trace
```

---

# 🧪 3. Run the Harness (Replay + Timing)

```bash
./harness batch_then_drain > csvs/batch_then_drain_results.csv
```

Preview:

```bash
head csvs/batch_then_drain_results.csv
```

---

# 📊 4. Plot Performance

Open:

```
charts/pq_multi_impl_anchor_heap_tooltips.html
```

Load:

```
csvs/batch_then_drain_results.csv
```

Save plot as:

```
charts/batch_then_drain_plot.png
```

---

