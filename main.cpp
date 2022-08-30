#include <iostream>
#include <string>
#include "table.h"
#include "database.h"
using namespace std;
#define ERROR_FILE 1                                                              //CODICE ERRORE PER GESTIONE FILE DI SALVATAGGIO

void introduction();                                                             //FUNZIONE INTERFACCIA MENU' DATABASE
enum comando_menu { create_table, drop_table, truncate_table, delete_from,
    terminate_program, insert_into, update, select, error};      //ELENCO COMANDI PRATICABILI DALL'UTENTE
comando_menu launch_control (const string& operazione);                          //FUNZIONE PANNELLO DI CONTROLLO AL TERMNINE DEL MAIN PER INDIRIZZAMENTO


int main () {

    //___DICHIARAZIONE VARIAIBLI___//
    table tab;
    database db;
    comando_menu cmd;                                                           //COMANDO PER LO SWITCH PER LA CHIAMATA NEL MENU'
    bool keep = true;                                                           //BOOLEANO PER CHIUSURA DEL PROGRAMMA
    bool error = false;                                                         //BOOLEANO PER GESTIONE DEGLI ERRORI DI INSERIMENTO DA UTENTE
    string command_1;                                                           //STRINGA PER 1° KEY_WORD INSERITA
    string command_2;                                                           //STRINGA PER 2° KEY_WORD INSERITA
    string cmd_unique;                                                          //STRINGA UNICA PER CONCATENAMENTO
    string buffer;

    //___INIZIO PROGRAMMA___//
    introduction();                                                              //STAMPA A VIDEO MENU INTERFACCIA CON L'UTENTE

    try{
        db.read_file();                                                          //LETTURA ED EVENTUALE CARICAMENTO DI DATI SUL DATABASE DA FILE BINARIO
    }
    catch(fstream::failure& e){                                                  //ECCEZIONI DI APERTURA DEL FILE
        cerr << endl << e.what() << endl;
        cout << "THE PROGRAM TERMINATED" << endl;
        return ERROR_FILE;
    }

    do {                                                                         //CICLO PRINCIPALE PER GESTIONE DELL'INDIRIZZAMENTO DEI COMANDI
        cout << "Insert command: \n";
        if(error){
            do{
                cin >> buffer;
                to_upper_string(buffer);                            //RENDE I COMANDI CASE_INSENSITIVE

            }while(buffer != "CREATE" && buffer != "INSERT" && buffer != "SELECT"&&
                   buffer != "DROP" && buffer != "DELETE"  && buffer != "TRUNCATE" &&
                   buffer != "UPDATE"&& buffer != "QUIT()");
        }

        cmd_unique.clear();

        if(!error) {
            cin >> command_1;
        }else{
            command_1 = buffer;
        }
        error = false;
       to_upper_string(command_1);

        if (command_1 != "CREATE" && command_1 != "INSERT" &&                   //CONTROLLO 1° KEY_WORD
            command_1 != "SELECT"&& command_1 != "DROP"&& command_1 != "DELETE" &&
            command_1 != "TRUNCATE" && command_1 != "UPDATE"  && command_1 != "QUIT()") {

            cmd_unique.assign("ERRORE");                                      //AVVERTE L'UTENTE DI UN ERRORE DI SINTASSI DELLA 1° KEY_WORD
        } else {
            if (command_1 != "SELECT"  && command_1 != "UPDATE" &&              //CONTROLLO SE E' NECESSARIO LEGGERE LA 2° KEY_WORD
                command_1 != "QUIT()") {

                cin >> command_2;                                               //LETTURA 2° KEY_WORD
                to_upper_string(command_2);
                if (command_2 != "TABLE" && command_2 != "INTO" &&              //AVVERTE L'UTENTE DI UN ERRORE DI SINTASSI DELLA 2° KEY_WORD
                    command_2 != "FROM") {

                    cmd_unique.assign("ERRORE");
                } else {
                    command_1.append(command_2);                               //CONCATENAMENTO KEY_WORDS PER LO SWITCH
                    cmd_unique.assign(command_1);                              //GENERA UNA STRINGA CON IL COMANDO UNICO
                }
            } else {
                cmd_unique.assign(command_1);                                  //GENERA UN COMANDO UNICO SENZA CONCATENAMENTO
            }
        }
        cmd = launch_control(cmd_unique);                                      //INSERISCE IL COMANDO UNICO NEL PANNELLO DI CONTROLLO

        switch (cmd) {
            case comando_menu::create_table:
                try {
                    db.create_table_method();                                  //METODO PER CREARE UNA NUOVA TABELLA NEL DATABASE
                    cout << "NEW TABLE CREATED SUCCESSFULLY IN YOUR DATABASE" << endl;
                }
                catch (logic_error& e){                                        //ECCEZIONI VARIE DEL METODO
                    cerr << '\n' << e.what() << endl;
                    error = true;
                }
                break;
            case comando_menu::drop_table:
                try {
                    db.drop_table_method();                                   //ELIMINA UNA TABELLA PRESENTE IN DATABASE
                    cout << "TABLE DROPPED SUCCESSFULLY" << endl;
                } catch (logic_error& e){                                     //ECCEZIONI VARIE DEL METODO
                    cerr <<"\n" << e.what() << endl;
                    error = true;
                }
                break;
            case comando_menu::truncate_table:
                try {
                    db.truncate_method();                                    //SVUOTA UNA TABELLA PRESENTE IN DATABASE SENZA ELIMINARLA
                    cout << "ALL DATA DELETED SUCCESSFULLY" << endl;
                }
                catch (invalid_argument& e) {                                //ECCEZIONI VARIE DEL METODO
                    cerr <<"\n" << e.what() << endl;
                    error = true;
                }
                catch (logic_error& e){                                      //ECCEZIONI VARIE DEL METODO
                    cerr <<"\n" << e.what() << endl;
                    error = true;
                }
                break;
            case comando_menu::insert_into:
                try {
                    db.insert_into_method();                                //INSERISCE I DATI DI UNA TABELLA PRESENTE NEL DATABASE
                    cout << "NEW ELEMENTS INSERTED SUCCESSFULLY" << endl;
                }
                catch (invalid_argument& e) {                               //ECCEZIONI VARIE DEL METODO
                    cerr <<"\n" << e.what() << endl;
                    error = true;
                }
                catch (logic_error& e) {                                    //ECCEZIONI VARIE DEL METODO
                    cerr <<"\n" << e.what() << endl;
                    error = true;
                }
                break;
            case comando_menu::update:
                try {
                    db.update_method();                                     //AGGIORNA UN RECORD DI UNA TABELLA PRESENTE
                    cout << " TABLE UPDATED SUCCESSFULLY"<< endl;
                }
                catch (invalid_argument& e) {                               //ECCEZIONI VARIE DEL METODO
                    cerr <<"\n" << e.what() << endl;
                    error = true;
                }
                catch (logic_error& e) {
                    cerr <<"\n" << e.what() << endl;
                    error = true;
                }
                break;
            case comando_menu::select:
                try {
                    cout << db.select_method();                             //STAMPA A VIDEO I DATI SELEZIONATI DALL'UTENTE DI UNA TABELLA (ANCHE CON ORDINAMENTO)
                }
                catch (invalid_argument& e){                                //ECCEZIONI VARIE DEL METODO
                    cerr <<"\n" << e.what() << endl;
                    error = true;
                }
                break;
            case comando_menu::delete_from:
                try {
                    db.delete_from_method();                               //CANCELLA UNO O PIU' RECORD SELEZIONATI DALL'UTENTE DI UNA TABELLA ESISTENTE
                    cout << "RECORDS DELETED SUCCESSFULLY" << endl;
                }
                catch (invalid_argument& e){                               //ECCEZIONI VARIE DEL METODO
                    cerr <<"\n" << e.what() << endl;
                    error = true;
                }
                break;
            case comando_menu::terminate_program:                          //TERMINA L'ESECUZIONE DEL PROGRAMMA
                keep = false;                                              // SETTO IL BOOLEANO PER USCIRE DAL CICLO

                try {
                    db.save_on_file();                                     //IL DATABASE VIENE SALAVATO O AGGIORNATO SUL FILE BINARIO
                }
                catch (fstream::failure& e){                               //CHIUSURA FORZATA PER ERRORE SALAVATAGGIO CON MESSAGGIO DI ERRORE
                    cerr << endl << e.what() << endl;
                    cout << "THE PROGRAM TERMINATED" << endl;
                    return ERROR_FILE;
                }

                cout << "THE PROGRAM TERMINATED" << endl;                  //STAMPA IL MESSAGGIO DI CHIUSURA NEL CASO DI CORRETTO AGGIORNAMENTO DEL FILE
                break;
            case comando_menu::error:                                      //CASO DI ERRATO INSERIMENTO DELLE KEY_WORD
                cerr << endl << "WRONG COMMAND, PLEASE TRY AGAIN..." << endl;
                error = true;
                break;
        }
    }while (keep);                                                          //CONDIZIONE PER IL MANTENIMENTO DEL CICLO
    return 0;
}
comando_menu launch_control (const string& operazione) {                    //PANNELLO DI CONTROLLO COMANDI ASSOCIATO AL "ENUM"
    if (operazione == "CREATETABLE")
        return comando_menu::create_table;
    else if (operazione == "DROPTABLE")
        return comando_menu::drop_table;
    else if (operazione == "TRUNCATETABLE")
        return comando_menu::truncate_table;
    else if (operazione == "SELECT")
        return comando_menu::select;
    else if(operazione == "DELETEFROM")
        return comando_menu::delete_from;
    else if (operazione == "INSERTINTO")
        return comando_menu::insert_into;
    else if (operazione == "UPDATE")
        return comando_menu::update;
    else if (operazione == "QUIT()")
        return comando_menu::terminate_program;
    else if (operazione == "ERRORE")
        return comando_menu::error;
    return error;
}

void introduction(){                                                           //FUNZIONE INTERFACCIA VIDEO UTENTE IN APERTURA PROGRAMMA
    cout << "                              " << "DATABASE MANAGEMENT" << endl;
    cout << "DATABASE MENU'" << endl;
    cout << "A) CREATE TABLE             : Creation of a new table to be inserted in database" << endl;
    cout << "B) DROP TABLE               : Erase a table from database" << endl;
    cout << "C) TRUNCATE TABLE           : Clear all data from an existent table" << endl;
    cout << "D) INSERT INTO ... VALUES   : Insert data inside an existent table" << endl;
    cout << "E) DELETE FROM ... WHERE    : Clear a single or more records from an existent table" << endl;
    cout << "F) UPDATE ... SET ... WHERE : Update record in an existent table" << endl;
    cout << "G) SELECT(*)FROM ... WHERE  : Print data from database selected by the user" << endl;
    cout << "H) ORDER BY ASC/DESC        : Sort data from an existent table in ascending or descending order" << endl;
    cout << "I) QUIT()                   : Terminate the program and save the database" << endl << endl;
}