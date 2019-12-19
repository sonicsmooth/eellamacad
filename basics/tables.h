#ifndef TABLES_H
#define TABLES_H

#include <string>
#include <vector>
#include "../sqlite3/sqlite3.h"

/*
Main SqlTable defines table
* add column definitions
* create, drop
    Derived eg SymbolTable
    * static prepare, insert
    * return typedef map symbol

Need to create concrete row instance
* map<string, smart_ptr_to_value>

need value class hierarchy

*/



typedef std::string ColumnDef;
typedef std::string symbol;


class SymbolTable {
private:
    static const char *name;
    static std::vector<ColumnDef> columnDefs;
    static std::vector<std::string> columnNames;
	static sqlite3_stmt* stmt; // shared by all instances
public:
	static void init(sqlite3 *);
	static void drop(sqlite3 *);
    static void prep_insert(sqlite3 *);
    static void insert(const symbol &);
    static std::string keycol(sqlite3 *);
    static std::vector<std::string> primarykeys(sqlite3 *);
	virtual ~SymbolTable() {};
};







#endif