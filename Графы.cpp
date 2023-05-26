#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <iostream>
using namespace std;

int menu(); 
int** InputMatrix(int**& matrix, int& points, int& edges); 
void SaveToFile(int**& matr, int& points, int& edges); 
void LoadFromFile(int**& matr, int& points, int& edges); 
int** FillRandMatrix(int**& matr, int& points, int& edges); 
int** AdjencyMatrix(int**& matrix, int**& adj_matrix, int points, int edges); 
void PrintAllPaths(int**& matr, int& p);
void PrintRoad(int**& matr, int& p, int first, int path);
int** AddEdge(int**& matrix, int& points, int& edges);

int main()
{
    SetConsoleOutputCP(1251);

    int** matrix = NULL; // матрица инцидентности
    int** adj_matrix = NULL; // матрица смежности
    int points = NULL;
    int edges = NULL;

    while (1)
    {
        if (matrix != NULL) 
        {
            cout << "Матрица инцидентности: " << endl;
            for (int i = 0; i < edges; i++)
            {
                for (int j = 0; j < points; j++)
                {
                    cout << matrix[i][j] << " ";
                }
                printf("\n");
            }
        }

        switch (menu())
        {
        case 1:
        {
            system("cls");
            InputMatrix(matrix, points, edges); 
            system("pause");
            break;
        }
        case 2:
        {
            system("cls");
            SaveToFile(matrix, points, edges);
            system("pause");
            break;
        }
        case 3:
        {
            system("cls");
            LoadFromFile(matrix, points, edges);
            system("pause");
            break;
        }
        case 4:
        {
            system("cls");
            FillRandMatrix(matrix, points, edges);
            system("pause");
            break;
        }
        case 5:
        {
            adj_matrix = AdjencyMatrix(matrix, adj_matrix, points, edges);

            cout << "Матрица смежности: " << endl;
            for (int i = 0; i < points; i++)
            {
                for (int j = 0; j < points; j++)
                {
                    cout << adj_matrix[i][j] << " ";
                }
                printf("\n");
            }
            system("pause");
            break;
        }
        case 6:
        {
            system("cls");
            adj_matrix = AdjencyMatrix(matrix, adj_matrix, points, edges);
            PrintAllPaths(adj_matrix, points);
            system("pause");
            break;
        }
        case 7:
        {
            matrix = AddEdge(matrix, points, edges);
            edges++;
            break;
        }
        case 8:
        {
            system("cls");
            return 0;
            break;
        }
        default: 
        {
            cout << "Ошибка " << endl;
            system("pause");
        }
        }
    }
}

int menu()
{
    int choice;
    cout << "Выберите вариант:" << endl;
    cout << "1.Ввод матрицы инцидентности" << endl;
    cout << "2.Запись матрицы в файл" << endl;
    cout << "3.Чтение матрицы из файла" << endl;
    cout << "4.Заполнение матрицы рандомными значениями" << endl;
    cout << "5.Вывод матрицы смежности" << endl;
    cout << "6.Вывод всех цепей" << endl;
    cout << "7.Добавление ребра" << endl;
    cout << "8.Выход" << endl;
    cin >> choice;

    return choice;
}

int** InputMatrix(int**& matrix, int& points, int& edges)
{
    if (matrix != NULL) 
    {
        for (int i = 0; i < points; i++)
        {
            free(matrix[i]);
        }
        free(matrix);
        matrix = NULL;
        points = 0;
        edges = 0;
    }

   cout << "Введите количество вершин: ";
    do
    {
        cin >> points;
        if (points < 1) cout << "Должна быть хотя бы 1 вершина" << endl;
    } while (points < 1);

    cout << "Введите количество ребер: ";
    cin >> edges;
    matrix = (int**)malloc(sizeof(int*) * points); 
    for (int i = 0; i < points; i++)
    {
        matrix[i] = (int*)malloc(sizeof(int) * edges); 
    }

    int kolvo = 0; // счетчик проверенных ребер
    int k = 0; // бегунок для матрицы (строка)
    int point_Smez; // введенная смежная вершина
    //          количество вершин - количество столбцов
    //          количество ребер  - количество строк
    for (int i = 0; i < edges; i++) 
    {
        for (int j = 0; j < points; j++)
        {
            matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < points; i++) 
    {
        cout << "вершина " << i + 1 << ": ";
        cout << "Введите номера смежных вершин или 0 для окончания ввода" << endl; 

        point_Smez = 0;
        if (kolvo < edges) // Пока не проверим смежность для всех ребер 
        {
            do
            {
                cin >> point_Smez;
                if (point_Smez > points) // проверка на существование вершины
                {
                    cout << "Такой вершины не существует" << endl;
                }
                else if (point_Smez == i + 1) cout << "Граф должен быть без петель" << endl; 
                else if (point_Smez != 0) 
                {
                    matrix[k][i] = 1; // Мы вводим смежные вершины для X вершины, Х вершине в матрице присваивается 1
                    matrix[k][point_Smez - 1] = 1; // Введенной смежной вершине в матрице присваивается 1
                    kolvo++; // ребро проверили => увеличили счетчик
                    ++k; // изменяем строку
                }
            } while (point_Smez != 0);
        }
        else cout << "Нужное количество ребер уже было введено!" << endl; 
    }

    return matrix; 
}


void SaveToFile(int**& matr, int& points, int& edges)
{
    system("cls");

    if (matr == NULL) 
    {
        cout << "Матрица не создана" << endl;
        system("pause");
        return;
    }

    FILE* f = fopen("Matrix", "w"); 
    if (f) 
    {
        fprintf(f, "%d %d\n", points, edges); 
        for (int i = 0; i < edges; i++) 
        {
            for (int j = 0; j < points; j++)
            {
                fprintf(f, "%d  ", matr[i][j]);
            }
            fprintf(f, "\n");
        }
        cout << "Данные записаны!" << endl;
        fclose(f);
    }
    else 
    {
        cout << "Ошибка" << endl;
        system("pause");
    }
 
}

void LoadFromFile(int**& matr, int& points, int& edges)
{
    system("cls");

    if (matr != NULL) 
    {
        for (int i = 0; i < points; i++)
        {
            free(matr[i]);
        }
        free(matr);

        matr = NULL;
        points = 0;
    }

    FILE* f = fopen("Matrix", "r+"); 
    if (f)
    {
        fscanf(f, "%d", &points); 
        fseek(f, 1, SEEK_CUR); 
        fscanf(f, "%d", &edges); 
        matr = (int**)malloc(sizeof(int*) * edges); 
        for (int i = 0; i < edges; i++)
        {
            matr[i] = (int*)malloc(sizeof(int) * points); 
        }

        for (int i = 0; i < edges; i++)
        {
            for (int j = 0; j < points; j++)
            {
                fscanf(f, "%d", &matr[i][j]); 
            }
        }

        fclose(f); 
    }
    else 
    {
        cout << "Ошибка" << endl;
        system("pause");
    }
   
}

int** FillRandMatrix(int**& matr, int& points, int& edges)
{
    if (matr != NULL) 
    {
        for (int i = 0; i < points; i++)
        {
            free(matr[i]);
        }
        free(matr);

        matr = NULL;
        points = 0;
        edges = 0;
    }

    cout << "Введите количество вершин: ";
    cin >> points;
    while (points <= 1)
    {
        cin >> points;
        if (points <= 1)
        {
            cout << "Такой вершины не существует" << endl;
        }
    }
    cout << "Введите количество ребер: ";
    cin >> edges;

    matr = (int**)malloc(sizeof(int*) * points);
    for (int i = 0; i < points; i++)
    {
        matr[i] = (int*)malloc(sizeof(int) * (edges));
    }

    for (int i = 0; i < edges; i++)
    {
        for (int j = 0; j < points; j++)
        {
            matr[j][i] = 0;
        }

    }

    int kolvo_ = 0; // Счетчик рандомно введенных единиц

    for (int i = 0; i < edges; i++) 
    {
        for (int j = 0; j < points; j++)
        {
            if (kolvo_ < 2) // в матрице инцидентности может быть в ряду не более двух единиц
            {
                if (kolvo_ == 0 && j == points - 2)  matr[i][j] = 1;
                else if (kolvo_ == 1 && j == points - 1) matr[i][j] = 1;
                else
                {
                    matr[i][j] = rand() % 2; 
                }
                if (matr[i][j] == 1) kolvo_++;
            }
            else matr[i][j] = 0; 
        }
        kolvo_ = 0; 
    }
    return matr;
}

int** AdjencyMatrix(int**& matrix, int**& adj_matrix, int points, int edges)
{
    system("cls");

    int** adj_matrix_ = (int**)malloc(sizeof(int*) * points); 
    for (int i = 0; i < points; i++)
    {
        adj_matrix_[i] = (int*)malloc(sizeof(int) * points);
    }

    for (int i = 0; i < points; i++)
    {
        for (int j = 0; j < points; j++)
        {
            adj_matrix_[i][j] = 0;
        }
    }

    int temp = -1;

    for (int i = 0; i < edges; i++)
    {
        for (int j = 0; j < points; j++)
        {
            if (temp == -1 && matrix[i][j] == 1) temp = j;
            else if (temp != -1 && matrix[i][j] == 1) 
            {
                adj_matrix_[j][temp] = 1;
                adj_matrix_[temp][j] = 1;
                temp = -1; 
            }
        }
        temp = -1; 
    }

    return adj_matrix_;
}


void PrintAllPaths(int**& matr, int& p) 
{
    if (matr == NULL)
    {
        cout << "Матрица не создана" << endl;
        system("pause");
        return;
    }

    int first = 0;

    cout << "Введите первую вершину(от 1 до " << p << "): ";
    while (first <= 0 || first > p) 
    {
        cin >> first;

        if (first <= 0)
        {
           cout << "Неккоректный ввод" << endl;
        }
        else if (first > p)
        {
            cout << "Неккоректный ввод" << endl;
        }
    }

    first--;

    PrintRoad(matr, p, first, 0);
}

void PrintRoad(int**& matr, int& p, int first, int path)
{
    int subpath = path;
    int* arr = (int*)malloc(sizeof(int) * p); 
    for (int i = p - 1; i >= 0; i--)
    {
        arr[i] = subpath % 10;
        if (arr[i] == first + 1)
        {
            free(arr);
            return;
        }
        subpath /= 10;
    }

    int flag = 0;
    for (int i = 0; i < p; i++)
    {
        if (arr[i] != 0)
        {
            cout << arr[i] << " --> ";
            flag = 1;
        }
    }
    if (flag)cout << endl;

    for (int i = 0; i < p; i++)
    {
        if (matr[first][i] == 1) 
        {
            PrintRoad(matr, p, i, path * 10 + 1 + first);
        }
    }
    free(arr);
}

int** AddEdge(int**& matrix, int& points, int& edges)
{
    if (matrix == NULL)
    {
        printf("Матрица не создана\n");
        system("pause");
        return matrix;
    }

    int** newMatr = (int**)malloc(sizeof(int*) * (edges + 1));
    for (int i = 0; i <= edges; i++)
    {
        newMatr[i] = (int*)malloc(sizeof(int) * (points));
    }

    for (int i = 0; i < edges; i++)
    {
        for (int j = 0; j < points; j++)
        {
            newMatr[i][j] = matrix[i][j];
        }
    }

    for (int i = 0; i < points; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    for (int i = 0; i < points; i++)
    {
        newMatr[edges][i] = 0;
    }

    int begin = 0, end = 0;
    do
    {
        printf("Введите начало ребра: ");
        scanf("%d", &begin);
    } while (begin > points);
    do
    {
        printf("Введите конец ребра: ");
        scanf("%d", &end);
    } while (end > points);


    newMatr[edges][begin - 1] = 1;
    newMatr[edges][end - 1] = 1;

    return newMatr;
}
