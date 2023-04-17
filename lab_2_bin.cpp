#include <iostream>
#include <vector>
#include <limits.h>
#include <locale.h>
#include <queue>
#include <random>
#include <stdlib.h>
#include <chrono>

using namespace std;

class BinaryHeap {
private:
    vector<pair<int, int>> heap;

    // Helper functions for obtaining the indices of a heap element's parent, left and right children.
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    // Heapify-up or sift-up operation during insertion of a new element.
    void siftUp(int i) {
        while (i > 0 && heap[parent(i)].second > heap[i].second) {
            swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }

    // Heapify-down or sift-down operation during removal of the root element.
    void siftDown(int i) {
        int minIndex = i;
        int l = leftChild(i);
        if (l < heap.size() && heap[l].second < heap[minIndex].second) {
            minIndex = l;
        }
        int r = rightChild(i);
        if (r < heap.size() && heap[r].second < heap[minIndex].second) {
            minIndex = r;
        }
        if (i != minIndex) {
            swap(heap[i], heap[minIndex]);
            siftDown(minIndex);
        }
    }

public:
    BinaryHeap() {}

    //Insertion of a new element into the heap.
    void insert(pair<int, int> element) {
        heap.push_back(element);
        siftUp(heap.size() - 1);
    }

    // Extraction of the minimum element from the heap.
    pair<int, int> extractMin() {
        pair<int, int> result = heap[0];
        heap[0] = heap[heap.size() - 1];
        heap.pop_back();
        siftDown(0);
        return result;
    }

    // Checking if the heap is empty.
    bool empty() {
        return heap.size() == 0;
    }

    // Decreasing the key of an element in the heap.
    void decreaseKey(int vertex, int newKey) {
        for (int i = 0; i < heap.size(); i++) {
            if (heap[i].first == vertex) {
                heap[i].second = newKey;
                siftUp(i);
                break;
            }
        }
    }
};

vector<int> dijkstra(vector<vector<int>>& graph, int source) {
    int n = graph.size();

    // Creating a vector of distances, initially filled with infinities, except for the distance from the source to the source node, which is set to 0.
    vector<int> dist(n, INT_MAX);
    dist[source] = 0;

    // Creating a binary heap and adding all nodes of the graph to it appropriately.
    BinaryHeap minHeap;
    for (int i = 0; i < n; i++) {
        minHeap.insert(make_pair(i, dist[i]));
    }

    // Iterating over all nodes of the graph and updating distances to all its neighbors if the new distance is smaller than the current one.
    while (!minHeap.empty()) {
        pair<int, int> current = minHeap.extractMin();
        int u = current.first;

        // Iterating only over the neighbors of the current node that have edges to this node.
        for (int v = 0; v < n; v++) {
            if (graph[u][v] > 0) {
                int alt = dist[u] + graph[u][v];
                if (alt < dist[v]) {
                    dist[v] = alt;
                    minHeap.decreaseKey(v, alt);
                }
            }
        }
    }

    return dist;
}

void printAdjacencyMatrix(vector<vector<int>>& graph) {
    int n = graph.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph[i][j] < 10)
                cout << graph[i][j] << "   ";
            else
                cout << graph[i][j] << "  ";
        }
        cout << endl;
    }
}

// Class for representing an edge.
class Edge {
public:
    int dest;
    int weight;

    Edge(int dest, int weight) : dest(dest), weight(weight) {}
};

// Class for representing a graph as an adjacency list.
class Graph {
public:
    int V;// The number of vertices.
    vector<vector<Edge>> adj;// The adjacency list.

    Graph(int V) : V(V), adj(V) {}

    // Adding an edge to the graph.
    void addEdge(int src, int dest, int weight) {
        adj[src].push_back(Edge(dest, weight));
        adj[dest].push_back(Edge(src, weight));// If the graph is undirected.
    }
};

// Class for representing a node in the heap.
class FibHeap {
public:
    int vertex;
    int dist;

    FibHeap(int vertex, int dist) : vertex(vertex), dist(dist) {}

    // Comparison operator used for sorting in the heap.
    bool operator<(const FibHeap& other) const {
        return dist > other.dist;
    }
};

// Function for finding the shortest path in a graph using Dijkstra's algorithm.
vector<int> dijkstra_fib(Graph& graph, int start) {
    int n = graph.V;
    vector<int> dist(n, numeric_limits<int>::max());  // Initialize the distance to all vertices as infinity.
    dist[start] = 0;

    priority_queue<FibHeap> pq;
    pq.push(FibHeap(start, 0));// We add the starting vertex to the heap.

    vector<bool> visited(n, false);// Annotations on vertex visiting.

    while (!pq.empty()) {
        int u = pq.top().vertex;
        pq.pop();

        if (visited[u]) {
            continue;
        }
        visited[u] = true;

        for (auto& edge : graph.adj[u]) {
            int v = edge.dest;
            int weight = edge.weight;
            if (!visited[v]) {
                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    pq.push(FibHeap(v, dist[v]));// adding a vertex
                }
            }
        }
    }

    return dist;
}

void printGraph(Graph& graph) {
    int n = graph.V;

    // We create an adjacency matrix and fill it with zeros.
    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));

    // We fill the adjacency matrix with values from the adjacency vector.
    for (int i = 0; i < n; i++) {
        for (auto& edge : graph.adj[i]) {
            int j = edge.dest;
            int weight = edge.weight;
            adjMatrix[i][j] = weight;
            adjMatrix[j][i] = weight; // If the graph is undirected
        }
    }

    // printing the adjacency matrix to the console
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (adjMatrix[i][j] < 10)
                cout << adjMatrix[i][j] << "   ";
            else
                cout << adjMatrix[i][j] << "  ";
        }
        cout << endl;
    }
}

int main() {
    std::mt19937 rng; // Let's create an instance of a random number generator
    rng.seed(std::random_device()()); // We set the seed of the random number generator
    int random_number;
    int n, var, start, ch = -1;
    while (ch != 0)
    {
        cout << "\nChoose a type of heap: \n1. Binary\n2. Fibonacci\nTo exit the program, press 0\n";
        cin >> ch;
        if (ch == 0)
            break;
        cout << "Enter the number of vertices: ";
        cin >> n;
        if (ch == 2)
        {
            Graph graph_fib(n);

            for (int i = 0; i < n; i++) {
                for (int j = i; j < n; j++) {
                    if (j == i)
                        graph_fib.addEdge(i, j, 0);
                    else
                    {
                       random_number = rng() % 101;
                        if (random_number <= 70)
                            var = 0;
                        else {
                            var = rng() % 21;
                            if (var != 0)
                                graph_fib.addEdge(i, j, var);
                        }
                    }
                }
            }

            cout << "Adjacency matrix of the graph:" << endl;
            printGraph(graph_fib);

            cout << "Enter the starting vertex: " << "from 0 to " << n-1 << endl;
            cin >> start;

        //    auto start_time = std::chrono::high_resolution_clock::now();
            vector<int> dist = dijkstra_fib(graph_fib, start);
       //     auto end_time = std::chrono::high_resolution_clock::now();
        //    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

            cout << "Shortest distances from vertex " << start << ":" << endl;
            for (int i = 0; i < n; i++) {
                if (dist[i] == INT_MAX || dist[i] < 0)
                    cout << i << ": ?" << endl;
                else
                    cout << i << ": " << dist[i] << endl;
            }
       //     std::cout << "Time taken by code: " << duration.count() << " microseconds\n";
        }
        if (ch == 1)
        {
            vector<vector<int>> graph(n, vector<int>(n));
            for (int i = 0; i < n; i++) {
                for (int j = i; j < n; j++) {
                    if (j == i)
                        graph[i][j] = 0;
                    else
                    {
                        /*random_number = rng() % 101;
                        if (random_number <= 20)
                            graph[i][j] = 0;
                        else*/
                            graph[i][j] = rng() % 20 + 1;
                    }
                }
            }
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < i; j++) {
                    graph[i][j] = graph[j][i];
                }
            }

            cout << "Adjacency matrix:" << endl;
            printAdjacencyMatrix(graph);

            int source;
            cout << "Enter the starting vertex: " << "from 0 to " << n-1 << endl;
            cin >> source;

          //  auto start_time = std::chrono::high_resolution_clock::now();
            vector<int> dist = dijkstra(graph, source);
          //  auto end_time = std::chrono::high_resolution_clock::now();
          //  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

            cout << "Shortest distances from vertex " << source << ":" << endl;
            for (int i = 0; i < n; i++) {
                if (dist[i] == INT_MAX || dist[i] < 0)
                    cout << i << ": ?" << endl;
                else
                    cout << i << ": " << dist[i] << endl;
            }
         //   std::cout << "Time taken by code: " << duration.count() << " microseconds\n";
        }
    }
    return 0;
}