#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <algorithm>

using namespace std;

vector<int> prefix_function (string s) {
    int n = (int) s.length();
    vector<int> pi(n);
    for (int i=1; i<n; ++i) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j])
            j = pi[j-1];
        if (s[i] == s[j])  ++j;
        pi[i] = j;
    }
    return pi;
}

vector<int> KMP(string t, string p, vector<int> &pi){
    vector<int> ans;
    int n = t.length();
    int m = p.length();
    if(n == m){
        if(t == p){
            ans.push_back(0);
            return ans;
        }
        else {
            return ans;
        }
    }
    int k = 0, l = 0;
    while(k < n){
        if(t[k] == p[l]){
            k++; l++;
            if(l == m){ans.push_back(k-l);}
        }
        else {
            if(l == 0){
                k++;
            }
            else {
                l = pi[l-1];
            }
        }
    }
    return ans;
}

void split(string t, string p, int k, vector<string> &str, vector<int> &ans_current, vector<int> &ans, vector<int> &pi){
    int len_parts, flag = 0;
    //---------------------------------------------------
    //определяем длинну каждой части
    if(t.length() % k){
        len_parts = int(t.length()/k)+1; //длинна части строки
        flag = 1;
    }
    else {
        len_parts = t.length()/k;
    }
    //---------------------------------------------------
    int k1 = k - 1;
    int begin = 0;
    string part = "";
    //цикл для получения массива подстрок из текста
    while(k1 > 0){
        part = "";
        part.append(t, begin, len_parts);
        str.push_back(part);
        begin += len_parts;
        k1--;
    }
    if(flag){
        part = "";
        part.append(t, begin, (t.length()-(len_parts*(k-1))));
        str.push_back(part);
    }

    //цикл для получения и проверки подстрок на стыках на каждом стыке проверяется 2 стрки
    k1 = 1;
    while(k1 < k){
        part = "";
        part.append(t, (len_parts*k1)-1, p.length());
        ans_current = KMP(part,p,pi);
        if(ans_current.size() > 0){
            cout << "----------------------------------" << endl;
            cout << "{стык лево} Текущейя часть текста" << endl;
            cout << part << endl;
            cout << "{стык лево} Номер символа начала образца в данной части исходного текста" << endl;
            cout << ans_current[0] << endl;
            cout << "{стык лево}Номер символа начала образца в исходном тексте" << endl;
            ans_current[0] += (len_parts*k1-1); //определяем номер символа начала подстроки в исходном тексте
            cout << ans_current[0] << endl;
            ans.insert(ans.end(), ans_current.begin(), ans_current.end());
        }
        part = "";
        part.append(t, (len_parts*k1)-p.length()+1, p.length());
        ans_current = KMP(part,p,pi);
        if(ans_current.size() > 0){
            cout << "----------------------------------" << endl;
            cout << "{стык право} Текущейя часть текста" << endl;
            cout << part << endl;
            cout << "{стык право} Номер символа начала образца в данной части исходного текста" << endl;
            cout << ans_current[0] << endl;
            cout << "{стык право}Номер символа начала образца в исходном тексте" << endl;
            ans_current[0] += (len_parts*k1-p.length()+1); //определяем номер символа начала подстроки в исходном тексте
            cout << ans_current[0] << endl;
            ans.insert(ans.end(), ans_current.begin(), ans_current.end());
        }
        k1++;
    }
}


int main()
{
    cout << "\tСправка\nЧтобы запустить программу введите номер задачи или ее название.\n"
            "\tНайдите все вхождения обзарца в тексте:\nНомер задачи - 1\nНазвание - KMP или kmp\n"
            "\tОпределить, является ли стока 1 циклическим сдвигом строки 2:\nНомер задачи - 2\nНазвание - Rotation или rotation"<< endl;
    cout << endl;
    string task;
    cout << "Введите номер задачи или название алгоритма" << endl;
    getline(cin, task);
    if(task == "KMP" or task == "kmp" or task == "1"){
        string p,t;
        cout << "Введите текст" << endl;
        getline(cin, t);
        cout << "Введите образец (искомую подстроку)" << endl;
        getline(cin, p);
        int max_threads = sizeof(thread); // определяем максимально возможное число потоков
        //---------------------------------------------------
        // определяем на сколько частей можно раделить строку
        double alpha = (double)t.length()/(double)p.length();
        max_threads = min(max_threads, int(alpha)-1);
        if(max_threads == 0)
            max_threads = 1;
        int k = max_threads;
        //---------------------------------------------------
        vector<int> pi = prefix_function(p);
        vector<int> ans, ans_current;
        vector<string> str;
        cout << "--------------------------------------------" << endl;
        cout << "Строка будет разделена на " << k << " частей" << endl;
        if(k == 1)
            ans = KMP(t, p, pi);
        else {
            //---------------------------------------------------
            // определяем длинну каждой части
            int len_parts;
            if(t.length() % k){
                len_parts = int(t.length()/k)+1; //длинна части строки
            }
            else {
                len_parts = t.length()/k;
            }
            cout << "Максимальная длинна части исходного текста - " << len_parts << endl;
            cout << "--------------------------------------------" << endl;
            cout << endl;
            split(t, p, k, str, ans_current, ans, pi);
            //---------------------------------------------------
            //заполняем исходный массив ответов
            for(int i = 0; i< str.size(); i++){
                ans_current = KMP(str[i], p, pi);
                if(ans_current.size() > 0){
                    cout << "-----------------------------" << endl;
                    cout << "Текущейя часть текста" << endl;
                    cout << str[i] << endl;
                    cout << "Номер символа начала образца в данной части исходного текста" << endl;
                    for(int j = 0; j < ans_current.size(); j++)
                        cout << ans_current[j] << ' ';
                    cout << endl;
                    for(int j = 0; j < ans_current.size(); j++)
                        ans_current[j] += (len_parts*i); // определяем номер символа начала образца в исходном тексте
                    cout << "Номер символа начала образца в исходном тексте" << endl;
                    for(int j = 0; j < ans_current.size(); j++)
                        cout << ans_current[j] << ' ';
                    cout << endl;
                    ans.insert(ans.end(), ans_current.begin(), ans_current.end());
                }
            }
        }
        // Ввывод ответа
        if(ans.size() == 0)
            cout << "-1";
        else {
            sort(ans.begin(), ans.end()); //сортируем массив ответов для читабельности
            for(int i = 0; i < ans.size(); i++){
                if(i == ans.size()-1)
                    cout << ans[i];
                else {
                    cout << ans[i] << ',';
                }
            }
            cout << endl;
        }
    }
    else{
        if(task == "Rotation" or task == "rotation" or task == "2"){
            string a,b;
            cout << "Введите строки 1 и 2" << endl;
            cin >> a >> b;
            if(b.length() != a.length())
            {
                cout << "-1" << endl;
                return 0;
            }
            if(a == b){
                cout << 0 << endl;
                return 0;
            }
            int it_a = 0, it_b = 0;
            int cikle = 0;
            int al = a.length();
            while(true){
                if(a[it_a] == b[it_b]){
                    it_a++;
                    it_b++;
                }
                if(it_a == al && it_b != al){
                    it_a = 0;
                    cikle++;
                }
                if(it_b == al){
                    cout << it_a << endl;
                    return 0;
                }
                if(a[it_a] != b[it_b]){
                    if(it_b == 0)
                        it_a++;
                    else {
                        it_b = 0;
                    }
                }
                if(cikle >1){
                    cout << -1 << endl;
                    return 0;
                }
            }
        }
    }
    return 0;
}
