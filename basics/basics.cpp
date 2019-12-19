// basics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cassert>
#include <list>
#include "../sqlite3/sqlite3.h"
#include "tables.h"




int exec(sqlite3 *db, std::string & cmd) {
	sqlite3_stmt *stmt = NULL;
	int rc = sqlite3_prepare_v2(db, cmd.c_str(), int(cmd.length()) + 1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		printf("Prepare error: %s\n", sqlite3_errmsg(db));
		return rc;
	}

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		printf("Execute error: %s\n", sqlite3_errmsg(db));
		return rc;
	}

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK) {
		printf("Finalize error: %s\n", sqlite3_errmsg(db));
		return rc;
	}
	return SQLITE_OK;
}

// Execute some statements; don't assume any return rows
int exec_file(sqlite3 *db, const char *filename) {
	std::ifstream myfile(filename);
	std::stringstream buffer;
	buffer << myfile.rdbuf();
	std::string tmpstr = buffer.str();
	const char *ptr = tmpstr.c_str();
	char *last = &tmpstr.back();
	sqlite3_stmt *stmt = NULL;
	int rc;
	while (ptr < last) {
	    rc = sqlite3_prepare_v2(db, ptr, -1, &stmt, &ptr);
		if (rc != SQLITE_OK) {
			printf("Prepare error: %s\n", sqlite3_errmsg(db));
			return rc;
		}
		if (!stmt) {
			printf("Done with commands\n");
			return SQLITE_OK;
		}
		
		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			printf("Execute error: %s\n", sqlite3_errmsg(db));
			return rc;
		}

		rc = sqlite3_finalize(stmt);
		if (rc != SQLITE_OK) {
			printf("Finalize error: %s\n", sqlite3_errmsg(db));
			return rc;
		}
	}
	return rc;
}


int main()
{
    std::cout << "Hello World!\n";

	sqlite3* db = NULL;
	std::string cmds[] = {
		"DROP TABLE IF EXISTS hello",
		"CREATE TABLE hello (id INTEGER PRIMARY KEY, name TEXT)"};
	int rc;

	rc = sqlite3_open_v2("what.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (rc != SQLITE_OK)
		printf("Open error: %s\n", sqlite3_errmsg(db));
	
	//exec_file(db, "make-schlib.sql");

	SymbolTable::init(db);
	SymbolTable::prep_insert(db);
	symbol ss[] = { "res001", "res002", "res003", "res004" };
	for (auto s : ss)
		SymbolTable::insert(s);
	
	std::vector<std::string> pkvec = SymbolTable::primarykeys(db);

	sqlite3_close_v2(db);
	sqlite3_shutdown();

}



