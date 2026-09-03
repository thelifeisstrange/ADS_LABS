# ADS Labs — Advanced Data Structures

C implementations for Advanced Data Structures lab work: stacks, sorting, amortized analysis, and B-trees.

Each question lives in its own folder (`q1/`, `q2/`, …) under the lab directory (`LAB1/`, `LAB2/`, …).

## Labs

### Lab 1 — Stacks and sorting

- `q1/` — Stack using an array (`stackusingarr.c`)
- `q3/` — Quick sort (`quicksort.c`), target size `n = 500000`
- `q4/` — Merge sort (`mergesort.c`), target size `n = 500000`
- `q5/` — Heap sort (`heapsort.c`), target size `n = 500000`

Sample runs for Q3–Q5 are in each folder as `output.txt`, `output2.txt`, and `output3.txt`. Times vary slightly because the arrays are random.

### Lab 2 — Amortized analysis

- `q1/` — Dynamic array with PUSH, POP, and MULTIPOP; aggregate analysis (`dynamicarray.c`), sample in `output.txt`
- `q2/` — Binary counter increment using the accounting (credit) method (`binarycounter.c`), sample in `output.txt`

### Lab 3 — B-trees

- `q1/` — B-tree insert, search, delete, traversal, and level-wise print (`btree.c`); `output.txt` is `t = 3`, `output2.txt` is `t = 2`

## How to run

Compile from a question folder with `gcc`:

```bash
cd LAB2/q1
gcc dynamicarray.c -o dynamicarray
./dynamicarray
```

| Lab | Question | Compile and run |
| --- | --- | --- |
| 1 | Q1 | `gcc stackusingarr.c -o stackusingarr && ./stackusingarr` |
| 1 | Q3 | `gcc quicksort.c -o quicksort && ./quicksort` |
| 1 | Q4 | `gcc mergesort.c -o mergesort && ./mergesort` |
| 1 | Q5 | `gcc heapsort.c -o heapsort && ./heapsort` |
| 2 | Q1 | `gcc dynamicarray.c -o dynamicarray && ./dynamicarray` |
| 2 | Q2 | `gcc binarycounter.c -o binarycounter && ./binarycounter` |
| 3 | Q1 | `gcc btree.c -o btree && ./btree` |

Lab 3 Q1 prompts for the minimum degree `t` (use a value ≥ 2; invalid input defaults to 3).

## Notes

- Compiled binaries (`*.o`, `a.out`, `*.out`) are gitignored. Rebuild them with `gcc` as shown above.
- Lab 2 programs print operation tables and a short amortized-cost proof from a fixed sequence of operations.
- Lab 3 Q1 uses a hardcoded insert sequence after you enter `t`.
