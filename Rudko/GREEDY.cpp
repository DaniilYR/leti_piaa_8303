#include <iostream>
#include <vector>
#include <map>
#include <queue>

using namespace std;

map<char, vector<pair<char, double>>> way;
map<char, int> check;
string ans = "";

string search_greed_way(char start, char finish){
    char current = start; // текущая вершина - start
    ans += current;
    while(current != finish){ // пока не дошли но finish
        check[current] = 1;
        double min = 1000.0;
        char next;
        int num = -1;
        bool f = false;
        for(auto& i: way[current]){ // выбираем ребро с минимальным весом, которое выходит из текущей вершины
            if(i.second < min && !check[i.first]){
                next = i.first;
                min = i.second;
                num++;
                f = true;
            }
        }
        if(f){
            if(!check[next]){ // если вершина не помечена, идем в нее
                current = next;
                ans += current;
            }
            else {
                current = current;
            }
        }else{ // если из вершины нет путей, удаляем ее из ответа
            ans.pop_back();
            current = ans[ans.size() - 1];
        }


    }
    return ans;
}

int main()
{
    char start, finish, from, to;
    double weight;
    cin >> start >> finish;
    while(cin >> from && from != '!'){
        cin >> to;
        cin >> weight;
        way[from].push_back(make_pair(to, weight));
        check[from] = 0;
        check[to] = 0;
    }

    cout << "Жадный алгоритм." << endl;
    cout << search_greed_way(start, finish) << endl;
    return 0;
}
