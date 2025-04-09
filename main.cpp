#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <fstream>
#include <algorithm>

class Graph {
private:
    int vertices;
    std::vector<std::vector<int>> adjMatrix;
    std::vector<std::vector<int>> adjList;

public:
    Graph(int v) : vertices(v) {
        adjMatrix.resize(vertices, std::vector<int>(vertices, 0));
        adjList.resize(vertices);
    }

    void addEdge(int u, int v) {
        adjMatrix[u][v] = 1;
        adjMatrix[v][u] = 1;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
            exit(1);
        }

        int u, v;
        while (file >> u >> v) {
            addEdge(u, v);
        }
        file.close();
    }

    void bfs(int start, int end) {
        std::vector<bool> visited(vertices, false);
        std::vector<int> parent(vertices, -1);
        std::queue<int> q;

        visited[start] = true;
        q.push(start);
        bool found = false;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == end) {
                found = true;
                break;
            }
            for (int v : adjList[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        if (!found) {
            std::cout << "Nao ha caminho entre " << start << " e " << end << std::endl;
            return;
        }

        std::vector<int> path;
        for (int at = end; at != -1; at = parent[at]) {
            path.push_back(at);
        }
        std::reverse(path.begin(), path.end());

        std::cout << "Caminho BFS: ";
        for (int v : path) std::cout << v << " ";
        std::cout << std::endl;
    }

    void dfsIterative(int start) {
        std::vector<bool> visited(vertices, false);
        std::stack<int> st;
        st.push(start);

        std::cout << "DFS iterativo: ";
        while (!st.empty()) {
            int u = st.top(); st.pop();
            if (!visited[u]) {
                visited[u] = true;
                std::cout << u << " ";
                for (auto it = adjList[u].rbegin(); it != adjList[u].rend(); ++it) {
                    if (!visited[*it]) {
                        st.push(*it);
                    }
                }
            }
        }
        std::cout << std::endl;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo com arestas>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Nao foi possivel abrir o arquivo." << std::endl;
        return 1;
    }

    int u, v, maxVertex = -1;
    std::vector<std::pair<int, int>> edges;
    while (file >> u >> v) {
        edges.emplace_back(u, v);
        maxVertex = std::max(maxVertex, std::max(u, v));
    }
    file.close();

    Graph graph(maxVertex + 1);
    for (const auto& edge : edges) {
        graph.addEdge(edge.first, edge.second);
    }

    graph.bfs(0, 3);
    graph.dfsIterative(0);

    return 0;
}
