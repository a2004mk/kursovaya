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


// Ïðîâåðêà íà ðàñøèðåíèå .txt è äîáàâëåíèå .txt
string ensureTxtExtension(const string& filename) {
    if (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".txt") {
        return filename;
    }
    return filename + ".txt";
}

// Ôóíêöèÿ äëÿ óäàëåíèÿ ðàñøèðåíèÿ .txt
string removeTxtExtension(const string& filename) {
    if (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".txt") {
        return filename.substr(0, filename.length() - 4);
    }
    return filename;
}

// Ôóíêöèÿ ÷òåíèÿ òàáëèöû èç txt ôàéëà
vector<TableRow> readTable(string filename, int keyColumn) {
    vector<TableRow> table;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << COLOR_RED << "Îøèáêà îòêðûòèÿ ôàéëà: " << filename << COLOR_RESET << endl;
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
            cout << "Ïðåäóïðåæäåíèå: â ñòðîêå " << COLOR_GREEN << lineNum << COLOR_RESET << " íåò êîëîíêè " << COLOR_GREEN << keyColumn << COLOR_RESET << endl;
            row.key = "";
        }

        table.push_back(row);
    }

    file.close();
    cout << "Ïðî÷èòàíî " << COLOR_GREEN << table.size() << COLOR_RESET << " ñòðîê èç " << COLOR_GREEN << filename << COLOR_RESET << endl;
    return table;
}

// Ôóíêöèÿ äëÿ îïðåäåëåíèÿ òèïà äàííûõ êîëîíêè
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

            // Ïðîâåðÿåì, ÿâëÿåòñÿ ëè çíà÷åíèå ÷èñëîì
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

// Ôóíêöèÿ äëÿ ðàçäåëåíèÿ ñòðîê íà êîëîíêè
vector<string> splitLine(const string& line) {
    vector<string> columns;
    stringstream ss(line);
    string value;
    while (getline(ss, value, ';')) {
        columns.push_back(value);
    }
    return columns;
}

// Ôóíêöèÿ âûâîäà îòôîðìàòèðîâàííîé òàáëèöû íà êîíñîëü
void printBeautifulTable(const vector<vector<string>>& data) {
    if (data.empty()) return;

    //  ìàêñèìàëüíûå øèðèíû êîëîíîê
    vector<int> colWidths(data[0].size(), 0);
    for (const auto& row : data) {
        for (int i = 0; i < row.size() && i < colWidths.size(); i++) {
            colWidths[i] = max(colWidths[i], (int)row[i].length());
        }
    }

    // Âûâîä âåðõíåé ãðàíèöû
    cout << "+";
    for (int width : colWidths) {
        for (int j = 0; j < width + 2; j++) cout << "-";
        cout << "+";
    }
    cout << endl;

    // Âûâîä äàííûõ
    for (const auto& row : data) {
        cout << "|";
        for (int i = 0; i < row.size() && i < colWidths.size(); i++) {
            cout << " " << row[i];
            for (int j = row[i].length(); j < colWidths[i] + 1; j++) cout << " ";
            cout << "|";
        }
        cout << endl;

        // Ðàçäåëèòåëü ìåæäó ñòðîêàìè 
        cout << "+";
        for (int width : colWidths) {
            for (int j = 0; j < width + 2; j++) cout << "-";
            cout << "+";
        }
        cout << endl;
    }
}

// Ïðåäïðîñìîòð ôàéëà
void previewFile(const string& filename, int linesToShow) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << COLOR_RED << "Íå óäàëîñü îòêðûòü ôàéë: " << filename << COLOR_RESET << endl;
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

    cout << endl << COLOR_GREEN << "=== Ïðåâüþ ôàéëà: " << filename << " ===" << COLOR_RESET << endl;

    if (tableData.empty()) {
        cout << COLOR_RED << "Ôàéë ïóñò!" << COLOR_RESET << endl;
    }
    else {
        printBeautifulTable(tableData);
        cout << COLOR_GRAY << "Ïîêàçàíî ïåðâûå " << tableData.size() << " ñòðîê" << COLOR_RESET << endl;
    }
    cout << endl;
}

// Ôóíêöèÿ ñðàâíåíèÿ 
bool compareRows(TableRow a, TableRow b) {
    return a.key < b.key;
}

// Ôóíêöèÿ ñîåäèíåíèÿ òàáëèö
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

//Ôóíêöèÿ ñîçäàíèÿ íàçâàíèÿ äëÿ îáúåäèíåííîãî ôàéëà
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

// Ôóíêöèÿ âûâîäà ðåçóëüòàòà
void printResult(vector<vector<string>> result, string outputFile) {
    if (outputFile.empty()) {

        cout << endl << COLOR_GREEN << "=== Îáúåäèíåííàÿ òàáëèöà ===" << COLOR_RESET << endl;
        if (result.empty()) {
            cout << COLOR_RED << "Íåò äàííûõ äëÿ îòîáðàæåíèÿ" << COLOR_RESET << endl;
        }
        else {
            printBeautifulTable(result);
        }
    }
    else {
        // Âûâîä â ôàéë (÷åðåç ;)
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

