//
//  execution.h
//  TUI
//
//  Created by Денис Зиновьев on 21.09.2018.
//  Copyright © 2018 Денис Зиновьев. All rights reserved.
//

#ifndef execution_h
#define execution_h
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>

#endif /* execution_h */

using namespace std;

sqlite3 *db;
char *zErrMsg = 0;
int rc;
char *sql;



/*--------------------------Guide for Yarik--------------------------*/
/*    Тут будут написаны некоторые примеры                           */
/*    add("menu", {"1", "pizza", "23", "1"});                        */
/*    update("menu", "id", "1", {"1", "soupe", "13", str(1)})        */
/*    delete("menu", "id", "1")                                      */
/*    take("menu", "id", "3", "price") - взять по id цену            */
/*-------------------------------------------------------------------*/








vector <string> tablesNamesList;
map <string, vector <string>> fieldsNamesList;
map <string, vector <string>> fieldsTypesList;
vector <vector <string>> wholeTable;









int find_(vector <string> v, string value) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == value) {
            return i;
        }
    }
    return -1;
}


















int num(string s) {
    stringstream ss;
    ss << s;
    int r;
    ss >> r;
    return r;
}


















string str(int n) {
    stringstream s;
    s << n;
    string r;
    s >> r;
    return r;
}

















int ii = 0;
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    wholeTable.push_back({});
    for (int i = 0; i < argc; i++) {
        wholeTable[ii].push_back(argv[i]);
    }
    ii++;
    return 0;
}

















int elementNumber;
string returnElement;
static int callbackTake(void *NotUsed, int argc, char **argv, char **azColName) {
    returnElement = argv[elementNumber];
    return 0;
}



















void createTable(string tableName, vector <string> fieldNames, vector <string> fieldTypes) {
//    Типы - int, string, intkey, stringkey
//    nn в конце - not null
//    Пример createTable("MENU", {PRICE, NAME}, {intkey, stringkeynn}); ({} - вектор)
//    string = varchar(100)
    map <string, string> mp; //Список типов для удобства
    mp = {
        {"int", "INT"},
        {"intkey", "INT PRIMARY KEY"},
        {"intnn", "INT NOT NULL"},
        {"intkeynn", "INT PRIMARY KEY NOT NULL"},
        {"string", "VARCHAR(100)"},
        {"stringkey", "VARCHAR(100) PRIMARY KEY"},
        {"stringnn", "VARCHAR(100) NOT NULL"},
        {"stringkeynn", "VARCHAR(100) PRIMARY KEY NOT NULL"},
    };
    if (fieldNames.size() != fieldTypes.size()) {
        cout << "Invalid size of fieldNames or fieldTypes\n";
        return;
    }
//    --
    tablesNamesList.push_back(tableName);
    fieldsNamesList[tableName] = fieldNames;
    fieldsTypesList[tableName] = fieldTypes;
//    --
    string sql;
    sql =
    "CREATE TABLE IF NOT EXISTS " + tableName + "(";
    for (int i = 0; i < fieldTypes.size(); i++) {
        sql = sql + fieldNames[i] + " " + mp[fieldTypes[i]];
        i != fieldTypes.size() - 1 ? sql = sql + "," : sql = sql + ");";
    }
//    --
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if (rc) {
        cout << "Can't create table " << tableName << ": \n" << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "Created table " << tableName << " successfully\n";
    }
//    --
    cout << '\n';
}

















void add(string tableName, vector <string> values) {
//    Пример add("menu", {"0", "123", "pizza", "2"});
//    Все параметры в стрингах, для удобства можно использовать str
    if (find_(tablesNamesList, tableName) == -1) {
        cout << "Can't add values to table " << tableName << ": Invalid table name\n";
        return;
    }
//    --
    string sql;
    sql =
    "INSERT INTO " + tableName + "(";
    for (int i = 0; i < fieldsNamesList[tableName].size(); i++) {
        sql = sql + fieldsNamesList[tableName][i];
        i == fieldsNamesList[tableName].size() - 1 ? sql = sql + ") " : sql = sql + ", ";
    }
    sql = sql + "VALUES (";
    for (int i = 0; i < values.size(); i++) {
        if (fieldsTypesList[tableName][i].substr(0, 6) == "string") {
            sql = sql + char(39);
        }
        sql = sql + values[i];;
        if (fieldsTypesList[tableName][i].substr(0, 6) == "string") {
            sql = sql + char(39);
        }
        i == values.size() - 1 ? sql = sql + ");" : sql = sql + ", ";
    }
//    --
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if (rc) {
        cout << "Can't add element to " << tableName << ": \n" << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "Added element to " << tableName << " successfully\n";
    }
//    --
    cout << '\n';
}

















void delete_ (string tableName, string fieldName, string fieldValue) {
//    Пример delete("menu", "id", "1") = delete from menu where id = 1
    cout << '\n';
    if (find_(fieldsNamesList[tableName], fieldName) == -1) {
        cout << "Can't delete element from table:\nInvalid field name " + fieldName + "\n";
        return;
    }
    string sql;
    sql =
    "DELETE FROM " + tableName + " WHERE " + fieldName + " = ";
    if (fieldsTypesList[tableName][find_(fieldsNamesList[tableName], fieldName)].substr(0, 6) == "string") {
        sql = sql + char(39);
    }
    sql = sql + fieldValue;
    if (fieldsTypesList[tableName][find_(fieldsNamesList[tableName], fieldName)].substr(0, 6) == "string") {
        sql = sql + char(39);
    }
//    --
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if (rc) {
        cout << "Can't delete element from " << tableName << ": \n" << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "Deleted element from " << tableName << " successfully\n";
    }
//    --
    cout << '\n';
}

















void update(string tableName, string fieldName, string fieldValue, vector <string> newValues) {
//    Пример update("menu", "id", "1", {"1", "1", "1", "1"});
    if (newValues.size() != fieldsNamesList[tableName].size()) {
        cout << "Invalid new values list size\n";
        return;
    }
    if (find_(fieldsNamesList[tableName], fieldName) == -1) {
        cout << "Invalid field name\n";
        return;
    }
//    --
    string sql;
    sql =
    "UPDATE " + tableName + " SET ";
    for (int i = 0; i < fieldsNamesList[tableName].size(); i++) {
        sql = sql + fieldsNamesList[tableName][i] + " = ";
        if (fieldsTypesList[tableName][i].substr(0, 6) == "string") {
            sql = sql + char(39);
        }
        sql = sql + newValues[i];
        if (fieldsTypesList[tableName][i].substr(0, 6) == "string") {
            sql = sql + char(39);
        }
        i != fieldsNamesList[tableName].size() - 1 ? sql = sql + ", " : sql = sql + " ";
    }
    sql = sql + "WHERE " + fieldName + " = " + fieldValue + ";";
//    --
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if (rc) {
        cout << "Can't update " << tableName << ": \n" << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "Updated " << tableName << " successfully\n";
    }
//    --
    cout << '\n';
}

















string take(string tableName, string fieldNameFind, string value, string fieldNameTake) {
    string sql;
    sql =
    "SELECT * FROM " + tableName + " WHERE " + fieldNameFind + " = " + value + ";";
//    --
    elementNumber = find_(fieldsNamesList[tableName], fieldNameTake);
    int rc = sqlite3_exec(db, sql.c_str(), callbackTake, 0, &zErrMsg);
    if (rc) {
        cout << "Can't take element from " << tableName << ": \n" << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "Taked element from " << tableName << " successfully\n";
    }
//    --
    return returnElement;
//    --
    cout << '\n';
}

















void dropTable(string tableName) {
    string sql;
    sql =
    "DROP TABLE " + tableName;
    int rc = sqlite3_exec(db, sql.c_str(), callbackTake, 0, &zErrMsg);
    if (rc) {
        cout << "Can't drop " << tableName << ": \n" << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "Dropped " << tableName << " successfully\n";
    }
}

















void addOrder(vector <pair <int, string>> mealsList) {
        
}

















vector <vector <string>> takeTable(string tableName) {
    string sql;
    sql =
    "SELECT * FROM " + tableName + ";";
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if (rc) {
        cout << "Can't take " << tableName << ": \n" << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "Taked " << tableName << " successfully\n";
    }
    return wholeTable;
}
