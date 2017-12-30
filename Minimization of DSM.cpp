#include <iostream>
#include <set>
#include <queue>
#include <vector>

using namespace std;

//Deterministic State Machine
class DSM

{
public:
    //принимает матрицу смежности автомата и начальное разбиение вершин на классы
    DSM(vector< vector<char> >& _matrix, vector<int>& _classes);

    //ћинимизирующа€ ƒ ј функци€
    void Minimization();

private:
    //матрица смежности
    vector< vector<char> > matrix;

    //начальное деление на классы
    vector<int> classes;
};

DSM::DSM(vector< vector<char> >& _matrix, vector<int>& _classes) : matrix(_matrix.size()), classes(_classes.size())
{
    for (int i = 0; i < _matrix.size(); i++) {
        classes[i] = _classes[i];
        for (int j = 0; j < _matrix.size(); j++) {
            matrix[i].push_back(_matrix[i][j]);
        }
    }
}

void DSM::Minimization()
{
    //количество состо€ний
    int n = classes.size();

    //здесь будем хранить классы
    vector< set<int> > Mult(2);

    //«акидываем в Mult начальные классы
    //1 - не терминальные состо€ни€, 2 - терминальные
    for (int i = 0; i < n; i++) {
        if(classes[i] == 1) {
            Mult[0].insert(i);
        } else {
            Mult[1].insert(i);
        }
    }

    //очередь сплиттеров + начальна€ инициализаци€ очереди
    queue< pair< set<int>, char > > Q;
    Q.push(make_pair(Mult[0], 'a'));
    Q.push(make_pair(Mult[1], 'a'));
    Q.push(make_pair(Mult[0], 'b'));
    Q.push(make_pair(Mult[1], 'b'));

    while( !Q.empty() ) {
        //извлекаем сплиттер из очереди
        pair< set<int>, char > temp = Q.front();
        Q.pop();

        //будем итерироватьс€ по копии текущего разбиени€ на классы
        //тк разбиение будет измен€тьс€ во врем€ итераций
        vector< set<int> > cop_Mult(0);
        for ( auto i : Mult ) {
            cop_Mult.push_back(i);
        }

        //проходимс€ по каждому классу текущего разбиени€
        for ( auto i : cop_Mult ) {
            //в эти множества будем записывать попытки разбить текущий класс
            set<int> R1;
            set<int> R2;

            //по каждому элементу класса
            for ( auto j : i ) {
                //станет true если сплиттер достижим
                bool flag = false;

                //по каждому элементу сплиттера
                for ( auto k : temp.first ) {
                    if ( matrix[j][k] == temp.second ) {
                        flag = true;
                    }
                }

                //разбиение класса
                if ( flag == true ) {
                    R1.insert(j);
                } else {
                    R2.insert(j);
                }
            }

            //если непустое разбиение произошло
            if ( !R1.empty() and !R2.empty() ) {
                //удал€ем разбитый класс из множества классов
                vector<set<int>> copy_Mult;
                for ( auto y : Mult ) {
                    if ( y != i ) {
                        copy_Mult.push_back(y);
                    }
                }
                Mult.clear();
                for ( auto y : copy_Mult ) {
                    Mult.push_back(y);
                }

                //пополн€ем очередь
                Mult.push_back(R1);
                Mult.push_back(R2);
                Q.push(make_pair(R1, 'a'));
                Q.push(make_pair(R2, 'b'));
                Q.push(make_pair(R2, 'a'));
                Q.push(make_pair(R1, 'b'));
            }
        }
    }

    //¬ывод классов
    int k =0;
    for ( auto i : Mult ) {
        cout << "Class " << k << endl;
        for ( auto j : i ) {
            cout << j << endl;
        }
        k++;
    }
}

int main()
{
    // оличество состо€ний в ƒ ј
    int n;
    cout << "Enter |Q|:" << endl;
    cin >> n;
    cout << "1 - not final class" << endl << "2 - final class" << endl;

    vector< vector<char> > mat(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i].push_back(' ');
        }
    }

    vector<int> classes(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter " << i  << " class:" << endl;
        cin >> classes[i];
        cout << "For " << i << " enter edges (label) (j)" << endl;
        char lab;
        int k;
        cin >> lab >> k;
        mat[i][k] = lab;
        cin >> lab >> k;
        mat[i][k] = lab;
    }

    DSM obj(mat, classes);
    obj.Minimization();

    return 0;
}

