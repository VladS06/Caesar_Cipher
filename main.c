#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define ALPHABET_SIZE 26
#define TOP_N 3

void read_distribution(const char* filename, double distribution[ALPHABET_SIZE]) {
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        int i = 0;
        while (i < 26 && fscanf(fp, "%lf", &distribution[i]) != EOF) {\
            distribution[i] /= 100;
            i++;
        }
    }

}

void compute_histogram(const char* text, double histogram[ALPHABET_SIZE]) {
    int len = 0;
    while (*text != '\0') {
        histogram[*text - 'a']++;
        text++;
        len++;
    }
    for (int i = 0; i < 26; i++)
        histogram[i] /= len;
}

double chi_squared_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double sum = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sum += (hist2[i]-hist1[i])*(hist2[i]-hist1[i]) / hist1[i];
    }
    return sum;
}

double cosine_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double numerator = 0, denom1 = 0, denom2 = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        numerator += hist1[i] * hist2[i];
        denom1 += hist1[i]*hist1[i];
        denom2 += hist2[i]*hist2[i];
    }
    return 1- numerator/(sqrt(denom1) * sqrt(denom2));
}

double euclidean_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double sum = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sum +=(hist1[i]-hist2[i])*(hist1[i]-hist2[i]);
    }
    return sqrt(sum);
}

void shift_histogram(const double original[ALPHABET_SIZE], double shifted[ALPHABET_SIZE], int shift) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        int new_index = (i + shift) % ALPHABET_SIZE;
        shifted[new_index] = original[i];
    }
}

void break_caesar_cipher(const char* text, int top_shifts[TOP_N], double top_distances[TOP_N],  double (*distance_function)(const double[], const double[])){
    for (int i = 0; i < TOP_N; i++) {
        top_distances[i] = INFINITY;
        top_shifts[i] = -1;
    }
    double * distribution = (double*) malloc(26 * sizeof(double));
    read_distribution("/Users/vlad/PersonalProjects/C/OOP/Extra/Caesar Cipher/distribution.txt", distribution);
    double * histogram = calloc(26, sizeof(double));
    double test_histogram[26];
    compute_histogram(text, histogram);
    for (int i = 0; i < 26; i++) {
        shift_histogram(histogram, test_histogram, i);
        double current_score = distance_function(distribution, test_histogram);
        for (int j = 0 ; j < TOP_N; j++) {
            if (current_score < top_distances[j]) {
                for (int k = TOP_N - 1; k > j; k--) {
                    top_distances[k] = top_distances[k-1];
                    top_shifts[k] = top_shifts[k-1];
                }
                top_distances[j] = current_score;
                top_shifts[j] = i;
                break;
            }
        }
    }
    free(distribution);
    free(histogram);
}

int main(void) {
    int * top_shifts = malloc(TOP_N*sizeof(int));
    double * top_distances = malloc(TOP_N*sizeof(double));
    break_caesar_cipher("uwtlwfrrnsl", top_shifts, top_distances, euclidean_distance);
    for (int i = 0; i < TOP_N; i++)
        printf("%d\n", top_shifts[i]);
    free(top_distances);
    free(top_shifts);

}