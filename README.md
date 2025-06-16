# ⚽ Fantasy Football Team Optimizer

C++ implementation of algorithms (Exhaustive Search, Greedy, Metaheuristic) to optimize Fantasy Football team selection based on player statistics, budget constraints, and tactical formations.

## 📌 Project Context

Developed for the *Algorísmia i Programació III* (Algorithms and Programming III) course at **Universitat Politècnica de Catalunya (UPC)**, this project aims to create a support program for a Fantasy Football game. The core challenge is to select a team that maximizes total points from the previous year's performance, subject to various constraints.

## 🏆 Problem Definition

The goal is to select a Fantasy Football team that maximizes the sum of previous year's player points, adhering to the following rules:

-   **Total Budget:** A maximum total team cost of `T` euros.
-   **Player Cap:** A maximum individual player cost of `J` euros per player.
-   **Formation:** A predefined tactical formation given by $N_1$ defenders, $N_2$ midfielders, and $N_3$ forwards, where $1 \le N_1, N_2, N_3 \le 5$ and $N_1 + N_2 + N_3 = 10$.
-   **Goalkeeper:** Exactly one goalkeeper must be selected.

## 📊 Data Input

The program takes two input files:

1.  **Player Database:** Each line represents a player with the format: `Name; Position; Price; Team; Points`.
    -   **Position:** "por" (goalkeeper), "def" (defender), "mig" (midfielder), "dav" (forward).
    -   **Price/Points:** Natural numbers.
2.  **Query File:** Contains 5 natural numbers: $N_1, N_2, N_3, T, J$.

## ⚙️ Methodology

Three distinct C++ programs (`exh.cc`, `greedy.cc`, `mh.cc`) are implemented to solve the optimization problem:

### 1. `exh.cc` (Exhaustive Search)

-   Implements a pure exhaustive search algorithm.
-   Continuously overwrites the output file with the best solution found so far, ensuring that even if aborted, the best intermediate solution is preserved.
-   Not combined with greedy or metaheuristic approaches.
-   Execution time limited to 3 minutes.

### 2. `greedy.cc` (Greedy Algorithm)

-   Implements a greedy approach.
-   Designed for speed, aiming for instantaneous execution even on large instances.
-   Not expected to find the optimal solution due to the computational difficulty of the problem.
-   Execution time limited to 5 seconds.

### 3. `mh.cc` (Metaheuristic)

-   Implements a metaheuristic algorithm (excluding Basic Local Search) from those discussed in class.
-   Similar to the exhaustive search, it overwrites the output file with improved solutions as they are found.
-   Execution time limited to 1 minute.

## 📝 Output Format

The output is written to a file with 7 lines:

1.  Execution time in seconds (double, max 1 decimal).
2.  `POR: ` followed by goalkeeper's name.
3.  `DEF: ` followed by names of selected defenders, separated by `;`.
4.  `MIG: ` followed by names of selected midfielders, separated by `;`.
5.  `DAV: ` followed by names of selected forwards, separated by `;`.
6.  `Punts:` followed by total team points.
7.  `Preu: ` followed by total team price.

##  оценки Evaluation Criteria

-   **Code Readability:** 0.5 points.
-   **Solution Quality & Time:** 0.5 points, considering the quality of obtained solutions and their computation time.
-   Automated checker provided to verify output format and correct calculation of points/price.

## 👥 Authors

-   [Your Name(s) Here] (as per project guidelines)

## 📚 Course

**Algorísmia i Programació III**
Universitat Politècnica de Catalunya (UPC)
