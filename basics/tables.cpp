#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <vector>
#include "tables.h"
#include "errs.h"
#include "../sqlite3/sqlite3.h"

const char *SymbolTable::name = "symbols";
std::vector<ColumnDef> SymbolTable::columnDefs = { "symbol_id TEXT PRIMARY KEY" };
std::vector<ColumnDef> SymbolTable::columnNames = { "symbol_id" };
sqlite3_stmt* SymbolTable::stmt = NULL;

std::string strjoin(const std::vector<std::string> strs) {
    std::stringstream outsstr;
    int i = 0;
    for (i = 0; i < strs.size()-1; i++)
		outsstr << strs[i] << ", ";
	outsstr << strs[i];
    return outsstr.str();
}

std::string strrpt(const std::string s, int n) {
    std::stringstream ss;
    int i = 0;
    for (i = 0; i < n-1; i++)
        ss << s << ",";
    ss << s;
    return ss.str();
}

void SymbolTable::init(sqlite3* db) {
	std::stringstream sqss;
    int rc;
    sqss << "DROP TABLE IF EXISTS " << SymbolTable::name;
    rc = sqlite3_exec(db, sqss.str().c_str(), NULL, NULL, NULL);
    assertwarn(rc == SQLITE_OK, "Exec error: %s", sqlite3_errmsg(db));

    sqss.str(std::string()); // clear the stringstream
    sqss << "CREATE TABLE " << SymbolTable::name << "(" << strjoin(SymbolTable::columnDefs) << ")";
    rc = sqlite3_exec(db, sqss.str().c_str(), NULL, NULL, NULL);
    assertwarn(rc == SQLITE_OK, "Exec error: %s", sqlite3_errmsg(db));
}

void SymbolTable::prep_insert(sqlite3* db) {
    std::stringstream sqss;
    sqss << "INSERT INTO " << SymbolTable::name
        << " (" << strjoin(SymbolTable::columnNames)
        << ") VALUES ("
        << strrpt("?", SymbolTable::columnNames.size())
        << ");";
    int rc = sqlite3_prepare_v2(db, sqss.str().c_str(), int(sqss.str().length()) + 1, &stmt, NULL);
    assertwarn(rc == SQLITE_OK, "Prepare error: %s", sqlite3_errmsg(db));
}

void SymbolTable::insert(const symbol & s) {
    // reset, bind, insert
    int rc;
    rc = sqlite3_reset(stmt);
    assertwarn(rc == SQLITE_OK, "Reset error: %s", sqlite3_errmsg(sqlite3_db_handle(stmt)));
    rc = sqlite3_bind_text(stmt, 1, s.c_str(), int(s.length()), NULL);
    assertwarn(rc == SQLITE_OK, "Bind error: %s", sqlite3_errmsg(sqlite3_db_handle(stmt)));
    rc = sqlite3_step(stmt);
    assertwarn(rc == SQLITE_DONE, "Step error: %s", sqlite3_errmsg(sqlite3_db_handle(stmt)));
}

std::string SymbolTable::keycol(sqlite3* db) {
    // Returns name of column if it's designated as a primary key
    auto pkcb = [](void* userdata, int numcols, char** vals, char** colnames)->int {
        if (vals[5][0] == '1')
            ((std::string *)userdata)->assign(vals[1]);
        return SQLITE_OK;
    };

    std::string pkcol;
    std::stringstream ss;
    ss << "PRAGMA table_info(" << SymbolTable::name << ");";
    int rc = sqlite3_exec(db, ss.str().c_str(), pkcb, &pkcol, NULL);
    assertwarn(rc == SQLITE_OK, "Exec error: %s", sqlite3_errmsg(db));
    return pkcol;
}

std::vector<std::string> SymbolTable::primarykeys(sqlite3 *db) {
    // Retrieve primary keys from SymbolTable
    auto pkcb = [](void* userdata, int numcols, char** vals, char** colnames)->int {
        // We expect only one column, so we just capture that
        ((std::vector<std::string>*)userdata)->push_back(vals[0]);
        return SQLITE_OK;
    };
    
    std::vector<std::string> pkvec;
    std::stringstream ss;
    ss << "SELECT " << SymbolTable::keycol(db) << " from " << SymbolTable::name << ";";
    int rc = sqlite3_exec(db, ss.str().c_str(), pkcb, &pkvec, NULL);
    assertwarn(rc == SQLITE_OK, "Exec error: %s", sqlite3_errmsg(db));
    return pkvec;
}
	

