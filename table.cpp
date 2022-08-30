#include "table.h"


table::table(){
    _auto_increment = 1;
}

table::~table() = default;


table::table(const table &to_copy) {
    (*this)._name_table = to_copy._name_table;
    (*this)._columns_def=to_copy._columns_def;
    (*this)._primary_key=to_copy._primary_key;
    (*this)._tab_data = to_copy._tab_data;
    (*this)._auto_increment = to_copy._auto_increment;
    (*this)._foreign_key = to_copy._foreign_key;
    (*this)._references = to_copy._references;
}

