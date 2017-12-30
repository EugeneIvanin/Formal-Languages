/* Иванин Евгений, 697
 * Даны регулярное выражение и слово u,
 * состоящее из символов алфавита {a, b, c}.
 * Найти длину самого длинного подслова u,
 * являющегося также подсловом некоторого слова в L.
 */

/*Тесты:
 * ab+c.aba.*.bac.+.+*
 * babc
 * Вывод: 3
 * acb..bab.c.*.ab.ba.+.+*a.
 * abbaa
 * Вывод: 5
 */

#include <iostream>
#include <stack>
#include <vector>
#include <cstdlib>

//для нумерации вершин автомата
int numerator = 1;

using namespace std;

//узел автомата
struct Node {
    //номер вершины в автомате
    int num;

    //в какие вершины можно прийти по а
    vector<Node> a;

    //в какие вершины можно прийти по b
    vector<Node> b;

    //в какие вершины можно прийти по с
    vector<Node> c;

    //в какие вершины можно прийти по пустому ребру
    vector<Node> e;

    //является ли вершина терминальной
    bool terminal;
};

//НКА
class Automat {
public:

    Automat() {}

    //конструктор, строящий автомат с одним ребром _sym
    Automat(char _sym);

    //конструктор копирования
    Automat(const Automat &x);

    //построение матрицы смежности автомата
    void matrix_build();

    //распознается ли строка автоматом
    bool search(string s);

    //вывод автомата
    void print();

    //вектор вершин автомата
    vector<Node> aut;

private:
    //вспомогательная функция для поиска строки в автомате
    bool dfs(string str, int temp, int count_e);

    //матрицы смежности автомата
    vector<vector<char> > mat;

};

//Конструктор копирования автомата
Automat::Automat(const Automat &x) {
    for (int i = 0; i < x.aut.size(); i++) {
        aut.push_back(x.aut[i]);
    }
}

//Создание автомата, распознающего один символ (база построения)
Automat::Automat(char _sym) {
    //Инициализация стартовой вершины
    Node start;
    start.terminal = false;
    start.num = numerator;
    numerator++;

    switch (_sym) {
        case 'a': {
            Node vertex;
            vertex.terminal = true;
            vertex.num = numerator;
            numerator++;
            start.a.push_back(vertex);
            aut.push_back(start);
            aut.push_back(vertex);
            break;
        }
        case 'b': {
            Node vertex;
            vertex.terminal = true;
            vertex.num = numerator;
            numerator++;
            start.b.push_back(vertex);
            aut.push_back(start);
            aut.push_back(vertex);
            break;
        }
        case 'c': {
            Node vertex;
            vertex.terminal = true;
            vertex.num = numerator;
            numerator++;
            start.c.push_back(vertex);
            aut.push_back(start);
            aut.push_back(vertex);
            break;
        }
        case '1': {
            start.terminal = true;
            aut.push_back(start);
            break;
        }
    }
}

//вывод автомата (для решения задачи не нужна)
void Automat::print() {
    for (int i = 0; i < aut.size(); i++) {
        cout << "Node_num " << aut[i].num << endl;
        cout << "Terminal? " << aut[i].terminal << endl;
        cout << "Ways on a " << endl;
        for (int j = 0; j < aut[i].a.size(); j++) {
            cout << aut[i].a[j].num << " ";
        }
        cout << endl;
        cout << "Ways on b " << endl;
        for (int j = 0; j < aut[i].b.size(); j++) {
            cout << aut[i].b[j].num << " ";
        }
        cout << endl;
        cout << "Ways on c " << endl;
        for (int j = 0; j < aut[i].c.size(); j++) {
            cout << aut[i].c[j].num << " ";
        }
        cout << endl;
        cout << "Ways on e " << endl;
        for (int j = 0; j < aut[i].e.size(); j++) {
            cout << aut[i].e[j].num << " ";
        }
        cout << endl;
    }
}

//рекурсивный обход по автомату с целью считать полученную строку
bool Automat::dfs(string str, int temp, int count_e) {
    if (str == "") {
        return true;
    }
    switch (str[0]) {
        case 'a': {
            for (int i = 0; i < numerator; i++) {
                if (mat[temp][i] == 'a') {
                    count_e = 0;
                    if (dfs(str.substr(1, str.size() - 1), i, count_e)) {
                        return true;
                    }
                }
                if (mat[temp][i] == 'e') {
                    count_e++;
                    if (count_e < aut.size()) {
                        if (dfs(str, i, count_e)) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        case 'b': {

            for (int i = 0; i < numerator; i++) {
                if (mat[temp][i] == 'b') {
                    count_e = 0;
                    if (dfs(str.substr(1, str.size() - 1), i, count_e)) {
                        return true;
                    }
                }
                if (mat[temp][i] == 'e') {
                    if (count_e < aut.size()) {
                        if (dfs(str, i, count_e)) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        case 'c': {
            for (int i = 0; i < numerator; i++) {
                if (mat[temp][i] == 'c') {
                    count_e = 0;
                    if (dfs(str.substr(1, str.size() - 1), i, count_e)) {
                        return true;
                    }
                }
                if (mat[temp][i] == 'e') {
                    if (count_e < aut.size()) {
                        if (dfs(str, i, count_e)) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
    }
}

//построение матрицы смежности автомата (по ней удобнее делать обход)
void Automat::matrix_build() {
    mat.resize(numerator);
    for (int i = 0; i < numerator; i++) {
        mat[i].resize(numerator);
    }
    for (int i = 0; i < aut.size(); i++) {
        for (int j = 0; j < aut[i].e.size(); j++) {
            mat[aut[i].num][aut[i].e[j].num] = 'e';
        }
        for (int j = 0; j < aut[i].a.size(); j++) {
            mat[aut[i].num][aut[i].a[j].num] = 'a';
        }
        for (int j = 0; j < aut[i].b.size(); j++) {
            mat[aut[i].num][aut[i].b[j].num] = 'b';
        }
        for (int j = 0; j < aut[i].c.size(); j++) {
            mat[aut[i].num][aut[i].c[j].num] = 'c';
        }
    }
}

bool Automat::search(string s) {
    //пробуем начать обход с каждой вершины,
    //тк нужны подслова языка
    for (int i = 0; i < aut.size(); i++) {
        if (dfs(s, aut[i].num, 0)) {
            return true;
        }
    }
    return false;
}

class Regular_Expression {
public:
    //инициализирующий конструктор
    Regular_Expression(vector<char> &_regex);

    //функция создания автомата по регулярке
    Automat transform_to_automat();

private:
    //регулярное выражение
    vector<char> regex;

    //длина регулярного выражения
    int regex_len;
};

Regular_Expression::Regular_Expression(vector<char> &_regex) {
    regex = _regex;
    regex_len = regex.size();
}

Automat Regular_Expression::transform_to_automat() {
    stack<Automat> pack;
    for (int i = 0; i < regex_len; i++) {
        char sym = regex[i];
        switch (sym) {
            case 'a': {
                Automat obj(sym);
                pack.push(obj);
                break;
            }
            case 'b': {
                Automat obj(sym);
                pack.push(obj);
                break;
            }
            case 'c': {
                Automat obj(sym);
                pack.push(obj);
                break;
            }
            case '1': {
                Automat obj(sym);
                pack.push(obj);
                break;
            }
            case '+': {
                if (pack.size() < 2) {
                    cerr << "ERROR: uncorrected regular expression" << endl;
                    exit(0);
                }
                Automat first(pack.top());
                pack.pop();
                Automat second(pack.top());
                pack.pop();
                Automat merge;
                Node start;
                start.num = numerator;
                numerator++;
                start.terminal = false;
                start.e.push_back(first.aut[0]);
                start.e.push_back(second.aut[0]);
                merge.aut.push_back(start);
                for (int i = 0; i < first.aut.size(); i++) {
                    merge.aut.push_back(first.aut[i]);
                }
                for (int i = 0; i < second.aut.size(); i++) {
                    merge.aut.push_back(second.aut[i]);
                }
                pack.push(merge);
                break;
            }
            case '*': {
                if (pack.size() < 1) {
                    cerr << "ERROR: uncorrected regular expression" << endl;
                    exit(0);
                }
                Automat first(pack.top());
                pack.pop();
                Automat merge;
                Node start;
                start.num = numerator;
                numerator++;
                start.terminal = true;
                start.e.push_back(first.aut[0]);
                merge.aut.push_back(start);
                for (int i = 0; i < first.aut.size(); i++) {
                    if (first.aut[i].terminal) {
                        first.aut[i].terminal = false;
                        first.aut[i].e.push_back(merge.aut[0]);
                    }
                    merge.aut.push_back(first.aut[i]);
                }
                pack.push(merge);
                break;
            }
            case '.': {
                if (pack.size() < 2) {
                    cerr << "ERROR: uncorrected regular expression" << endl;
                    exit(0);
                }
                Automat first(pack.top());
                pack.pop();
                Automat second(pack.top());
                pack.pop();
                //порядок second first
                Automat merge;
                for (int i = 0; i < second.aut.size(); i++) {
                    if (second.aut[i].terminal) {
                        second.aut[i].terminal = false;
                        second.aut[i].e.push_back(first.aut[0]);
                    }
                    merge.aut.push_back(second.aut[i]);
                }
                for (int i = 0; i < first.aut.size(); i++) {
                    merge.aut.push_back(first.aut[i]);
                }
                pack.push(merge);
                break;
            }
            default: {
                cerr << "Error: invalid character in regular expression." << endl;
            }
        }
    }
    return pack.top();
}

int main() {
    //регулярное выражение
    string _regex;
    cin >> _regex;
    vector<char> regex;
    for (int i = 0; i < _regex.size(); i++) {
        if (regex.size() != 0) {
            if (regex.back() == '*' and _regex[i] == '*') {
            } else {
                regex.push_back(_regex[i]);
            }
        } else {
            regex.push_back(_regex[i]);
        }
    }
    //слово, в котором ищем максимальное подслово,
    //удовлетворяющее условию задачи
    string word;
    cin >> word;

    //пустое слово является подсловом любого слова
    if (word == "1") {
        cout << 0;
        return 0;
    }

    //получаем автомат, который соответствует рег. выр.
    Regular_Expression obj(regex);
    Automat x;
    x = obj.transform_to_automat();
    x.matrix_build();
    //ищем длину максимального подслова

    for (int i = word.size(); i > 0; i--) {
        for (int j = 0; j <= word.size() - i; j++) {
            string sub_word = word.substr(j, i);
            if (x.search(sub_word)) {
                cout << sub_word.size();
                return 0;
            }
        }
    }
    cout << 0;
    return 0;
}