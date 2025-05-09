#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <omp.h>

using namespace std;

void parallelDFS(int start, const vector<vector<int>>& adj, vector<bool>& visited) {
    stack<int> s;
    s.push(start);
    visited[start] = true;

    while (!s.empty()) {
        int node;
        
        #pragma omp critical
        {
            if (!s.empty()) {
                node = s.top();
                s.pop();
                cout << node << " ";
            }
        }

        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); ++i) {
            int neighbor = adj[node][i];

            if (!visited[neighbor]) {
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        s.push(neighbor);
                    }
                }
            }
        }
    }
    cout << endl;
}

void parallelBFS(int start, const vector<vector<int>>& adj, vector<bool>& visited) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int size = q.size();

        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            int node;

            #pragma omp critical
            {
                if (!q.empty()) {
                    node = q.front();
                    q.pop();
                    cout << node << " ";
                }
            }

            for (int j = 0; j < adj[node].size(); ++j) {
                int neighbor = adj[node][j];
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
    }
    cout << endl;
}

int main() {
    int n = 6;
    vector<vector<int>> adj(n);

    adj[0] = {1, 2};
    adj[1] = {0, 3, 4};
    adj[2] = {0, 5};
    adj[3] = {1};
    adj[4] = {1};
    adj[5] = {2};

    vector<bool> visitedDFS(n, false);
    cout << "Parallel DFS: ";
    double start = omp_get_wtime();
    parallelDFS(0, adj, visitedDFS);
    double end = omp_get_wtime();
    cout << "Time: " << end - start << " s\n\n";

    vector<bool> visitedBFS(n, false);
    cout << "Parallel BFS: ";
    start = omp_get_wtime();
    parallelBFS(0, adj, visitedBFS);
    end = omp_get_wtime();
    cout << "Time: " << end - start << " s\n";

    return 0;
}
