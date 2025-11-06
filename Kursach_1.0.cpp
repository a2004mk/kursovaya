#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#define NOMINMAX
#include <algorithm>
#include <windows.h>
#include <limits>

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_GRAY    "\033[2;37m"

#include "table_operations.h"
#include "menu_operations.h"

using namespace std;



int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    cout << COLOR_GREEN << "==========================================" << COLOR_RESET << endl;
    cout << COLOR_GREEN << "   ПРОГРАММА ОБЪЕДИНЕНИЯ ТАБЛИЦ" << COLOR_RESET << endl;
    cout << COLOR_GREEN << "==========================================" << COLOR_RESET << endl;
    cout << "Программа для объединения двух таблиц по ключевому столбцу" << endl;
    cout << "Формат файлов: TXT, разделитель - точка с запятой (;)" << endl<<endl;
    
    int keyColumn1 = 0;
    int keyColumn2 = 0;
    bool isSaved = false;
    string table1File, table2File;
    vector<TableRow> table1, table2;
    vector<vector<string>> result;

    // Первоначальная настройка и соединение таблиц
    if (!setupAndJoinTables(table1File, table2File, keyColumn1, keyColumn2, table1, table2, result)) {
        return 1;
    }

    while (true) {
        system("cls");

        if (isSaved) {
            // Меню когда файл уже сохранен
            if (!handleNextMenuAfterSave(table1File, table2File, keyColumn1, keyColumn2, table1, table2, result, isSaved)) {
                break;
            }
        }
        else {
            // Основное меню вывода результата
            if (!handleOutputMenu(table1File, table2File, result, isSaved)) {
                break;
            }

            // Если выводили на экран, показывает расширенное меню
            if (!isSaved) {
                if (!handleNextMenuAfterScreen(table1File, table2File, keyColumn1, keyColumn2, table1, table2, result, isSaved)) {
                    break;
                }
            }
        }
    }
    return 0;
}