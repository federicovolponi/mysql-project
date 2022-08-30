#ifndef PROJECT_DATABASE_H
#define PROJECT_DATABASE_H
#include "table.h"
#include <list>
#include <sstream>
#include <cctype>
#include <fstream>


void str_clean(string &to_clean);   // ELIMINA CARATTERI INDESIDERATI DA UNA STRINGA
bool search_char_to_delete(const char c);   // RITORNA UN BOOLEANO SE IL CARATTERE E' UNO DI QUELLI INDESIDERATI
date convert_date(const string &to_convert);    // CONVERTE UNA STRINGA IN UNA DATA E CONTROLLA CHE SIA CORRETTA
times convert_time(const string &to_convert);   // CONVERTE UNA STRINGA IN UN ORARIO E CONTROLLA CHE SIA CORRETTO
vector<vector<string>> sorting_int(vector<vector<string>> to_order,const int index, const bool order);  // ORDINAMENTO PER INTERI USANDO L'INSERTION SORT
vector<vector<string>> sorting_float(vector<vector<string>> to_order, const int index, const bool order);   // ORDINAMENTO PER NUMERI CON VIRGOLA USANDO L'INSERTION SORT
vector<vector<string>> sorting_date(vector<vector<string>> to_order, const int index, const bool order);    // ORDINAMENTO PER DATE USANDO L'INSERTION SORT
vector<vector<string>> sorting_time(vector<vector<string>> to_order, const int index, const bool order);    // ORDINAMENTO PER ORARI USANDO L'INSERTION SORT
vector<vector<string>> sorting_char(vector<vector<string>> to_order, const int index, const bool order);    // ORDINAMENTO PER CHAR USANDO L'INSERTION SORT (ORDINAMENTO BASATO SU ASCII)
vector<vector<string>> sorting_text(vector<vector<string>> to_order, const int index, const bool order);    // ORDINAMENTO PER TEXT USANDO L'INSERTION SORT IN ORDINE ALFABETICO
bool str_alphabetic_compare(const string first, const string second);   // RITORNA VERO SE LA STRINGA FIRST E' MAGGIORE (ALFABETICAMENTE) DELLA SECOND
bool compare_times(const times first, times second);    // RITORNA VERO SE L'ORARIO FIRST E' MAGGIORE DELL'ORARIO SECOND
bool compare_date(const date first, const date second); // RITORNA VERO SE LA DATA FIRST E' MAGGIORE DELLA DATA SECOND
bool is_int(const string &to_control);  // CONTROLLO CHE LA STRINGA INSERITA SIA UN NUMERO INTERO
bool is_float(const string &to_control);    // CONTROLLO CHE LA STRINGA INSERITA SIA UN NUMERO CON LA VIRGOLA
bool search_protected_word(string input);  // CONTROLLO CHE NON SIANO USATE PAROLE PROTETTE
void to_upper_string(string &to_upper); // PONE MAIUSCOLI TUTTI I CARATTERI DELLA STRINGA INSERITA

class database {
public:
    database(); // COSTRUTTORE
    ~database(); // DISTRUTTORE
    void create_table_method(); // METODO PER LA CREATE TABLE
    void insert_into_method();  // METODO PER LA INSERT INTO
    void drop_table_method();   // METODO PER LA DROP TABLE
    void truncate_method(); // METODO PER LA TRUNCATE
    void delete_from_method();  // METODO PER LA DELETE
    void update_method();   // METODO PER LA UPDATE
    string select_method(); // METODO PER LA SELECT
    void read_file();   // LETTURA DA FILE
    void save_on_file();    // SCRITTURA SU FILE

private:
    list<table> _tab_list;  // LISTA CONTENENTE TUTTE LE TABALLE DEL DATABASE
    table search_table(const string &name_table);   // RICERCA LA TABELLA IN BASE AL NOME E LA RITORNA SE PRESENTE
    bool search_table_bool(const string &name_table);   // RITORNA VERO SE E' PRESENTE LA TABELLA
    void update_table(table &tab_to_update);    // AGGIORNA LA TABELLA
    vector<int> where_command(table &tab, string &next_command); // COMANDO WHERE RITORNA GLI INDICI DELLE POSIZIONI DEI DATI DA CONSIDERARE
    ostringstream order_command(table &tab, vector<int> &row_index, vector<int> &col_index);    // COMANDO ORDER BY RITORNA LO STREAM ORDINATO DA STAMPARE
};


#endif //PROJECT_DATABASE_H
