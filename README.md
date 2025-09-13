# Algorithms & Data Structures
Politecnico di Milano — Algoritmi e Principi dell'Infomrmatica — _AY 2022–2023_

## About
“Pianifica Percorso” is a command-line program that plans optimal trips along a highway made of service stations. Each station sits at a unique, non-negative distance from the start and offers rental EVs with specific ranges. The aim is to plan a route between two stations minimizing stops, with a deterministic tie-break rule among equally short routes.

Deliverables:
- **[PathFinder.c](./PathFinder.c)**

## Goal and requirements
- Model the highway as stations at unique distances; each station has up to 512 EVs with positive integer ranges.  
- A trip proceeds only forward: at each stop you rent a new car; the next station must be reachable by at least one vehicle from the current station.  
- Objective: given two stations, compute a path with the **minimum number of stops**; if multiple exist, choose the one that prefers **earlier (shorter-distance) intermediate stops** first.  
- Supported commands:
  - `aggiungi-stazione d n r1 ... rn` — add a station at distance `d` with `n` vehicles of ranges `r1..rn`.  
  - `demolisci-stazione d` — remove the station at distance `d`.  
  - `aggiungi-auto d r` — add a vehicle (range `r`) to the station at `d`.  
  - `rottama-auto d r` — remove a vehicle (range `r`) from the station at `d`.  
  - `pianifica-percorso s t` — print the optimal route from `s` to `t` or `nessun percorso` if none.

## Key learnings
- **Algorithm–DS fit:** match operations and constraints to the right algorithms and data structures.
- **Targeted adaptations:** modify standard techniques (e.g., traversal, shortest path, greedy) to encode project-specific rules and tie-breakers.
- **Complexity-driven choices:** reason about asymptotics and constant factors to meet time limits on worst-case inputs.
- **Space–time trade-offs:** design memory layouts and indices that cut lookups/copies while keeping runtime low.
- **Invariant-guided optimization:** preserve correctness with clear invariants and pre/postconditions when tuning implementations.
- **Evidence-based tuning:** compare variants with benchmarks/profiling and keep the simplest version that meets targets.

## Team Members
- [Andrea Pesciotti](https://github.com/AndreaPes)

## Copyright
This project is licensed under the terms of the [Apache License 2.0](./LICENSE).
