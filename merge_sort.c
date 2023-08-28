#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(WIN32) || (_WIN64)

#include <windows.h>
#define pthread_t DWORD
#define pthread_create(THREAD_ID_PTR, ATTR, ROUTINE, PARAMS) CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ROUTINE, (void *)PARAMS, 0, THREAD_ID_PTR)
#define sleep(ms) Sleep(ms)

#else // Linux

#include <pthread.h>
#include <unistd.h>

#endif

typedef struct TASK
{
	int low;
	int high;
	int busy;
	int *a;
} TASK;

// merge function for merging two parts
void merge(int *a, int low, int mid, int high)
{

	// n1 is size of left side and n2 is size of right side
	int n1 = mid - low + 1;
	int n2 = high - mid;

	int *left = (int *)malloc(n1 * sizeof(int));
	int *right = (int *)malloc(n2 * sizeof(int));

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
void merge_sort(int *a, int low, int high)
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

void *merge_sort_thread(void *arg)
{
	TASK *task = (TASK *)arg;
	int low;
	int high;

	// calculating low and high
	low = task->low;
	high = task->high;

	// evaluating mid point
	int mid = low + (high - low) / 2;

	if (low < high)
	{
		merge_sort(task->a, low, mid);
		merge_sort(task->a, mid + 1, high);
		merge(task->a, low, mid, high);
	}
	task->busy = 0;
	return 0;
}

float sequential_driver(int size)
{
	int MAX_ARRAY_ELEMENTS = size;

	// allocate the array
	int *array = (int *)malloc(sizeof(int) * MAX_ARRAY_ELEMENTS);

	// generating random values in array
	srand(clock());
	for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++)
		array[i] = rand();

	clock_t time = clock();

	merge_sort(array, 0, MAX_ARRAY_ELEMENTS - 1);

	clock_t end_time = clock();

	int last = 0;
	for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++)
	{
		if (array[i] < last)
		{
			return -1;
		}
		last = array[i];
	}

	free(array);

	return (float)(end_time - time) / CLOCKS_PER_SEC;
}

float paralel_driver(int size, int num_threads)
{
	int MAX_ARRAY_ELEMENTS = size;
	int MAX_THREADS = num_threads;

	// allocate the array
	int *array = (int *)malloc(sizeof(int) * MAX_ARRAY_ELEMENTS);

	// generating random values in array
	srand(clock());
	for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++)
		array[i] = rand();

	pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * MAX_THREADS);
	TASK *tasklist = (TASK *)malloc(sizeof(TASK) * MAX_THREADS);

	int len = MAX_ARRAY_ELEMENTS / MAX_THREADS;

	TASK *task;
	int low = 0;


	for (int i = 0; i < MAX_THREADS; i++, low += len)
	{
		task = &tasklist[i];
		task->low = low;
		task->high = low + len - 1;
		if (i == (MAX_THREADS - 1))
			task->high = MAX_ARRAY_ELEMENTS - 1;
	}

	clock_t time = clock();

	// create the threads
	for (int i = 0; i < MAX_THREADS; i++)
	{
		task = &tasklist[i];
		task->a = array;
		task->busy = 1;
		pthread_create(&threads[i], 0, merge_sort_thread, task);
	}

	// wait for all threads
	for (int i = 0; i < MAX_THREADS; i++)
		while (tasklist[i].busy)
			sleep(50);

	TASK *taskm = &tasklist[0];
	for (int i = 1; i < MAX_THREADS; i++)
	{
		TASK *task = &tasklist[i];
		merge(taskm->a, taskm->low, task->low - 1, task->high);
	}

	clock_t end_time = clock();

	int last = 0;
	for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++)
	{
		if (array[i] < last)
		{
			return -1;
		}
		last = array[i];
	}
	free(tasklist);
	free(threads);

	return (float)(end_time - time)/CLOCKS_PER_SEC;
}

int paralel_tests() {
	int arr_size = 500000;
	int threads[] = {1,2,4,8};

	printf("\n\nPARALEL TESTS:\n\n");
	printf("arr_size,\tthreds,\ttime\n");
	for (int i = 0; i < 4; i++)
	{
		float acc = 0.0;
		for (int j = 0; j < 2; i++)
		{
			acc += paralel_driver(arr_size, threads[i]) / 2;
		}
		printf("%d,\t%d,\t%fs\n", arr_size, threads[i], acc);
	}
	return 0;
}

int sequential_tests() {
	int arr_size = 500000;

	printf("\n\nSEQUENTIAL TESTS:\n\n");
	printf("arr_size,\tthreds,\ttime\n");
	float acc = 0.0;
	for (int i = 0; i < 3; i++)
	{
		acc += sequential_driver(arr_size) / 3;
	}
	printf("%d,\t%d,\t%fs\n", arr_size, 1, acc);
	return 0;
}

// driver
int main(int argc, char **argv)
{
	sequential_tests();
	paralel_tests();
	return 0;
}