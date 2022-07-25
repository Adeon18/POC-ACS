#include <stdio.h>
#include <stdint.h>

void func (uint32_t* input_array, size_t size);

int main() {

	uint32_t arr[] = {3, 4, 1, 8, 2};
	size_t size = 5;
	printf("Before sort Arr: [%d, %d, %d, %d, %d]\n", arr[0], arr[1], arr[2], arr[3], arr[4]);
	func(arr, size);
	printf("After sort Arr: [%d, %d, %d, %d, %d]\n", arr[0], arr[1], arr[2], arr[3], arr[4]);
	return 0;
}
