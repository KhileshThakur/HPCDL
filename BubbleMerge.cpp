#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
using namespace std;

void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        int start = i % 2;
        #pragma omp parallel for
        for (int j = start; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);
            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}

void printArray(const vector<int>& arr, const string& label) {
    cout << label << ": ";
    for (int val : arr) cout << val << " ";
    cout << endl;
}

int main() {
    const int size = 20;
    vector<int> original(size), bubbleArr, mergeArr;
    srand(time(0));
    for (int i = 0; i < size; i++) original[i] = rand() % 100;

    bubbleArr = original;
    mergeArr = original;

    printArray(original, "Original");

    double t1 = omp_get_wtime();
    parallelBubbleSort(bubbleArr);
    double t2 = omp_get_wtime();

    double t3 = omp_get_wtime();
    parallelMergeSort(mergeArr, 0, size - 1);
    double t4 = omp_get_wtime();

    printArray(bubbleArr, "Bubble Sorted");
    printArray(mergeArr, "Merge Sorted");

    cout << "Bubble Sort Time: " << (t2 - t1) << " sec" << endl;
    cout << "Merge Sort Time:  " << (t4 - t3) << " sec" << endl;

    return 0;
}
