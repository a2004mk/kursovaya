#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <windows.h>

using namespace std;

// Структура для строки таблицы
struct TableRow {
    vector<string> values;
    string key;
};

// Функция чтения таблицы из txt файла
vector<TableRow> readTable(const string& filename, int keyColumn) {
    vector<TableRow> table;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Ошибка: не могу открыть файл " << filename << endl;
        return table;
    }

    string line;
    int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;
        if (line.empty()) continue; // Пропускаем пустые строки

        TableRow row;
        stringstream ss(line);
        string value;

        while (getline(ss, value, ';')) {
            row.values.push_back(value);
        }

        if (keyColumn < row.values.size()) {
            row.key = row.values[keyColumn];
        }
        else {
            cout << "Предупреждение: в строке " << lineNum << " нет колонки " << keyColumn << endl;
            row.key = "";
        }

        table.push_back(row);
    }

    file.close();
    cout << "Прочитано " << table.size() << " строк из " << filename << endl;
    return table;
}

// Функция сравнения для сортировки
bool compareRows(const TableRow& a, const TableRow& b) {
    return a.key < b.key;
}

// Функция соединения таблиц
vector<vector<string>> mergeJoin(vector<TableRow>& table1, vector<TableRow>& table2) { // внешний вектор - таблица (все строки результата), внутренний вектор - одна строка (все ячейки строки), string -  одна ячейка данных
    vector<vector<string>> result;

    // Сортируем таблицы по ключу
    sort(table1.begin(), table1.end(), compareRows);
    sort(table2.begin(), table2.end(), compareRows);

    int i = 0, j = 0;

    while (i < table1.size() && j < table2.size()) {
       // cout << "Сравниваем: " << table1[i].key << " и " << table2[j].key << endl;

        if (table1[i].key == table2[j].key) {
         //   cout << "Найдено совпадение!" << endl;

            vector<string> joinedRow;

            // Добавляем все значения из первой таблицы
            for (const auto& val : table1[i].values) {
                joinedRow.push_back(val);
            }

            // Добавляем все значения из второй таблицы
            for (size_t k = 1; k < table2[j].values.size(); k++) {
                joinedRow.push_back(table2[j].values[k]);
            }

            result.push_back(joinedRow);
            i++;
            j++;
        }
        else if (table1[i].key < table2[j].key) {
            i++;
        }
        else {
            j++;
        }
    }

    return result;
}

// Функция вывода результата
void printResult(const vector<vector<string>>& result, const string& outputFile = "") {
    if (outputFile.empty()) {
        // Вывод на экран
        for (const auto& row : result) {
            for (size_t i = 0; i < row.size(); i++) {
                cout << row[i];
                if (i < row.size() - 1) {
                    cout << ";";
                }
            }
            cout << endl;
        }
    }
    else {
        // Вывод в файл
        ofstream file(outputFile);
        for (const auto& row : result) {
            for (size_t i = 0; i < row.size(); i++) {
                file << row[i];
                if (i < row.size() - 1) {
                    file << ";";
                }
            }
            file << endl;
        }
        file.close();
    }
}

int main() {
    setlocale(LC_ALL, "Ru");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    string table1File = "table1.txt";
    string table2File = "table2.txt";
    string outputFile = "result.txt";
    int keyColumn1 = 0;
    int keyColumn2 = 0;

    cout << "Начинаем соединение таблиц..." << endl;

    vector<TableRow> table1 = readTable(table1File, keyColumn1);
    vector<TableRow> table2 = readTable(table2File, keyColumn2);

    if (table1.empty() || table2.empty()) {
        cout << "Одна из таблиц пустая, соединение невозможно" << endl;
        return 1;
    }

    vector<vector<string>> result = mergeJoin(table1, table2);
    cout << "Найдено " << result.size() << " совпадений" << endl;
  //  printResult(result, outputFile);
    // Спрашиваем куда выводить результат
    cout <<endl<< "Куда вывести результат?" << endl;
    cout << "1 - в файл result.txt" << endl;
    cout << "2 - на экран" << endl;
    int choice;
    cin >> choice;

    if (choice == 1) {
        // Вывод в файл
        printResult(result, "result.txt");
        cout << "Результат сохранен в result.txt" << endl;
    }
    else {
        // Вывод на экран
        printResult(result, "");

        // Спрашиваем о сохранении в файл
        cout <<endl<< "Хотите сохранить результат в файл?" << endl;
        cout << "1 - да" << endl;
        cout << "2 - нет" << endl;
        int saveChoice;
        cin >> saveChoice;

        if (saveChoice == 1) {
            printResult(result, "result.txt");
            cout <<endl<< "Результат сохранен в result.txt" << endl;
        }
        else {
            cout <<endl<< "Данные не сохранены в файл" << endl;
        }
    }
    return 0;
}