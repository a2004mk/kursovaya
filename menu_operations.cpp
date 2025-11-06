#include "menu_operations.h"
#include "table_operations.h"
// simpleMenu
// inputFilename
// inputKeyColumns
// performTableJoin
// offerFilePreview
// setupAndJoinTables
// handleOutputMenu
// handleNextMenuAfterScreen
// handleNextMenuAfterSave

//Менюшка с вводом от пользователя цифры
int simpleMenu(const vector<string>& items, const string& header) {
    int choice;

    while (true) {
        system("cls");
        if (!header.empty()) cout << header << "\n\n";

        // Вывод пунктов меню с номерами
        for (int i = 0; i < items.size(); i++) {
            cout << COLOR_GREEN << "[" << i + 1 << "] " << COLOR_RESET << items[i] << endl;
        }

        cout << "\nВыберите пункт (" << COLOR_GREEN << "1-" << items.size() << COLOR_RESET << "): ";

        if (cin >> choice && choice >= 1 && choice <= items.size()) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice - 1;
        }

        // Очищаем при неверном вводе
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

//Ввод названия файлов 
string inputFilename(const string& prompt) {
    string filename;
    while (true) {
        cout << prompt;
        getline(cin, filename);

        // Удаляем пробелы в начале и конце
        filename.erase(0, filename.find_first_not_of(" \t"));
        filename.erase(filename.find_last_not_of(" \t") + 1);

        if (filename.empty()) {
            cout << COLOR_RED << "Название файла не может быть пустым!" << COLOR_RESET << endl;
            continue;
        }

        // Добавляем расширение .txt если его нет
        filename = ensureTxtExtension(filename);

        // Проверяем существование файла
        ifstream testFile(filename);
        if (!testFile.is_open()) {
            cout << COLOR_RED << "Файл '" << filename << "' не найден!" << COLOR_RESET << endl;
            cout << "Проверьте правильность названия и наличие файла." << endl;
            continue;
        }
        testFile.close();

        return filename;
    }
}

// Функция для ввода ключевых колонок
void inputKeyColumns(int& keyColumn1, int& keyColumn2) {
    cout << "Номер ключевого столбца первой таблицы: ";
    while (!(cin >> keyColumn1)) {
        cout << COLOR_RED << "Неверный ввод!" << COLOR_RESET << endl << "Пожалуйста, введите число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Номер ключевого столбца второй таблицы: ";
    while (!(cin >> keyColumn2)) {
        cout << COLOR_RED << "Неверный ввод!" << COLOR_RESET << endl << "Пожалуйста, введите число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (keyColumn1 != 0 || keyColumn2 != 0) {
        if (keyColumn1 != 0) keyColumn1 -= 1;
        if (keyColumn2 != 0) keyColumn2 -= 1;
    }
}

// Функция для проверки типов данных и выполнения соединения
bool performTableJoin(const string& table1File, const string& table2File,
    int keyColumn1, int keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result) {
    cout << endl << "Начало соединения таблиц..." << endl << endl;

    table1 = readTable(table1File, keyColumn1);
    table2 = readTable(table2File, keyColumn2);

    if (table1.empty() || table2.empty()) {
        cout << COLOR_RED << "Одна из таблиц пустая, соединение невозможно" << COLOR_RESET << endl;
        system("pause");
        return false;
    }

    // Проверка типов данных
    cout << "\nПроверка типов данных...\n" << endl;
    string type1 = getColumnType(table1, keyColumn1);
    string type2 = getColumnType(table2, keyColumn2);

    cout << "Тип ключевой колонки первой таблицы: " << COLOR_GREEN << type1 << COLOR_RESET << endl;
    cout << "Тип ключевой колонки второй таблицы: " << COLOR_GREEN << type2 << COLOR_RESET << endl << endl;

    if (type1 != type2 && type1 != "empty" && type2 != "empty") {
        cout << COLOR_RED << "Ошибка: несовместимые типы данных в ключевых колонках! Соединение невозможно." << COLOR_RESET << endl;
        system("pause");
        return false;
    }
    if (type1 == "empty" || type2 == "empty") {
        cout << COLOR_RED << "Предупреждение: одна из ключевых колонок пустая!" << COLOR_RESET << endl;
        cout << "Результат соединения может быть некорректным." << endl;
    }

    result = mergeJoin(table1, table2);
    cout << endl << COLOR_GREEN << "Найдено " << result.size() << " совпадений!" << COLOR_RESET << endl << endl;
    system("pause");
    return true;
}

// Функция для предпросмотра файлов
void offerFilePreview(const string& table1File, const string& table2File) {
    vector<string> previewMenu = { "да", "нет" };
    int previewChoice = simpleMenu(previewMenu, "Хотите посмотреть первые строки файлов?");
    if (previewChoice == 0) {
        previewFile(table1File);
        previewFile(table2File);
    }
}

// Функция для полного процесса ввода данных и соединения таблиц
bool setupAndJoinTables(string& table1File, string& table2File,
    int& keyColumn1, int& keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result) {
    table1File = inputFilename("Введите название первого файла: ");
    table2File = inputFilename("Введите название второго файла: ");

    offerFilePreview(table1File, table2File);
    inputKeyColumns(keyColumn1, keyColumn2);

    return performTableJoin(table1File, table2File, keyColumn1, keyColumn2, table1, table2, result);
}

// Основная логика меню вывода результата
bool handleOutputMenu(const string& table1File, const string& table2File,
    vector<vector<string>>& result, bool& isSaved) {
    vector<string> mainMenu = {
        "в файл",
        "на экран",
        "выйти из программы"
    };

    int choice = simpleMenu(mainMenu, "Куда вывести результат?");

    if (choice == 0) {
        // Сохранение в файл
        string outputFile = generateOutputFilename(table1File, table2File);
        printResult(result, outputFile);
        cout << endl << "Результат сохранен в " << COLOR_GREEN << outputFile << COLOR_RESET << endl;
        isSaved = true;
        system("pause");

        vector<string> viewMenu = { "да", "нет" };
        int viewChoice = simpleMenu(viewMenu, "Посмотреть сохраненный файл?");
        if (viewChoice == 0) {
            previewFile(outputFile, 15);
            system("pause");
        }
        return true;
    }
    else if (choice == 1) {
        // Вывод на экран
        cout << endl << "Объединенная таблица:" << endl << endl;
        printResult(result, "");
        cout << endl;
        system("pause");
        return true;
    }
    else {
        // Выход из программы
        cout << "Выход из программы..." << endl;
        return false;
    }
}

// Логика меню "Что дальше?" после вывода на экран
bool handleNextMenuAfterScreen(string& table1File, string& table2File,  // Убрать const
    int& keyColumn1, int& keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result, bool& isSaved) {
    vector<string> nextMenu = {
        "объединить другие таблицы",
        "сохранить выведенную таблицу в файл",
        "выйти из программы"
    };

    int nextChoice = simpleMenu(nextMenu, "Что дальше?");

    if (nextChoice == 0) {
        // Перезапуск
        isSaved = false;
        return setupAndJoinTables(table1File, table2File, keyColumn1, keyColumn2, table1, table2, result);
    }
    else if (nextChoice == 1) {
        // Сохранение
        string outputFile = generateOutputFilename(table1File, table2File);
        printResult(result, outputFile);
        cout << endl << "Результат сохранен в " << COLOR_GREEN << outputFile << COLOR_RESET << endl;
        isSaved = true;
        system("pause");
        return true;
    }
    else {
        return false;
    }
}

// Логика меню "Что дальше?" когда файл уже сохранен
bool handleNextMenuAfterSave(string& table1File, string& table2File,  // Убрать const
    int& keyColumn1, int& keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result, bool& isSaved) {
    vector<string> nextMenu = {
        "объединить другие таблицы",
        "выйти из программы"
    };

    int nextChoice = simpleMenu(nextMenu, "Что дальше?");

    if (nextChoice == 0) {
        isSaved = false;
        return setupAndJoinTables(table1File, table2File, keyColumn1, keyColumn2, table1, table2, result);
    }
    else {
        return false;
    }
}
