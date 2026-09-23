/**
 * @file main.c
 * @brief A comprehensive utility for encrypting, decrypting, and breaking Caesar Ciphers.
 * * This program allows users to manipulate text using the Caesar cipher and utilizes
 * frequency analysis to break unknown ciphers. It supports three distance metrics
 * to compare letter distributions:
 * * @note Analysis Summary:
 * - Chi-Squared is the most accurate overall metric because it correctly weights the
 * importance of common letters over rare ones, making it reliable even for shorter texts.
 * - Cosine distance performs best on very long texts by matching the overall geometric
 * shape of the frequency distribution.
 * - Euclidean distance is the weakest and most fragile metric, as it treats all frequency
 * deviations equally and is easily broken by short or anomalous texts.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tests.h"

#define ALPHABET_SIZE 26
#define TOP_N 3

/**
 * @brief Reads the standard English letter distribution from a file.
 * * @param filename The path to the file containing the frequency of letters.
 * @param distribution Array where the normalized frequencies (0.0 to 1.0) will be stored.
 */
void read_distribution(const char* filename, double distribution[ALPHABET_SIZE]) {
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        int i = 0;
        while (i < 26 && fscanf(fp, "%lf", &distribution[i]) != EOF) {
            distribution[i] /= 100.0;
            i++;
        }
        fclose(fp);
    } else {
        printf("Error: Could not open distribution file.\n");
    }
}

/**
 * @brief Computes the normalized frequency histogram of lowercase letters in a text.
 * * @param text The input string to analyze.
 * @param histogram Array where the calculated frequencies of each letter will be stored.
 */
void compute_histogram(const char* text, double histogram[ALPHABET_SIZE]) {
    for (int i = 0; i < ALPHABET_SIZE; i++)
        histogram[i] = 0;

    int len = 0;
    while (*text != '\0') {
        if (*text >= 'a' && *text <= 'z') {
            histogram[*text - 'a']++;
            len++;
        }
        else if (*text >= 'A' && *text <= 'Z') {
            histogram[*text - 'A']++;
            len++;
        }
        text++;
    }

    if (len > 0) {
        for (int i = 0; i < 26; i++)
            histogram[i] /= len;
    }
}

/**
 * @brief Computes the Chi-square distance between two frequency histograms.
 * * @param hist1 The expected distribution array (standard English frequencies).
 * @param hist2 The observed distribution array (the text being analyzed).
 * @return The calculated Chi-square distance.
 */
double chi_squared_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double sum = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (hist1[i] > 0) {
            sum += (hist2[i] - hist1[i]) * (hist2[i] - hist1[i]) / hist1[i];
        }
    }
    return sum;
}

/**
 * @brief Computes the Cosine distance between two frequency histograms.
 * * @param hist1 The expected distribution array.
 * @param hist2 The observed distribution array.
 * @return The calculated Cosine distance.
 */
double cosine_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double numerator = 0, denom1 = 0, denom2 = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        numerator += hist1[i] * hist2[i];
        denom1 += hist1[i] * hist1[i];
        denom2 += hist2[i] * hist2[i];
    }
    if (denom1 == 0 || denom2 == 0) return INFINITY;
    return 1.0 - (numerator / (sqrt(denom1) * sqrt(denom2)));
}

/**
 * @brief Computes the Euclidean distance between two frequency histograms.
 * * @param hist1 The expected distribution array.
 * @param hist2 The observed distribution array.
 * @return The calculated Euclidean distance.
 */
double euclidean_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double sum = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sum += (hist1[i] - hist2[i]) * (hist1[i] - hist2[i]);
    }
    return sqrt(sum);
}

/**
 * @brief Shifts a histogram array by a specific amount, wrapping around the alphabet.
 * * @param original The base histogram to be shifted.
 * @param shifted The output array where the shifted histogram is stored.
 * @param shift The number of positions to shift the array.
 */
void shift_histogram(const double original[ALPHABET_SIZE], double shifted[ALPHABET_SIZE], int shift) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        int new_index = (i + shift) % ALPHABET_SIZE;
        shifted[new_index] = original[i];
    }
}

/**
 * @brief Breaks a Caesar cipher by testing all shifts and comparing frequency distributions.
 * * @param text The encrypted message to decrypt.
 * @param top_shifts Output array storing the three most likely shift values.
 * @param top_distances Output array storing the corresponding distance scores for those shifts.
 * @param distance_function A pointer to the specific distance metric function to use.
 */
void break_caesar_cipher(const char* text, int top_shifts[TOP_N], double top_distances[TOP_N], double (*distance_function)(const double[], const double[])) {
    for (int i = 0; i < TOP_N; i++) {
        top_distances[i] = INFINITY;
        top_shifts[i] = -1;
    }

    double* distribution = (double*)malloc(26 * sizeof(double));
    read_distribution("./distribution.txt", distribution);

    double* histogram = calloc(26, sizeof(double));
    double test_histogram[26];
    compute_histogram(text, histogram);

    for (int i = 0; i < 26; i++) {
        shift_histogram(histogram, test_histogram, i);
        double current_score = distance_function(distribution, test_histogram);

        for (int j = 0; j < TOP_N; j++) {
            if (current_score < top_distances[j]) {
                for (int k = TOP_N - 1; k > j; k--) {
                    top_distances[k] = top_distances[k-1];
                    top_shifts[k] = top_shifts[k-1];
                }
                top_distances[j] = current_score;
                // Convert histogram shift to actual decryption shift
                top_shifts[j] = (26 - i) % 26;
                break;
            }
        }
    }
    free(distribution);
    free(histogram);
}

/**
 * @brief Prints the main menu options to the console.
 */
void print_menu() {
    printf("\n--- Caesar Cipher Menu ---\n"
           "1. Read a text from the keyboard\n"
           "2. Read a text from a file\n"
           "3. Encrypt a text with a specific shift\n"
           "4. Decrypt a text with a known shift\n"
           "5. Compute and display the frequency distribution of a text\n"
           "6. Break the cipher\n"
           "7. Break ciphers from a batch file\n"
           "0. Exit\n");
}

/**
 * @brief Safely reads a string with spaces from the standard input.
 * * @param str The buffer where the input string will be stored.
 */
void read_text_from_keyboard(char * str) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Clear buffer
    fgets(str, 1000, stdin);
    str[strcspn(str, "\n")] = '\0'; // Remove trailing newline
}

/**
 * @brief Reads the first line of text from a specified file.
 * * @param str The buffer where the file's content will be stored.
 * @param filename The name of the file to read from.
 */
void read_text_from_file(char * str, const char * filename) {
    char formatted_file_name[200];
    snprintf(formatted_file_name, sizeof(formatted_file_name), "./%s", filename);
    FILE* file = fopen(formatted_file_name, "r");
    if (file != NULL) {
        fgets(str, 1000, file);
        str[strcspn(str, "\n")] = '\0';
        fclose(file);
    } else {
        printf("No such file!\n");
        str[0] = '\0';
    }
}


/**
 * @brief Core Caesar cipher shift function (used for both encryption and decryption).
 * * Shifts alphabetical characters in the given message by the specified offset.
 * It handles both uppercase and lowercase letters independently while leaving
 * spaces, punctuation, and numbers untouched. It safely processes negative
 * offsets and offsets greater than 26 using modulo arithmetic.
 * * @param message The null-terminated string to be modified in-place.
 * @param offset  The number of positions to shift the characters (can be negative).
 */
void encrypt(char message[], int offset) {
    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] >= 'a' && message[i] <= 'z') {
            message[i] = (char)((((message[i] - 'a') + offset) % 26 + 26) % 26 + 'a');
        }
        else if (message[i] >= 'A' && message[i] <= 'Z') {
            message[i] = (char)((((message[i] - 'A') + offset) % 26 + 26) % 26 + 'A');
        }
    }
}

/**
 * @brief Encrypts a string in-place using a Caesar cipher.
 * * This function serves as a wrapper around the core encrypt() function,
 * shifting all alphabetical characters forward by the given amount.
 * * @param str   The null-terminated string to encrypt.
 * @param shift The number of positions to shift characters forward.
 */void encrypt_with_shift(char * str, const int shift) {
    encrypt(str, shift);
}

/**
 * @brief Decrypts a Caesar cipher string in-place given a known shift.
 * * This function reverses the encryption process by passing the negative
 * of the original shift to the core encrypt() function.
 * * @param str   The null-terminated string to decrypt.
 * @param shift The number of positions characters were originally shifted forward.
 */
void decrypt_with_shift(char * str, const int shift) {
    encrypt(str, -shift); // Decryption is just a negative shift
}

/**
 * @brief Encrypts a string in-place using a Caesar cipher.
 * * @param str The null-terminated string to encrypt.
 * @param shift The number of positions to shift characters forward.
 */

/**
 * @brief Drives the main user interface and handles user inputs.
 */
void UI() {
    int option = -1;
    while (option != 0) {
        print_menu();
        printf("Enter your option: ");
        scanf(" %d", &option);

        switch (option) {
            case 1: {
                char str[1000];
                printf("Enter text: ");
                read_text_from_keyboard(str);
                printf("%s\n", str);
                break;
            }
            case 2: {
                char str[1000];
                printf("Enter file name: ");
                char file_name[100];
                scanf(" %99s", file_name);
                read_text_from_file(str, file_name);
                if (str[0] != '\0') {
                    printf("%s\n", str);
                }
                break;
            }
            case 3: {
                char str[1000];
                printf("Enter the word to be encrypted: ");
                read_text_from_keyboard(str);
                printf("Enter the shift: ");
                int shift;
                scanf(" %d", &shift);
                encrypt_with_shift(str, shift);
                printf("Encrypted: %s\n", str);
                break;
            }
            case 4: {
                char str[1000];
                printf("Enter the word to be decrypted: ");
                read_text_from_keyboard(str);
                printf("Enter the shift: ");
                int shift;
                scanf(" %d", &shift);
                decrypt_with_shift(str, shift);
                printf("Decrypted: %s\n", str);
                break;
            }
            case 5: {
                char str[1000];
                printf("Enter the text to analyze: ");
                read_text_from_keyboard(str);
                double histogram[ALPHABET_SIZE];
                compute_histogram(str, histogram);
                for (int i = 0; i < ALPHABET_SIZE; i++) {
                    if (histogram[i] > 0) {
                        printf("%c: %.2lf%%\n", i + 'a', histogram[i] * 100);
                    }
                }
                break;
            }
            case 6: {
                char str[1000];
                printf("Enter the encrypted sentence to break: ");
                read_text_from_keyboard(str);

                printf("Enter which type of distance metric to use:\n"
                       "1. Chi-Square distance\n"
                       "2. Cosine Distance\n"
                       "3. Euclidean Distance\n"
                       "Option: ");
                int distance_choice = 0;
                scanf(" %d", &distance_choice);

                int top_shifts[TOP_N];
                double top_distances[TOP_N];

                if (distance_choice == 1) {
                    break_caesar_cipher(str, top_shifts, top_distances, chi_squared_distance);
                } else if (distance_choice == 2) {
                    break_caesar_cipher(str, top_shifts, top_distances, cosine_distance);
                } else if (distance_choice == 3) {
                    break_caesar_cipher(str, top_shifts, top_distances, euclidean_distance);
                } else {
                    printf("Invalid input!\n");
                    break;
                }

                printf("\n--- Top %d Possible Shifts ---\n", TOP_N);
                for (int i = 0; i < TOP_N; i++) {
                    printf("%d. Shift: %d (Score: %.4f)\n", i + 1, top_shifts[i], top_distances[i]);
                }

                if (top_shifts[0] != -1) {
                    char best_guess[1000];
                    strcpy(best_guess, str);
                    decrypt_with_shift(best_guess, top_shifts[0]);

                    printf("\nMost Plausible Decrypted Text (Shift %d):\n", top_shifts[0]);
                    printf("> %s\n\n", best_guess);
                }
                break;
            }
            case 7: {
                printf("Enter file name: ");
                char file_name[100];
                scanf(" %99s", file_name);

                char formatted_file_name[200];
                snprintf(formatted_file_name, sizeof(formatted_file_name), "./%s", file_name);
                FILE* file = fopen(formatted_file_name, "r");

                if (file == NULL) {
                    printf("No such file!\n");
                    break;
                }

                char line[1024];
                int line_num = 1;

                while (fgets(line, sizeof(line), file)) {
                    line[strcspn(line, "\n")] = '\0';
                    if (strlen(line) == 0) continue;
                    char *last_comma = strrchr(line, ',');
                    if (last_comma == NULL) {
                        printf("\nLine %d: Invalid format. Expected 'text,metric'\n", line_num++);
                        continue;
                    }
                    *last_comma = '\0';
                    char *text = line;
                    int metric = atoi(last_comma + 1);

                    printf("\n--- Line %d | Metric: %d ---\n", line_num++, metric);
                    printf("Ciphertext: %s\n", text);

                    int top_shifts[TOP_N];
                    double top_distances[TOP_N];

                    if (metric == 1) {
                        break_caesar_cipher(text, top_shifts, top_distances, chi_squared_distance);
                    } else if (metric == 2) {
                        break_caesar_cipher(text, top_shifts, top_distances, cosine_distance);
                    } else if (metric == 3) {
                        break_caesar_cipher(text, top_shifts, top_distances, euclidean_distance);
                    } else {
                        printf("Invalid metric number (%d) on this line!\n", metric);
                        continue;
                    }

                    if (top_shifts[0] != -1) {
                        char best_guess[1000];
                        strcpy(best_guess, text);
                        decrypt_with_shift(best_guess, top_shifts[0]);
                        printf("Best Shift: %d (Score: %.4f)\n", top_shifts[0], top_distances[0]);
                        printf("Decrypted:  %s\n", best_guess);
                    }
                }
                fclose(file);
                break;
            }
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option\n");
        }
    }
}

/**
 * @brief Entry point of the application.
 * * @return 0 upon successful execution.
 */
int main(void) {
    run_tests(true);
    UI();
    return 0;
}