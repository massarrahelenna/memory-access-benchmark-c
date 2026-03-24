# 🖥️ RAM Memory Access Benchmark

> **Course:** Computer Architecture and Organization  
> **Goal:** Compare the performance between sequential and random memory access patterns

---

## 📋 Description

This project implements a **memory access benchmark** written in C, developed as a practical assignment for the Computer Architecture and Organization course.

The program measures and compares RAM access time under two distinct patterns:

- **Sequential Access** — reads/writes to contiguous memory addresses, leveraging CPU cache and hardware prefetching.
- **Random Access** — reads/writes to arbitrary memory addresses, causing a higher number of cache misses and direct RAM accesses.

These two patterns illustrate fundamental concepts covered in the course, such as memory hierarchy, spatial and temporal locality, L1/L2/L3 cache behavior, and access latency.

---

## 🧠 Computer Architecture Concepts Explored

| Concept | Relation to the Benchmark |
|---|---|
| Memory Hierarchy | Measures the performance impact of accessing data in cache vs. RAM |
| Spatial Locality | Sequential accesses exploit this property |
| Temporal Locality | Reuse of cache-loaded blocks |
| Cache Miss / Cache Hit | Random accesses generate significantly more misses |
| Hardware Prefetching | CPU anticipates sequential accesses, but not random ones |
| Memory Latency | Measurable time difference between the two access patterns |

---

## 📁 Repository Structure

```
TrabalhoAOC/
│
├── codigo.c               # Benchmark source code
├── benchmark_results.txt  # Results file generated after execution
└── README.md              # This file
```

---

## ⚙️ Prerequisites

Before compiling and running, make sure you have the following installed:

- **GCC** (GNU Compiler Collection) — version 7 or higher
- **POSIX `librt` library** — for high-precision time measurement (`clock_gettime`)
- **Linux** operating system (Ubuntu, Debian, Fedora, etc.)

To check if GCC is installed:

```bash
gcc --version
```

---

## 🚀 How to Build and Run

### 1. Clone the repository

```bash
git clone https://github.com/massarrahelenna/TrabalhoAOC
cd TrabalhoAOC
```

### 2. Compile the program

```bash
gcc -o benchmark codigo.c -lrt
```

> The `-lrt` flag links the real-time library (`librt`), required for the `clock_gettime()` function used in time measurements.

### 3. Run the benchmark

```bash
./benchmark
```

The program will perform both sequential and random access tests and display the results in the terminal.

### 4. View the saved results

```bash
cat benchmark_results.txt
```

The measured times are saved to this file for further analysis.

---

## 📊 Interpreting the Results

When running the benchmark, you should observe:

- **Sequential Access** → **lower** execution time, as the CPU can efficiently load contiguous memory blocks into cache.
- **Random Access** → **higher** execution time, as each access tends to result in a *cache miss*, forcing the CPU to fetch data directly from RAM.

This performance gap is a practical demonstration of the impact of the **memory hierarchy** on computational performance.

---

## 🔬 Expected Output Example

```
=== Memory Access Benchmark ===

Array size: 64 MB

Sequential Access:
  Total time: 0.045321 s
  Bandwidth:  14123.45 MB/s

Random Access:
  Total time: 1.238476 s
  Bandwidth:  516.78 MB/s

Results saved to benchmark_results.txt
```

> Values will vary depending on the hardware used (CPU model, cache size, RAM speed).

---

## 🛠️ Common Errors and Solutions

| Error | Likely Cause | Solution |
|---|---|---|
| `gcc: command not found` | GCC not installed | `sudo apt install gcc` |
| `cannot find -lrt` | librt missing | `sudo apt install libc6-dev` |
| `Permission denied: ./benchmark` | Binary lacks execute permission | `chmod +x benchmark` |
| `Segmentation fault` | Array size too large for available memory | Reduce the array size in the source code |

---

## 👥 Authors

Developed as a practical assignment for the **Computer Architecture and Organization** course.

---

## 📄 License

This project is for academic use. Feel free to use and modify it for educational purposes.
