#include <stdio.h>
#include <stdint.h>

void func(int32_t* input_array, size_t size, int32_t* min, int32_t* max, double* mean, double* variance);

int main() {
	int32_t array[] = {(int32_t)-2,(int32_t)-2, (int32_t)-3, (int32_t)-4, (int32_t)-5 ,(int32_t)-6, (int32_t)-7};
    size_t size = 7;
    int32_t min;
    int32_t max;
    double mean;
    double variance;

    func(array, size, &min, &max, &mean, &variance);

    printf("min: %d\n", min);
    printf("max: %d\n", max);
    printf("mean: %f\n", mean);
    printf("variance: %f\n", variance);

	return 0;
}
