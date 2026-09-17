import random

class DirectedGraph:
    """A directed graph represented as two dictionaries: one for outbound edges and one for inbound edges.
    Vertices are integers from 0 to n-1. Edges are identified by (source, target) tuples with associated integer values.
    """

    def __init__(self, n):
        self._dictOut = {i: set() for i in range(n)}
        self._dictIn = {i: set() for i in range(n)}
        self._costs = {}  # dictionary with key: tuple of vertices and value: cost of the edge

    def get_number_of_vertices(self):  # Returns the total number of vertices in the graph
        return len(self._dictOut)

    def parse_vertices(self):  # Returns an iterator over all vertices in the graph
        return iter(self._dictOut.keys())

    def edge_exists(self, x, y):  # Checks if an edge from x to y exists
        return y in self._dictOut[x]

    def get_in_degree(self, vertex):  # Returns the number of incoming edges for a given vertex
        return len(self._dictIn.get(vertex, set()))

    def get_out_degree(self, vertex):  # Returns the number of outgoing edges for a given verte
        return len(self._dictOut.get(vertex, set()))

    def add_vertex(self, vertex):  # Adds a new vertex to the graph
        if vertex not in self._dictOut:
            self._dictOut[vertex] = set()
            self._dictIn[vertex] = set()

    def remove_vertex(self, vertex):  # Removes a vertex and all its associated edges from the graph
        if vertex in self._dictOut:
            for target in self._dictOut[vertex]:
                self._dictIn[target].discard(vertex)
                self._costs.pop((vertex, target), None)
            for source in self._dictIn[vertex]:
                self._dictOut[source].discard(vertex)
                self._costs.pop((source, vertex), None)
            del self._dictOut[vertex]
            del self._dictIn[vertex]

    def add_edge(self, source, target, value=0):  # Adds a directed edge from source to target with a given cost
        if source in self._dictOut and target in self._dictOut:
            if not self.edge_exists(source, target):
                self._dictOut[source].add(target)
                self._dictIn[target].add(source)
                self._costs[(source, target)] = value

    def remove_edge(self, source, target):  # Removes an edge from source to target if it exists
        if self.edge_exists(source, target):
            self._dictOut[source].discard(target)
            self._dictIn[target].discard(source)
            self._costs.pop((source, target), None)

    def get_edge_value(self, source, target):  # Retrieves the cost of an edge if it exists, otherwise returns None
        return self._costs.get((source, target), None)

    def set_edge_value(self, source, target, value):  # Updates the cost of an existing edge
        if self.edge_exists(source, target):
            self._costs[(source, target)] = value

    def get_number_of_edges(self):  # Returns the total number of edges in the graph
        return len(self._costs)

    def parse_outbound_edges(self, vertex):  # Returns an iterator over all outbound edges from a given vertex
        return iter(self._dictOut.get(vertex, set()))

    def parse_inbound_edges(self, vertex):  # Returns an iterator over all inbound edges to a given vertex
        return iter(self._dictIn.get(vertex, set()))

    def copy(self):  # Creates a deep copy of the graph
        new_graph = DirectedGraph(len(self._dictOut))
        new_graph._dictOut = {v: set(self._dictOut[v]) for v in self._dictOut}
        new_graph._dictIn = {v: set(self._dictIn[v]) for v in self._dictIn}
        new_graph._costs = self._costs.copy()
        return new_graph

    @staticmethod
    def read_from_file(filename):  # Reads graph data from a file and creates a DirectedGraph instance
        with open(filename, 'r') as f:
            n, m = map(int, f.readline().split())
            graph = DirectedGraph(n)
            for _ in range(m):
                x, y, cost = map(int, f.readline().split())
                graph.add_edge(x, y, cost)
        return graph

    def write_to_file(self, filename):  # Writes the graph data to a file
        with open(filename, 'w') as f:
            f.write(f"{len(self._dictOut)} {len(self._costs)}\n")
            for (x, y), cost in self._costs.items():
                f.write(f"{x} {y} {cost}\n")

    @staticmethod
    def random_graph(n, m):  # Generates a random directed graph with n vertices and m edges
        graph = DirectedGraph(n)
        added_edges = 0
        while added_edges < m:
            x, y = random.randint(0, n - 1), random.randint(0, n - 1)
            if x != y and not graph.edge_exists(x, y):
                graph.add_edge(x, y, random.randint(1, 100))
                added_edges += 1
        return graph


def menu():
    graph = DirectedGraph(0)

    while True:
        print("\nGraph Operations Menu:")
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
        print("12. Display outbound edges")
        print("13. Display inbound edges")
        print("14. Get in-degree of a vertex")
        print("15. Get out-degree of a vertex")
        print("16. Generate 2 random graphs (From the statement)")
        print("17. Exit")

        choice = input("Choose an option: ")

        if choice == "1":
            filename = input("Enter filename: ")
            graph = DirectedGraph.read_from_file(filename)
            print("Graph loaded successfully.")

        elif choice == "2":
            filename = input("Enter filename to save: ")
            graph.write_to_file(filename)
            print("Graph saved successfully.")

        elif choice == "3":
            n = int(input("Enter number of vertices: "))
            m = int(input("Enter number of edges: "))
            graph = DirectedGraph.random_graph(n, m)
            print("Random graph created.")

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
            source = int(input("Enter source vertex: "))
            target = int(input("Enter target vertex: "))
            cost = int(input("Enter edge cost: "))
            graph.add_edge(source, target, cost)
            print(f"Edge {source} -> {target} with cost {cost} added.")

        elif choice == "8":
            source = int(input("Enter source vertex: "))
            target = int(input("Enter target vertex: "))
            graph.remove_edge(source, target)
            print(f"Edge {source} -> {target} removed.")

        elif choice == "9":
            source = int(input("Enter source vertex: "))
            target = int(input("Enter target vertex: "))
            cost = graph.get_edge_value(source, target)
            if cost is not None:
                print(f"Edge cost {source} -> {target}: {cost}")
            else:
                print("Edge does not exist.")

        elif choice == "10":
            source = int(input("Enter source vertex: "))
            target = int(input("Enter target vertex: "))
            cost = int(input("Enter new cost: "))
            graph.set_edge_value(source, target, cost)
            print(f"Edge {source} -> {target} updated to cost {cost}.")

        elif choice == "11":
            print("Your graph is:")
            for (x, y), cost in graph._costs.items():
                print(f"Edge {x} -> {y} with cost {cost}")

        elif choice == "12":
            vertex = int(input("Enter vertex: "))
            print(f"Outbound edges from {vertex}: {list(graph.parse_outbound_edges(vertex))}")

        elif choice == "13":
            vertex = int(input("Enter vertex: "))
            print(f"Inbound edges to {vertex}: {list(graph.parse_inbound_edges(vertex))}")

        elif choice == "14":
            vertex = int(input("Enter vertex: "))
            in_degree = graph.get_in_degree(vertex)
            print(f"In-degree of vertex {vertex}: {in_degree}")

        elif choice == "15":
            vertex = int(input("Enter vertex: "))
            out_degree = graph.get_out_degree(vertex)
            print(f"Out-degree of vertex {vertex}: {out_degree}")

        elif choice == "16":
            print(f"We are going to generate randomly 2 directed graphs with costs and save them in 2 files:")
            n1 = 7
            m1 = 20
            rgraph1 = DirectedGraph.random_graph(n1, m1)  # the statement says that is has to be a graph with 7 vertices and 20 edges
            if m1 > n1 * n1:
                raise ValueError(f"Too many edges! Max possible for {n1} vertices is {n1 * n1}.")
            else:
                rgraph1.write_to_file('random_graph1.txt')
                print(f"First graph generated and saved!")

            print(f"Let's go with the 2nd graph:")
            n2 = 6
            m2 = 40
            if m2 > n2 * n2:
                print(f"ERROR: Too many edges! Max possible for {n2} vertices is {n2 * n2}, and we have {m2} edges ")
            print("Returning to the menu...")

        elif choice == "17":
            print("Thank you, bye!.")
            break

        else:
            print("Invalid option, please try again.")


if __name__ == "__main__":
    menu()

