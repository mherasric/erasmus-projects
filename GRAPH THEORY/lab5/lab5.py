import random
import heapq

class UndirectedGraph:
    """An undirected graph represented as an adjacency list.
    Vertices are integers from 0 to n-1. Edges are bidirectional and identified by (u, v) tuples with associated integer costs.
    For an edge (u, v), both (u, v) and (v, u) are stored to simplify access, but they represent the same undirected edge.
    """

    def __init__(self, n=0):
        self._adj = {i: set() for i in range(n)} #Initialize adjacency list: each vertex maps to a set of its neighbors ( an edge (u,v) implies (v,u))
        self._costs = {} #Dictionary to store edge costs: key is a tuple (min(u,v), max(u,v)) to ensure (u,v) and (v,u) map to the same cost

    def get_number_of_vertices(self):
        """Returns the total number of vertices in the graph."""
        return len(self._adj)

    def get_number_of_edges(self):
        """Returns the total number of edges in the graph.
        Since edges are undirected, each edge is counted once in _costs.
        """
        return len(self._costs)

    def parse_vertices(self):
        """Returns an iterator over all vertices in the graph."""
        return iter(self._adj.keys())

    def edge_exists(self, u, v):
        """Checks if an edge exists between vertices u and v."""
        return v in self._adj.get(u, set())

    def get_degree(self, vertex):
        """Returns the degree (number of edges) of a given vertex."""
        return len(self._adj.get(vertex, set()))

    def add_vertex(self, vertex):
        """Adds a new vertex to the graph if it doesn't already exist."""
        if vertex not in self._adj:
            self._adj[vertex] = set()
        else:
            print(f"The vertex {vertex} already exists.")

    def remove_vertex(self, vertex):
        """Removes a vertex and all its associated edges from the graph."""
        if vertex in self._adj:
            # Remove all edges incident to this vertex
            for neighbor in list(self._adj[vertex]):
                self.remove_edge(vertex, neighbor)
            # Remove the vertex itself
            del self._adj[vertex]

    def add_edge(self, u, v, cost=0):
        """Adds an undirected edge between u and v with a given cost.
        Adds both (u, v) and (v, u) to the adjacency list to represent the undirected nature.
        Stores the cost with a canonical key (min(u,v), max(u,v)).
        """
        if u in self._adj and v in self._adj and u != v:
            if not self.edge_exists(u, v):
                self._adj[u].add(v)
                self._adj[v].add(u)
                # Store cost with vertices in sorted order to avoid duplication
                edge_key = (min(u, v), max(u, v))
                self._costs[edge_key] = cost
            else:
                print(f"Edge between {u} and {v} already exists.")
        else:
            print("Invalid vertices or self-loop attempted.")

    def remove_edge(self, u, v):
        """Removes an undirected edge between u and v if it exists."""
        if self.edge_exists(u, v):
            self._adj[u].discard(v)
            self._adj[v].discard(u)
            edge_key = (min(u, v), max(u, v))
            self._costs.pop(edge_key, None)

    def get_edge_value(self, u, v):
        """Retrieves the cost of an edge between u and v if it exists, otherwise returns None."""
        edge_key = (min(u, v), max(u, v))
        return self._costs.get(edge_key, None)

    def set_edge_value(self, u, v, cost):
        """Updates the cost of an existing edge between u and v."""
        if self.edge_exists(u, v):
            edge_key = (min(u, v), max(u, v))
            self._costs[edge_key] = cost
        else:
            print(f"No edge exists between {u} and {v}.")

    def parse_neighbors(self, vertex):
        """Returns an iterator over all neighbors of a given vertex."""
        return iter(self._adj.get(vertex, set()))

    def copy(self):
        """Creates a deep copy of the graph."""
        new_graph = UndirectedGraph()
        new_graph._adj = {v: set(self._adj[v]) for v in self._adj}
        new_graph._costs = self._costs.copy()
        return new_graph

    @staticmethod
    def read_from_file(filename):
        """Reads graph data from a file and creates an UndirectedGraph instance.
        File format: First line contains n (vertices) and m (edges).
        Each subsequent line contains u, v, cost for an edge.
        """
        with open(filename, 'r') as f:
            n, m = map(int, f.readline().split())
            graph = UndirectedGraph(n)
            for _ in range(m):
                u, v, cost = map(int, f.readline().split())
                graph.add_edge(u, v, cost)
        return graph

    def write_to_file(self, filename):
        """Writes the graph data to a file.
        Format: First line has n (vertices) and m (edges).
        Subsequent lines list edges as u v cost.
        Includes isolated vertices at the end.
        """
        with open(filename, 'w') as f:
            f.write(f"{len(self._adj)} {len(self._costs)}\n")
            for (u, v), cost in self._costs.items():
                f.write(f"{u} {v} {cost}\n")
            isolated = [v for v in self._adj if not self._adj[v]]
            for v in isolated:
                f.write(f"{v}\n")

    @staticmethod
    def random_graph(n, m):
        """Generates a random undirected graph with n vertices and m edges.
        Ensures no self-loops and no duplicate edges.
        """
        if m > n * (n - 1) // 2:  # Maximum edges in an undirected graph
            print(f"Error: Maximum number of edges for {n} vertices is {n * (n - 1) // 2}.")
            return None
        graph = UndirectedGraph(n)
        added_edges = 0
        possible_edges = [(i, j) for i in range(n) for j in range(i + 1, n)]
        random.shuffle(possible_edges)
        for u, v in possible_edges[:m]:
            graph.add_edge(u, v, random.randint(1, 100))
            added_edges += 1
            if added_edges == m:
                break
        return graph

    def minimum_vertex_cover(self):
        """Finds an approximate minimum vertex cover using a greedy algorithm.
        Repeatedly selects the vertex with the highest degree, includes it in the cover,
        and removes all incident edges until no edges remain.
        Returns a list of vertices forming the vertex cover.

        We are going to use a greedy algorithm because finding the minimum vertex cover is NP-complete, so an exact
        solution is computationally expensive for large graphs. The greedy approach is a 2-approximation algorithm,
        meaning the size of the vertex cover is at most twice the optimal size , which is practical for many solutions
        """
        # Create a copy of the graph to avoid modifying the original
        graph = self.copy()
        vertex_cover = []
        remaining_edges = set(self._costs.keys())  # Track uncovered edges

        while remaining_edges:
            # Find the vertex with the highest degree
            max_degree = -1
            max_vertex = None
            for v in graph.parse_vertices():
                degree = graph.get_degree(v)
                if degree > max_degree:
                    max_degree = degree
                    max_vertex = v

            if max_vertex is None or max_degree == 0:
                break  # No more edges to cover

            # Add the vertex to the vertex cover
            vertex_cover.append(max_vertex)

            # Remove all edges incident to this vertex
            neighbors = list(graph.parse_neighbors(max_vertex))
            for neighbor in neighbors:
                edge_key = (min(max_vertex, neighbor), max(max_vertex, neighbor))
                remaining_edges.discard(edge_key)
                graph.remove_edge(max_vertex, neighbor)

        return vertex_cover

def menu():
    """Interactive menu to perform operations on an undirected graph.
    Includes options to manipulate the graph and find a minimum vertex cover.
    """
    graph = UndirectedGraph(0)

    while True:
        print("\nUndirected Graph Operations Menu:")
        print("1. Load graph from file")
        print("2. Save graph to file")
        print("3. Create a random graph")
        print("4. Display number of vertices and edges")
        print("5. Add a vertex")
        print("6. Remove a vertex")
        print("7. Add an edge")
        print("8. Remove an edge")
        print("9. Get edge cost")
        print("10. Set edge cost")
        print("11. Print the graph")
        print("12. Display neighbors of a vertex")
        print("13. Get degree of a vertex")
        print("14. Find minimum vertex cover")
        print("15. Exit")

        choice = input("Choose an option: ")

        if choice == "1":
            filename = input("Enter filename: ")
            try:
                graph = UndirectedGraph.read_from_file(filename)
                print("Graph loaded successfully.")
            except FileNotFoundError:
                print(f"File {filename} not found.")

        elif choice == "2":
            filename = input("Enter filename to save: ")
            graph.write_to_file(filename)
            print("Graph saved successfully.")

        elif choice == "3":
            n = int(input("Enter number of vertices: "))
            m = int(input("Enter number of edges: "))
            graph = UndirectedGraph.random_graph(n, m)
            if graph:
                print("Random graph created.")
            else:
                print("Failed to create random graph.")

        elif choice == "4":
            print(f"Number of vertices: {graph.get_number_of_vertices()}")
            print(f"Number of edges: {graph.get_number_of_edges()}")

        elif choice == "5":
            vertex = int(input("Enter vertex to add: "))
            graph.add_vertex(vertex)
            print(f"Vertex {vertex} added.")

        elif choice == "6":
            vertex = int(input("Enter vertex to remove: "))
            graph.remove_vertex(vertex)
            print(f"Vertex {vertex} removed.")

        elif choice == "7":
            u = int(input("Enter first vertex: "))
            v = int(input("Enter second vertex: "))
            cost = int(input("Enter edge cost: "))
            graph.add_edge(u, v, cost)
            print(f"Edge {u} - {v} with cost {cost} added.")

        elif choice == "8":
            u = int(input("Enter first vertex: "))
            v = int(input("Enter second vertex: "))
            graph.remove_edge(u, v)
            print(f"Edge {u} - {v} removed.")

        elif choice == "9":
            u = int(input("Enter first vertex: "))
            v = int(input("Enter second vertex: "))
            cost = graph.get_edge_value(u, v)
            if cost is not None:
                print(f"Edge cost {u} - {v}: {cost}")
            else:
                print(f"No edge exists between {u} and {v}.")

        elif choice == "10":
            u = int(input("Enter first vertex: "))
            v = int(input("Enter second vertex: "))
            cost = int(input("Enter new cost: "))
            graph.set_edge_value(u, v, cost)
            print(f"Edge {u} - {v} updated to cost {cost}.")

        elif choice == "11":
            print("Your graph is:")
            for (u, v), cost in graph._costs.items():
                print(f"Edge {u} - {v} with cost {cost}")

        elif choice == "12":
            vertex = int(input("Enter vertex: "))
            print(f"Neighbors of {vertex}: {list(graph.parse_neighbors(vertex))}")

        elif choice == "13":
            vertex = int(input("Enter vertex: "))
            degree = graph.get_degree(vertex)
            print(f"Degree of vertex {vertex}: {degree}")

        elif choice == "14":
            vertex_cover = graph.minimum_vertex_cover()
            print(f"Minimum vertex cover (approximate): {vertex_cover}")
            print(f"Size of vertex cover: {len(vertex_cover)}")

        elif choice == "15":
            print("Thank you, bye!")
            break

        else:
            print("Invalid option, please try again.")

if __name__ == "__main__":
    menu()


"""
EXPLICACIONES MIAS:
- The vertex cover problem involves finding a set of vertices in an undirected graph such that every edge has at least 
one endpoint in the set. The goal is to minimize the size of this set. It’s NP-complete, meaning exact solutions are 
computationally expensive for large graphs.
- I chose a greedy algorithm because it’s simple, efficient, and provides a 2-approximation guarantee, meaning the 
solution is at most twice the size of the optimal vertex cover. This is suitable for educational purposes and practical 
for small to medium graphs.
--WEAKNESS: The greedy algorithm may not always produce the optimal solution (e.g., in the 6-vertex graph, it returns a 
cover of size 4 when the optimal might be 3). This is expected for an approximation algorithm,

- The original DirectedGraph used two dictionaries (_dictOut and _dictIn) to track outbound and inbound edges, reflecting 
the directional nature of edges. For the undirected graph, I replaced these with a single adjacency list (_adj) where 
each vertex maps to its neighbors, as an edge (u,v) implies (v,u).
- Edge costs are stored in a _costs dictionary with keys as (min(u,v),max(u,v)) to ensure (u,v) and (v,u) refer to the 
same edge, avoiding duplication.
- The random_graph method was updated to respect the maximum edge count for undirected graphs (n(n-1)/2)

- How does your program handle edge cases, like empty graphs, disconnected graphs, or isolated vertices?
Empty Graph: If the graph has no vertices or edges, minimum_vertex_cover returns an empty list, as no edges need covering.
Disconnected Graph: The algorithm works correctly, as it processes all edges regardless of connectivity. Each component’s 
edges are covered independently.
Isolated Vertices: These have degree 0 and are never selected unless necessary, as they don’t cover any edges. 
The algorithm correctly ignores them.
Invalid Inputs: The add_edge method checks for valid vertices and prevents self-loops (u != v)
"""
