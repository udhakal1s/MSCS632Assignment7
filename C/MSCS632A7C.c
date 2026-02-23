// Umesh Dhakal
// MSCS632A7
// Mean, Median and Mode
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// qsort to sort integers
static int compare_ints(const void *a, const void *b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    return (x > y) - (x < y);
}

//Mean = sum / total number
static double calculate_mean(const int *numbers_list, size_t numbers_count) {
    long long total_sum = 0;
    for (size_t i = 0; i < numbers_count; i++) {
        total_sum += numbers_list[i];
    }
    return (double)total_sum / (double)numbers_count;
}

//Median (sorting list of integer)
static double calculate_median_sorted(const int *sorted_list, size_t numbers_count) {
    if (numbers_count % 2 == 1) {
        return (double)sorted_list[numbers_count / 2];
    }
    return ((double)sorted_list[(numbers_count / 2) - 1] + (double)sorted_list[numbers_count / 2]) / 2.0;
}

//Mode
static int calculate_modes_sorted(const int *sorted_list, size_t numbers_count,
                                  int **out_modes, size_t *out_mode_count, size_t *out_max_freq) {
    size_t max_freq = 1;

    // find max frequency
    for (size_t i = 0; i < numbers_count; ) {
        size_t j = i + 1;
        while (j < numbers_count && sorted_list[j] == sorted_list[i]) j++;
        size_t freq = j - i;
        if (freq > max_freq) max_freq = freq;
        i = j;
    }

    // If max frequency is 1, Mode is zero
    if (max_freq == 1) {
        *out_modes = NULL;
        *out_mode_count = 0;
        *out_max_freq = 1;
        return 1;
    }

    // freqency == max frequency
    int *modes_list = NULL;
    size_t mode_count = 0, capacity = 0;

    for (size_t i = 0; i < numbers_count; ) {
        size_t j = i + 1;
        while (j < numbers_count && sorted_list[j] == sorted_list[i]) j++;
        size_t freq = j - i;

        if (freq == max_freq) {
            if (mode_count == capacity) {
                size_t new_capacity = (capacity == 0) ? 4 : capacity * 2;
                int *new_ptr = (int*)realloc(modes_list, new_capacity * sizeof(int));
                if (!new_ptr) {
                    free(modes_list);
                    return 0;
                }
                modes_list = new_ptr;
                capacity = new_capacity;
            }
            modes_list[mode_count++] = sorted_list[i];
        }

        i = j;
    }

    *out_modes = modes_list;
    *out_mode_count = mode_count;
    *out_max_freq = max_freq;
    return 1;
}

// Input
static int read_numbers_interactive(int **out_list, size_t *out_count) {
    char line[4096];

    printf("Enter all the integers separated by spaces: ");
    if (!fgets(line, sizeof(line), stdin)) return 0;

    int *numbers_list = NULL;
    size_t numbers_count = 0, capacity = 0;

    char *token = strtok(line, " \t\r\n");
    while (token) {
        char *endptr = NULL;
        long value = strtol(token, &endptr, 10);
        if (endptr == token || *endptr != '\0') {
            free(numbers_list);
            return 0;
        }

        if (numbers_count == capacity) {
            size_t new_capacity = (capacity == 0) ? 8 : capacity * 2;
            int *new_ptr = (int*)realloc(numbers_list, new_capacity * sizeof(int));
            if (!new_ptr) {
                free(numbers_list);
                return 0;
            }
            numbers_list = new_ptr;
            capacity = new_capacity;
        }

        numbers_list[numbers_count++] = (int)value;
        token = strtok(NULL, " \t\r\n");
    }

    if (numbers_count == 0) {
        free(numbers_list);
        return 0;
    }

    *out_list = numbers_list;
    *out_count = numbers_count;
    return 1;
}

int main(int argc, char **argv) {
    int *numbers_list = NULL;
    size_t numbers_count = 0;

    
    if (argc > 1) {
        numbers_count = (size_t)(argc - 1);
        numbers_list = (int*)malloc(numbers_count * sizeof(int));
        if (!numbers_list) return 1;

        for (int i = 1; i < argc; i++) {
            char *endptr = NULL;
            long value = strtol(argv[i], &endptr, 10);
            if (endptr == argv[i] || *endptr != '\0') {
                fprintf(stderr, "Invalid integer: %s\n", argv[i]);
                free(numbers_list);
                return 1;
            }
            numbers_list[i - 1] = (int)value;
        }
    } else {
        if (!read_numbers_interactive(&numbers_list, &numbers_count)) {
            fprintf(stderr, "Input error.\n");
            return 1;
        }
    }

    int *sorted_list = (int*)malloc(numbers_count * sizeof(int));
    if (!sorted_list) {
        free(numbers_list);
        return 1;
    }
    memcpy(sorted_list, numbers_list, numbers_count * sizeof(int));
    qsort(sorted_list, numbers_count, sizeof(int), compare_ints);

    // Calculating results
    double mean_value = calculate_mean(numbers_list, numbers_count);
    double median_value = calculate_median_sorted(sorted_list, numbers_count);

    int *modes_list = NULL;
    size_t mode_count = 0, max_freq = 1;
    if (!calculate_modes_sorted(sorted_list, numbers_count, &modes_list, &mode_count, &max_freq)) {
        fprintf(stderr, "Mode calculation failed.\n");
        free(sorted_list);
        free(numbers_list);
        return 1;
    }

    // Output for mean, median and mode 
    printf("\nCount  : %zu\n", numbers_count);
    printf("Mean   : %.4f\n", mean_value);
    printf("Median : %.4f\n", median_value);

    if (mode_count == 0) {
        printf("Mode   : No mode\n");
    } else {
        printf("Mode   : ");
        for (size_t i = 0; i < mode_count; i++) {
            printf("%d%s", modes_list[i], (i + 1 < mode_count) ? ", " : "");
        }
        printf(" (frequency: %zu)\n", max_freq);
    }
    printf("\n");

    //free memory 
    free(modes_list);
    free(sorted_list);
    free(numbers_list);
    return 0;
}
