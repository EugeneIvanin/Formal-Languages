#include <iostream>
#include <set>
#include <queue>
#include <vector>

using namespace std;

//Deterministic State Machine
class DSM

{
public:
    //��������� ������� ��������� �������� � ��������� ��������� ������ �� ������
    DSM(vector< vector<char> >& _matrix, vector<int>& _classes);

    //�������������� ��� �������
    void Minimization();

private:
    //������� ���������
    vector< vector<char> > matrix;

    //��������� ������� �� ������
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
    //���������� ���������
    int n = classes.size();

    //����� ����� ������� ������
    vector< set<int> > Mult(2);

    //���������� � Mult ��������� ������
    //1 - �� ������������ ���������, 2 - ������������
    for (int i = 0; i < n; i++) {
        if(classes[i] == 1) {
            Mult[0].insert(i);
        } else {
            Mult[1].insert(i);
        }
    }

    //������� ���������� + ��������� ������������� �������
    queue< pair< set<int>, char > > Q;
    Q.push(make_pair(Mult[0], 'a'));
    Q.push(make_pair(Mult[1], 'a'));
    Q.push(make_pair(Mult[0], 'b'));
    Q.push(make_pair(Mult[1], 'b'));

    while( !Q.empty() ) {
        //��������� �������� �� �������
        pair< set<int>, char > temp = Q.front();
        Q.pop();

        //����� ������������� �� ����� �������� ��������� �� ������
        //�� ��������� ����� ���������� �� ����� ��������
        vector< set<int> > cop_Mult(0);
        for ( auto i : Mult ) {
            cop_Mult.push_back(i);
        }

        //���������� �� ������� ������ �������� ���������
        for ( auto i : cop_Mult ) {
            //� ��� ��������� ����� ���������� ������� ������� ������� �����
            set<int> R1;
            set<int> R2;

            //�� ������� �������� ������
            for ( auto j : i ) {
                //������ true ���� �������� ��������
                bool flag = false;

                //�� ������� �������� ���������
                for ( auto k : temp.first ) {
                    if ( matrix[j][k] == temp.second ) {
                        flag = true;
                    }
                }

                //��������� ������
                if ( flag == true ) {
                    R1.insert(j);
                } else {
                    R2.insert(j);
                }
            }

            //���� �������� ��������� ���������
            if ( !R1.empty() and !R2.empty() ) {
                //������� �������� ����� �� ��������� �������
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

                //��������� �������
                Mult.push_back(R1);
                Mult.push_back(R2);
                Q.push(make_pair(R1, 'a'));
                Q.push(make_pair(R2, 'b'));
                Q.push(make_pair(R2, 'a'));
                Q.push(make_pair(R1, 'b'));
            }
        }
    }

    //����� �������
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
    //���������� ��������� � ���
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

