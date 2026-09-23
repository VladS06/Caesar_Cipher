# Caesar Cipher Cryptanalysis Engine

A C-based statistical cryptanalysis utility that computationally breaks Caesar Ciphers through frequency vector analysis and $n$-dimensional distance metrics.

## Core Architecture
- **Algorithmic Decryption:** Computes all possible $K$ cipher shifts (where $K = 26$) and evaluates them against a standard English frequency distribution.
- **Statistical Metrics:** Implements Chi-Squared ($\chi^2$), Cosine Distance, and Euclidean Distance to measure vector similarity.
- **I/O Pipeline:** Handles direct CLI string injection or local text file parsing, normalizing alphabet inputs while strictly preserving non-alphabetical ASCII characters (spaces, punctuation, numeric values).

## Distance Metrics & Cryptanalysis
The engine compares the observed frequency vector $O$ of the shifted ciphertext against the expected standard English frequency vector $E$.
### 1. Chi-Squared Statistic ($\chi^2$) - Primary Heuristic

$$
\chi^2 = \sum_{i=1}^{26} \frac{(O_i - E_i)^2}{E_i}
$$

Highly resilient across variable text lengths. The $E_i$ denominator actively penalizes anomalies by normalizing the squared error relative to expected letter scarcity. This explicitly prevents false positives when evaluating highly anomalous strings, making it the most robust metric.

### 2. Cosine Distance

$$
1 - \frac{\sum_{i=1}^{26} O_i E_i}{\sqrt{\sum_{i=1}^{26} O_i^2} \sqrt{\sum_{i=1}^{26} E_i^2}}
$$

Evaluates the angle between the two 26-dimensional frequency vectors rather than absolute magnitude. This is effective for longer texts with unusual letter clusters, but it suffers on extremely short strings where the vector angle skews wildly due to a lack of data points.

### 3. Euclidean Distance

$$
d(O, E) = \sqrt{\sum_{i=1}^{26} (O_i - E_i)^2}
$$

Measures absolute geometric distance. Performs poorly on short or lipogrammatic texts, as it evaluates raw magnitude and fails to weight the statistical importance of individual letter frequencies.

## Algorithmic Complexity
- **Time Complexity:** $\mathcal{O}(N + K^2)$, where $N$ is the length of the ciphertext and $K$ is the alphabet size (26). Generating the initial frequency vector requires a linear scan $\mathcal{O}(N)$. Computing the shifts against 3 distance metrics across a 26-element array takes constant time $\mathcal{O}(K^2)$.
- **Space Complexity:** $\mathcal{O}(K)$ auxiliary space for allocating the 26-element floating-point vectors required for distribution comparison.
