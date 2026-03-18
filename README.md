**A comprehensive C utility for encrypting, decrypting, and computationally breaking Caesar Ciphers.**

This program allows users to manipulate text using standard Caesar shifts and utilizes advanced frequency analysis to break unknown ciphers automatically. It features a fully interactive command-line interface (CLI) and supports three different statistical distance metrics to compare letter distributions.

**✨ Features**
Interactive CLI Menu

**Encrypt/Decrypt**: Shift alphabetical characters by any positive or negative offset while preserving spaces, punctuation, and numbers.

**File I/O**: Read input texts directly from the keyboard or from local text files.

**Frequency Distribution**: Compute and display the normalized letter frequency of any given text.

**Automated Cipher Breaking**: Uses standard English letter distributions (distribution.txt) to computationally guess the shift of an unknown encrypted text.

**📊 Caesar Cipher Cryptanalysis: Performance Report**

1. The Impact of Text Length (Short vs. Long Texts)
The length of the encrypted string is the single most significant factor in decryption accuracy.
  Long Texts (200+ characters): All three methods (Chi-Squared, Cosine, and Euclidean) perform exceptionally well. When the   sample size is large, the shifts are almost always decrypted perfectly on the first guess.

  Short Texts (10-20 characters):Short texts rarely contain a perfect distribution of the English alphabet. In these cases, Chi-Squared proves to be the most resilient. Euclidean distance frequently fails on short texts because it calculates raw geometric distance, which is easily skewed by small sample sizes.

2. The Impact of Letter Distribution & Anomalies
Standard frequency analysis assumes that the encrypted text is "normal" English. When testing texts with highly unusual letter frequencies (such as lipograms with no 'E' or 'T', or sentences like "pizza jazz buzz fuzz" from the test data), the accuracy drops drastically.

Why it happens: If a text happens to have five 'Z's and zero 'E's, the statistical profile looks entirely alien. The algorithm will try to shift the text so that the 'Z's align with the 'E' slot in the standard distribution, resulting in a completely incorrect shift (a "false positive").

Method Comparison: Cosine Distance actually handles these anomalous distributions slightly better than Euclidean distance because it measures the angle between the frequency vectors rather than the absolute magnitude, making it slightly more forgiving of exact frequency spikes. However, all metrics struggle significantly if the most common English letters are artificially missing.

3. Errors, Edge Cases, and Failures
During testing, the program reliably selects the wrong shift under specific edge cases:

The "Single Character" or "Too Short" Anomaly: If the text is a single word (e.g., "Hello") or a single character (e.g., "A"), frequency analysis completely breaks down. A string of length 1 has a 100% frequency of one letter, making it mathematically impossible to compare to standard English.

Highly Repetitive Patterns: Texts like "Eeeeeeeee" or repeating acronyms confuse the metrics. The algorithm assumes the highest frequency letter must be 'E', 'T', or 'A', and will force a shift to match that, regardless of the actual word.

Acronyms and Passwords: Strings like "CRYPTOGRAPHY" or "1234567890 !@#" are immune to frequency analysis because they lack standard grammatical filler words (the, and, is) that usually ground the statistical distribution.

4. Summary and Final Verdict
After evaluating the decryption of multiple ciphertexts across varying lengths and distributions, the methods rank as follows:

🏆 Chi-Squared Distance (Most Effective): This is the most accurate overall metric. It works best because of how it handles weights: it penalizes the absence of highly expected letters (like 'E') much more heavily than the absence of rare letters (like 'Z'). This weighting system makes it the most robust choice for texts of all lengths.

📈 Cosine Distance (Strong Runner-Up): Highly effective for long texts and relatively stable. Because it evaluates the "shape" of the data rather than raw numbers, it is a solid mathematical alternative but occasionally loses to Chi-Squared on shorter, choppy sentences.

⚠️ Euclidean Distance (Least Effective): The weakest of the three. It treats a 2% deviation in 'E' exactly the same as a 2% deviation in 'Z'. Because it does not weight the importance of common English letters, it is highly fragile and easily confused by short texts or slight anomalies.
