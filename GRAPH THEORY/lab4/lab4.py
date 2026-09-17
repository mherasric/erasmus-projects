import random
import heapq
"""
We use heapq to implement a priority queue that always gives us the vertex with the smallest current distance 
efficiently. This is essential for Dijkstra's algorithm to run fast, especially on large graphs, because it reduces the 
time to find the minimum node from O(n) to O(log n).
"""

class DirectedGraph:
    """A directed graph represented as two dictionaries: one for outbound edges and one for inbound edges.
    Vertices are integers from 0 to n-1. Edges are identified by (source, target) tuples with associated integer values.
    """

    def __init__(self, n=0):
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
        else:
            print(f"The vertex {vertex} it already exists.")

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

            isolated = [v for v in self._dictOut if not self._dictOut[v] and not self._dictIn[v]]
            for v in isolated:
                f.write(f"{v} \n")

    @staticmethod
    def random_graph(n, m):  # Generates a random directed graph with n vertices and m edges
        if m > n * n:  # For a directed graph with self-loops, maximum number of edges is n^2.
            print(f"Error: The maximum number of edges for {n} vertices is {n * n}. It can't be created the graph.")
            return None
        graph = DirectedGraph(n)
        added_edges = 0
        while added_edges < m:
            x, y = random.randint(0, n - 1), random.randint(0, n - 1)
            if x != y and not graph.edge_exists(x, y):
                graph.add_edge(x, y, random.randint(1, 100))
                added_edges += 1
        return graph

    def dijkstra(self, start, end):
        """
        Finds the lowest cost walk from start to end using Dijkstra's algorithm.
        Returns (total_cost, path_list) or (float('inf'), []) if no path exists.
        """

        # Check if the start and end vertices exist
        if start not in self._dictOut or end not in self._dictOut:
            return float('inf'), [] #if they don't exist, we can't calculate the path so we return infinite and an empty list

        # Initialize distances to all vertices as infinity and previous nodes as None
        vertices = list(self.parse_vertices()) #we get all the vertices of the graph
        dist = {v: float('inf') for v in vertices} #dictionary with minimun distances, initialize with inf
        prev = {v: None for v in vertices} #dictionary with the precessors, initialize with None. Here we keep the previous node in the shortest path to the vertex
        dist[start] = 0  # Distance to the node start to itself is zero

        # Priority queue to select the node with the smallest distance
        # Priority queue holds tuples of (distance, vertex).
        # It ensures the vertex with the current smallest distance is processed first.
        pq = [(0, start)]

        while pq: #while there are nodes in the heapq
            current_dist, u = heapq.heappop(pq) #we take out the node u with the least actual distance

            # Skip if we found a shorter path to u
            if current_dist > dist[u]:
                continue

            # Iterate over neighbors v (adjacents) to u (all of the nodes which u has a directed edge)
            for v in self.parse_outbound_edges(u):
                cost = self.get_edge_value(u, v) #we get the cost of the edge between u and v
                if cost is not None and dist[u] + cost < dist[v]: #If there exists a valid edge and a new path to v through u is shorter than the previous one
                    dist[v] = dist[u] + cost #we actualize the better distance known to v
                    prev[v] = u
                    heapq.heappush(pq, (dist[v], v))

        # Reconstruct path from end to start using the prev dictionary
        if dist[end] == float('inf'):
            return float('inf'), [] #if we don't find any path to end, it returns infinite and empty list

        path = [] #we reconstruct the shortest path from end backwards, using the dictionary prev
        v = end
        while v is not None:
            path.append(v) #Each node is appended while tracing backwards from the end to the start using the prev dictionary, and then the list is reversed to obtain the correct order from start to end.
            v = prev[v]
        path.reverse()

        return dist[end], path #finally it return the total minimal distance and the list of nodes in the shortest path

    #--------------LAB4-------------
    def topological_sort(self):
        visited = set()  # Set to keep track of visited nodes
        stack = []  # Stack to store the topological order
        on_path = set()  # Set of nodes currently in the recursion stack (to detect cycles), DFS
        has_cycle = False  # Flag to indicate whether a cycle was found

        def dfs(vertex):
            nonlocal has_cycle  # We modify the outer variable 'has_cycle' inside this function dfs()
            visited.add(vertex)  # Mark the current node as visited
            on_path.add(vertex)  # Add it to the current DFS path

            for neighbor in self._dictOut[vertex]:  # Iterate through all outbound neighbors, recorremos vecinos, las aristas salientes vamos
                if neighbor not in visited:
                    dfs(neighbor)  # Visit unvisited neighbors recursively
                    if has_cycle:  # If a cycle was detected deeper, stop
                        return
                elif neighbor in on_path:
                    has_cycle = True  # A back edge was found, meaning there’s a cycle, so it's not a DAG
                    return

            on_path.remove(vertex)  # Remove from current DFS path, because we'd finish processing this path
            stack.append(vertex)  # Add node to the stack after exploring all its neighbors

        for vertex in self.parse_vertices():  # Go through all vertices
            if vertex not in visited:
                dfs(vertex)  # Run DFS from unvisited nodes
            if has_cycle:
                return False, []  # If a cycle exists, return False – not a DAG

        stack.reverse()  # Reverse the stack to get the topological order
        return True, stack  # Return that the graph is a DAG and the topo order

    def highest_cost_path_dag(self, start, end):
        is_dag, topo_order = self.topological_sort()  # First check if the graph is a DAG
        if not is_dag:
            return float('-inf'), []  # If it's not a DAG, longest path is undefined, we can't continue

        # Initialize distances: -inf for all except the start node
        dist = {v: float('-inf') for v in self.parse_vertices()}
        prev = {v: None for v in self.parse_vertices()}  # Dictionary to track path reconstruction
        dist[start] = 0  # Distance to start node is zero

        # Traverse nodes in topological order
        for u in topo_order:
            for v in self._dictOut[u]:  # Go through each neighbor (nodo alcanzable)
                cost = self.get_edge_value(u, v)  # Get the cost of the edge (u->v)
                if dist[u] + cost > dist[v]:  # if the new path is better we actualize the distance
                    dist[v] = dist[u] + cost
                    prev[v] = u  # Track the path, we save the previous one

        # If the distance to end is still -inf, there's no path
        if dist[end] == float('-inf'):
            return float('-inf'), []

        path = [] # Reconstruct path from end to start
        v = end
        while v is not None:
            path.append(v)
            v = prev[v]
        path.reverse()  # Reverse to get the correct order from start to end

        return dist[end], path  # Return the maximum cost and the corresponding path


def dfs_connected_components_as_graphs(graph):
    """
    Finds the connected components of the graph using depth-first search (DFS).
    DFS is used here to explore all reachable nodes from each unvisited node.
    Treats the graph as undirected: each edge is considered bidirectional, we take both inbound and outbound edges into
    account.

    Parameters:
    - graph: DirectedGraph object

    Returns:
    - List of DirectedGraph objects, each representing a connected component
    """
    visited = set()
    components = []

    def dfs(v, component_graph):
        visited.add(v)
        component_graph.add_vertex(v)
        neighbors = set(graph.parse_outbound_edges(v)) | set(graph.parse_inbound_edges(v))
        for neighbor in neighbors:
            component_graph.add_vertex(neighbor)
            component_graph.add_edge(v, neighbor)
            if neighbor not in visited:
                dfs(neighbor, component_graph)

    for vertex in graph.parse_vertices():
        if vertex not in visited:
            new_component = DirectedGraph()
            dfs(vertex, new_component)
            components.append(new_component)

    return components

def test_on_large_graphs():
    filenames = ["graph1k.txt", "graph10k.txt", "graph100k.txt"]
    for filename in filenames:
        try:
            graph = DirectedGraph.read_from_file(filename)
            print(f"Processing {filename}...")
            cost1, path1 = graph.dijkstra(1, 100)
            cost2, path2 = graph.dijkstra(100, 1)
            output_filename = f"results_{filename}"
            with open(output_filename, 'w') as f:
                f.write(f"Shortest path from 1 to 100 in {filename}:\n")
                f.write(f"Cost: {cost1}\n")
                f.write(f"Path: {' -> '.join(map(str, path1)) if path1 else 'No path'}\n\n")
                f.write(f"Shortest path from 100 to 1 in {filename}:\n")
                f.write(f"Cost: {cost2}\n")
                f.write(f"Path: {' -> '.join(map(str, path2)) if path2 else 'No path'}\n")
            print(f"Results saved in {output_filename}")
        except FileNotFoundError:
            print(f"File {filename} not found. Skipping.")


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
        print("17. Find connected components in the graph using DFS")
        print("18. Find minimum cost path using Dijkstra")
        print("19. Run Dijkstra tests on large graphs and save results")
        print("20. Check if graph is DAG and get topological sort")
        print("21. Find highest cost path (DAG only)")
        print("22. Exit")

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
            else:
                rgraph2 = DirectedGraph.random_graph(n2, m2)
                rgraph2.write_to_file('random_graph2.txt')
                print(f"Second graph generated and saved!")
            print("Returning to the menu...")

        elif choice == "17":
            print("Finding connected components...")
            components = dfs_connected_components_as_graphs(graph)
            print(f"\nConnected Components ({len(components)}):")

            for i, comp in enumerate(components):
                print(f"Component {i + 1}: {list(comp.parse_vertices())}")
                for v in comp.parse_vertices():
                    print(f" Outbound from {v}: {list(comp.parse_outbound_edges(v))}")

        elif choice == "18":
            source = int(input("Enter start vertex: "))
            target = int(input("Enter target vertex: "))
            cost, path = graph.dijkstra(source, target)
            if path:
                print(f"Minimum cost path from {source} to {target}: {' -> '.join(map(str, path))}")
                print(f"Total cost: {cost}")
            else:
                print(f"No path exists from {source} to {target}.")

        elif choice == "19":
            test_on_large_graphs()

        elif choice == "20":
            is_dag, order = graph.topological_sort()
            if is_dag:
                print("The graph is a DAG.")
                print(f"Topological order: {order}")
            else:
                print("The graph has cycles and is not a DAG.")

        elif choice == "21":
            is_dag, topo_order = graph.topological_sort()
            if not is_dag:
                print("The graph is not a DAG. Cannot compute longest path.")
            else:
                start = int(input("Enter the start vertex: "))
                end = int(input("Enter the end vertex: "))
                cost, path = graph.highest_cost_path_dag(start, end)
                if cost == float('-inf'):
                    print("No path found or graph is not a DAG.")
                else:
                    print(f"Highest cost path from {start} to {end}: {' -> '.join(map(str, path))}")
                    print(f"Total cost: {cost}")

        elif choice == "22":
            print("Thank you, bye!.")
            break

        else:
            print("Invalid option, please try again.")


if __name__ == "__main__":
    menu()
