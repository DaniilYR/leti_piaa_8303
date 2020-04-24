#include <iostream>
#include <vector>
#include <map>
#include <queue>

using namespace std;

map<char, vector<pair<char, double>>> way;

string search_ASTAR(char start, char finish1, char finish2){
    priority_queue<pair<double, char>> queue; //очередь с приоритетом
    map<char, pair<string, double>> way_to_vertex; // путь до вершины
    way_to_vertex[start].first = start; // путь до start есть сама вершина start
    queue.push(make_pair(abs(finish1-start), start)); // добавляем start в очередь
    while(!queue.empty()){ // пока очередь не пуста
        priority_queue<pair<double, char>> queue_copy(queue);
        cout << "Очередь" << endl;
        while(!queue_copy.empty()){
            cout << queue_copy.top().second << "(" << queue_copy.top().first  << ") ";
            queue_copy.pop();
        }
        cout << endl;
        pair<char, double> current_vertex;
        // берем очередной элемент очереди
        current_vertex.first = queue.top().second;
        current_vertex.second = queue.top().first;
        queue.pop();
        cout << "Обрабатываемая вершина: " << current_vertex.first << "(" << current_vertex.second  << ")" << endl;
        if(current_vertex.first == finish1 || current_vertex.first == finish2)
            cout << current_vertex.first << " - конечня вершина" << endl;
        else{
            cout << "Смежные с ней вершины:" << endl;
            for(auto& i: way[current_vertex.first])
                cout << i.first << ' ';
            cout << endl;
        }
        if(current_vertex.first == finish1){ // если очередная вершина это finish1, то выводим ответ
            return way_to_vertex[current_vertex.first].first;
        }
        if(current_vertex.first == finish2){ // если очередная вершина это finish2, то выводим ответ
            return way_to_vertex[current_vertex.first].first;
        }

        for(auto& i: way[current_vertex.first]){ // проходим по смежным вершинам current_vertex
            double e1 = abs(finish1 - i.first); //эвристика для очередной вершины
            double e2 = abs(finish2 - i.first); //эвристика для очередной вершины
            if(way_to_vertex[i.first].second > way_to_vertex[current_vertex.first].second + i.second || way_to_vertex[i.first].second == 0){
                way_to_vertex[i.first].second = way_to_vertex[current_vertex.first].second + i.second;
                way_to_vertex[i.first].first = way_to_vertex[current_vertex.first].first + i.first;
            }
            queue.push(make_pair(-(e1 + way_to_vertex[i.first].second), i.first));
            queue.push(make_pair(-(e2 + way_to_vertex[i.first].second), i.first));
        }
    }
    return "No path";
}

int main()
{
    char start, finish1, finish2, from, to;
    double weight;
    cin >> start >> finish1 >> finish2;
    while(cin >> from && from != '!'){
        cin >> to;
        cin >> weight;
        way[from].push_back(make_pair(to, weight));

    }

    cout << "Алгоритм АSTAR" << endl;
    cout << search_ASTAR(start, finish1, finish2) << endl;
    return 0;
}
