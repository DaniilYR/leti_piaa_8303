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
    cout << "Префикс-функция для образца  " << p << endl;
    for(int i = 0; i < pi.size(); i++)
        cout << pi[i] << ' ';
    cout << endl;
    int n = t.length();
    int m = p.length();
    int k = 0, l = 0;
    while(k < n){
        if(t[k] == p[l]){
            cout << "Совпадение:   " << t[k] << "==" << p[l] <<" index: "<< k << " " << l << endl;
            k++; l++;
            if(l == m){ans.push_back(k-l); cout << "Найдена подстрака\n-----------------------" << endl; }
        }
        else {
            if(l == 0){
                cout << "Несовпадение: " << t[k] << "!=" << p[l] <<" index: "<< k << " " << l << endl;
                k++;
            }
            else {
                cout << "Несовпадение: " << t[k] << "!=" << p[l] <<" index: "<< k << " " << l << endl;
                l = pi[l-1];
            }
        }
    }
    return ans;
}

void split(string t, string p, int k, vector<string> &str, vector<int> &ans_current, vector<int> &ans, vector<int> &pi){
    int len_parts, flag = 0;
    int k1;
    //---------------------------------------------------
    //определяем длинну каждой части
    if(t.length() % k){
        len_parts = int(t.length()/k)+1; //длинна части строки
        flag = 1;
        k1 = k-1;
    }
    else {
        k1 = k;
        len_parts = t.length()/k;
    }
    //---------------------------------------------------
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
        part.append(t, (len_parts*k1)-p.length()+1, 2*p.length()-2);
        int top = (len_parts*k1)-p.length()+1;
        cout << "Подстракас центром на месте разреза - " << part << endl;
        ans_current = KMP(part, p, pi);
        if(ans_current.size() > 0){
            ans_current[0] += top; //определяем номер символа начала подстроки в исходном тексте
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
        int k ;//= max_threads;
        if(max_threads == 1){
            cout << "Длина исходного текста недостаточна для деления строки" << endl;
            k = 1;
        }
        else{
            cout << "Введите число от 1 до "<< max_threads << endl;
            cin >> k;
            while(k < 1 or k > max_threads){
                cout << "Введите число от 1 до "<< max_threads << endl;
                cin >> k;

            }
        }
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
                cout << "------------------------\nЧасть исходного текста  " << str[i] << endl;
                ans_current = KMP(str[i], p, pi);
                if(ans_current.size() > 0){
                    for(int j = 0; j < ans_current.size(); j++)
                        ans_current[j] += (len_parts*i); // определяем номер символа начала образца в исходном тексте
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
            vector<int> pi = prefix_function(b);
            cout << "Префикс-функция для строки 2" << endl;
            for(int i = 0; i < pi.size(); i++)
                cout << pi[i] << ' ';
            cout << endl;
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
                    cout << "Совпадение:   " << a[it_a] << "==" << b[it_b] <<" index: "<< it_a << " " << it_b << endl;
                    it_a++;
                    it_b++;
                }
                if(it_a == al){
                    it_a = 0;
                    cikle++;
                }
                if(it_b == al){
                    cout << "Цикл: ";
                    cout << it_a << endl;
                    return 0;
                }
                else{
                    if(a[it_a] != b[it_b]){
                        cout << "Несовпадение: " << a[it_a] << "!=" << b[it_b] <<" index: "<< it_a << " " << it_b << endl;
                        if(it_b == 0)
                            it_a++;
                        else {
                            it_b = pi[it_b-1];
                        }
                    }
                }
                if(cikle > 1){
                    cout << -1 << endl;
                    return 0;
                }

            }
        }
    }
    return 0;
}
