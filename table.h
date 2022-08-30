#ifndef PROJECT_TABLE_H
#define PROJECT_TABLE_H
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
struct field {              //STRUTTURA PER GESTIRE LE INFORMAZIONI NECESSARIE DI UNA COLONNA (NOME, TIPO DI DATO, SE NOT NULL, SE AUTO_INCREMENT)
    string field_name;
    string field_type;
    bool NOT_NULL;
    bool AUTO_INCREMENT;
};
struct foreign {
    string name_foreign_tab;
    string name_cols;
};

struct times {              //STRUTTURA PER GESTIRE GLI ORARI
    int hour;
    int minute;
    int second;
};
struct date {               // STRUTTURA PER GESTIRE LE DATE
    int day;
    int month;
    int year;
};

class table {
public:
    table();    // COSTRUTTORE
    ~table();   // DISTRUTTORE
    table(const table& to_copy);    // COSTRUTTORE DI COPIA
    friend class database;
private:
    string _name_table;
    vector<field> _columns_def; // VETTORE CHE CONTIENE LE INFORMAZIONI SU OGNI COLONNA
    string _primary_key;
    vector<string> _foreign_key;
    vector<vector<string>> _tab_data; // VETTORE DI VETTORI DI STRINGHE CONTENENTE TUTTI I DATI ALL'INTERNO DELLA TABELLA
    int _auto_increment;
    vector<foreign> _references;

};


#endif //PROJECT_TABLE_H
