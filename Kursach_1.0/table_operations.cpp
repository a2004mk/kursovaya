#include "table_operations.h"
#include "menu_operations.h"
// table_operations.h
// ensureTxtExtension
// removeTxtExtension
// readTable
// getColumnType
// splitLine
// printBeautifulTable
// previewFile
// mergeJoin
// generateOutputFilename
// printResult


// Проверка на расширение .txt и добавление .txt
string ensureTxtExtension(const string& filename) {
    if (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".txt") {
        return filename;
    }
    return filename + ".txt";
}

// Функция для удаления расширения .txt
string removeTxtExtension(const string& filename) {
    if (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".txt") {
        return filename.substr(0, filename.length() - 4);
    }
    return filename;
}

// Функция чтения таблицы из txt файла
vector<TableRow> readTable(string filename, int keyColumn) {
    vector<TableRow> table;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << COLOR_RED << "Ошибка открытия файла: " << filename << COLOR_RESET << endl;
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
            cout << "Предупреждение: в строке " << COLOR_GREEN << lineNum << COLOR_RESET << " нет колонки " << COLOR_GREEN << keyColumn << COLOR_RESET << endl;
            row.key = "";
        }

        table.push_back(row);
    }

    file.close();
    cout << "Прочитано " << COLOR_GREEN << table.size() << COLOR_RESET << " строк из " << COLOR_GREEN << filename << COLOR_RESET << endl;
    return table;
}

// Функция для определения типа данных колонки
string getColumnType(const vector<TableRow>& table, int columnIndex) {
    if (table.empty() || columnIndex >= table[0].values.size()) {
        return "unknown";
    }

    bool isNumeric = true;
    bool hasData = false;

    for (const auto& row : table) {
        if (columnIndex < row.values.size() && !row.values[columnIndex].empty()) {
            hasData = true;
            bool hasDot = false;
            string value = row.values[columnIndex];

            // Проверяем, является ли значение числом
            for (char c : value) {
                unsigned char uc = static_cast<unsigned char>(c);
                if (!isdigit(uc) && c != '.' && c != '-' && c != ',') {
                    isNumeric = false;
                    break;
                }
                if (c == '.' || c == ',') {
                    if (hasDot) {
                        isNumeric = false;
                        break;
                    }
                    hasDot = true;
                }

            }

            if (!isNumeric) break;
        }
    }

    if (!hasData) return "empty";
    return isNumeric ? "numeric" : "text";
}

// Функция для разделения строк на колонки
vector<string> splitLine(const string& line) {
    vector<string> columns;
    stringstream ss(line);
    string value;
    while (getline(ss, value, ';')) {
        columns.push_back(value);
    }
    return columns;
}

// Функция вывода отформатированной таблицы на консоль
void printBeautifulTable(const vector<vector<string>>& data) {
    if (data.empty()) return;

    //  максимальные ширины колонок
    vector<int> colWidths(data[0].size(), 0);
    for (const auto& row : data) {
        for (int i = 0; i < row.size() && i < colWidths.size(); i++) {
            colWidths[i] = max(colWidths[i], (int)row[i].length());
        }
    }

    // Вывод верхней границы
    cout << "+";
    for (int width : colWidths) {
        for (int j = 0; j < width + 2; j++) cout << "-";
        cout << "+";
    }
    cout << endl;

    // Вывод данных
    for (const auto& row : data) {
        cout << "|";
        for (int i = 0; i < row.size() && i < colWidths.size(); i++) {
            cout << " " << row[i];
            for (int j = row[i].length(); j < colWidths[i] + 1; j++) cout << " ";
            cout << "|";
        }
        cout << endl;

        // Разделитель между строками 
        cout << "+";
        for (int width : colWidths) {
            for (int j = 0; j < width + 2; j++) cout << "-";
            cout << "+";
        }
        cout << endl;
    }
}

// Предпросмотр файла
void previewFile(const string& filename, int linesToShow) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << COLOR_RED << "Не удалось открыть файл: " << filename << COLOR_RESET << endl;
        return;
    }

    vector<vector<string>> tableData;
    string line;
    int lineCount = 0;

    while (getline(file, line) && lineCount < linesToShow) {
        if (!line.empty()) {
            tableData.push_back(splitLine(line));
            lineCount++;
        }
    }
    file.close();

    cout << endl << COLOR_GREEN << "=== Превью файла: " << filename << " ===" << COLOR_RESET << endl;

    if (tableData.empty()) {
        cout << COLOR_RED << "Файл пуст!" << COLOR_RESET << endl;
    }
    else {
        printBeautifulTable(tableData);
        cout << COLOR_GRAY << "Показано первые " << tableData.size() << " строк" << COLOR_RESET << endl;
    }
    cout << endl;
}

// Функция сравнения 
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

//Функция создания названия для объединенного файла
string generateOutputFilename(const string& file1, const string& file2) {

    string name1 = removeTxtExtension(file1);
    string name2 = removeTxtExtension(file2);

    string baseName = name1 + "_" + name2 + "_merged";
    string outputFile = ensureTxtExtension(baseName);

    int counter = 1;
    while (ifstream(outputFile)) {
        outputFile = baseName + "(" + to_string(counter) + ").txt";
        counter++;
    }

    return outputFile;
}

// Функция вывода результата
void printResult(vector<vector<string>> result, string outputFile) {
    if (outputFile.empty()) {

        cout << endl << COLOR_GREEN << "=== Объединенная таблица ===" << COLOR_RESET << endl;
        if (result.empty()) {
            cout << COLOR_RED << "Нет данных для отображения" << COLOR_RESET << endl;
        }
        else {
            printBeautifulTable(result);
        }
    }
    else {
        // Вывод в файл (через ;)
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

