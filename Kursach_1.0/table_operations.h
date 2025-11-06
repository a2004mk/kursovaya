#pragma once

#ifndef TABLE_OPERATIONS_H
#define TABLE_OPERATIONS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#define NOMINMAX
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

// Прототипы функций
string ensureTxtExtension(const string& filename);
string removeTxtExtension(const string& filename);
vector<TableRow> readTable(string filename, int keyColumn);
string getColumnType(const vector<TableRow>& table, int columnIndex);
vector<string> splitLine(const string& line);
void printBeautifulTable(const vector<vector<string>>& data);
void previewFile(const string& filename, int linesToShow = 10);
vector<vector<string>> mergeJoin(vector<TableRow> table1, vector<TableRow> table2);
string generateOutputFilename(const string& file1, const string& file2);
void printResult(vector<vector<string>> result, string outputFile);

#endif