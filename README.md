# ADS Labs — CSS 5101 Advanced Data Structures and Algorithms

C implementations for the ADS lab sets: stacks, sorting, amortized analysis, B-trees, Fibonacci heaps, and van Emde Boas trees.

Each question lives in its own folder (`q1/`, `q2/`, …) under the lab directory (`LAB1/`, `LAB2/`, …).

## Lab Set 1 — Stacks and sorting

- `q1/` — Stack using an array (`stackusingarr.c`)
- `q2/` — Stack using a linked list (`stackusingll.c`)
- `q3/` — Quick sort (`quicksort.c`), n = 500000
- `q4/` — Merge sort (`mergesort.c`), n = 500000
- `q5/` — Heap sort (`heapsort.c`), n = 500000

## Lab Set 2 — Amortized analysis

- `q1/` — Array stack with PUSH / POP / MULTIPOP; aggregate analysis (`stackaggregate.c`)
- `q2/` — Binary counter increment using the accounting method (`binarycounter.c`)

## Lab Set 3 — B-trees

- `q1/` — B-tree insert, search, delete (order t). Sample keys: 10 20 5 6 12 30 7 17; search 6; delete 6 (`btree.c`)

## Lab Set 4 — Fibonacci heaps and vEB trees

- `q1/` — Insert, Find-Min, Union (`fibheap.c`)
- `q2/` — Decrease-Key with cascading cuts, then Delete (`fibheap_decrease.c`)
- `q3/` — van Emde Boas tree with U = 16 (`veb.c`)

## Lab Set 5 — Shortest paths (Bellman-Ford, Floyd-Warshall)

- `q1/` — Single-source shortest paths with negative edges (`bellmanford.c`)
- `q2/` — All-pairs shortest paths (`floydwarshall.c`)

## Lab Set 6 — Johnson's algorithm

- `q1/` — All-pairs shortest paths on sparse graphs with reweighting (`johnson.c`)

## How to run

```bash
cd LAB2/q1
gcc stackaggregate.c -o stackaggregate
./stackaggregate
```

| Lab | Question | Compile and run |
| --- | --- | --- |
| 1 | Q1(a) | `gcc stackusingarr.c -o stackusingarr && ./stackusingarr` |
| 1 | Q1(b) | `gcc stackusingll.c -o stackusingll && ./stackusingll` |
| 1 | Quick / Merge / Heap | `gcc quicksort.c -o quicksort && ./quicksort` (same for merge/heap) |
| 2 | Exp 1 | `gcc stackaggregate.c -o stackaggregate && ./stackaggregate` |
| 2 | Exp 2 | `gcc binarycounter.c -o binarycounter && ./binarycounter` |
| 3 | Q1 | `gcc btree.c -o btree && ./btree` |
| 4 | Q1 | `gcc fibheap.c -o fibheap && ./fibheap` |
| 4 | Q2 | `gcc fibheap_decrease.c -o fibheap_decrease && ./fibheap_decrease` |
| 4 | Q3 | `gcc veb.c -o veb && ./veb` |
| 5 | Q1 | `gcc bellmanford.c -o bellmanford && ./bellmanford` |
| 5 | Q2 | `gcc floydwarshall.c -o floydwarshall && ./floydwarshall` |
| 6 | Q1 | `gcc johnson.c -o johnson && ./johnson` |

Lab 3 Q1 prompts for the minimum degree `t` (use a value ≥ 2; invalid input defaults to 3).

Lab 6 Q1 reads `V E` then `E` lines of `u v w` (0-based vertices; if any endpoint equals `V`, input is treated as 1-based and converted).

## Notes

- Compiled binaries are gitignored. Rebuild them with `gcc` as shown above.
- Lab 2 programs print operation tables and the amortized-cost argument.
- Lab 4 Q2 first consolidates with Extract-Min so Decrease-Key can show cuts and cascading cuts.
