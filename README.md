# ADS Labs — CSS 5101 Advanced Data Structures and Algorithms

C implementations organized by lab set. Each program is under `LABn/qm/` with its `.c` source file.

## Lab Set 1 — Stacks and sorting

- `q1/` — Stack using an array (`stackusingarr.c`)
- `q2/` — Stack using a linked list (`stackusingll.c`)
- `q3/` — Quick sort (`quicksort.c`), n = 500000
- `q4/` — Heap sort (`heapsort.c`), n = 500000
- `q5/` — Merge sort (`mergesort.c`), n = 500000

## Lab Set 2 — Amortized analysis

- `q1/` — Dynamic table (doubling array) with aggregate analysis (`dynamictable.c`)
- `q2/` — Binary counter increment using the accounting method (`binarycounter.c`)

## Lab Set 3 — B-trees

- `q1/` — B-tree insert, search, delete (order t). Sample keys: 10 20 5 6 12 30 7 17; search 6; delete 6 (`btree.c`)

## Lab Set 4 — Fibonacci heap

- `q1/` — Insert, Find-Min, Union (`fibheap.c`)
- `q2/` — Decrease-Key with cascading cuts, then Delete (`fibheap_decrease.c`)

## Lab Set 5 — van Emde Boas tree

- `q1/` — vEB tree with universe size U = 16 (`veb.c`)

## Lab Set 6 — Bellman-Ford

- `q1/` — Single-source shortest paths with negative edges (`bellmanford.c`)

## Lab Set 7 — Floyd-Warshall

- `q1/` — All-pairs shortest paths (`floydwarshall.c`)

## Lab Set 8 — Johnson's algorithm

- `q1/` — All-pairs shortest paths on sparse graphs (`johnson.c`)

## Lab Set 9 — Multithreaded algorithms basics

- `q1/` — Matrix multiplication with one thread per row (`matmult_pthread.c`)
- `q2/` — Merge sort with recursive pthread splits (`mergesort_pthread.c`)

## How to run

```bash
cd LAB2/q1
gcc dynamictable.c -o dynamictable
./dynamictable
```

| Lab | Question | Compile and run |
| --- | --- | --- |
| 1 | Q1(a) | `gcc stackusingarr.c -o stackusingarr && ./stackusingarr` |
| 1 | Q1(b) | `gcc stackusingll.c -o stackusingll && ./stackusingll` |
| 1 | Quick / Heap / Merge | `gcc quicksort.c -o quicksort && ./quicksort` (same pattern for heap/merge) |
| 2 | Q1 | `gcc dynamictable.c -o dynamictable && ./dynamictable` |
| 2 | Q2 | `gcc binarycounter.c -o binarycounter && ./binarycounter` |
| 3 | Q1 | `gcc btree.c -o btree && ./btree` |
| 4 | Q1 | `gcc fibheap.c -o fibheap && ./fibheap` |
| 4 | Q2 | `gcc fibheap_decrease.c -o fibheap_decrease && ./fibheap_decrease` |
| 5 | Q1 | `gcc veb.c -o veb && ./veb` |
| 6 | Q1 | `gcc bellmanford.c -o bellmanford && ./bellmanford` |
| 7 | Q1 | `gcc floydwarshall.c -o floydwarshall && ./floydwarshall` |
| 8 | Q1 | `gcc johnson.c -o johnson && ./johnson` |
| 9 | Q1 | `gcc -pthread matmult_pthread.c -o matmult_pthread && ./matmult_pthread` |
| 9 | Q2 | `gcc -pthread mergesort_pthread.c -o mergesort_pthread && ./mergesort_pthread` |

Lab 3 Q1 prompts for the minimum degree `t` (use a value ≥ 2; invalid input defaults to 3).

Lab 8 Q1 reads `V E` then `E` lines of `u v w` (0-based vertices; if any endpoint equals `V`, input is treated as 1-based and converted).

## Notes

- Compiled binaries are gitignored. Rebuild them with `gcc` as shown above.
- Lab 2 programs print operation tables and the amortized-cost argument.
- Lab 9 requires linking with pthreads (`-pthread` on gcc/clang).
