// Merge sorts an array of integers entered by user

// Include necessary libraries
#include <stdio.h>
#include <cs50.h>

// Declare variables
int array[64];
int count;

// Declare functions
void mergesort(int arr[], int l, int r);
void merge(int arr[], int l, int m, int r);

int main(void)
{
    // Get number of integers from user
    do
    {
        count = get_int("How many integers? ");
        if (count < 2 || count > 64)
        {
            printf("Must be between 2 and 64 inclusive\n");
        }
    }
    while (count < 2 || count > 64);


    // Fill array with integers from user
    for (int i = 0; i < count; i++)
    {
        array[i] = get_int("Integer %i: ", i + 1);
    }

    // Mergesort array and print sorted array
    mergesort(array, 0, count - 1);
    printf("Sorted array: ");
    for (int i = 0; i < count; i++)
    {
        printf("%i ", array[i]);
    }
    printf("\n");
    return 0;
}

// Define mergesort function
void mergesort(int arr[], int l, int r)
{
    // If array is more than 1 element, divide into halves
    if (l < r)
    {
        int m = (l + r) / 2;

        // Mergesort each half and then merge
        mergesort(arr, l, m);
        mergesort(arr, m + 1 , r);
        merge(arr, l, m, r);
    }
}

// Define merge function
void merge(int arr[], int l, int m, int r)
{
    // Create temporary subarrays for each segment
    int x = m - l + 1;
    int y = r - m;
    int L[x], R[y];

    // Copy elements to subarrays
    for (int i = 0; i < x; i++)
    {
        L[i] = arr[l + i];
    }
    for (int i = 0; i < y; i++)
    {
        R[i] = arr[m + 1 + i];
    }

    // Sort the elements back into original array
    int i = 0, j = 0;
    while (i < x && j < y)
    {
        if (L[i] <= R[j])
        {
            arr[l] = L[i];
            i++;
        }
        else
        {
            arr[l] = R[j];
            j++;
        }
        l++;
    }

    // Transfer remaining elements from L
    while (i < x)
    {
        arr[l] = L[i];
        i++;
        l++;
    }

    // Transfer remaining elements from R
    while (j < y)
    {
        arr[l] = R[j];
        j++;
        l++;
    }
}