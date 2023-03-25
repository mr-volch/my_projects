import sys
sys.setrecursionlimit(100500)

def search_deep(graph, vertex, visited, comp):
    visited[vertex] = True
    for neighbour in graph[vertex]:
        if not visited[neighbour]:
            comp.append(neighbour)
            search_deep(graph, neighbour, visited, comp)

def main_func():
    vertex_amount, nodes_amount = map(int, input().split())
    graph = {i: set() for i in range(1, vertex_amount + 1)}
    visited = {i : False for i in range(1, vertex_amount + 1)}
    comp = [1]
    
    for _ in range(nodes_amount):
        first, second = map(int, input().split())
        
        if first != second:
            graph[first].add(second)
            graph[second].add(first)

    search_deep(graph, 1, visited, comp)
    print(len(comp))
    print(*sorted(comp))

   
main_func()
