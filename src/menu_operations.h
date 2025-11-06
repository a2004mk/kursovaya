#pragma once

#ifndef MENU_OPERATIONS_H
#define MENU_OPERATIONS_H

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "table_operations.h"

using namespace std;

// Ïðîòîòèïû ôóíêöèé ìåíþ
int simpleMenu(const vector<string>& items, const string& header);
string inputFilename(const string& prompt);
void inputKeyColumns(int& keyColumn1, int& keyColumn2);
bool performTableJoin(const string& table1File, const string& table2File,
    int keyColumn1, int keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result);
void offerFilePreview(const string& table1File, const string& table2File);
bool setupAndJoinTables(string& table1File, string& table2File,
    int& keyColumn1, int& keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result);
bool handleOutputMenu(const string& table1File, const string& table2File,
    vector<vector<string>>& result, bool& isSaved);
bool handleNextMenuAfterScreen(string& table1File, string& table2File,
    int& keyColumn1, int& keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result, bool& isSaved);
bool handleNextMenuAfterSave(string& table1File, string& table2File,
    int& keyColumn1, int& keyColumn2,
    vector<TableRow>& table1, vector<TableRow>& table2,
    vector<vector<string>>& result, bool& isSaved);

#endif
