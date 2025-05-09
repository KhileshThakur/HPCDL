#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
using namespace std;

void printArray(const vector<int>& arr) {
        for (int val : arr) cout << val << " ";
        cout << "\n";
}

void sequentialBubbleSort(vector<int>& arr) {
        int n = arr.size();
        for (int i = 0; i < n-1; i++) {
                for (int j = 0; j < n - i - 1; j++) {
                        if (arr[j] > arr[j+1]) {
                                swap(arr[j], arr[j+1]);
                        }
                }
        }
}

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

void merge(vector<int>& arr, int l, int m, int r) {
        vector<int> left(arr.begin() + l, arr.begin() + m + 1);
        vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);
        int i = 0, j = 0, k = l;
        while (i < left.size() && j < right.size()) {
                if (left[i] < right[j])
                        arr[k++] = left[i++];
                else
                        arr[k++] = right[j++];
        }
        while (i < left.size()) arr[k++] = left[i++];
        while (j < right.size()) arr[k++] = right[j++];
}

void parallelMergeSort(vector<int>& arr, int l, int r, int depth = 0) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        if (depth <= 3) {
                #pragma omp parallel sections
                {
                        #pragma omp section
                        parallelMergeSort(arr, l, m, depth + 1);
                        #pragma omp section
                        parallelMergeSort(arr, m + 1, r, depth + 1);
                }
        } else {
                parallelMergeSort(arr, l, m, depth + 1);
                parallelMergeSort(arr, m + 1, r, depth + 1);
        }
        merge(arr, l, m, r);
}

void sequentialMergeSort(vector<int>& arr, int l, int r) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        sequentialMergeSort(arr, l, m);
        sequentialMergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
}

int main() {
        const int size = 100000;
        vector<int> data(size), data1, data2, data3, data4;
        srand(time(0));
        for (int i = 0; i < size; i++) {
                data[i] = rand() % 100;
        }
        data1 = data;
        data2 = data;
        data3 = data;
        data4 = data;

        cout << "Original Array:\n";
        printArray(data);
        cout << "\n";

        double t1 = omp_get_wtime();
        sequentialBubbleSort(data1);
        double t2 = omp_get_wtime();

        cout << "Sequential Bubble Sort:\n";
        printArray(data1);
        cout << "Time: " << t2 - t1 << " sec\n\n";

        double t3 = omp_get_wtime();
        parallelBubbleSort(data2);
        double t4 = omp_get_wtime();

        cout << "Parallel Bubble Sort:\n";
        printArray(data2);
        cout << "Time: " << t4 - t3 << " sec\n\n";

        double t5 = omp_get_wtime();
        sequentialMergeSort(data3, 0, size - 1);
        double t6 = omp_get_wtime();

        cout << "Sequential Merge Sort:\n";
        printArray(data3);
        cout << "Time: " << t6 - t5 << " sec\n\n";

        double t7 = omp_get_wtime();
        parallelMergeSort(data4, 0, size - 1);
        double t8 = omp_get_wtime();

        cout << "Parallel Merge Sort:\n";
        printArray(data4);
        cout << "Time: " << t8 - t7 << " sec\n";

        return 0;
}
