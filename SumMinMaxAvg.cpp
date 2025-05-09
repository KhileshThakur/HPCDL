#include <iostream>
#include <vector>
#include <omp.h>
#include <climits>

using namespace std;

int main() {
    int n = 1000000;
    vector<int> arr(n);

    for (int i = 0; i < n; i++)
        arr[i] = rand() % 1000;

    int max_val = INT_MIN;
    int min_val = INT_MAX;
    long long sum = 0;
    double avg = 0;

    #pragma omp parallel for reduction(max:max_val) reduction(min:min_val) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_val)
            max_val = arr[i];
        if (arr[i] < min_val)
            min_val = arr[i];
        sum += arr[i];
    }

    avg = (double)sum / n;

    cout << "Parallel Reduction Results:" << endl;
    cout << "Max: " << max_val << endl;
    cout << "Min: " << min_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;

    return 0;
}
