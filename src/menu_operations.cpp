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

//Ìåíþøêà ñ ââîäîì îò ïîëüçîâàòåëÿ öèôðû
int simpleMenu(const vector<string>& items, const string& header) {
    int choice;

    while (true) {
        system("cls");
        if (!header.empty()) cout << header << "\n\n";

        // Âûâîä ïóíêòîâ ìåíþ ñ íîìåðàìè
        for (int i = 0; i < items.size(); i++) {
            cout << COLOR_GREEN << "[" << i + 1 << "] " << COLOR_RESET << items[i] << endl;
        }

        cout << "\nÂûáåðèòå ïóíêò (" << COLOR_GREEN << "1-" << items.size() << COLOR_RESET << "): ";

        if (cin >> choice && choice >= 1 && choice <= items.size()) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice - 1;
        }

        // Î÷èùàåì ïðè íåâåðíîì ââîäå
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

//Ââîä íàçâàíèÿ ôàéëîâ 
string inputFilename(const string& prompt) {
    string filename;
    while (true) {
        cout << prompt;
        getline(cin, filename);

        // Óäàëÿåì ïðîáåëû â íà÷àëå è êîíöå
        filename.erase(0, filename.find_first_not_of(" \t"));
        filename.erase(filename.find_last_not_of(" \t") + 1);

        if (filename.empty()) {
            cout << COLOR_RED << "Íàçâàíèå ôàéëà íå ìîæåò áûòü ïóñòûì!" << COLOR_RESET << endl;
            continue;
        }

        // Äîáàâëÿåì ðàñøèðåíèå .txt åñëè åãî íåò
        filename = ensureTxtExtension(filename);

        // Ïðîâåðÿåì ñóùåñòâîâàíèå ôàéëà
        ifstream testFile(filename);
        if (!testFile.is_open()) {
            cout << COLOR_RED << "Ôàéë '" << filename << "' íå íàéäåí!" << COLOR_RESET << endl;
            cout << "Ïðîâåðüòå ïðàâèëüíîñòü íàçâàíèÿ è íàëè÷èå ôàéëà." << endl;
            continue;
        }
        testFile.close();

        return filename;
    }
}

// Ôóíêöèÿ äëÿ ââîäà êëþ÷åâûõ êîëîíîê
void inputKeyColumns(int& keyColumn1, int& keyColumn2) {
    cout << "Íîìåð êëþ÷åâîãî ñòîëáöà ïåðâîé òàáëèöû: ";
    while (!(cin >> keyColumn1)) {
        cout << COLOR_RED << "Íåâåðíûé ââîä!" << COLOR_RESET << endl << "Ïîæàëóéñòà, ââåäèòå ÷èñëî: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Íîìåð êëþ÷åâîãî ñòîëáöà âòîðîé òàáëèöû: ";
    while (!(cin >> keyColumn2)) {
        cout << COLOR_RED << "Íåâåðíûé ââîä!" << COLOR_RESET << endl << "Ïîæàëóéñòà, ââåäèòå ÷èñëî: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (keyColumn1 != 0 || keyColumn2 != 0) {
        if (keyColumn1 != 0) keyColumn1 -= 1;
        if (keyColumn2 != 0) keyColumn2 -= 1;
    }
}

// Ôóíêöèÿ äëÿ ïðîâåðêè òèïîâ äàííûõ è âûïîëíåíèÿ ñîåäèíåíèÿ
bool performTableJoin(const string& table1File, const string& table2File,
    int keyColumn1, int keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result) {
    cout << endl << "Íà÷àëî ñîåäèíåíèÿ òàáëèö..." << endl << endl;

    table1 = readTable(table1File, keyColumn1);
    table2 = readTable(table2File, keyColumn2);

    if (table1.empty() || table2.empty()) {
        cout << COLOR_RED << "Îäíà èç òàáëèö ïóñòàÿ, ñîåäèíåíèå íåâîçìîæíî" << COLOR_RESET << endl;
        system("pause");
        return false;
    }

    // Ïðîâåðêà òèïîâ äàííûõ
    cout << "\nÏðîâåðêà òèïîâ äàííûõ...\n" << endl;
    string type1 = getColumnType(table1, keyColumn1);
    string type2 = getColumnType(table2, keyColumn2);

    cout << "Òèï êëþ÷åâîé êîëîíêè ïåðâîé òàáëèöû: " << COLOR_GREEN << type1 << COLOR_RESET << endl;
    cout << "Òèï êëþ÷åâîé êîëîíêè âòîðîé òàáëèöû: " << COLOR_GREEN << type2 << COLOR_RESET << endl << endl;

    if (type1 != type2 && type1 != "empty" && type2 != "empty") {
        cout << COLOR_RED << "Îøèáêà: íåñîâìåñòèìûå òèïû äàííûõ â êëþ÷åâûõ êîëîíêàõ! Ñîåäèíåíèå íåâîçìîæíî." << COLOR_RESET << endl;
        system("pause");
        return false;
    }
    if (type1 == "empty" || type2 == "empty") {
        cout << COLOR_RED << "Ïðåäóïðåæäåíèå: îäíà èç êëþ÷åâûõ êîëîíîê ïóñòàÿ!" << COLOR_RESET << endl;
        cout << "Ðåçóëüòàò ñîåäèíåíèÿ ìîæåò áûòü íåêîððåêòíûì." << endl;
    }

    result = mergeJoin(table1, table2);
    cout << endl << COLOR_GREEN << "Íàéäåíî " << result.size() << " ñîâïàäåíèé!" << COLOR_RESET << endl << endl;
    system("pause");
    return true;
}

// Ôóíêöèÿ äëÿ ïðåäïðîñìîòðà ôàéëîâ
void offerFilePreview(const string& table1File, const string& table2File) {
    vector<string> previewMenu = { "äà", "íåò" };
    int previewChoice = simpleMenu(previewMenu, "Õîòèòå ïîñìîòðåòü ïåðâûå ñòðîêè ôàéëîâ?");
    if (previewChoice == 0) {
        previewFile(table1File);
        previewFile(table2File);
    }
}

// Ôóíêöèÿ äëÿ ïîëíîãî ïðîöåññà ââîäà äàííûõ è ñîåäèíåíèÿ òàáëèö
bool setupAndJoinTables(string& table1File, string& table2File,
    int& keyColumn1, int& keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result) {
    table1File = inputFilename("Ââåäèòå íàçâàíèå ïåðâîãî ôàéëà: ");
    table2File = inputFilename("Ââåäèòå íàçâàíèå âòîðîãî ôàéëà: ");

    offerFilePreview(table1File, table2File);
    inputKeyColumns(keyColumn1, keyColumn2);

    return performTableJoin(table1File, table2File, keyColumn1, keyColumn2, table1, table2, result);
}

// Îñíîâíàÿ ëîãèêà ìåíþ âûâîäà ðåçóëüòàòà
bool handleOutputMenu(const string& table1File, const string& table2File,
    vector<vector<string>>& result, bool& isSaved) {
    vector<string> mainMenu = {
        "â ôàéë",
        "íà ýêðàí",
        "âûéòè èç ïðîãðàììû"
    };

    int choice = simpleMenu(mainMenu, "Êóäà âûâåñòè ðåçóëüòàò?");

    if (choice == 0) {
        // Ñîõðàíåíèå â ôàéë
        string outputFile = generateOutputFilename(table1File, table2File);
        printResult(result, outputFile);
        cout << endl << "Ðåçóëüòàò ñîõðàíåí â " << COLOR_GREEN << outputFile << COLOR_RESET << endl;
        isSaved = true;
        system("pause");

        vector<string> viewMenu = { "äà", "íåò" };
        int viewChoice = simpleMenu(viewMenu, "Ïîñìîòðåòü ñîõðàíåííûé ôàéë?");
        if (viewChoice == 0) {
            previewFile(outputFile, 15);
            system("pause");
        }
        return true;
    }
    else if (choice == 1) {
        // Âûâîä íà ýêðàí
        cout << endl << "Îáúåäèíåííàÿ òàáëèöà:" << endl << endl;
        printResult(result, "");
        cout << endl;
        system("pause");
        return true;
    }
    else {
        // Âûõîä èç ïðîãðàììû
        cout << "Âûõîä èç ïðîãðàììû..." << endl;
        return false;
    }
}

// Ëîãèêà ìåíþ "×òî äàëüøå?" ïîñëå âûâîäà íà ýêðàí
bool handleNextMenuAfterScreen(string& table1File, string& table2File,  // Óáðàòü const
    int& keyColumn1, int& keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result, bool& isSaved) {
    vector<string> nextMenu = {
        "îáúåäèíèòü äðóãèå òàáëèöû",
        "ñîõðàíèòü âûâåäåííóþ òàáëèöó â ôàéë",
        "âûéòè èç ïðîãðàììû"
    };

    int nextChoice = simpleMenu(nextMenu, "×òî äàëüøå?");

    if (nextChoice == 0) {
        // Ïåðåçàïóñê
        isSaved = false;
        return setupAndJoinTables(table1File, table2File, keyColumn1, keyColumn2, table1, table2, result);
    }
    else if (nextChoice == 1) {
        // Ñîõðàíåíèå
        string outputFile = generateOutputFilename(table1File, table2File);
        printResult(result, outputFile);
        cout << endl << "Ðåçóëüòàò ñîõðàíåí â " << COLOR_GREEN << outputFile << COLOR_RESET << endl;
        isSaved = true;
        system("pause");
        return true;
    }
    else {
        return false;
    }
}

// Ëîãèêà ìåíþ "×òî äàëüøå?" êîãäà ôàéë óæå ñîõðàíåí
bool handleNextMenuAfterSave(string& table1File, string& table2File,  // Óáðàòü const
    int& keyColumn1, int& keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result, bool& isSaved) {
    vector<string> nextMenu = {
        "îáúåäèíèòü äðóãèå òàáëèöû",
        "âûéòè èç ïðîãðàììû"
    };

    int nextChoice = simpleMenu(nextMenu, "×òî äàëüøå?");

    if (nextChoice == 0) {
        isSaved = false;
        return setupAndJoinTables(table1File, table2File, keyColumn1, keyColumn2, table1, table2, result);
    }
    else {
        return false;
    }
}
