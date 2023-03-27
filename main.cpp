#include "sqlite/sqlite3.h"
#include <iostream>
#include <fstream>
#include <string>
// TODO: Create a vector to store all active tables in memory -> Be able to edit them

// To make logging easier
class LOGGING
{
private:
    std::ofstream log_file;
public:
    LOGGING(){
        log_file.open("logs.log", std::ios::app);
    }
    ~LOGGING(){
        if(log_file.is_open()){
            log_file.close();
        }
    }
    // Method for an informational log
    template<typename T>
    void log(const T& msg){
        std::cout << "[LOG] " << msg << std::endl;
        if(log_file.is_open()){
            log_file << "[LOG] " << msg << "\n";
        }
    }
    // Method for a warn log
    template<typename T>
    void warn(const T& msg){
        std::cout << "[WARN] " << msg << std::endl;
        if(log_file.is_open()){
            log_file << "[WARN] " << msg << "\n";
        }
    }
    // Method for a error log
    template<typename T>
    void error(const T& msg){
        std::cout << "[ERROR] " << msg << std::endl;
        if(log_file.is_open()){
            log_file << "[ERROR] " << msg << "\n";
        }
    }
    template<typename T>
    void print(const T& msg){
        std::cout << "[PRINT] " << msg << std::endl;
    }
    template<typename T>
    void writeToLogFile(const T& msg){
        if(log_file.is_open()){
            log_file << "[INFO] " << msg << "\n";
        }
    }
    // Function to check errors, if detects any -> logs it
    int checkForErrors(sqlite3* db,int& rc){
        error(sqlite3_errmsg(db));
        return 1;
    }

};

class DATABASE : public LOGGING
{
private:
    sqlite3* db;
    int rc;
public:

    DATABASE(const char* dbName){
        writeToLogFile("DATABASE object has been created...");
        rc = sqlite3_open(dbName, &db);
        log(dbName);
        if(rc){
            checkForErrors(db,rc);
        }
    }

    // Function to create an sqlite table
    void createTable(){
        const char* sql = "CREATE TABLE dict (czCode TEXT, btCode TEXT)";
        rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
        if(rc){
            checkForErrors(db, rc);
        }
    }
    // Function to delete an sqlite table
    void deleteTable(){
        const char* sql = "DROP TABLE dict";
        rc = sqlite3_exec(db, sql, NULL,NULL,NULL);
        if(rc){
            checkForErrors(db, rc);
        }
    }

    void addToTable(const std::string cz_code, const std::string bt_code){
        std::string sql = "INSERT INTO dict(czCode, btCode) VALUES ('" 
                            +cz_code + "','" 
                            +bt_code + "');";
        const char* sql_cstr = sql.c_str();
        rc = sqlite3_exec(db, sql_cstr,NULL,NULL,NULL);
        if(rc){
            checkForErrors(db, rc);
        }
        log(("Added to table" + std::string(cz_code) + " " + std::string(bt_code)));
    }
    void removeFromTable(const std::string cz_code){
        std::string sql = "DELETE FROM dict WHERE czCode = \"" + cz_code + "\";";
        const char* sql_cstr = sql.c_str();
        rc = sqlite3_exec(db,sql_cstr, NULL,NULL,NULL);
        if(rc){
            checkForErrors(db,rc);
        }
        log(("Removed from table " + cz_code));
    }
};

class MENU : public LOGGING
{
private:
    char chosenOption;
    DATABASE* database;
public:

    MENU(const char* _db){
        DATABASE _database(_db);
        database = &_database;
        writeToLogFile("MENU object has been created...");
    }

    void tAdd(){
        std::string cz_code;
        std::string bt_code;
        print("Add cz-code");
        std::cout << ">> ";
        std::cin >> cz_code;
        print("Add bt-code");
        std::cout << ">> ";
        std::cin >> bt_code;
        database->addToTable(cz_code, bt_code);
        displayMenu();
    }
    
    void tRemove(){
        std::string cz_code;
        print("Which code set do you want to remove (enter cz_code)");
        std::cout << ">> ";
        std::cin >> cz_code;
        database->removeFromTable(cz_code);
        displayMenu();
    }
    void displayMenu(){
        print("Database Menu");
        print("Choose one of the options");
        std::cout << "1 (Add to the table)" << std::endl;
        std::cout << "2 (Remove from the table)" << std::endl;
        std::cout << ">> ";
        std::cin >> chosenOption;
        switch (chosenOption)
        {
        case '1':
            tAdd();
            break;
        case '2':
            tRemove();
            break;
        default:
            error("Invalid option");
            break;
        }

    }

};

// Main fucntion
int main() {
    const char* _db = "beta.db";
    MENU menu(_db);    
    menu.displayMenu();
}


