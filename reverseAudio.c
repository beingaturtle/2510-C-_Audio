#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "wav.h"

// Function to read WAV header from file
int readWavHeader(FILE *file, WAVHEADER *header) {
    if (file == NULL || header == NULL) {
        return 0; // Error: Invalid input think of 0 as false
    }
    fread(header, sizeof(WAVHEADER), 1, file);
    return 1; // Success think 1 as true
}

// Function to read audio samples from file  - - - -- -- - - --
int readAudioSamples(FILE *file, int16_t *samples, size_t num_samples) {
    if (file == NULL || samples == NULL) {
        return 0; // Error: Invalid input
    }
    fread(samples, sizeof(int16_t), num_samples, file);
    return 1; // Success
}

// Function to reverse audio samples  - - -- -  - - - - -
void reverseSamples(int16_t *samples, int16_t *reversedSamples, size_t num_samples) {
    for (size_t i = 0; i < num_samples; i++) {
        reversedSamples[i] = samples[num_samples - i - 1];
    }
}

// Function to write WAV header to file
void writeWavHeader(FILE *file, WAVHEADER *header) {
    fwrite(header, sizeof(WAVHEADER), 1, file);
}

// Function to write audio samples to file
void writeAudioSamples(FILE *file, int16_t *samples, size_t num_samples) {
    fwrite(samples, sizeof(int16_t), num_samples, file);
}

/**
 * How to use:
 * Simply running the code using a compiler like Clion will directly convert a soundclip called "test3.wav"
 * inside a directory called "nativeSoundClip". Then it will output a file called "reversedAudio" inside a folder called
 * "output"
 *
 * @return none
 */
int main() {
    FILE *inputFile;
    FILE *outputFile;
    WAVHEADER header;
    int16_t *samples;
    int16_t *reversedSamples;

    // Open input WAV file
    inputFile = fopen("./nativeSoundClips/Dixie.wav", "rb"); //find Dixie inside nativeSoundclip directory
    if (inputFile == NULL) {
        printf("Error: Unable to open input WAV file\n");       return 1;
    }

    // Read WAV header
    if (!readWavHeader(inputFile, &header)) {
        printf("Error: Unable to read WAV header\n");
        fclose(inputFile);      return 1;
    }

    // Allocate memory for audio samples
    samples = (int16_t *)malloc(header.subchunk2Size);
    if (samples == NULL) {
        printf("Error: Memory allocation failed\n");
        fclose(inputFile);      return 1;
    }

    // Read audio samples
    if (!readAudioSamples(inputFile, samples, header.subchunk2Size / sizeof(int16_t))) {
        printf("Error: Unable to read audio samples\n");
        fclose(inputFile);      free(samples);
        return 1;
    }

    // Close input WAV file
    fclose(inputFile);

    // Allocate memory for reversed audio samples
    reversedSamples = (int16_t *)malloc(header.subchunk2Size);
    if (reversedSamples == NULL) {
        printf("Error: Memory allocation failed\n");
        free(samples);
        return 1;
    }

    // Reverse audio samples
    reverseSamples(samples, reversedSamples, header.subchunk2Size / sizeof(int16_t));

    // Open output WAV file
    outputFile = fopen("./output/reversedAudio.wav", "wb"); // --- Output: reversedAudio.wav inside output directory
    if (outputFile == NULL) {
        printf("Error: Unable to create output WAV file\n");
        free(samples); free(reversedSamples);       return 1;
    }

    // Update file size in header
    header.chunkSize = sizeof(WAVHEADER) + header.subchunk2Size - 8;

    // Write WAV header
    writeWavHeader(outputFile, &header);

    // Write reversed audio samples
    writeAudioSamples(outputFile, reversedSamples, header.subchunk2Size / sizeof(int16_t));

    // Close output WAV file
    fclose(outputFile);

    // Free allocated memory
    free(samples);
    free(reversedSamples);

    printf("WAV file reversed successfully\n");

    return 0;
}
