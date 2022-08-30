#include "database.h"


database::database() = default;                                                                              //________COSTRUTTORE________//

database::~database() = default;                                                                             //________DISTRUTTORE________//


void database::create_table_method() {                                                                       //________CREATE_TABLE_______//
    table tab;
    table foreign_tab;
    string buffer;
    string sec_buffer;
    string name_table;
    string name_foreign_tab;
    string foreign_col;
    foreign buff_foreign;
    bool found;
    bool foreign_flag;
    bool flag_auto_increment = false;
    bool flag_notnull = false;
    bool foreign_control;
    bool primary_control = false;
    field buff_field;
    bool flag_in;
    vector<field>::iterator it;
    getline(cin, name_table, '(');
    str_clean(name_table);
    if(search_table_bool(name_table)){                                                                      //CONTROLLO SE ESISTE UNA TABELLA CON LO STESSO NOME
        throw invalid_argument ("A TABLE WITH THIS NAME IS ALREADY INSERTED");
    }
    if(search_protected_word(name_table)){
        throw invalid_argument("CANNOT USE A PROTECTED WORD AS THE NAME OF A TABLE");
    }
    tab._name_table = name_table;                                                                           //ASSEGNA NOME ALLA TABELLA

    do {                                                                                                    //CICLA FIN TANTO CHE NON RAGGIUNGO STRINGA D'USCITA ( ");" )
        flag_in = true;
        buff_field.AUTO_INCREMENT = false;
        buff_field.NOT_NULL = false;
        cin >> buffer;

        if (buffer != ");") {
            if (buffer != ",") {
                sec_buffer = buffer;
                to_upper_string(sec_buffer);
                if (sec_buffer != "PRIMARY" && sec_buffer != "FOREIGN") {

                    found = false;
                    for(it = tab._columns_def.begin(); (it != tab._columns_def.end()) && (!found); it++){  //CONTROLLO CHE IL NOME DELLA COLONNA NON SIA UGUALE A QUELLO DI UN'ALTRA COLONNA
                        if(buffer == (*it).field_name){
                            found = true;
                        }
                    }
                    if(found){
                        throw invalid_argument("A COLUMN WITH THIS NAME IS ALREADY INSERTED");
                    }
                    if(search_protected_word(name_table)){
                        throw invalid_argument("CANNOT USE A PROTECTED WORD AS THE NAME OF A COLUMN");
                    }
                    buff_field.field_name.assign(buffer);

                    cin >> buffer;
                    if(buffer.back() ==','){
                        buffer.pop_back();
                        flag_in = false;
                    }
                    to_upper_string(buffer);
                    if(buffer != "INT" && buffer != "FLOAT" && buffer != "DATE" && buffer != "TIME"
                       && buffer != "CHAR" && buffer != "TEXT"){                                             //CONTROLLO CHE IL TIPO  INSERITO SIA VALIDO
                        throw invalid_argument("TYPE OF FIELD INSERTED ISN'T VALID");
                    }
                    buff_field.field_type.assign(buffer);

                    tab._tab_data.emplace_back();                                                         //CREO UNA VETTORE DI STRINGHE VUOTO CORRISPONDENTE ALLA COLONNA

                    flag_auto_increment = false;
                    flag_notnull = false;
                    if(flag_in) {
                        cin >> buffer;
                        if (buffer.back() == ',') {
                            buffer.pop_back();
                            flag_in = false;
                        }
                        to_upper_string(buffer);
                        if (buffer == "NOT") {                                                           //CONTROLLO SE NOT NULL
                            flag_in = true;
                            cin >> buffer;
                            if (buffer.back() == ',') {
                                buffer.pop_back();
                                flag_in = false;
                            }
                            to_upper_string(buffer);
                            if (buffer == "NULL") {
                                buff_field.NOT_NULL = true;
                                flag_notnull = true;
                            } else
                                throw invalid_argument("WRONG SYNTAX COMMAND");
                        }
                        else if(buffer == "AUTO_INCREMENT"){                                             //CONTROLLO SE AUTO_INCREMENT
                            buff_field.AUTO_INCREMENT = true;
                            flag_auto_increment = true;
                            if (buff_field.field_type != "INT") {
                                throw logic_error("A COLUMN CAN BE AUTO INCREMENT ONLY FOR INTEGER TYPE");
                            }
                        }
                        else{
                            throw invalid_argument("WRONG SYNTAX COMMAND");
                        }

                        if (flag_in) {
                            cin >> buffer;
                            if (buffer.back() == ',')
                                buffer.pop_back();

                            to_upper_string(buffer);
                            if (buffer == "AUTO_INCREMENT" && !flag_auto_increment) {                     //CONTROLLO CHE AUTO_INCREMENT NON SIA GIA' STATO INSERITO
                                buff_field.AUTO_INCREMENT = true;
                                if (buff_field.field_type != "INT") {
                                    throw logic_error("A column can be AUTO INCREMENT only for integer type");
                                }
                            }
                            else if (buffer == "NOT" && !flag_notnull) {                                  //CONTROLLO CHE NOT NULL NON SIA GIA' STATO INSERITO
                                cin >> buffer;
                                if (buffer.back() == ',') {
                                    buffer.pop_back();
                                }
                                to_upper_string(buffer);
                                if (buffer == "NULL") {
                                    buff_field.NOT_NULL = true;
                                } else
                                    throw invalid_argument("WRONG SYNTAX COMMAND");
                            }
                            else{
                                throw invalid_argument("WRONG SYNTAX COMMAND");
                            }

                        }
                    }

                    tab._columns_def.push_back(buff_field);                                                //INSERISCO LA DEFINIZIONE DELLA COLONNA NEL VETTORE DI FIELD

                } else if (sec_buffer == "PRIMARY") {
                    if (!tab._primary_key.empty()) {                                                       //CONTROLLO CHE NON SI PROVI AD INSERIRE DUE PRIMARY KEY
                        throw logic_error("PRIMARY KEY MUST BE UNIQUE");
                    }
                    cin >> buffer;
                    to_upper_string(buffer);
                    if (buffer == "KEY") {
                        getline(cin, buffer, '(');
                        getline(cin, buffer, ')');
                        str_clean(buffer);
                        found = false;
                        for (it = tab._columns_def.begin(); it != tab._columns_def.end() && !found; it++) {
                            if (buffer == (*it).field_name) {
                                found = true;
                            }
                        }
                        if (found) {
                            tab._primary_key = buffer;                                                      //INSERISCO PRIMARY KEY
                            primary_control = true;
                        } else {
                            throw invalid_argument("THE KEY INSERTED ISN'T A FIELD OF THIS TABLE");
                        }
                    }
                }
                else if(sec_buffer == "FOREIGN") {
                    cin >> buffer;
                    to_upper_string(buffer);
                    if (buffer == "KEY") {
                        getline(cin, buffer, '(');
                        getline(cin, buffer, ')');
                        str_clean(buffer);
                        found = false;
                        for (it = tab._columns_def.begin(); it != tab._columns_def.end() && !found; it++) {
                            if (buffer == (*it).field_name) {
                                found = true;
                            }
                        }
                        foreign_control = false;
                        vector<string>::iterator iter;
                        for (iter = tab._foreign_key.begin(); iter != tab._foreign_key.end(); iter++) {
                            if (buffer == (*iter)) {
                                foreign_control = true;
                            }
                        }
                        if (buffer == tab._primary_key) {
                            foreign_control = true;
                        }
                        if (found &&
                            !foreign_control) {                                                                      //CONTROLLO CHE LA FOREIGN KEY ABBIA UN NOME UNICO E CHE SIA ASSOCIATA AD UNA COLONNA DELLA TABELLA
                            tab._foreign_key.push_back(buffer);
                        }
                        if (!found) {
                            throw invalid_argument("THE KEY INSERTED ISN'T A FIELD OF THIS TABLE");
                        }
                        if (foreign_control) {
                            throw invalid_argument("THE NAME OF THE KEY MUST BE UNIQUE");
                        }
                        cin >> buffer;
                        to_upper_string(buffer);
                        if (buffer == "REFERENCES") {
                            foreign_flag = true;
                            cin >> buffer;
                            name_foreign_tab = buffer;
                            foreign_tab = search_table(buffer);
                            getline(cin, buffer, '(');
                            getline(cin, buffer, ')');
                            str_clean(buffer);
                            found = false;
                            for (it = foreign_tab._columns_def.begin(); (it != foreign_tab._columns_def.end()) &&
                                                                        (!found); it++) {                   //CONTROLLO ESISTENZA DEL CAMPO DELLA TABELLA ESTERNA
                                if (buffer == (*it).field_name) {
                                    found = true;
                                    foreign_col = buffer;
                                }
                            }
                            if (!found) {
                                throw invalid_argument("FIELD NOT PRESENT IN FOREIGN TABLE");
                            }

                            buff_foreign.name_foreign_tab = name_foreign_tab;
                            buff_foreign.name_cols = buffer;
                            tab._references.push_back(buff_foreign);
                            buff_foreign.name_cols.clear();
                        } else {
                            throw invalid_argument("WRONG SYNTAX COMMAND");
                        }
                    }
                }
                else{
                    throw invalid_argument("WRONG SYNTAX COMMAND");
                }
            }
        } else {
            if(tab._columns_def.empty()){
                throw logic_error("CANNOT CREATE A TABLE WITHOUT AT LEAST ONE COLUMN");
            }
            if(!primary_control){
                throw logic_error("CANNOT CREATE A TABLE WITHOUT PRIMARY KEY");
            }
            (*this)._tab_list.push_back(tab);                                                              //INSERIMENTO NUOVA TABELLA NEL DATABASE
        }
    } while (buffer != ");");                                                                              //CONTROLLO SU STRINGA D'USCITA
}

void database::insert_into_method() {                                                                      //________INSERT_INTO_______//
    string buffer;
    string sec_buffer;
    vector<string> buff_col_definition;
    vector<string> buff_tab_data;
    bool text_type = false;
    bool date_type = false;
    bool time_type = false;
    bool float_type = false;
    bool int_type = false;
    bool flag_auto_incr = false;
    bool flag;
    bool found;
    int i;
    int index;
    int foreign_index = 0;
    vector<field>::iterator it;
    table tab;
    table foreign_tab;

    getline(cin, buffer, '(');
    str_clean(buffer);
    tab = (*this).search_table(buffer);                                                                  //RITORNA SE ESISTE LA TABELLA CORISPONDENTE AL NOME

    flag = false;
    do {
        cin >> buffer;                                                                                   //CICLO PER LEGGERE IN CHE COLONNE SI VOGLIONO INSERIRE I DATI
        if (buffer != ")") {
            if (buffer != ",") {
                if(buffer.back() == ',' || buffer.back() == ')'){
                    if(buffer.back() == ')'){
                        flag = true;
                    }
                    str_clean(buffer);
                    found = false;
                }
                buff_col_definition.push_back(buffer);                                                  //INSERISCO I NOMI DELLE COLONNE  IN UN VETTORE DI APPOGGIO
            }
        }
        else{
            flag = true;                                                                                //QUANDO LEGGO LA PARENTESI ESCO DAL CICLO
        }
    } while (!flag);

    getline(cin, buffer, '(');
    str_clean(buffer);
    to_upper_string(buffer);
    if (buffer == "VALUES") {

        for (i = 0; i < buff_col_definition.size(); i++) {
            text_type = false;
            date_type = false;
            time_type = false;
            int_type = false;
            float_type = false;
            found = false;
            for (it = tab._columns_def.begin(); (it != tab._columns_def.end()) && (!found); it++) {    //CONTROLLO SE LA COLONNA INSERITA IN CUI AGGIUNGERE I DATI SIA PRESENTE NELLA TABELLA
                if ((buff_col_definition[i] ==it->field_name)) {                                       //E GUARDO DI CHE TIPO E' IL DATO
                    if (it->field_type == "TEXT") {
                        text_type = true;
                    }
                    if (it->field_type == "DATE") {
                        date_type = true;
                    }
                    if (it->field_type == "TIME") {
                        time_type = true;
                    }
                    if (it->field_type == "FLOAT") {
                        float_type = true;
                    }
                    if (it->field_type == "INT") {
                        int_type = true;
                    }
                    found = true;
                }
            }
            if (!found) {
                throw invalid_argument("THE COLUMN INSERTED ISN'T PRESENT IN THIS TABLE");
            }
            if (text_type) { // ACQUISICO IL DATO SE DI TIPO TEXT
                flag = false;
                getline(cin, buffer, '"');
                getline(cin, buffer, '"');
                /* do {
                     if(flag) {
                         getline(cin, sec_buffer, '"');
                         buffer.append(sec_buffer);
                     }
                     if (buffer.back() == '\\') {
                         buffer.pop_back();
                         buffer.push_back('"');
                         flag = true;
                     }else{
                         flag = false;
                     }
                 }while(flag);*/

                buff_tab_data.push_back(buffer);                                                        //INSERISCO IL DATO IN UNA MATRICE DI APPOGGIO SE E' DI TIPO TEXT
            }
            if (i < buff_col_definition.size() - 1) {                                                   //ACQUISICO IL DATO NEL CASO GENERALE
                getline(cin, buffer, ',');
            } else {
                getline(cin, buffer, ')');
            }
            str_clean(buffer);
            if (date_type) {                                                                           //CONTROLLI SULLA COERENZA DEL DATO
                convert_date(buffer);
            }
            if (time_type) {
                convert_time(buffer);
            }
            if (float_type) {
                is_float(buffer);
            }
            if (int_type) {
                is_int(buffer);
            }
            if (!text_type) {
                buff_tab_data.push_back(buffer);                                                        //INSERISCO IL DATO IN UNA MATRICE DI APPOGGIO SE NON E' DI TIPO TEXT
            }
        }
        getline(cin, buffer, ';');

        index = 0;
        for (it = tab._columns_def.begin(); it != tab._columns_def.end(); it++) {
            found = false;
            for (i = 0; i < buff_col_definition.size() && (!found); i++) {
                if ((*it).field_name == buff_col_definition[i]) {
                    if((*it).field_name == tab._primary_key){
                        vector<string>::iterator res;
                        res = find(tab._tab_data[index].begin(), tab._tab_data[index].end(), buff_tab_data[i]);
                        if(res != tab._tab_data[index].end()){
                            throw logic_error("EVERY VALUE OF THE PRIMARY KEY COLUMN MUST BE UNIQUE");
                        }
                    }
                    for(int j = 0; j < tab._foreign_key.size(); j++){
                        if((*it).field_name == tab._foreign_key[j]){
                           foreign_tab = search_table(tab._references[j].name_foreign_tab);
                           vector<field>::iterator h;
                           for(h = foreign_tab._columns_def.begin(); h != foreign_tab._columns_def.end(); h++){
                               if((*h).field_name == tab._references[j].name_cols){
                                   vector<string>::iterator res;
                                   res = find(foreign_tab._tab_data[foreign_index].begin(), foreign_tab._tab_data[foreign_index].end(), buff_tab_data[i]);
                                   if(res == foreign_tab._tab_data[foreign_index].end()){
                                       throw logic_error("VALUE NOT FOUND IN THE FOREIGN TABLE");
                                   }
                               }
                               foreign_index++;
                           }
                        }
                    }

                    tab._tab_data[index].push_back(buff_tab_data[i]);                                   //INSERISCO I DATI NELLA TABELLA
                    found = true;
                }
            }
            if(!found){
                if(((*it).NOT_NULL) && (!(*it).AUTO_INCREMENT)){
                    throw logic_error("No element inserted in a NOT NULL column");                      //SE LA COLONNA NON E' STATA TROVATA CONTROLLO CHE NON SIA NOT NULL
                }
            }
            if ((*it).AUTO_INCREMENT) {                                                                   //CONTROLLO SE AUTO_INCREMENT E AUMENTO IL CONTATORE E LANCIO ECCEZIONE SE SI PROVA AD INSERIRE UN VALORE IN UNA COLONNA AUTO_INCREMENT
                if (!found) {
                    tab._tab_data[index].push_back(to_string(tab._auto_increment));
                    flag_auto_incr = true;
                } else {
                    throw logic_error("Can't insert a value in a AUTO_INCREMENT column");
                }
            }
            index++;
        }

        if(flag_auto_incr) {
            tab._auto_increment++;
        }
        (*this).update_table(tab);                                                                   //AGGIORNO IL DATABASE CON I NUOVI DATI DELLA TABELLA
    }else{
        throw invalid_argument("WRONG SYNTAX COMMAND");
    }
}

void database::drop_table_method(){
    bool erase = false;
    string name_to_drop;
    getline(cin.ignore(1), name_to_drop, ';');
    list<table>::iterator i = _tab_list.begin();
    for(; i != _tab_list.end() && (!erase); i++){                                                       //CERCO LA TABELLA DA ELIMINARE
        if((*i)._name_table == name_to_drop){
            _tab_list.erase(i);                                                                         //ELIMINO LA TABELLA DALLA LISTA
            erase = true;
        }
    }
    if(!erase){
        throw invalid_argument("TABLE NOT FOUND");
    }
}

void database::truncate_method() {
    string name_to_truncate;
    bool found = false;
    int size_to_reserve = 0;
    getline(cin.ignore(1), name_to_truncate, ';');
    list<table>::iterator i = _tab_list.begin();
    for(; i != (_tab_list.end()) && (!found); i++){                                                     //CERCO TABELLA DA SVUOTARE
        if((*i)._name_table == name_to_truncate){
            size_to_reserve = (*i)._columns_def.size();
            (*i)._tab_data.clear();                                                                     //PULISCO LA MATRICE CHE CONTIENE TUTTI I DATI MANTENEDO LA DIMENSIONE PER NON AVERE PROBLEMI DI INSERIMENTO NELLA INSERT INTO
            (*i)._tab_data.resize(size_to_reserve);
            found = true;
        }
    }
    if(!found){
        throw invalid_argument("TABLE NOT FOUND");
    }
}

void database::delete_from_method() {                                                                   //_______DELETE_FROM________//
    vector<int> rows_indexes;
    string buffer;
    int k;
    int index = 0;
    cin >> buffer;
    table tab = (*this).search_table(buffer);
    cin >> buffer;
    to_upper_string(buffer);
    if(buffer == "WHERE") {
        rows_indexes = where_command(tab, buffer);                                              //SALVO GLI INDICI DELLE RIGHE DA ELIMINARE
        vector<vector<string>>::iterator it = tab._tab_data.begin();

        for (it = tab._tab_data.begin(); it != tab._tab_data.end(); it++) {                            //CICLO SULLA MATRICE CONTENENTE I DATI
            index = 0;
            k = 0;
            for (vector<string>::iterator j = it->begin(); j <= it->end(); j++) {
                for (int i = 0; i < rows_indexes.size(); i++) {
                    if (index == rows_indexes[i] - k) {                                                //SE GLI INDICI CORRISPONDONO ELIMINO LA CELLA
                        it->erase(j);
                        k++;                                                                           //INCREMENTO K SICCOME OGNI VOLTA CHE ELIMINO UNA CELLA IL REALE INDICE DA CORRISPONDENTE ALLA CELLA DA ELIMINARE DIMINUISCE
                    }
                }
                index++;
            }
        }
        (*this).update_table(tab);
    }
    else{
        throw invalid_argument ("WRONG SYNTAX COMMAND");
    }
}

void database::update_method() {                                                                        //________UPDATE________//
    string buffer;
    vector<int> rows_indexes;
    bool flag_in;
    bool flag;
    int index_in = 0;
    int index_ext = 0;
    int k = 0;
    int h;
    bool found;
    bool text_type;
    bool int_type;
    bool float_type;
    bool date_type;
    bool time_type;
    bool primary_flag;
    bool foreign_flag;
    int index = 0;
    int j;
    int foreign_index = 0;
    vector<string> buff_col_name;
    vector<string> buff_data;
    table foreign_tab;

    cin >> buffer;
    table tab = (*this).search_table(buffer);
    cin >> buffer;
    to_upper_string(buffer);
    if(buffer == "SET") {
        do {
            found = false;
            text_type = false;
            time_type = false;
            float_type = false;
            int_type = false;
            date_type = false;
            primary_flag = false;
            foreign_flag = false;
            getline(cin, buffer, '=');
            str_clean(buffer);

            for (vector<field>::iterator it = tab._columns_def.begin();(it != tab._columns_def.end()) && (!found); it++) {                    //CONTROLLO SE LA COLONNA INSERITA E' PRESENTE NELLA TABELLA E VERIFICO LA COERENZA DEI DATI
                if (buffer == (*it).field_name) {
                    if((*it).AUTO_INCREMENT){
                        throw logic_error("CANNOT UPDATE AN AUTO_INCREMENT COLUMN");
                    }
                    found = true;
                    if ((*it).field_type == "TEXT") {
                        text_type = true;
                    }
                    if (it->field_type == "DATE") {
                        date_type = true;
                    }
                    if (it->field_type == "TIME") {
                        time_type = true;
                    }
                    if (it->field_type == "FLOAT") {
                        float_type = true;
                    }
                    if (it->field_type == "INT") {
                        int_type = true;
                    }
                    if(buffer == tab._primary_key){
                        primary_flag = true;
                    }
                    for( j = 0; j < tab._foreign_key.size(); j++){
                        if(buffer == tab._foreign_key[j]){}
                        foreign_flag = true;
                    }
                    j--;
                }
                index ++;
            }
            index--;
            if (found) {
                buff_col_name.push_back(buffer);
                if (text_type) {
                    getline(cin, buffer, '"');
                    getline(cin, buffer, '"');
                    flag_in = false;
                } else {
                    cin >> buffer;
                    if (buffer.back() == ',') {
                        flag_in = true;
                        buffer.pop_back();
                    }
                }
                if (date_type) {                                                                      //CONTROLLI SULLA COERENZA DEL DATO
                    convert_date(buffer);
                }
                if (time_type) {
                    convert_time(buffer);
                }
                if (float_type) {
                    is_float(buffer);
                }
                if (int_type) {
                    is_int(buffer);
                }
                if(primary_flag){
                    vector<string>::iterator res;
                    res = find(tab._tab_data[index].begin(), tab._tab_data[index].end(), buffer);
                    if(res != tab._tab_data[index].end()){
                        throw logic_error("EVERY VALUE OF THE PRIMARY KEY COLUMN MUST BE UNIQUE");
                    }
                }
                if(foreign_flag) {
                    foreign_tab = search_table(tab._references[j].name_foreign_tab);
                    vector<field>::iterator iter;
                    for (iter = foreign_tab._columns_def.begin(); iter != foreign_tab._columns_def.end(); iter++) {
                        if ((*iter).field_name == tab._references[j].name_cols) {
                            vector<string>::iterator res;
                            res = find(foreign_tab._tab_data[j].begin(),
                                       foreign_tab._tab_data[j].end(), buffer);
                            if (res == foreign_tab._tab_data[j].end()) {
                                throw logic_error("VALUE NOT FOUND IN THE FOREIGN TABLE");
                            }
                            j++;
                        }
                    }
                }

                buff_data.push_back(buffer);                                                         //INSERISCO I DATI IN UN VETTORE DI APPOGGIO
                if (!flag_in) {
                    cin >> buffer;
                    if (buffer == ",") {
                        flag_in = true;
                    } else {
                        flag_in = false;
                    }
                }
            }
            else{
                throw invalid_argument("THE COLUMN INSERTED ISN'T PRESENT IN THIS TABLE");
            }

        } while (flag_in);                                                                           //CICLO FINO A QUANDO NON TROVO PIU' VIRGOLE CHE INDICANO L'INSERIMENTO DI PIU' DATI

        to_upper_string(buffer);
        if (buffer == "WHERE") {
            rows_indexes = where_command(tab, buffer);
            vector<vector<string>>::iterator it = tab._tab_data.begin();
            for (it = tab._tab_data.begin(); it != tab._tab_data.end(); it++) {                     //CICLO SULLA MATRICE
                found = false;
                index_in = 0;
                k = 0;

                for (h = 0; (h < buff_col_name.size()) && (!found); h++) {
                    if (buff_col_name[h] == tab._columns_def[index_ext].field_name) {
                        found = true;
                        k = h;
                    }
                }

                if (found) {
                    for (vector<string>::iterator j = it->begin(); j <= it->end(); j++) {
                        for (int i = 0; i < rows_indexes.size(); i++) {
                            if (index_in == rows_indexes[i]) {
                                (*j) = buff_data[k];                                               //AGGIORNO IL DATO
                            }
                        }
                        index_in++;
                    }
                }
                index_ext++;
            }
            (*this).update_table(tab);
        }
    }
    else{
        throw invalid_argument("WRONG SYNTAX COMMAND");
    }
}

string database::select_method() {                                                              //________SELECT________//
    ostringstream stream;
    bool flag;
    vector<int> col_indexes;
    vector<int> rows_indexes;
    string name_table;
    vector<string> buff_col_def;
    int index = 0;
    bool found;
    bool flag_in;
    string buffer;
    table tab;

    cin >> buffer;
    if(buffer == "*") {                                                                          //CASO SELECT * FROM
        cin >> buffer;
        to_upper_string(buffer);
        if (buffer == "FROM") {
            cin >> buffer;

            if(buffer.back() != ';'){
                flag = true;
            }
            else{
                flag = false;
            }
            str_clean(buffer);
            tab = (*this).search_table(buffer);

            for (int i = 0; i < tab._columns_def.size(); i++) {                                //INSERISCO NELLO STREAM IL NOME DELLE COLONNE
                stream << tab._columns_def[i].field_name << '\t' <<'\t' << '\t';
            }
            stream << endl;

            if(!flag) {                                                                        //CASO SENZA ORDINAMENTO
                for (int i = 0; i < tab._tab_data[0].size(); i++) {
                    for (int j = 0; j < tab._tab_data.size(); j++) {
                        stream << tab._tab_data[j][i] << '\t' << '\t' << '\t';
                    }
                    stream << endl;
                }
            }
            else{
                cin >> buffer;
                to_upper_string(buffer);

                if(buffer == "WHERE"){                                                        //CASO CON WHERE
                    rows_indexes = where_command(tab, buffer);
                    if(buffer == ";") {                                                       //CHECK SE NON SI ARRIVA AL ; PER INSERIRE NUOVO COMANDO
                        for (int i = 0; i < rows_indexes.size(); i++) {
                            for (int j = 0; j < tab._tab_data.size(); j++) {
                                stream << tab._tab_data[j][rows_indexes[i]] << '\t' << '\t' << '\t';
                            }
                            stream << endl;
                        }
                    }
                }
                else if (buffer == "ORDER") {                                                //CASO ORDINATO
                    stream = order_command(tab, rows_indexes, col_indexes);
                }
                else{
                    throw invalid_argument ("WRONG SYNTAX ERROR");
                }
            }
        }else {
            throw invalid_argument ("WRONG SYNTAX ERROR");
        }
    } else {                                                                                //CASO SELEZIONE COLONNE

        to_upper_string(buffer);
        while (buffer != "FROM") {
            str_clean(buffer);
            buff_col_def.push_back(buffer);
            cin >> buffer;
            to_upper_string(buffer);
        }

        cin >> buffer;                                                                      //LEGGO NOME TABELLA
        if (buffer.back() == ';') {
            buffer.pop_back();
            flag_in = false;
        }
        name_table = buffer;

        tab = (*this).search_table(name_table);
        vector<field>::iterator it = tab._columns_def.begin();
        found = false;
        for (; it != tab._columns_def.end(); it++) {
            for (int i = 0; (i < buff_col_def.size()); i++) {
                if (buff_col_def[i] == it->field_name) {                                   //SALVO INDICI DELLE COLONNE DA STAMPARE E CONTROLLO SE E' STATO INSERITO CORRETTAMENTE IL NOME DELLA COLONNA
                    found = true;
                    col_indexes.push_back(index);
                }
            }
            index++;
        }
        if (!found) {
            throw invalid_argument ("THE COLUMN INSERTED ISN'T PRESENT IN THIS TABLE");
        }

        sort(col_indexes.begin(), col_indexes.end());                                      //ORDINO GLI INDICI DELLE COLONNE IN ORDINE CRESCENTE

        for (int i = 0; i < col_indexes.size(); i++) {                                     //INSERISCO NELLO STREAM I NOMI DELLE COLONNE DA STAMPARE
            stream << tab._columns_def[col_indexes[i]].field_name << '\t' << '\t' << '\t';
        }
        stream << endl;

        if (flag_in) {
            cin >> buffer;
            to_upper_string(buffer);
            if (buffer == ";") {
                flag_in = false;
            }
        }
        if (!flag_in) {                                                                     //RIEMPIO STREAM SENZA WHERE
            for (int i = 0; i < tab._tab_data[0].size(); i++) {
                for (int j = 0; j < col_indexes.size(); j++) {
                    stream << tab._tab_data[col_indexes[j]][i] << '\t' << '\t' << '\t';
                }
                stream << endl;
            }
        } else {                                                                            //RIEMPIO STREAM CON WHERE
            flag_in = true;
            if (buffer == "WHERE") {
                rows_indexes = where_command(tab, buffer);

                if (buffer == ";") {
                    for (int i = 0; i < rows_indexes.size(); i++) {
                        for (int j = 0; j < col_indexes.size(); j++) {
                            stream << tab._tab_data[col_indexes[j]][rows_indexes[i]] << '\t' << '\t' << '\t';
                        }
                        stream << endl;
                    }

                } else {                                                                     //CASO ORDINATO CON WHERE
                    if (buffer == "ORDER") {
                        stream = order_command(tab, rows_indexes, col_indexes);
                    }
                }
            } else if (buffer == "ORDER") {                                                 //CASO ORDINATO SENZA WHERE
                stream = order_command(tab, rows_indexes, col_indexes);
            }
        }
    }
    return stream.str();
}


void database::read_file() {                                                                //________READ_FILE_______//
    table tab;
    ifstream file;
    int number_of_table = 0;
    int size_string = 0;
    int j;
    int k;
    int h;
    int size_references;
    int size_foreign_cols;
    string table_name_buffer;
    int columns_def_size;
    field field_buffer;
    foreign foreign_buff;
    string primary_key_buffer;
    int tab_data_rows;
    string str_buffer;
    bool bool_buff;
    int foreign_size;
    vector<string> vect_buffer;
    char buff;

    file.open("database.bin", ios::in | ios::binary);                           //APERTURA FILE E CONTROLLO
    if (!file.is_open()) {
        throw fstream::failure("ERROR OPENING FILE");
    }

    file.read((char *) &number_of_table, sizeof(int));                                  //LETTURA NUMERO TABELLE ALL'INTERNO DEL DATABASE
    if (number_of_table == 0) {
        cout << "DATABASE IS EMPTY" << endl << endl;
    } else {
        cout << "DATABASE HAS " << number_of_table;
        if (number_of_table == 1) {
            cout << " TABLE" << endl << endl;
        } else {
            cout << " TABLES" << endl << endl;
        }

        for (int i = 0; i < number_of_table; i++) {                                     //CICLO SUL NUMERO DELLE TABELLE

            file.read((char *) &size_string, sizeof(int));                              //LETTURA NOME TABELLA
            for(j = 0; j < size_string; j++) {
                file.read((char *) &buff, sizeof(char));
                tab._name_table.push_back(buff);
            }


            file.read((char *) &size_string, sizeof(int));                              //LETTURA CHIAVE PRIMARIA
            for(j = 0; j < size_string; j++) {
                file.read((char *) &buff, sizeof(char));
                tab._primary_key.push_back(buff);
            }

            file.read((char *) &columns_def_size, sizeof(int));                        //LETTURA NUMERO DI COLONNE

            for ( k = 0; k < columns_def_size; k++) {                                  //LETTURA DI NOME TIPO E CONDIZIONI DELLE COLONNE
                file.read((char *) &size_string, sizeof(int));
                for (j = 0; j < size_string; j++) {
                    file.read((char *) &buff, sizeof(char));
                    field_buffer.field_name.push_back(buff);
                }
                file.read((char *) &size_string, sizeof(int));
                for (j = 0; j < size_string; j++) {
                    file.read((char *) &buff, sizeof(char));
                    field_buffer.field_type.push_back(buff);
                }
                file.read((char *) &field_buffer.AUTO_INCREMENT, sizeof(bool));
                file.read((char *) &field_buffer.NOT_NULL, sizeof(bool));
                tab._columns_def.push_back(field_buffer);
                field_buffer.field_name.clear();
                field_buffer.field_type.clear();
            }

            //LETTURA NUMERO RIGHE DELLA TABELLA
            file.read((char *) &tab_data_rows, sizeof(int));
            for(k = 0; k < columns_def_size; k++){                                   //LETTURA DATI ALL'INTERNO DELLA TABELLA
                tab._tab_data.emplace_back();
                for(h = 0; h < tab_data_rows; h++){
                    file.read((char *) &size_string, sizeof(int));
                    for (j = 0; j < size_string; j++) {
                        file.read((char *) &buff, sizeof(char));
                        str_buffer.push_back(buff);
                    }
                    tab._tab_data[k].push_back(str_buffer);
                    str_buffer.clear();
                }
            }

            file.read((char *) &tab._auto_increment, sizeof(int));                   //LETTURA AUTO INCREMENT

            file.read((char *) &bool_buff, sizeof(bool));                            //BOOLEANO PER VEDERE SE CI SONO DELLE FOREIGN KEY
            if(bool_buff){
                file.read((char *) &foreign_size, sizeof(int));                      //LETTURA FOREIGN KEY
                for(k = 0; k < foreign_size; k++){
                    file.read((char *) &size_string, sizeof(int));
                    for (j = 0; j < size_string; j++) {
                        file.read((char *) &buff, sizeof(char));
                        str_buffer.push_back(buff);
                    }
                    tab._foreign_key.push_back(str_buffer);
                    str_buffer.clear();
                }


                file.read((char *) &size_references, sizeof(int));
                for(k = 0; k < size_references; k++){

                    file.read((char *) &size_string, sizeof(int));
                    for(j = 0; j < size_string; j++){
                        file.read((char *) &buff, sizeof(char));
                        str_buffer.push_back(buff);
                    }
                    foreign_buff.name_foreign_tab = str_buffer;
                    str_buffer.clear();

                    file.read((char *) &size_string, sizeof(int));
                    for(j = 0; j < size_string; j++){
                        file.read((char *) &buff, sizeof(char));
                        str_buffer.push_back(buff);
                    }

                    foreign_buff.name_cols = str_buffer;
                    str_buffer.clear();


                    tab._references.push_back(foreign_buff);
                    foreign_buff.name_foreign_tab.clear();
                    foreign_buff.name_cols.clear();
                }
            }

            (*this)._tab_list.push_back(tab);                                       //INSERIMENTO IN DATABASE

            tab._name_table.clear();                                                //PULIZIA VARIABILI
            tab._primary_key.clear();
            tab._columns_def.clear();
            tab._foreign_key.clear();
            tab._tab_data.clear();

        }
    }
    file.close();
}

void database::save_on_file() {                                                         //_______SAVE_ON_FILE_______//
    ofstream file;
    int number_of_table;
    int index = 0;
    int k;
    int h;
    int size_references;
    int size_foreign_cols;
    int size_string = 0;
    int columns_def_size = 0;
    string table_name_buffer;
    field field_buffer;
    string primary_key_buffer;
    int auto_increment_buffer;
    int tab_data_rows;
    bool bool_buff;
    string str_buffer;
    int foreign_size = 0;

    file.open("database.bin", ios::out | ios::binary);                        //APERTURA FILE E CONTROLLO
    if (!file.is_open()) {
        throw fstream::failure("ERROR OPENING FILE");
    }

    number_of_table = _tab_list.size();
    file.write((char *) &number_of_table, sizeof(int));                                 //SCRITTURA NUMERO TABELLE

    list<table>::iterator i = _tab_list.begin();
    for (; i != _tab_list.end() && index < number_of_table; i++) {
        string::iterator j;

        size_string = (*i)._name_table.size();
        file.write((char *) &size_string, sizeof(int));                                 //SCRITTURA NOME TABELLA
        for (j = (*i)._name_table.begin(); j != (*i)._name_table.end(); j++) {
            file.write((char *) &(*j), sizeof(char));

        }

        size_string = (*i)._primary_key.size();
        file.write((char *) &size_string, sizeof(int));                                 //SCRITTURA PRIMARY KEY
        for (j = (*i)._primary_key.begin(); j != (*i)._primary_key.end(); j++) {
            file.write((char *) &(*j), sizeof(char));
        }

        columns_def_size = (*i)._columns_def.size();                                    //SCRITTURA NUMERO COLONNE
        file.write((char * ) &columns_def_size, sizeof(int));
        for ( k = 0; k < columns_def_size; k++) {                                       //SCRITTURA INFORMAZIONI SULLE COLONNE
            size_string = (*i)._columns_def[k].field_name.size();
            file.write((char *) &size_string, sizeof(int));
            for (j = (*i)._columns_def[k].field_name.begin(); j != (*i)._columns_def[k].field_name.end(); j++) {
                file.write((char *) &(*j), sizeof(char));
            }

            size_string = (*i)._columns_def[k].field_type.size();
            file.write((char *) &size_string, sizeof(int));
            for (j = (*i)._columns_def[k].field_type.begin(); j != (*i)._columns_def[k].field_type.end(); j++) {
                file.write((char *) &(*j), sizeof(char));
            }

            file.write((char *) &(*i)._columns_def[k].AUTO_INCREMENT, sizeof(bool));
            file.write((char *) &(*i)._columns_def[k].NOT_NULL, sizeof(bool));
        }

        vector<vector<string>>::iterator col = (*i)._tab_data.begin();
        vector<string>::iterator row = col->begin();

        tab_data_rows = (*i)._tab_data[0].size();
        file.write((char *) &tab_data_rows, sizeof(int));                              //SCRITTURA RIGHE TABELLA

        for(; col != (*i)._tab_data.end(); col++){                                     //SCRITTURA DATI TABELLA
            for(row = col->begin(); row != col->end(); row++){
                size_string = row->size();
                file.write((char *) &size_string, sizeof(int));
                for (j = row->begin(); j != row ->end(); j++) {
                    file.write((char *) &(*j), sizeof(char));
                }
            }
        }

        auto_increment_buffer = (*i)._auto_increment;                                 //SCRITTURA AUTO INCREMENT
        file.write((char *) &auto_increment_buffer, sizeof(int));

        if((*i)._foreign_key.empty()){                                                //SCRITTURA PARAMETRO DI CONTROLLO SULL'ESISTENZA DI FOREIGN KEY
            bool_buff = false;
            file.write((char *) &bool_buff, sizeof(bool));
        }
        else{                                                                         //SCRITTURA FOREIGN KEY SE ESISTE
            bool_buff = true;
            file.write((char *) &bool_buff, sizeof(bool));

            foreign_size = (*i)._foreign_key.size();
            file.write((char *) &foreign_size, sizeof(int));

            for(k = 0; k < foreign_size; k++){
                size_string = (*i)._foreign_key[k].size();
                file.write((char *) &size_string, sizeof(int));
                for (j = (*i)._foreign_key[k].begin(); j != (*i)._foreign_key[k].end(); j++) {
                    file.write((char *) &(*j), sizeof(char));
                }
            }

            size_references = (*i)._references.size();
            file.write((char *) &size_references, sizeof(int));
            for(k = 0; k < size_references; k++){
                size_string = (*i)._references[k].name_foreign_tab.size();
                file.write((char *) &size_string, sizeof(int));
                for(j = (*i)._references[k].name_foreign_tab.begin(); j != (*i)._references[k].name_foreign_tab.end(); j++){
                    file.write((char *) &(*j), sizeof(char));
                }


                size_string = (*i)._references[k].name_cols.size();
                file.write((char *) &size_string, sizeof(int));
                for(j = (*i)._references[k].name_cols.begin(); j != (*i)._references[k].name_cols.end(); j++){
                    file.write((char *) &(*j), sizeof(char));
                }

            }
        }

    }
    file.close();
}



table database::search_table(const string &name_table) {
    table output_tab;
    bool found = false;
    list<table>::iterator i = _tab_list.begin();
    for(; i != _tab_list.end(); i++){                                                  //CERCA LA TABELLA PER NOME E SE NON ESISTE LANCIA ECCEZIONE
        if((*i)._name_table == name_table){
            output_tab = (*i);
            found = true;
        }
    }
    if(!found){
        throw invalid_argument("TABLE NOT IN DATABASE");
    }
    return (output_tab);

}

void database::update_table(table &tab_to_update) {
    list<table>::iterator i = _tab_list.begin();
    for(; i != _tab_list.end(); i++){                                                 //CERCA LA TABELLA PER NOME E L'AGGIORNA
        if((*i)._name_table == tab_to_update._name_table){
            (*i) = tab_to_update;
        }
    }
}

vector<int> database::where_command(table &tab, string &next_command) {               //________WHERE_COMMAND________//
    vector<int> indexes;
    string buffer;
    string to_search;
    string to_search_bet;
    int index = 0;
    bool equal = false;
    bool less = false;
    bool greater = false;
    bool different = false;
    bool found = false;
    bool text_type = false;
    bool int_type = false;
    bool float_type = false;
    bool date_type = false;
    bool time_type = false;
    bool char_type = false;
    bool between = false;

    cin.ignore(1) >> buffer;
    vector<field>::iterator it = tab._columns_def.begin();
    for(; (it != tab._columns_def.end()) && (!found); it++){                          //CICLO PER VEDERE SE IL NOME DELLA COLONNA E' CORRETTO E DI CHE TIPO E' QUEST'ULTIMA
        if((*it).field_name == buffer){
            found = true;
            if((*it).field_type == "TEXT"){
                text_type = true;
            }
            if((*it).field_type == "INT"){
                int_type = true;
            }
            if((*it).field_type == "FLOAT"){
                float_type = true;
            }
            if((*it).field_type == "DATE"){
                date_type = true;
            }
            if((*it).field_type == "TIME"){
                time_type = true;
            }
            if((*it).field_type == "CHAR"){
                char_type = true;
            }
        }
        index++;
    }
    index--;

    if(found){

        cin >> buffer;                                                              //LEGGO L' OPERATORE DA UTILIZZARE
        if(buffer == "=")
            equal = true;
        if(buffer == "<>")
            different = true;
        if(buffer == "<")
            less = true;
        if(buffer == ">")
            greater = true;
        if(buffer == ">="){
            greater = true;
            equal = true;
        }
        if(buffer == "<="){
            less = true;
            equal = true;
        }
        if(buffer == "BETWEEN"){
            between = true;
        }

        if (text_type) {                                                            //CONTROLLO SE TEXT PER FARE ACQUISIZIONE
            getline(cin, buffer, '"');
            getline(cin, buffer, '"');
            str_clean(buffer);

            if(between){                                                           //CONTROLLO SE E'BETWEEN PER ACQUISIRE CORRETTAMENTE
                to_search_bet = buffer;
                cin >> buffer;
                if(buffer == "AND"){
                    getline(cin, buffer, '"');
                    getline(cin, buffer, '"');
                }
            }
            str_clean(buffer);
            to_search = buffer;

            cin >> next_command;                                                   //SALVO IN NEXT COMMAND L'EVENTUALE OPERAZIONE DA FARE SUCCESSIVAMENTE NEL CASO NON SIA STATO LETTO ";"
            to_upper_string(next_command);

        } else {                                                                   //LETTURA SE NON E' TEXT
            cin >> buffer;

            if(between){                                                           //CONTROLLO SE E'BETWEEN PER ACQUISIRE CORRETTAMENTE
                to_search_bet = buffer;
                cin >> buffer;
                if(buffer == "AND"){
                    cin >> buffer;
                }
            }
            if (buffer.back() == ';') {
                buffer.pop_back();
                next_command = ";";
            }
            else{
                cin >> next_command;
            }
            str_clean(buffer);
            to_search = buffer;
        }
        //SEGUONO CASI DEI VARI OPERATORI UTILIZZABILI E GESTIONE DEL COMPORTAMENTO DEGLI OPERATORI PER I DIVERSI TIPI
        if(equal && greater){
            if(char_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (to_search.at(0) <= tab._tab_data[index][i].at(0)) {
                        indexes.push_back(i);
                    }
                }
            }
            if(int_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (stoi(to_search) <= stoi(tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(float_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (stof(to_search) <= stof(tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(text_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (!str_alphabetic_compare(to_search,tab._tab_data[index][i]) || to_search == tab._tab_data[index][i]) {
                        indexes.push_back(i);
                    }
                }
            }
            if(time_type){
                times t_to_search = convert_time(to_search);
                times t_tab_data;
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    t_tab_data = convert_time(tab._tab_data[index][i]);
                    if(!compare_times(t_to_search, t_tab_data) || to_search == tab._tab_data[index][i]){
                        indexes.push_back(i);
                    }
                }
            }
            if(date_type){
                date d_to_search = convert_date(to_search);
                date d_tab_data;
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    d_tab_data = convert_date(tab._tab_data[index][i]);
                    if(!compare_date(d_to_search, d_tab_data) || to_search == tab._tab_data[index][i]){
                        indexes.push_back(i);
                    }
                }
            }
            equal = false;
            greater = false;
        }
        if(equal && less){
            if(char_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (to_search.at(0) >= tab._tab_data[index][i].at(0)) {
                        indexes.push_back(i);
                    }
                }
            }
            if(int_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (stoi(to_search) >= stoi(tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(float_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (stof(to_search) >= stof(tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(text_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (str_alphabetic_compare(to_search,tab._tab_data[index][i]) || to_search == tab._tab_data[index][i]) {
                        indexes.push_back(i);
                    }
                }
            }
            if(date_type){
                date d_to_search = convert_date(to_search);
                date d_tab_data;
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    d_tab_data = convert_date(tab._tab_data[index][i]);
                    if(compare_date(d_to_search, d_tab_data) || to_search == tab._tab_data[index][i]){
                        indexes.push_back(i);
                    }
                }
            }
            if(time_type){
                times t_to_search = convert_time(to_search);
                times t_tab_data;
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    t_tab_data = convert_time(tab._tab_data[index][i]);
                    if(compare_times(t_to_search, t_tab_data) || to_search == tab._tab_data[index][i]){
                        indexes.push_back(i);
                    }
                }
            }
            equal = false;
            less = false;
        }
        if(equal) {
            for (int i = 0; i < tab._tab_data[index].size(); i++) {
                if (to_search == tab._tab_data[index][i]) {
                    indexes.push_back(i);
                }
            }
        }
        if(different){
            for (int i = 0; i < tab._tab_data[index].size(); i++) {
                if (to_search != tab._tab_data[index][i]) {
                    indexes.push_back(i);
                }
            }
        }
        if(less){
            if(char_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (to_search.at(0) > tab._tab_data[index][i].at(0) ) {
                        indexes.push_back(i);
                    }
                }
            }
            if(int_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (stoi(to_search) > stoi(tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(float_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (stof(to_search) > stof(tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(text_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (str_alphabetic_compare(to_search,tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(date_type){
                date d_to_search = convert_date(to_search);
                date d_tab_data;
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    d_tab_data = convert_date(tab._tab_data[index][i]);
                    if(compare_date(d_to_search, d_tab_data)){
                        indexes.push_back(i);
                    }
                }
            }
            if(time_type){
                times t_to_search = convert_time(to_search);
                times t_tab_data;
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    t_tab_data = convert_time(tab._tab_data[index][i]);
                    if(compare_times(t_to_search, t_tab_data)){
                        indexes.push_back(i);
                    }
                }
            }
        }
        if(greater){
            if(char_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (to_search.at(0) < tab._tab_data[index][i].at(0) ) {
                        indexes.push_back(i);
                    }
                }
            }
            if(int_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (stoi(to_search) < stoi(tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(float_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (stof(to_search) < stof(tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(text_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (!str_alphabetic_compare(to_search,tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(date_type){
                date d_to_search = convert_date(to_search);
                date d_tab_data;
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    d_tab_data = convert_date(tab._tab_data[index][i]);
                    if(!compare_date(d_to_search, d_tab_data)){
                        indexes.push_back(i);
                    }
                }
            }
            if(time_type){
                times t_to_search = convert_time(to_search);
                times t_tab_data;
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    t_tab_data = convert_time(tab._tab_data[index][i]);
                    if(!compare_times(t_to_search, t_tab_data)){
                        indexes.push_back(i);
                    }
                }
            }
        }
        if(between){
            if(char_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (to_search_bet.at(0) < tab._tab_data[index][i].at(0) && to_search.at(0) > tab._tab_data[index][i].at(0)) {
                        indexes.push_back(i);
                    }
                }
            }
            if(int_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (stoi(to_search_bet) < stoi(tab._tab_data[index][i]) && stoi(to_search) > stoi(tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(float_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (stof(to_search_bet) < stof(tab._tab_data[index][i]) && stof(to_search) > stof(tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(text_type){
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    if (!str_alphabetic_compare(to_search_bet,tab._tab_data[index][i]) && str_alphabetic_compare(to_search,tab._tab_data[index][i])) {
                        indexes.push_back(i);
                    }
                }
            }
            if(date_type){
                date d_to_search_bet = convert_date(to_search_bet);
                date d_to_search = convert_date(to_search);
                date d_tab_data;
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    d_tab_data = convert_date(tab._tab_data[index][i]);
                    if(!compare_date(d_to_search_bet, d_tab_data) && compare_date(d_to_search, d_tab_data) ){
                        indexes.push_back(i);
                    }
                }
            }
            if(time_type){
                times t_to_search_bet = convert_time(to_search_bet);
                times t_to_search = convert_time(to_search);
                times t_tab_data;
                for (int i = 0; i < tab._tab_data[index].size(); i++) {
                    t_tab_data = convert_time(tab._tab_data[index][i]);
                    if(!compare_times(t_to_search_bet, t_tab_data) && compare_times(t_to_search, t_tab_data)){
                        indexes.push_back(i);
                    }
                }
            }
        }

    }
    else{
        throw invalid_argument("THE COLUMN INSERTED ISN'T PRESENT IN THIS TABLE");
    }
    if(indexes.empty()){
        throw invalid_argument("VALUE NOT FOUND WITH WHERE COMMAND");
    }
    return indexes;
}

ostringstream database::order_command(table &tab, vector<int> &row_index, vector<int> &col_index) { //________ORDER_COMMAND________//
    ostringstream  stream;
    string buffer;
    vector<vector<string>> tab_data_print;
    bool control = false;
    int index = 0;
    int k = 0;
    bool found = false;
    bool int_type = false;
    bool float_type = false;
    bool text_type = false;
    bool char_type = false;
    bool date_type = false;
    bool time_type = false;
    cin >> buffer;
    to_upper_string(buffer);
    if(buffer == "BY"){

        cin >> buffer;
        vector<field>::iterator it = tab._columns_def.begin();
        for(; (it != tab._columns_def.end()) && (!found); it++){                                    //CONTROLLO ESISTENZA COLONNA E TIPO DEL DATO
            if((*it).field_name == buffer){
                found = true;
                if((*it).field_type == "INT")
                    int_type = true;
                if((*it).field_type == "FLOAT")
                    float_type = true;
                if((*it).field_type == "TEXT")
                    text_type = true;
                if((*it).field_type == "CHAR")
                    char_type = true;
                if((*it).field_type == "DATE")
                    date_type = true;
                if((*it).field_type == "TIME")
                    time_type = true;
            }
            index++;
        }
        if(!found){
            throw invalid_argument("THE COLUMN INSERTED ISN'T PRESENT IN THIS TABLE");
        }
        index--;
        cin >> buffer;
        buffer.pop_back();
        to_upper_string(buffer);
        //SEGUONO CHIAMATE ALLE FUNZIONI DI ORDINAMENTO SIA ASC CHE DESC PER OGNI TIPO DI DATO
        if(buffer == "DESC"){
            control = true;
            if(int_type){
                tab_data_print = sorting_int(tab._tab_data, index, false);
            }
            if(float_type){
                tab_data_print = sorting_float(tab._tab_data, index, false);
            }
            if(date_type){
                tab_data_print = sorting_date(tab._tab_data, index, false);
            }
            if(time_type){
                tab_data_print = sorting_time(tab._tab_data, index, false);
            }
            if(char_type){
                tab_data_print = sorting_char(tab._tab_data, index, false);
            }
            if(text_type){
                tab_data_print = sorting_text(tab._tab_data, index, false);
            }
        }
        else if(buffer == "ASC"){
            control = true;
            if(int_type){
                tab_data_print = sorting_int(tab._tab_data, index, true);
            }
            if(float_type){
                tab_data_print = sorting_float(tab._tab_data, index, true);
            }
            if(date_type){
                tab_data_print = sorting_date(tab._tab_data, index, true);
            }
            if(time_type){
                tab_data_print = sorting_time(tab._tab_data, index, true);
            }
            if(char_type){
                tab_data_print = sorting_char(tab._tab_data, index, true);
            }
            if(text_type){
                tab_data_print = sorting_text(tab._tab_data, index, true);
            }
        }else{
            throw invalid_argument("WRONG SYNTAX COMMAND");
        }

        if(row_index.empty() && col_index.empty()){                                                 //RIEMPIO LO STREAM CON I DATI ORDINATI COME RICHIESTO
            for (int i = 0; i < tab._columns_def.size(); i++) {
                stream << tab._columns_def[i].field_name << '\t' << '\t';
            }
            stream << endl;
            for (int i = 0; i < tab_data_print[0].size(); i++) {
                for (int j = 0; j < tab_data_print.size(); j++) {
                    stream << tab_data_print[j][i] << '\t' << '\t';
                }
                stream << endl;
            }
        }

        else if(row_index.empty() && !col_index.empty()) {
            for (int i = 0; i < col_index.size(); i++) {
                stream << tab._columns_def[col_index[i]].field_name << '\t' << '\t';
            }
            stream << endl;
            for (int i = 0; i < tab_data_print[0].size(); i++) {
                for (int j = 0; j < col_index.size(); j++) {
                    stream << tab_data_print[col_index[j]][i] << '\t' << '\t';
                }
                stream << endl;
            }
        }

        else if(!row_index.empty() && !col_index.empty()){
            for (int i = 0; i < col_index.size(); i++) {
                stream << tab._columns_def[col_index[i]].field_name << '\t' << '\t';
            }
            stream << endl;
            for (int i = 0; i < row_index.size(); i++) {
                for (int j = 0; j < col_index.size(); j++) {
                    stream << tab_data_print[col_index[j]][row_index[i]] << '\t' << '\t';
                }
                stream << endl;
            }
        }

        else if (!row_index.empty() && col_index.empty()){
            for (int i = 0; i < tab._columns_def.size(); i++) {
                stream << tab._columns_def[i].field_name << '\t' << '\t';
            }
            stream << endl;

            for (int i = 0; i < row_index.size(); i++) {
                for (int j = 0; j < tab_data_print.size(); j++) {
                    stream << tab_data_print[j][row_index[i]] << '\t' << '\t';
                }
                stream << endl;
            }
        }


    }
    else{
        throw invalid_argument("WRONG SYNTAX COMMAND");
    }
    return stream;
}

bool database::search_table_bool(const string &name_table) {                                            //________SEARCH_TABLE_BOOL________//
    list<table>::iterator i = _tab_list.begin();
    for(; i != _tab_list.end(); i++){                                                                   //RITORNA VERO SE TROVA LA TABELLA
        if((*i)._name_table == name_table){
            return true;
        }
    }
    return false;
}

//SEGUONO FUNZIONI DI ORDINAMENTO USANDO INSERTION SORT PER TUTTI I TIPI

vector<vector<string>> sorting_int(vector<vector<string>> to_order, const int index, const bool order){ //________SORTING_INT________//
    int i = 0;
    int j = 0;
    int k = 0;
    vector<int> vect;
    int temp;
    vector<string> temp_str;
    for(i = 0; i < to_order[index].size(); i++){
        vect.push_back(stoi(to_order[index][i]));
    }
    for(i = 1; i < to_order[index].size(); i++){
        temp = vect[i];
        temp_str.clear();
        for(k = 0; k < to_order.size(); k++){
            temp_str.push_back(to_order[k][i]);
        }
        j = i -1;
        if(order){
            while((vect[j] > temp) && (j >= 0)){
                vect[j+1] = vect[j];
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
            }
        }
        else{
            while((vect[j] < temp) && (j >= 0)){
                vect[j+1] = vect[j];
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
            }
        }

        for(k = 0; k < to_order.size(); k++){
            to_order[k][j+1] = temp_str[k];
        }
        vect[j+1] = temp;
    }
    return to_order;
}

vector<vector<string>> sorting_float(vector<vector<string>> to_order, const int index, const bool order){  //________SORTING_FLOAT________//
    int i = 0;
    int j = 0;
    int k = 0;
    vector<float> vect;
    float temp;
    vector<string> temp_str;
    for(i = 0; i < to_order[index].size(); i++){
        vect.push_back(stof(to_order[index][i]));
    }
    for(i = 1; i < to_order[index].size(); i++){
        temp = vect[i];
        temp_str.clear();
        for(k = 0; k < to_order.size(); k++){
            temp_str.push_back(to_order[k][i]);
        }
        j = i -1;
        if(order){
            while((vect[j] > temp) && (j >= 0)){
                vect[j+1] = vect[j];
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
            }
        }
        else{
            while((vect[j] < temp) && (j >= 0)){
                vect[j+1] = vect[j];
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
            }
        }

        for(k = 0; k < to_order.size(); k++){
            to_order[k][j+1] = temp_str[k];
        }
        vect[j+1] = temp;
    }
    return to_order;
}

vector<vector<string>> sorting_date(vector<vector<string>> to_order, const int index, const bool order){   //________SORTING_DATE________//
    date temp;
    int i;
    int j;
    int k;
    vector<string> temp_str;
    bool stop = false;
    vector<date> vect;
    for(i = 0; i < to_order[index].size(); i++){
        vect.push_back(convert_date(to_order[index][i]));
    }

    for(i = 1; i < to_order[index].size(); i++){
        temp = vect[i];
        temp_str.clear();
        for(k = 0; k < to_order.size(); k++){
            temp_str.push_back(to_order[k][i]);
        }
        j = i - 1;
        stop = false;
        if(order) {
            while (compare_date(vect[j],temp) && (j >= 0) && (!stop)) {
                vect[j + 1].year = vect[j].year;
                vect[j + 1].month = vect[j].month;
                vect[j + 1].day = vect[j].day;
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
            }
        }
        else{
            while (!compare_date(vect[j],temp) && (j >= 0) && (!stop)) {
                vect[j + 1].year = vect[j].year;
                vect[j + 1].month = vect[j].month;
                vect[j + 1].day = vect[j].day;
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
            }
        }
        for(k = 0; k < to_order.size(); k++){
            to_order[k][j+1] = temp_str[k];
        }
        vect[j+1] = temp;
    }
    return to_order;
}

vector<vector<string>> sorting_time(vector<vector<string>> to_order, const int index, const bool order){  //________SORTING_TIME________//
    times temp;
    int i;
    int j;
    int k;
    vector<string> temp_str;
    bool stop = false;
    vector<times> vect;
    for(i = 0; i < to_order[index].size(); i++){
        vect.push_back(convert_time(to_order[index][i]));
    }
    for(i = 1; i < to_order[index].size(); i++){
        temp = vect[i];
        temp_str.clear();
        for(k = 0; k < to_order.size(); k++){
            temp_str.push_back(to_order[k][i]);
        }
        j = i - 1;
        stop = false;
        if(order) {
            while (compare_times(vect[j], temp) && (j >= 0) && (!stop)) {
                /*if (vect[j].hour == temp.hour) {
                    if (vect[j].minute <= temp.minute) {
                        if (vect[j].minute == temp.minute) {
                            if (vect[j].second <= temp.second) {
                                vect[j + 1].second = vect[j].second;
                                for( k = 0; k < to_order.size(); k++){
                                    to_order[k][j+1] = to_order[k][j];
                                }
                                j--;
                            } else {
                                stop = true;
                            }
                        } else {
                            vect[j + 1].minute = vect[j].minute;
                            vect[j + 1].second = vect[j].second;
                            for( k = 0; k < to_order.size(); k++){
                                to_order[k][j+1] = to_order[k][j];
                            }
                            j--;
                        }
                    } else {
                        stop = true;
                    }
                } else {}*/
                vect[j + 1].hour = vect[j].hour;
                vect[j + 1].minute = vect[j].minute;
                vect[j + 1].second = vect[j].second;
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
            }
        }
        else{
            while (!compare_times(vect[j], temp) && (j >= 0) && (!stop)) {
                /*if (vect[j].hour == temp.hour) {
                    if (vect[j].minute <= temp.minute) {
                        if (vect[j].minute == temp.minute) {
                            if (vect[j].second <= temp.second) {
                                vect[j + 1].second = vect[j].second;
                                for( k = 0; k < to_order.size(); k++){
                                    to_order[k][j+1] = to_order[k][j];
                                }
                                j--;
                            } else {
                                stop = true;
                            }
                        } else {
                            vect[j + 1].minute = vect[j].minute;
                            vect[j + 1].second = vect[j].second;
                            for( k = 0; k < to_order.size(); k++){
                                to_order[k][j+1] = to_order[k][j];
                            }
                            j--;
                        }
                    } else {
                        stop = true;
                    }
                } else {}*/
                vect[j + 1].hour = vect[j].hour;
                vect[j + 1].minute = vect[j].minute;
                vect[j + 1].second = vect[j].second;
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
            }
        }
        for(k = 0; k < to_order.size(); k++){
            to_order[k][j+1] = temp_str[k];
        }
        vect[j+1] = temp;
    }
    return to_order;
}

vector<vector<string>> sorting_char(vector<vector<string>> to_order, const int index, const bool order) {   //_______SORTING_CHAR________//
    int i = 0;
    int j = 0;
    int k = 0;
    vector<char> vect;
    char  temp;
    vector<string> temp_str;
    for(i = 0; i < to_order[index].size(); i++){
        vect.push_back(to_order[index][i].at(0));
    }
    for(i = 1; i < to_order[index].size(); i++){
        temp = vect[i];
        temp_str.clear();
        for(k = 0; k < to_order.size(); k++){
            temp_str.push_back(to_order[k][i]);
        }
        j = i -1;
        if(order){
            while((vect[j] > temp) && (j >= 0)){
                vect[j+1] = vect[j];
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
            }
        }
        else{
            while((vect[j] < temp) && (j >= 0)){
                vect[j+1] = vect[j];
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
            }
        }

        for(k = 0; k < to_order.size(); k++){
            to_order[k][j+1] = temp_str[k];
        }
        vect[j+1] = temp;
    }
    return to_order;
}

vector<vector<string>> sorting_text(vector<vector<string>> to_order, const int index, const bool order){    //________SORTING_CHAR________//
    int i = 0;
    int j = 0;
    int k = 0;
    int h = 0 ;
    vector<string> vect;
    string temp;
    vector<string> temp_str;
    for(i = 0; i < to_order[index].size(); i++){
        vect.push_back(to_order[index][i]);
    }
    for(i = 1; i < to_order[index].size(); i++){
        temp = vect[i];
        temp_str.clear();
        for(k = 0; k < to_order.size(); k++){
            temp_str.push_back(to_order[k][i]);
        }
        j = i -1;
        h = j;
        if(order){
            while(str_alphabetic_compare(vect[h], temp) && (j >= 0)){
                vect[j+1] = vect[j];
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
                h = j;
                if(h < 0)
                    h = 0;
            }
        }
        else{
            while((!str_alphabetic_compare(vect[j], temp)) && (j >= 0)){
                vect[j+1] = vect[j];
                for( k = 0; k < to_order.size(); k++){
                    to_order[k][j+1] = to_order[k][j];
                }
                j--;
            }
        }
        for(k = 0; k < to_order.size(); k++){
            to_order[k][j+1] = temp_str[k];
        }
        vect[j+1] = temp;
    }
    return to_order;
}


bool str_alphabetic_compare(const string first, const string second){                                      //________STR_ALPHABETIC_COMPARE________//
    string min;
    bool ret;
    bool flag = false;
    if(first < second){
        min = first;
    }
    else{
        min = second;
    }
    for(int i = 0; (i < min.size()) && (!flag); i++){
        if(toupper(first[i]) > toupper(second[i])){
            return true;
            flag = true;
        }
        else if(toupper(first[i]) < toupper(second[i])){
            return false;
            flag = true;
        }
    }
    return true;
}

times convert_time(const string &to_convert){                                                             //________CONVERT_TIME________//
    times converted;
    char hour[2], minute[2], second[2];
    to_convert.copy(hour, 2, 0);
    to_convert.copy(minute, 2, 3);
    to_convert.copy(second, 2, 6);
    converted.hour = atoi(hour);
    if(converted.hour < 0 || converted.hour > 23){
        throw invalid_argument("HOUR INSERTED IS NOT VALID");
    }
    converted.minute = atoi(minute);
    if(converted.minute < 0 || converted.minute > 59){
        throw invalid_argument("MINUTE INSERTED IS NOT VALID");
    }
    converted.second = atoi(second);
    if(converted.second < 0 || converted.second > 59){
        throw invalid_argument("SECOND INSERTED IS NOT VALID");
    }
    return converted;
}

date convert_date(const string &to_convert){                                                            //________CONVERT_DATE________//
    date converted;
    char day[3], month[3], year[5];
    to_convert.copy(day, 2, 0);
    to_convert.copy(month, 2, 3);
    to_convert.copy(year, 4, 6);
    converted.day = 0;
    converted.month = 0;
    converted.year = 0;
    converted.day = stoi(day);
    if(converted.day <= 0 || converted.day > 28){
        if(converted.day > 31 && (converted.month == 1 || converted.month == 3 || converted.month == 5 || converted.month == 7 || converted.month == 8 || converted.month == 10 || converted.month == 12)){
            throw invalid_argument("DAY INSERTED IS NOT VALID");
        }
        if(converted.day > 30 && (converted.month == 4 || converted.month == 6 || converted.month == 9 || converted.month == 11)){
            throw invalid_argument("DAY INSERTED IS NOT VALID");
        }
        if(converted.month == 2){
            if(converted.year%400 == 0){
                if(converted.day > 29){
                    throw invalid_argument("DAY INSERTED IS NOT VALID");
                }
            }
            else if(converted.year%4 == 0 && converted.year % 100 != 0){
                if(converted.day > 29){
                    throw invalid_argument("DAY INSERTED IS NOT VALID");
                }
            } else{
                if(converted.day > 28){
                    throw invalid_argument("DAY INSERTED IS NOT VALID");
                }
            }
        }
    }
    converted.month = stoi(month);
    if(converted.month < 1 || converted.month > 12){
        throw invalid_argument("MONTH INSERTED IS NOT VALID");
    }
    converted.year = stoi(year);
    if(converted.year < 1000 || converted.year > 2050){
        throw invalid_argument("YEAR INSERTED IS NOT VALID");
    }
    return converted;
}

bool compare_times(const times first, const times second){                                                  //________COMPARE_TIMES________//
    if (first.hour >= second.hour) {
        if(first.hour == second.hour){
            if(first.minute >= second.minute){
                if(first.minute == second.minute){
                    if(first.second > second.second){
                        return true;
                    }
                }
                else{
                    return true;
                }
            }
        }
        else{
            return true;
        }

    }
    return false;
}

bool compare_date(const date first, const date second){                                                     //_________COMPARE_DATE________//
    if (first.year >= second.year) {
        if(first.year == second.year){
            if(first.month >= second.month){
                if(first.month == second.month){
                    if(first.day > second.day){
                        return true;
                    }
                }
                else{
                    return true;
                }
            }
        }
        else{
            return true;
        }
    }
    return false;
}

void str_clean(string &to_clean){                                                                           //________STR_CLEAN________//
    string buffer = to_clean;
    to_clean.clear();
    for(int i = 0; i < buffer.size(); i++){
        if(!search_char_to_delete(buffer[i])){
            to_clean.push_back(buffer[i]);
        }
    }
}

bool search_char_to_delete(const char c) {                                                                  //________SEARCH_CHAR_TO_DELETE________//
    return (c == ' ' || c == ',' || c == ';' || c == ')' || c == '(' || c == '\n' || c == '"');
}

void to_upper_string(string &to_upper){                                                                     //________TO_UPPER_STRING_________//
    char char_buff;
    string buffer = to_upper;
    to_upper.clear();
    for(int i = 0; i < buffer.size(); i++){
        char_buff = buffer[i];
        if(islower(char_buff)){
            char_buff = toupper(char_buff);
        }
        to_upper.push_back(char_buff);
    }

}

bool is_int(const string &to_control){                                                                      //________IS_INT_______//
    for(int i = 0; i < to_control.size(); i++){
        if(!isdigit(to_control[i])){
            throw invalid_argument("DATA INSERTED ISN'T AN INTEGER");
        }
    }
    return true;
}

bool is_float(const string &to_control){                                                                    //________IS_FLOAT________//
    for(int i = 0; i < to_control.size(); i++){
        if(!isdigit(to_control[i]) && to_control[i] != '.'){
            throw invalid_argument("DATA INSERTED ISN'T A FLOATING POINT NUMBER");
        }
        if((i == 0 || i == to_control.size() - 1) && to_control[i] == '.'){
            throw invalid_argument("DATA INSERTED ISN'T AN FLOATING POINT NUMBER");
        }
    }
    return true;
}

bool search_protected_word(string input){                                                                  //________SEARCH_PROTECTED_WORD________//
    to_upper_string(input);
    if( input == "CREATE" || input == "TABLE" || input == "INSERT" || input == "INTO" || input == "DROP" ||
        input == "TRUNCATE" || input == "UPDATE" || input == "VALUES" || input == "DELETE" || input == "FROM" ||
        input == "SET" || input == "WHERE" || input == "ASC" || input == "DESC" || input == "ORDER" || input == "BY" ||
        input == "PRIMARY" || input == "KEY" || input == "FOREIGN" || input == "AUTO_INCREMENT" || input == "SELECT" ||
        input == "*" || input == "NOT" || input == "NULL" || input == "REFERENCES" || input == "INT" || input == "FLOAT" ||
        input == "TEXT" || input == "DATE" || input == "TIME" || input == "CHAR" || input == "BETWEEN" || input == "AND"){
        return true;
    }
    return false;
}