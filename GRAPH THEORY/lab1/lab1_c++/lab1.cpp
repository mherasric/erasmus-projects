#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <set>

// Directed Graph class
class DirectedGraph {
private:
    std::unordered_map<int, std::unordered_map<int, int>> adjList; // adjacency list with costs (outbound)
    std::unordered_map<int, std::set<int>> inEdges; // inbound edges for efficiency
    int vertexCount = 0;

public:
    // Add a vertex
    void addVertex(int v) {
        if (adjList.find(v) == adjList.end()) {
            adjList[v] = {};
            inEdges[v] = {};
            vertexCount++;
        }
    }

    // Add an edge with a cost
    void addEdge(int src, int dest, int cost) {
        addVertex(src);
        addVertex(dest);
        if (adjList[src].find(dest) == adjList[src].end()) {
            adjList[src][dest] = cost;
            inEdges[dest].insert(src);
        } else {
            adjList[src][dest] = cost; // Update cost if edge exists
        }
    }

    // Remove an edge
    void removeEdge(int src, int dest) {
        if (hasEdge(src, dest)) {
            adjList[src].erase(dest);
            inEdges[dest].erase(src);
        }
    }

    // Remove a vertex and all connected edges
    void removeVertex(int v) {
        if (adjList.find(v) != adjList.end()) {
            // Remove outgoing edges
            for (const auto& edge : adjList[v]) {
                inEdges[edge.first].erase(v);
            }
            adjList.erase(v);
            // Remove incoming edges
            for (const auto& src : inEdges[v]) {
                adjList[src].erase(v);
            }
            inEdges.erase(v);
            vertexCount--;
        }
    }

    // Get the number of vertices
    int getVertexCount() const {
        return vertexCount;
    }

    // Get the number of edges
    int getEdgeCount() const {
        int count = 0;
        for (const auto& pair : adjList) {
            count += pair.second.size();
        }
        return count;
    }

    // Check if an edge exists
    bool hasEdge(int src, int dest) const {
        return adjList.find(src) != adjList.end() && adjList.at(src).find(dest) != adjList.at(src).end();
    }

    // Get in-degree of a vertex
    int getInDegree(int v) const {
        return inEdges.find(v) != inEdges.end() ? inEdges.at(v).size() : 0;
    }

    // Get out-degree of a vertex
    int getOutDegree(int v) const {
        return adjList.find(v) != adjList.end() ? adjList.at(v).size() : 0;
    }

    // Retrieve the cost of an edge
    int getEdgeCost(int src, int dest) const {
        if (hasEdge(src, dest)) {
            return adjList.at(src).at(dest);
        }
        return -1; // Edge not found
    }

    // Modify the cost of an edge
    void setEdgeCost(int src, int dest, int cost) {
        if (hasEdge(src, dest)) {
            adjList[src][dest] = cost;
        }
    }

    // Print all vertices
    void printVertices() const {
        std::cout << "Vertices: ";
        for (const auto& pair : adjList) {
            std::cout << pair.first << " ";
        }
        std::cout << "\n";
    }

    // Print all edges
    void printEdges() const {
        std::cout << "Edges:\n";
        for (const auto& pair : adjList) {
            for (const auto& edge : pair.second) {
                std::cout << pair.first << " -> " << edge.first << " (cost: " << edge.second << ")\n";
            }
        }
    }

    // Print outbound edges of a vertex (NEW)
    void printOutboundEdges(int v) const {
        std::cout << "Outbound edges from " << v << ":\n";
        if (adjList.find(v) != adjList.end()) {
            for (const auto& edge : adjList.at(v)) {
                std::cout << v << " -> " << edge.first << " (cost: " << edge.second << ")\n";
            }
        } else {
            std::cout << "No outbound edges.\n";
        }
    }

    // Print inbound edges of a vertex (NEW)
    void printInboundEdges(int v) const {
        std::cout << "Inbound edges to " << v << ":\n";
        if (inEdges.find(v) != inEdges.end() && !inEdges.at(v).empty()) {
            for (const auto& src : inEdges.at(v)) {
                std::cout << src << " -> " << v << " (cost: " << getEdgeCost(src, v) << ")\n";
            }
        } else {
            std::cout << "No inbound edges.\n";
        }
    }

    // Read graph from a file
    void readFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening file!\n";
            return;
        }
        int n, m, src, dest, cost;
        file >> n >> m;
        adjList.clear();
        inEdges.clear();
        vertexCount = 0;
        for (int i = 0; i < m; ++i) {
            file >> src >> dest >> cost;
            addEdge(src, dest, cost);
        }
        file.close();
    }

    // Write graph to a file
    void writeToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error writing to file!\n";
            return;
        }
        file << getVertexCount() << " " << getEdgeCount() << "\n";
        for (const auto& pair : adjList) {
            for (const auto& edge : pair.second) {
                file << pair.first << " " << edge.first << " " << edge.second << "\n";
            }
        }
        file.close();
    }

    // Generate a random graph
    void generateRandomGraph(int vertices, int edges) {
        adjList.clear();
        inEdges.clear();
        vertexCount = 0;
        for (int i = 0; i < vertices; ++i)
            addVertex(i);
        srand(static_cast<unsigned>(time(nullptr)));
        int count = 0;
        int maxEdges = vertices * (vertices - 1); // Max possible edges in a directed graph
        if (edges > maxEdges) {
            std::cout << "Too many edges! Max possible for " << vertices << " vertices is " << maxEdges << "\n";
            edges = maxEdges;
        }
        while (count < edges) {
            int src = rand() % vertices;
            int dest = rand() % vertices;
            if (src != dest && !hasEdge(src, dest)) {
                int cost = rand() % 100 + 1;
                addEdge(src, dest, cost);
                count++;
            }
        }
    }

    // Generate two random graphs and save them (NEW)
    void generateTwoRandomGraphs() {
        DirectedGraph g1, g2;

        // First graph: 7 vertices, 20 edges
        g1.generateRandomGraph(7, 20);
        g1.writeToFile("random_graph1.txt");
        std::cout << "First random graph (7 vertices, 20 edges) generated and saved as random_graph1.txt\n";

        // Second graph: 6 vertices, 40 edges (will be capped at 30 due to max edges)
        g2.generateRandomGraph(6, 40); // Max edges for 6 vertices is 6 * 5 = 30
        g2.writeToFile("random_graph2.txt");
        std::cout << "Second random graph (6 vertices, up to 40 edges) generated and saved as random_graph2.txt\n";
    }
};

void printMenu() {
    std::cout << "\n======== Directed Graph Menu ========\n";
    std::cout << "1. Add Vertex\n";
    std::cout << "2. Add Edge\n";
    std::cout << "3. Remove Vertex\n";
    std::cout << "4. Remove Edge\n";
    std::cout << "5. Get Vertex Count\n";
    std::cout << "6. Get Edge Count\n";
    std::cout << "7. Check Edge Existence\n";
    std::cout << "8. Get In-degree of Vertex\n";
    std::cout << "9. Get Out-degree of Vertex\n";
    std::cout << "10. Get/Set Edge Cost\n";
    std::cout << "11. Print Vertices and Edges\n";
    std::cout << "12. Generate Random Graph\n";
    std::cout << "13. Read Graph from File\n";
    std::cout << "14. Write Graph to File\n";
    std::cout << "15. Print Outbound Edges\n";      // NEW
    std::cout << "16. Print Inbound Edges\n";       // NEW
    std::cout << "17. Generate Two Random Graphs\n"; // NEW
    std::cout << "0. Exit\n";
    std::cout << "======================================\n";
    std::cout << "Enter your choice: ";
}

int main() {
    DirectedGraph g;
    int choice;

    do {
        printMenu();
        std::cin >> choice;

        if (choice == 1) {
            int v;
            std::cout << "Enter vertex (integer): ";
            std::cin >> v;
            g.addVertex(v);
            std::cout << "Vertex " << v << " added.\n";
        }
        else if (choice == 2) {
            int src, dest, cost;
            std::cout << "Enter source, destination and cost: ";
            std::cin >> src >> dest >> cost;
            g.addEdge(src, dest, cost);
            std::cout << "Edge added from " << src << " to " << dest << " with cost " << cost << ".\n";
        }
        else if (choice == 3) {
            int v;
            std::cout << "Enter vertex to remove: ";
            std::cin >> v;
            g.removeVertex(v);
            std::cout << "Vertex " << v << " removed.\n";
        }
        else if (choice == 4) {
            int src, dest;
            std::cout << "Enter source and destination of the edge to remove: ";
            std::cin >> src >> dest;
            g.removeEdge(src, dest);
            std::cout << "Edge from " << src << " to " << dest << " removed.\n";
        }
        else if (choice == 5) {
            std::cout << "Total vertices: " << g.getVertexCount() << "\n";
        }
        else if (choice == 6) {
            std::cout << "Total edges: " << g.getEdgeCount() << "\n";
        }
        else if (choice == 7) {
            int src, dest;
            std::cout << "Enter source and destination: ";
            std::cin >> src >> dest;
            std::cout << "Edge from " << src << " to " << dest << (g.hasEdge(src, dest) ? " exists.\n" : " does not exist.\n");
        }
        else if (choice == 8) {
            int v;
            std::cout << "Enter vertex: ";
            std::cin >> v;
            std::cout << "In-degree of vertex " << v << ": " << g.getInDegree(v) << "\n";
        }
        else if (choice == 9) {
            int v;
            std::cout << "Enter vertex: ";
            std::cin >> v;
            std::cout << "Out-degree of vertex " << v << ": " << g.getOutDegree(v) << "\n";
        }
        else if (choice == 10) {
            int subChoice;
            std::cout << "1. Get Edge Cost\n2. Set Edge Cost\nEnter choice: ";
            std::cin >> subChoice;
            if (subChoice == 1) {
                int src, dest;
                std::cout << "Enter source and destination: ";
                std::cin >> src >> dest;
                int cost = g.getEdgeCost(src, dest);
                if (cost != -1)
                    std::cout << "Cost of edge (" << src << ", " << dest << ") is " << cost << ".\n";
                else
                    std::cout << "Edge does not exist.\n";
            }
            else if (subChoice == 2) {
                int src, dest, cost;
                std::cout << "Enter source, destination and new cost: ";
                std::cin >> src >> dest >> cost;
                g.setEdgeCost(src, dest, cost);
                std::cout << "Edge cost updated.\n";
            }
        }
        else if (choice == 11) {
            g.printVertices();
            g.printEdges();
        }
        else if (choice == 12) {
            int vertices, edges;
            std::cout << "Enter number of vertices and edges for random graph: ";
            std::cin >> vertices >> edges;
            g.generateRandomGraph(vertices, edges);
            std::cout << "Random graph generated.\n";
        }
        else if (choice == 13) {
            std::string filename;
            std::cout << "Enter filename to read graph from: ";
            std::cin >> filename;
            g.readFromFile(filename);
            std::cout << "Graph loaded from file.\n";
        }
        else if (choice == 14) {
            std::string filename;
            std::cout << "Enter filename to write graph to: ";
            std::cin >> filename;
            g.writeToFile(filename);
            std::cout << "Graph written to file.\n";
        }
        else if (choice == 15) { // NEW: Print Outbound Edges
            int v;
            std::cout << "Enter vertex: ";
            std::cin >> v;
            g.printOutboundEdges(v);
        }
        else if (choice == 16) { // NEW: Print Inbound Edges
            int v;
            std::cout << "Enter vertex: ";
            std::cin >> v;
            g.printInboundEdges(v);
        }
        else if (choice == 17) { // NEW: Generate Two Random Graphs
            g.generateTwoRandomGraphs();
        }
        else if (choice == 0) {
            std::cout << "Exiting...\n";
        }
        else {
            std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
