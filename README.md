# ⚽ Fantasy Football Team Optimizer

C++ implementation of algorithms (Exhaustive Search, Greedy, Metaheuristic) to optimize Fantasy Football team selection based on player statistics, budget constraints, and tactical formations.

## 📌 Project Context

Developed for the *Algorísmia i Programació III* (Algorithms and Programming III) course at **Universitat Politècnica de Catalunya (UPC)**, this project aims to create a support program for a Fantasy Football game. The core challenge is to select a team that maximizes total points from the previous year's performance, subject to various constraints.

## 🏆 Problem Definition

The goal is to select a Fantasy Football team that maximizes the sum of previous year's player points, adhering to the following rules:

-   [cite_start]**Total Budget:** A maximum total team cost of `T` euros[cite: 9].
-   [cite_start]**Player Cap:** A maximum individual player cost of `J` euros per player[cite: 9].
-   [cite_start]**Formation:** A predefined tactical formation given by $N_1$ defenders, $N_2$ midfielders, and $N_3$ forwards, where $1 \le N_1, N_2, N_3 \le 5$ and $N_1 + N_2 + N_3 = 10$[cite: 10].
-   [cite_start]**Goalkeeper:** Exactly one goalkeeper must be selected[cite: 11].

## 📊 Data Input

The program takes two input files:

1.  **Player Database:** Each line represents a player with the format: `Name; Position; Price; Team; [cite_start]Points`[cite: 13].
    -   [cite_start]**Position:** "por" (goalkeeper), "def" (defender), "mig" (midfielder), "dav" (forward)[cite: 14].
    -   [cite_start]**Price/Points:** Natural numbers[cite: 14, 15].
2.  [cite_start]**Query File:** Contains 5 natural numbers: $N_1, N_2, N_3, T, J$[cite: 16].

## ⚙️ Methodology

Three distinct C++ programs (`exh.cc`, `greedy.cc`, `mh.cc`) are implemented to solve the optimization problem:

### 1. `exh.cc` (Exhaustive Search)

-   [cite_start]Implements a pure exhaustive search algorithm[cite: 34].
-   [cite_start]Continuously overwrites the output file with the best solution found so far, ensuring that even if aborted, the best intermediate solution is preserved[cite: 31, 32].
-   [cite_start]Not combined with greedy or metaheuristic approaches[cite: 33].
-   [cite_start]Execution time limited to 3 minutes[cite: 40].

### 2. `greedy.cc` (Greedy Algorithm)

-   [cite_start]Implements a greedy approach[cite: 34].
-   [cite_start]Designed for speed, aiming for instantaneous execution even on large instances[cite: 36].
-   [cite_start]Not expected to find the optimal solution due to the computational difficulty of the problem[cite: 35].
-   [cite_start]Execution time limited to 5 seconds[cite: 40].

### 3. `mh.cc` (Metaheuristic)

-   [cite_start]Implements a metaheuristic algorithm (excluding Basic Local Search) from those discussed in class[cite: 37].
-   [cite_start]Similar to the exhaustive search, it overwrites the output file with improved solutions as they are found[cite: 38].
-   [cite_start]Execution time limited to 1 minute[cite: 40].

## 📝 Output Format

[cite_start]The output is written to a file with 7 lines[cite: 19]:

1.  [cite_start]Execution time in seconds (double, max 1 decimal)[cite: 19].
2.  [cite_start]`POR: ` followed by goalkeeper's name[cite: 20].
3.  [cite_start]`DEF: ` followed by names of selected defenders, separated by `;`[cite: 21].
4.  [cite_start]`MIG: ` followed by names of selected midfielders, separated by `;`[cite: 22].
5.  [cite_start]`DAV: ` followed by names of selected forwards, separated by `;`[cite: 23].
6.  [cite_start]`Punts:` followed by total team points[cite: 24].
7.  [cite_start]`Preu: ` followed by total team price[cite: 25].

##  оценки Evaluation Criteria

-   [cite_start]**Code Readability:** 0.5 points[cite: 39].
-   [cite_start]**Solution Quality & Time:** 0.5 points, considering the quality of obtained solutions and their computation time[cite: 39].
-   [cite_start]Automated checker provided to verify output format and correct calculation of points/price[cite: 42, 43].

## 👥 Authors

-   [Your Name(s) Here] (as per project guidelines)

## 📚 Course

**Algorísmia i Programació III**
Universitat Politècnica de Catalunya (UPC)
