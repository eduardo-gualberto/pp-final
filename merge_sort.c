#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// merge function for merging two parts
void merge(int* a, int low, int mid, int high)
{

	// n1 is size of left side and n2 is size of right side
	int n1 = mid - low + 1;
	int n2 = high - mid;

	int* left = (int*)malloc(n1 * sizeof(int));
	int* right = (int*)malloc(n2 * sizeof(int));

	int i;
	int j;

	// storing values in left part
	for (i = 0; i < n1; i++)
		left[i] = a[i + low];

	// storing values in right part
	for (i = 0; i < n2; i++)
		right[i] = a[i + mid + 1];

	int k = low;

	i = j = 0;

	// merge left and right in ascending order
	while (i < n1 && j < n2)
	{
		if (left[i] <= right[j])
			a[k++] = left[i++];
		else
			a[k++] = right[j++];
	}

	// insert remaining values from left
	while (i < n1)
		a[k++] = left[i++];

	// insert remaining values from right
	while (j < n2)
		a[k++] = right[j++];

	free(left);
	free(right);
}

// merge sort function
void merge_sort(int* a, int low, int high)
{

	// calculating mid point of array
	int mid = low + (high - low) / 2;

	if (low < high)
	{
		// call 1st half
		merge_sort(a, low, mid);

		// call 2nd half
		merge_sort(a, mid + 1, high);

		// merge 1st and 2nd halves
		merge(a, low, mid, high);
	}
}

// driver
int main(int argc, char** argv)
{
	char* sz;

	int MAX_ARRAY_ELEMENTS = 2000;

	// parse command line arguments
	for (--argc, ++argv; argc > 0; --argc, ++argv)
	{
		sz = *argv;
		if (*sz != '-')
			break;

		switch (sz[1])
		{
		case 'A':  // array max
			MAX_ARRAY_ELEMENTS = atoi(sz + 2);
			break;
		}
	}

	printf("\n\nArray[%d]", MAX_ARRAY_ELEMENTS);

	// allocate the array
	int* array = (int*)malloc(sizeof(int) * MAX_ARRAY_ELEMENTS);

	// generating random values in array
	srand(clock());
	for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++)
		array[i] = rand();

	printf("\n\nArray Randomized");

	clock_t time = clock();

	merge_sort(array, 0, MAX_ARRAY_ELEMENTS - 1);

	printf("\n\nSorted in %f Seconds", (clock() - time) / 1000.0L);

	int last = 0;
	for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++)
	{
		if (array[i] < last)
		{
			printf("\n\nArray Not Sorted");
			return 0;
		}
		last = array[i];
	}

	printf("\n\nArray Sorted");
	if (MAX_ARRAY_ELEMENTS < 50)
		for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++)
			printf(" %d", array[i]);
	printf("\n");

	free(array);

	return 0;
}
