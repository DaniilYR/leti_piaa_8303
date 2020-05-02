#include <iostream>
#include <algorithm>
#include <map>
#include <deque>
#include <queue>
#include <climits>

using namespace std;
typedef map<char, map<char, int>> Graph; //Граф для алгоритма


bool bfs(Graph &graph, char source, char sink, map<char, char>& path){
    cout << endl;
    map<char,bool> visited; //Посещенные вершины
    queue<char> q_vertex;  //Очередь вершин
    q_vertex.push(source);
    visited[source] = true;
    while (!q_vertex.empty())  //Пока очередь не пустая
    {
        char vertex = q_vertex.front();
        q_vertex.pop();
        cout << "Текущая вешина " << vertex << ", соседи: " << endl;
        for (auto i : graph[vertex]){                      //Просматриваются соседи и кладутся в очередь
            if (i.second > 0 && !(visited[i.first])) {
                q_vertex.push(i.first);
                visited[i.first] = true;
                path[i.first] = vertex;
                cout << "\t" << i.first << " пропускная способность ребра = " << i.second << endl;
                if (i.first == sink) {
                    q_vertex = *(new queue<char>());
                    break;
                }
            }

        }
    }

    return visited[sink]; //Была ли достигнута финиш
}

void printResult(Graph &graph, Graph flow_graph, int max_flow){           //Функция печати результата
    cout << "----------------------------------------------------------" << endl;
    int flow;
    cout <<"Максимальный поток = "<< max_flow << endl; //Печать максимального потока
    cout << "\nПоток, протекающий через ребра:" << endl;
    for (auto vertex: graph)
        for (auto i: graph[vertex.first]) {
            if (i.second - flow_graph[vertex.first][i.first] < 0)
                flow = 0;
            else
                flow = i.second - flow_graph[vertex.first][i.first];

            cout << vertex.first << " " << i.first << " " << flow << endl;   // Печать потока через ребро
        }
}

void FordFulkerson(Graph &graph, char source, char sink){
    Graph flow_graph = graph;                                //Граф с потоками
    char from, to;
    map<char,char> path;                               //Пары, составляющие путь
    int max_flow = 0;
    while (bfs(flow_graph, source, sink, path))        //Пока возможен поиск в ширину
    {
        int current_flow = INT_MAX;
        for (to = sink; to != source; to = path[to])           //Восстанавливается путь от финиша к началу
        {
            from = path[to];
            current_flow = min(current_flow, flow_graph[from][to]);  //Находится поток пути
        }
        for (to = sink; to != source; to = path[to]) //Восстанавливается путь от финиша к началу
        {
            from = path[to];
            flow_graph[from][to] -= current_flow;  //Изменяется граф с потоком
            flow_graph[to][from] += current_flow;
        }

        max_flow += current_flow;  //Изменяется число максимального потока
    }

    printResult(graph, flow_graph, max_flow);
}

int main() {

    Graph graph;
    char source, sink;
    int n; //число ребер

    cin >> n >> source >> sink;

    for (int i = 0; i < n; i++) { //Считывание вершин графа
        char from, to;
        int capacity;
        cin >> from >> to >> capacity;
        graph[from][to] = capacity;
    }

    FordFulkerson(graph, source, sink); //Запуск алгоритма Форда-Фалкерсона

    return 0;
}
