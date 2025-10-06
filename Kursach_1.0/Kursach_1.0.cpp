#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <conio.h>
#include <algorithm>
#include <windows.h>

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_GRAY    "\033[2;37m"

using namespace std;

// Структура для строки таблицы
struct TableRow {
    vector<string> values;
    string key;
};

// Функция чтения таблицы из txt файла
vector<TableRow> readTable(string filename, int keyColumn) {
    vector<TableRow> table;
    ifstream file(filename);

    if (!file.is_open()) {
        cout <<COLOR_RED<< "Ошибка: не могу открыть файл " << filename <<COLOR_RESET<< endl;
        return table;
    }

    string line;
    int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;
        if (line.empty()) continue; 

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
            cout << "Предупреждение: в строке " << COLOR_GREEN << lineNum  <<COLOR_RESET<<  " нет колонки "<<COLOR_GREEN << keyColumn << COLOR_RESET << endl;
            row.key = "";
        }

        table.push_back(row);
    }

    file.close();
    cout << "Прочитано " << COLOR_GREEN << table.size()<<COLOR_RESET << " строк из " <<COLOR_GREEN<< filename<<COLOR_RESET << endl;
    return table;
}

// Функция сравнения для сортировки
bool compareRows(TableRow a, TableRow b) {
    return a.key < b.key;
}

// Функция соединения таблиц
vector<vector<string>> mergeJoin(vector<TableRow> table1, vector<TableRow> table2) {
    vector<vector<string>> result;

    sort(table1.begin(), table1.end(), compareRows);
    sort(table2.begin(), table2.end(), compareRows);

    int i = 0, j = 0;

    while (i < table1.size() && j < table2.size()) {

        if (table1[i].key == table2[j].key) {

            vector<string> joinedRow;
            for (int idx = 0; idx < table1[i].values.size(); idx++) {
                joinedRow.push_back(table1[i].values[idx]);
            }
            for (int k = 1; k < table2[j].values.size(); k++) {
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
void printResult(vector<vector<string>> result, string outputFile) {
    if (outputFile.empty()) {
        // Вывод на экран
        for (int i = 0; i < result.size(); i++) {
            for (int j = 0; j < result[i].size(); j++) {
                cout << result[i][j];
                if (j < result[i].size() - 1) {
                    cout << ";";
                }
            }
            cout << endl;
        }
    }
    else {
        // Вывод в файл
        ofstream file(outputFile);
        for (int i = 0; i < result.size(); i++) {
            for (int j = 0; j < result[i].size(); j++) {
                file << result[i][j];
                if (j < result[i].size() - 1) {
                    file << ";";
                }
            }
            file << endl;
        }
        file.close();
    }
}

// Пункты меню
vector<string> mainMenu = {
    "в файл result.txt",
    "на экран"
};

// Отображение меню с возможностью выбора пунктов 
void showMenu(const vector<string>& items, int selected, const string& header = "") {
    system("cls"); // очистка экрана
    if (!header.empty()) cout << header << "\n\n"; // вывод заголовка меню если он задан

    // Инструкции по управлению
    cout << "Используйте стрелки" <<COLOR_GREEN<< " ВВЕРХ / ВНИЗ" << COLOR_RESET << " для выбора | " << COLOR_GREEN << "ENTER" << COLOR_RESET << " - подтвердить.\n\n" << endl;

    for (int i = 0; i < items.size(); i++) {  // Вывод всех пунктов меню
        if (i == selected) {// Подсветка выбранного пункта
            cout << COLOR_GREEN << "-> " << items[i] << COLOR_RESET << endl;
        }
        else {
            cout << "   " << items[i] << endl;
        }
    }
}

// Работа с навигацией по меню с помощью стрелочек
int arrowMenu(const vector<string>& items, const string& header) {
    int selected = 0; // Индекс выбранного пункта
    while (true) {
        showMenu(items, selected, header);
        int key = _getch();// Ожидание ввода пользователя
        if (key == 224) { // Обработка специальных клавиш
            key = _getch();
            if (key == 72) { // Вверх
                selected = (selected == 0) ? items.size() - 1 : selected - 1;
            }
            else if (key == 80) { // Вниз
                selected = (selected == items.size() - 1) ? 0 : selected + 1;
            }
        }
        else if (key == 13) { // Enter
            return selected;
        }
        else if (key == 27) { // ESC
            return -1;
        }
    }
}

int main() {
   //setlocale(LC_ALL, "Russian");
   SetConsoleOutputCP(1251);
   SetConsoleCP(1251);
    string table1File = "table1.txt";
    string table2File = "table2.txt";
    string outputFile = "result.txt";
    int keyColumn1 = 0;
    int keyColumn2 = 0;

    cout << "Начало соединения таблиц..." << endl<<endl;

    vector<TableRow> table1 = readTable(table1File, keyColumn1);
    vector<TableRow> table2 = readTable(table2File, keyColumn2);

    if (table1.empty() || table2.empty()) {
        cout << COLOR_RED << "Одна из таблиц пустая, соединение невозможно"<<COLOR_RESET << endl;
        system("pause");
        return 1;
    }

    vector<vector<string>> result = mergeJoin(table1, table2);
    cout <<endl<< COLOR_GREEN<< "Найдено "  << result.size()  << " совпадений!" << COLOR_RESET << endl<<endl;
    system("pause");

    // Используем меню для выбора вывода
    int choice = arrowMenu(mainMenu, "Куда вывести результат?");

    if (choice == 0) {
        printResult(result, "result.txt");
        cout <<endl<< "Результат сохранен в "<<COLOR_GREEN<< "result.txt"<<COLOR_RESET << endl;
        system("pause");
    }
    else if (choice == 1) {
        cout << endl<<"Объединенная таблица:"<<endl<<endl;
        printResult(result, "");
        cout << endl;
        system("pause");
        // Меню для сохранения после показа на экран
        vector<string> saveMenu = { "да", "нет" };
        int saveChoice = arrowMenu(saveMenu, "Хотите сохранить результат в файл?");
        cout << endl;
        if (saveChoice == 0) {
            printResult(result, "result.txt");
            cout <<endl<< "Результат сохранен в " <<COLOR_GREEN<< "result.txt"<<COLOR_RESET << endl<<endl;
            system("pause");
        }
        else {
            cout <<endl<<  "Данные"<<COLOR_RED << " НЕ " << COLOR_RESET<< "сохранены в файл." << endl<<endl;
            system("pause");
        }
    }

    return 0;
}