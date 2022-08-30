//
// Created by feder on 18/06/2020.
//

#include <iostream>
#include <string>
#include "table.h"
#include "database.h"
#include <fstream>
using namespace std;
#define ERROR_FILE 1
#define ERROR_CODE 1

enum comando_menu { create_table, drop_table, truncate_table, delete_from, terminate_program, insert_into, update, select, error};                      //ELENCO COMANDI PRATICABILI E CHIAMABILI
comando_menu launch_control (const string& operazione);                                                 //FUNZIONE PANNELLO DI CONTROLLO AL TERMNINE DEL MAIN PER SPLITTARE I COMANDI


int main () {
    fstream file;
    table tab;
    database db;
    comando_menu cmd;               //Comando per lo switch per la chiamata del menu
    bool keep = true;//Serve per la quit per far terminare il programma
    string command_1;               //Variabile per la prima parola inserita dall'utente
    string command_2;               //Variabile per la seconda parola inserita dall'utente
    string cmd_unique;   //Variabile in cui unisco le due stringhe per il comando
    int list_size = 0;
    string table_name;
    table tab_buffer;
    int index = 0;
    db.read_file();

    do {
        cout << "Insert command: \n";
        cin >> command_1;

        if (command_1 != "QUIT()" && command_1 != "UPDATE" && command_1 != "SELECT") {
            cin >> command_2;
            command_1.append(command_2);                        //CONCATENAMENTO STRINGHE PER COMANDO DELLO SWITCH
            cmd_unique.assign(command_1);                      //UNITE ASSIEME IN UNA UNICA STRINGA
        }else {
            cmd_unique.assign(command_1);                               //spostamento in cmd_unique
        }

        cmd = launch_control(cmd_unique);

        switch (cmd) {
            case comando_menu::create_table:
                try {
                    db.create_table_method(); //LANCIO DEL METODO PER CREARE UNA NUOVA TABELLA
                    cout << "NEW TABLE CREATED SUCCESSFULLY IN YOUR DATABASE" << endl;
                }
                catch (logic_error& e){
                    cerr << '\n' << e.what() << endl;
                }
                break;
            case comando_menu::drop_table:
                db.drop_table_method();
                cout << "TABLE DROPPED SUCCESSFULLY" << endl;
                break;
            case comando_menu::truncate_table:
                db.truncate_method();
                cout << "ALL DATA DELETED SUCCESSFULLY" << endl;
                break;
            case comando_menu::insert_into:
                try {
                    db.insert_into_method();
                    cout << "NEW ELEMENTS INSERTED SUCCESSFULLY" << endl;
                }
                catch (logic_error& e) {
                    cerr <<"\n" << e.what() << endl;
                }
                break;
            case comando_menu::update:
                db.update_method();
                break;
            case comando_menu::select:
                cout << "stampa di prova tutti i record das tabella esistente" << endl;
                break;
            case comando_menu::delete_from:
                db.delete_from_method();
                cout << "RECORDS DELETED SUCCESSFULLY" << endl;
                break;
            case comando_menu::terminate_program:
                keep = false;

               db.save_on_file();
                cout << "THE PROGRAM IS TERMINATED" << endl;

                break;
                /*case comando_menu::error:
                    cerr<<"ERROR IN INPUT DATA, PLEASE TRY AGAIN,..."<<endl;
                    //ERRORS HANDLING {}
                    return ERROR_CODE;*/

        }
    }while (keep);
    return 0;
}


comando_menu launch_control (const string& operazione) {
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
    return comando_menu::error;
}

