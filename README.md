A comprehensive C utility for encrypting, decrypting, and computationally breaking Caesar Ciphers.

This program allows users to manipulate text using standard Caesar shifts and utilizes advanced frequency analysis to break unknown ciphers automatically. It features a fully interactive command-line interface (CLI) and supports three different statistical distance metrics to compare letter distributions.

✨ Features
Interactive CLI Menu

Encrypt/Decrypt: Shift alphabetical characters by any positive or negative offset while preserving spaces, punctuation, and numbers.

File I/O: Read input texts directly from the keyboard or from local text files.

Frequency Distribution: Compute and display the normalized letter frequency of any given text.

Automated Cipher Breaking: Uses standard English letter distributions (distribution.txt) to computationally guess the shift of an unknown encrypted text.

📊 Performance Analysis: Breaking the Cipher
The cipher-breaking algorithm tests all 26 possible shifts and compares the resulting letter frequencies against standard English using three mathematical distance metrics. Based on testing data, here is how the metrics compare:

Metric	Accuracy / Best Use Case	Description
Chi-Squared	🏆 Best Overall	The most accurate metric. It correctly weights the importance of common letters over rare ones, making it highly reliable even for shorter texts.
Cosine Distance	📈 Best for Long Texts	Performs exceptionally well on very long texts by matching the overall geometric "shape" of the frequency distribution rather than exact values.
Euclidean	⚠️ Weakest	The most fragile metric. It treats all frequency deviations equally and is easily thrown off by short, anomalous, or unusual texts.
