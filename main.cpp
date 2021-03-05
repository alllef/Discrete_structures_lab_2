#include <iostream>
#include<vector>
#include <Windows.h>
#include<string>
#include<fstream>

using namespace std;

void outputMatrix(int *matrix, int picks);

void initializeGraph(int &picks, int &ribs, vector<int> &start, vector<int> &end);

void makeDistanceAndReachMatrix(const int *matrix, int picks, int *distanceMatrix, int *reachMatrix);


void
makeContiguityMatrix(int *contiguityMatrix, vector<int> start, vector<int> end, int picks, int ribs, bool isOriented);

vector<int> findCenterAndRadius(int *matrix, int picks);

vector<vector<vector<int>>> findTier(int *matrix, int picks);

int defineCoherenceType(int *reach, int *contiguity, int picks);

int findDiameter(int *matrix, int picks);

void printCoherenceType(int type);

void printTier(vector<vector<vector<int>>> tier, int picks);

void printCenterAndRadius(vector<int> centers);

void makeMenu(vector<int> &startVector, vector<int> &endVector, int &picks, int &ribs, int *contiguityMatrix,
              int *distanceMatrix, int *reachMatrix);

int main() {

    SetConsoleOutputCP(CP_UTF8);

    vector<int> startVector = {};
    vector<int> endVector = {};
    vector<int> vectorOfPower = {};
    int n = 0, m = 0;

    initializeGraph(n, m, startVector, endVector);

    int *contiguityMatrix = new int[n * n];
    int *distanceMatrix = new int[n * n];
    int *reachMatrix = new int[n * n];

    makeMenu(startVector, endVector, n, m, contiguityMatrix, distanceMatrix, reachMatrix);
    return 0;
}

void initializeGraph(int &picks, int &ribs, vector<int> &start, vector<int> &end) {
    int counter = 0;
    int startNumber, endNumber;

    ifstream inFile;
    inFile.open(R"(C:\Users\alekz29\Documents\myGraph.txt)");

    if (!inFile.is_open()) cout << "It is not open" << endl;
    inFile >> picks >> ribs;

    while (!inFile.eof()) {


        if (counter % 2 == 0) {
            inFile >> startNumber;
            start.push_back(startNumber);
        }
        if (counter % 2 == 1) {
            inFile >> endNumber;
            end.push_back(endNumber);
        }
        counter++;
    }
    inFile.close();
}

void
makeContiguityMatrix(int *contiguityMatrix, vector<int> start, vector<int> end, int picks, int ribs, bool isOriented) {

    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            *(contiguityMatrix + i * picks + j) = 0;
        }
    }

    for (int i = 0; i < ribs; i++) {

        *(contiguityMatrix + (start[i] - 1) * picks + (end[i] - 1)) = 1;
    }

    if (!isOriented) {
        for (int i = 0; i < ribs; i++) {
            *(contiguityMatrix + (end[i] - 1) * picks + (start[i] - 1)) = 1;
        }
    }

}

void makeDistanceAndReachMatrix(const int *matrix, int picks, int *distanceMatrix, int *reachMatrix) {


    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            *(distanceMatrix + i * picks + j) = *(matrix + i * picks + j);
        }
    }

    int sum = 0;
    for (int m = 0; m < picks - 1; m++) {
        for (int i = 0; i < picks; i++) {
            for (int j = 0; j < picks; j++) {
                if (i == j ||(*(distanceMatrix + i * picks + j)) != 0 )continue;
                for (int k = 0; k < picks; k++) {


                    sum += (*(distanceMatrix + i * picks + k)) *
                           (*(matrix + k * picks + j));

                }
                if (sum > 0) (*(distanceMatrix + i * picks + j)) =m+2;
                sum = 0;
            }

        }

    }

    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if ((*(distanceMatrix + i * picks + j)) > 0) *(reachMatrix + i * picks + j) = 1;
            else *(reachMatrix + i * picks + j) = 0;
        }
    }

}

void outputMatrix(int *matrix, int picks) {
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            cout << *(matrix + i * picks + j) << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int findDiameter(int *matrix, int picks) {
    int maxLength = 0;
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if (*(matrix + i * picks + j) > maxLength) maxLength = *(matrix + i * picks + j);
        }
    }
    return maxLength;
}

vector<int> findCenterAndRadius(int *matrix, int picks) {
    int maxPickDistance = 0;

    vector<int> maxPickDistances;
    vector<int> centers;
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if (maxPickDistance < *(matrix + i * picks + j))maxPickDistance = *(matrix + i * picks + j);
        }
        maxPickDistances.push_back(maxPickDistance);


        maxPickDistance = 0;
    }
    int radius = maxPickDistances[0];
    for (int i = 0; i < maxPickDistances.size(); i++) {
        if (radius > maxPickDistances[i])radius = maxPickDistances[i];
    }
    centers.push_back(radius);
    for (int i = 0; i < picks; i++) {
        if (maxPickDistances[i] == radius) centers.push_back(i + 1);
    }
    return centers;
}

vector<vector<vector<int>>> findTier(int *matrix, int picks) {
    vector<int> emptyVector;
    vector<vector<vector<int>>> tiers(picks);
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            tiers[i].push_back(emptyVector);
        }
    }

    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if (*(matrix + i * picks + j) != 0)
                tiers[i][*(matrix + i * picks + j)].push_back(j + 1);
        }
    }

    return tiers;
}

int defineCoherenceType(int *reach, int *contiguity, int picks) {
    int counter = 0;

    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if (*(reach + i * picks + j) == 1)counter++;
            else break;
        }
    }
    if (counter == picks * picks)return 1;
    counter = 0;

    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if ((*(reach + i * picks + j) + *(reach + j * picks + i)) == 1)counter++;
            else break;
        }
    }
    if (counter == picks * picks)return 2;
    counter = 0;

    int *isWeakCoherenceMatrix = new int[picks * picks];
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            *(isWeakCoherenceMatrix + i * picks + j) =
                    1 + *(contiguity + i * picks + j) + *(contiguity + j * picks + i);
        }
    }
    for (int i = 0; i < picks; i++) {
        for (int j = 0; j < picks; j++) {
            if (*(isWeakCoherenceMatrix + i * picks + j) == 1)counter++;
            else break;
        }
    }
    if (counter == picks * picks)return 3;
    return -1;
}

void printCoherenceType(int type) {
    if (type == 1) cout << "Граф сильно связанный\n";
    if (type == 2) cout << "Граф односторонне связанный\n";
    if (type == 3) cout << "Граф слабо связанный\n";
    if (type == -1) cout << "Граф не связанный\n";
}

void printTier(vector<vector<vector<int>>> tier, int picks) {
    for (int i = 0; i < picks; i++) {

        for (int j = 0; j < picks; j++) {
            if (!tier[i][j].empty())
                cout << "Вершина " << i + 1 << " Находится на ярусе " << j << " с вершинами ";
            for (int k = 0; k < tier[i][j].size(); k++) {
                cout << tier[i][j][k] << " ";
            }
            if (!tier[i][j].empty()) cout << endl;
        }
        cout << endl;
    }
}

void printCenterAndRadius(vector<int> centers) {
    cout << "Радиус графа " << centers[0] << endl;
    cout << "Центром графа является ";
    for (int i = 1; i < centers.size(); i++) {
        cout << centers[i] << " ";
    }
}

void makeMenu(vector<int> &startVector, vector<int> &endVector, int &picks, int &ribs, int *contiguityMatrix,
              int *distanceMatrix, int *reachMatrix) {
    bool isOriented;
    bool choise;
    cout << "Введите тип графа 0- неориентированныйб 1- ориентированный\n";
    cin >> isOriented;
    if (!isOriented) {
        cout
                << "Нажмите 1, чтобы увидеть матрицы расстояний и достижимости, а также метрические характеристики графа\n";
        cin >> choise;
        if (choise) {
            makeContiguityMatrix(contiguityMatrix, startVector, endVector, picks, ribs, isOriented);
            makeDistanceAndReachMatrix(contiguityMatrix, picks, distanceMatrix, reachMatrix);
            cout << "Матрица расстояний\n";
            outputMatrix(distanceMatrix, picks);
            cout << "Матрица достижимости\n";
            outputMatrix(reachMatrix, picks);
            findTier(distanceMatrix, picks);
            cout << "Диаметр графа" << findDiameter(distanceMatrix, picks) << endl;
            printTier(findTier(distanceMatrix, picks), picks);
            printCenterAndRadius(findCenterAndRadius(distanceMatrix, picks));
        } else cout << "Нажмите 1";
    }
    if (isOriented) {
        cout
                << "Нажмите 1, чтобы увидеть матрицы расстояний и достижимости, а также тип связности графа\n";

        cin >> choise;
        if (choise) {
            makeContiguityMatrix(contiguityMatrix, startVector, endVector, picks, ribs, isOriented);
            makeDistanceAndReachMatrix(contiguityMatrix, picks, distanceMatrix, reachMatrix);
            cout << "Матрица расстояний\n";
            outputMatrix(distanceMatrix, picks);
            cout << "Матрица достижимости\n";
            outputMatrix(reachMatrix, picks);
            printCoherenceType(defineCoherenceType(reachMatrix, contiguityMatrix, picks));
        } else cout << "Нажмите 1";
    }
}