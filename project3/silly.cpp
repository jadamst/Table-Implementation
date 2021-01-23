//C0F4DFE8B340D81183C208F70F9D2D797908754D
//  main.cpp
//  realp3
//
//  Created by Joe Adams on 10/31/20.
//  Copyright © 2020 Joe Adams. All rights reserved.
//

#include <iostream>
#include "TableEntry.h"
#include <getopt.h>
#include <vector>
#include <deque>
#include <queue>
#include <cmath>
#include <cstdlib>
#include <string>
#include <map>
#include <algorithm>
#include <unordered_map>
using namespace std;
#include "xcode_redirect.hpp"


class tableObject{
public:
    tableObject(){
        hash_current = false;
        g_index = -1;
    }
    void create(){
        string curr_word;
        int count = 0;
        int to_add_num = 0;
        while(cin>>curr_word){
            if(count == 0){
                table_name = curr_word;
            }
            else if(count == 1){
                to_add_num = stoi(curr_word);
                reserve_space(to_add_num);
            }
            else if(count <= to_add_num+1){
                if(curr_word[0] == 's'){
                    col_types.emplace_back(EntryType::String);
                }
                else if(curr_word[0] == 'i'){
                    col_types.emplace_back(EntryType::Int);
                }
                else if(curr_word[0] == 'd'){
                    col_types.emplace_back(EntryType::Double);
                }
                else{
                    col_types.emplace_back(EntryType::Bool);
                }
            }
            else{
                col_names.push_back(curr_word);
            }
            count++;
            if(count == 2*to_add_num+2){
                break;
            }
        }//while
    }
    void insertion(int rows){
        string string_holder;
        double double_holder;
        int count = 0;
        int count2 = 0;
        int int_holder;
        size_t og_rowspot = data.size();
        size_t row_spot = data.size();
        bool bool_holder;
        data.resize(data.size()+(size_t)rows);
        while(count != rows){
            while (count2 != static_cast<int>(get_num_cols())) {
                if(col_types[(size_t)count2] == EntryType::String){
                    cin>>string_holder;
                    data[row_spot].emplace_back(string_holder);
                }
                else if(col_types[(size_t)count2] == EntryType::Int){
                    cin>>int_holder;
                    data[row_spot].emplace_back(int_holder);
                }
                else if(col_types[(size_t)count2] == EntryType::Double){
                    cin>>double_holder;
                    data[row_spot].emplace_back(double_holder);
                }
                else{
                    cin>>bool_holder;
                    data[row_spot].emplace_back(bool_holder);
                }
                count2++;
            }//inner while
            count2 = 0;
            row_spot++;
            count++;
        }//outer while
        if(g_index!=-1 && hash_current){
            generation("hash", g_index, false,og_rowspot,false);
        }
        else if(g_index!=-1 && !hash_current){
            generation("bst", g_index, false,og_rowspot,false);
        }
        else{
            //do nothing
        }
        cout << "Added " << rows << " rows to " << table_name << " from position " << og_rowspot<< " to " << row_spot-1 << "\n";
    }
    void print(int num_cols,bool quiet){
        vector<int> cols_to_print;
        string name_col;
        int indexes = 0;
        for(int i = 0; i < num_cols; i++){
            cin>>name_col;
            indexes = get_col_index(name_col);
            if(indexes!= -1){
                cols_to_print.push_back(indexes);
            }
            else{
                cout << "Error: " << name_col <<" does not name a column in " << table_name << "\n";
                getline(cin, name_col);
                return;
            }
        }//for
        cin>>name_col;
        if(name_col[0] == 'A'){
            if(!quiet){
                for(size_t i = 0; i < (size_t)num_cols; i++){
                    cout<<col_names[(size_t)(cols_to_print[i])] << " ";
                }//for
                cout << "\n";
            }
            if(!quiet){
                for(size_t j = 0; j < data.size(); j++){
                    for(size_t i = 0; i < (size_t)num_cols; i++){
                        cout<<data[j][(size_t)(cols_to_print[i])] << " ";
                    }
                    cout << "\n";
                }//for
            }
            cout << "Printed " << data.size() << " matching rows from " << table_name << "\n";
        }//if
        else{
            char symbol;
            cin>>name_col;
            int col_index = get_col_index(name_col);
            if(col_index == -1){
                cout << "Error: " << name_col <<" does not name a column in " << table_name << "\n";
                getline(cin, name_col);
                return;
                //cin extra stuff
            }//if
            if(!quiet){
                for(size_t i = 0; i < (size_t)num_cols; i++){
                    cout<<col_names[(size_t)(cols_to_print[i])] << " ";
                }//for
                cout << "\n";
            }
            cin>>symbol;
            create_and_pass_functor(print_or_delete_where((size_t)col_index), cols_to_print,col_index,true,symbol,quiet);
        }//else
    }//print()
    void delete_from(){
        vector<int> nada = {1};
        string name_col;
        char symbol;
        cin>>name_col;
        int col_index = get_col_index(name_col);
        if(col_index == -1){
            cout << "Error: " << name_col <<" does not name a column in " << table_name << "\n";
            getline(cin,name_col);
            return;
            //cin extra stuff
        }//if
        cin >> symbol;
        create_and_pass_functor(print_or_delete_where((size_t)col_index), nada, col_index, false,symbol,false);
        if(g_index!=-1 && hash_current){
            generation("hash", g_index, false,0,true);
        }
        else if(g_index!=-1 && !hash_current){
            generation("bst", g_index, false,0,true);
        }
        else{
            //do nothing
        }
    }//delete_from()
    void generation(string type_of, int col_index, bool is_new, size_t start,bool recreate){
        if(g_index != -1 && recreate){
            bst.clear();
            hash.clear();
            hash_current = false; // do i need this var
        }
        if(type_of == "bst"){
            g_index = col_index;
            hash_current = false;
            for(size_t i = start; i < data.size(); i++){
                bst[data[i][(size_t)g_index]].push_back(i);
            }
            if(is_new){
                cout << "Created bst index for table " << table_name << " on column " << col_names[(size_t)g_index] << "\n";
            }
        }else{
            g_index = col_index;
            hash_current = true;
            for(size_t i = start; i < data.size(); i++){
                hash[data[i][(size_t)g_index]].push_back(i);
            }
            if(is_new){
                cout << "Created hash index for table " << table_name << " on column " << col_names[(size_t)g_index] << "\n";
            }
        }
    }
    void join(tableObject table2, bool quiet){
        unordered_map<TableEntry, vector<size_t>> temp_hash;
        vector<pair<int, size_t>> cols_to_print;
        string col1, col2, junk;
        int num_to_print, table_num,col1_index, col2_index;
        size_t rows_printed = 0;
        cin>>col1>>junk>>col2>>junk>>junk>>num_to_print;
        col1_index = get_col_index(col1);
        if(col1_index == -1){
            cout << "Error: " << col1 <<" does not name a column in " << table_name << "\n";
            getline(cin, junk);
            return;
        }
        col2_index = table2.get_col_index(col2);
        if(col2_index == -1){
            cout << "Error: " << col2 <<" does not name a column in " << table2.get_name() << "\n";
            getline(cin, junk);
            return;
        }
        for(size_t i = 0; i < table2.data.size(); i++){
            temp_hash[table2.data[i][(size_t)col2_index]].push_back(i);
        }
        int indexes = 0;
        for(int i = 0; i < num_to_print; i++){
            cin>>junk>>table_num;
            if(table_num == 1){
                indexes = get_col_index(junk);
                if(indexes!= -1){
                    cols_to_print.push_back({indexes,1});
                }
                else{
                    cout << "Error: " << junk <<" does not name a column in " << table_name << "\n";
                    getline(cin, junk);
                    return;
                }
            }else{
                indexes = table2.get_col_index(junk);
                if(indexes!= -1){
                    cols_to_print.push_back({indexes,2});
                }
                else{
                    cout << "Error: " << junk <<" does not name a column in " << table2.get_name() << "\n";
                    getline(cin, junk);
                    return;
                }
            }
        }//for
        if(!quiet){
            for(size_t i = 0; i < cols_to_print.size();i++){
                if(cols_to_print[i].second == 1){
                    cout << col_names[(size_t)cols_to_print[i].first] << " ";
                }else{
                    cout << table2.col_names[(size_t)cols_to_print[i].first] << " ";
                }
            }
            cout << "\n";
        }
        for(size_t i = 0; i < data.size(); i++){
            if(temp_hash.end() != temp_hash.find(data[i][(size_t)col1_index])){
            for(size_t j = 0; j < temp_hash[data[i][(size_t)col1_index]].size();j++){
                    if(!quiet){
                    for(size_t k = 0; k < cols_to_print.size();k++){
                        if(cols_to_print[k].second == 2){
                            cout << table2.data[temp_hash[data[i][(size_t)col1_index]][j]][(size_t)cols_to_print[k].first] << " ";
                        }else{
                            cout << data[i][(size_t)cols_to_print[k].first] << " ";
                        }
                    }
                    cout << "\n";
                    }
                    rows_printed++;
                }
            }
        }
        
        cout << "Printed " << rows_printed << " rows from joining " << table_name << " to " << table2.table_name << "\n";
    }
    vector<string> get_col_names(){
        return col_names;
    }
    void print_col_names(){
        for(size_t i = 0; i < col_names.size(); i++){
            cout << col_names[i] << " ";
        }
    }//print_col_names
    int get_col_index(string col_name){
        int index = -1;
        for(size_t i = 0; i < col_names.size(); i++){
            if(col_names[i] == col_name){
                index = (int)i;
                break;
            }
        }
        return index;
    }//get_col_index()
    string get_name(){
        return table_name;
    }//set name
    void reserve_space(int space){
        col_types.reserve((size_t)space);
        col_names.reserve((size_t)space);
    }//reserve_space()
    size_t get_num_cols(){
        return col_names.size();
    }//get_num_cols()
private:
    unordered_map<TableEntry, vector<size_t>> hash;
    map<TableEntry, vector<size_t>, std::less<TableEntry>> bst;
    vector<vector<TableEntry>> data;
    vector<EntryType> col_types;
    vector<string> col_names;
    string table_name;
    int g_index;
    bool hash_current;
    TableEntry print_or_delete_where(size_t col_index){
        EntryType temp = determine_type(col_index);
        if(temp == EntryType::String){
            string string_in;
            cin>>string_in;
            TableEntry entry(string_in);
            return entry;
        }else if(temp == EntryType::Double){
            double double_in;
            cin>>double_in;
            TableEntry entry(double_in);
            return entry;
        }else if(temp == EntryType::Bool){
            bool bool_in;
            cin>>bool_in;
            TableEntry entry(bool_in);
            return entry;
        }else{
            int int_in;
            cin>>int_in;
            TableEntry entry(int_in);
            return entry;
        }
    }//print_or_delete_where
    void create_and_pass_functor(TableEntry entry, vector<int> cols_to_print,int col_index,bool print,char symbol,bool quiet){
        if(symbol == '>'){
            compute_greater compute(entry,(size_t)col_index);
            if(print && g_index==col_index && !hash_current){
                print_row_bst(cols_to_print, compute, quiet, entry, symbol);
            }else if(print){
                print_row(cols_to_print, compute,quiet);
            }else{
                delete_row(compute);
            }
        }else if(symbol == '<'){
            compute_less compute(entry,(size_t)col_index);
            if(print && g_index==col_index && !hash_current){
                print_row_bst(cols_to_print, compute, quiet, entry, symbol);
            }else if(print){
                print_row(cols_to_print, compute,quiet);
            }else{
                delete_row(compute);
            }
        }else{
            compute_equal compute(entry,(size_t)col_index);
            if(print && hash_current && g_index == col_index){
                print_row_hash(cols_to_print, compute,quiet,entry);
            }else if(print && !hash_current && g_index == col_index){
                print_row_bst(cols_to_print, compute,quiet,entry,symbol);
            }else if(print){
                print_row(cols_to_print, compute, quiet);
            }
            else{
                delete_row(compute);
            }
        }
    }
    template<typename Computer>
    void print_row(vector<int> cols_to_print, Computer computer, bool quiet){
        size_t rows_printed = 0;
        bool printed_stuff = false;
        for(size_t i = 0; i < data.size(); i++){
            if(computer(data[i])){
                if(!quiet){
                    for(size_t j = 0; j < cols_to_print.size(); j++){
                        cout << data[i][(size_t)cols_to_print[j]] << " ";
                    }
                }
                printed_stuff = true;
            }
            if(printed_stuff){
                rows_printed++;
                if(!quiet){
                    cout << "\n";
                }
                printed_stuff=false;
            }
        }
        cout << "Printed " << rows_printed << " matching rows from " << table_name << "\n";
    }
    template<typename Computer>
    void print_row_hash(vector<int> cols_to_print, Computer computer, bool quiet, TableEntry entry){
        size_t rows_printed = 0;
        bool printed_stuff = false;
        for(size_t i = 0; i < hash[entry].size(); i++){
            if(computer(data[hash[entry][i]])){
                if(!quiet){
                    for(size_t j = 0; j < cols_to_print.size(); j++){
                        cout << data[hash[entry][i]][(size_t)cols_to_print[j]] << " ";
                    }
                }
                printed_stuff = true;
            }
            if(printed_stuff){
                rows_printed++;
                if(!quiet){
                    cout << "\n";
                }
                printed_stuff=false;
            }
        }
        cout << "Printed " << rows_printed << " matching rows from " << table_name << "\n";
    }
    template<typename Computer>
    void print_row_bst(vector<int> cols_to_print, Computer computer, bool quiet, TableEntry entry, char symbol){
        size_t rows_printed = 0;
        if(symbol == '>'){
            auto it2 = bst.upper_bound(entry);
            auto end = bst.end();
            while(it2!=end){
                for(size_t i = 0; i < bst[it2->first].size(); i++){
                    if(!quiet){
                        for(size_t j = 0; j < cols_to_print.size(); j++){
                            cout << data[bst[it2->first][i]][(size_t)cols_to_print[j]] << " ";
                        }
                    }
                    rows_printed++;
                    if(!quiet){
                        cout << "\n";
                        }
                }
                it2++;
            }
            cout << "Printed " << rows_printed << " matching rows from " << table_name << "\n";
        }else if(symbol == '<'){
            auto it1 = bst.lower_bound(entry);
            auto begin = bst.begin();
            while(begin!=it1){
                for(size_t i = 0; i < bst[begin->first].size(); i++){
                    if(!quiet){
                        for(size_t j = 0; j < cols_to_print.size(); j++){
                            cout << data[bst[begin->first][i]][(size_t)cols_to_print[j]] << " ";
                        }
                    }
                    rows_printed++;
                    if(!quiet){
                        cout << "\n";
                        }
                }
                begin++;
            }
            cout << "Printed " << rows_printed << " matching rows from " << table_name << "\n";
        }else{
            bool printed_stuff = false;
            for(size_t i = 0; i < bst[entry].size(); i++){
                if(computer(data[bst[entry][i]])){
                    if(!quiet){
                        for(size_t j = 0; j < cols_to_print.size(); j++){
                            cout << data[bst[entry][i]][(size_t)cols_to_print[j]] << " ";
                        }
                    }
                    printed_stuff = true;
                }
                if(printed_stuff){
                    rows_printed++;
                    if(!quiet){
                        cout << "\n";
                    }
                    printed_stuff=false;
                }
            }
            cout << "Printed " << rows_printed << " matching rows from " << table_name << "\n";
        }
    }
    template<typename Computer>
    void delete_row(Computer comp){
        size_t num_deleted = 0;
        auto it = remove_if(data.begin(), data.end(), comp);
        while(it != data.end()){
            it++;
            num_deleted++;
        }
        for(size_t i = 0; i < num_deleted;i++){
            data.pop_back();
        }
        cout<<"Deleted " << num_deleted <<" rows from "<< table_name <<"\n";
    }
    EntryType determine_type(size_t num_col){
        return col_types[(num_col)];
    }
    struct compute_less{
        compute_less(TableEntry entry, size_t in_index):to_compare(entry),index(in_index) {}
        bool operator()(vector<TableEntry> row){
            return row[index] < to_compare;
        }
    private:
        TableEntry to_compare;
        size_t index;
    };
    struct compute_greater{
        compute_greater(TableEntry entry, size_t in_index):to_compare(entry),index(in_index) {}
        bool operator()(vector<TableEntry> row){
            return row[index] > to_compare;
        }
    private:
        TableEntry to_compare;
        size_t index;
    };
    struct compute_equal{
        compute_equal(TableEntry entry, size_t in_index):to_compare(entry),index(in_index) {}
        bool operator()(vector<TableEntry> row){
            return row[index] == to_compare;
        }
    private:
        TableEntry to_compare;
        size_t index;
    };
};

class tableReader{
public:
    tableReader(){
        quiet = false;
    }
void get_options(int argc, char * argv[]){
    opterr = true;
    int choice;
    int option_index = 0;
    struct option long_options[] = {
        {"quiet", no_argument, nullptr,'q'},
        {"help", no_argument, nullptr, 'h'},
        { nullptr, 0,                 nullptr, '\0' }
    };
    while ((choice = getopt_long(argc, argv, "qh", long_options, &option_index)) != -1) {
        switch (choice) {
            case 'h':
                cerr << "You asked for help" << flush;
                exit(0);
            case 'q':
                quiet = true;
                break;
            default:
                cerr << "Error: invalid option" << flush;
                exit(1);
        } // switch
    } // while
}//get_options()
    void read(){
        string cmd;
        do{
            cout << "% ";
            cin >> cmd;
            if(cmd == "QUIT"){
                break;
            }
            else if(cmd[0] == '#'){
                getline(cin, cmd);
                continue;
            }
            else{
                if(cmd[0] == 'C'){
                    create();
                }
                else if(cmd[0] == 'I'){
                    insert_into();
                }
                else if(cmd[0] == 'D'){
                    delete_from();
                }
                else if(cmd[0] == 'G'){
                    generate_index();
                }
                else if(cmd[0] == 'P'){
                    print();
                }
                else if(cmd[0] == 'J'){
                    join();
                }
                else if(cmd[0] == 'R'){
                    remove();
                }
                else{
                    cout << "Error: unrecognized command\n";
                    getline(cin, cmd); //What if I just write 'K' in console?
                }
            }
        }while(cmd != "QUIT");
        cout<<"Thanks for being silly!\n";
    }//read()
    void create(){
        tableObject temp;
        string junk;
        temp.create();
        if(tables.find(temp.get_name()) == tables.end()){
            tables.emplace(temp.get_name(),temp);
            cout << "New table " << temp.get_name() << " with column(s) ";
            tables[temp.get_name()].print_col_names();
            cout << "created\n";
        }
        else{
            cout << "Error: Cannot create already existing table " << temp.get_name() << "\n";
            getline(cin, junk);
        }
    }//create()
    void insert_into(){
        string table_name, junk;
        int rows;
        cin >> junk >> table_name >> rows >> junk;
        cin.ignore();
        if(tables.find(table_name) != tables.end()){
            tables[table_name].insertion(rows);
        }//if
        else{
            cout << "Error: " << table_name << " does not name a table in the database\n";
            for(int i = 0; i < rows; i++){
                getline(cin, junk);
            }
            //read in all the 'fake' data?
        }
    }//insert_into()
    void delete_from(){
        string table_name, junk;
        cin>>junk>>table_name>>junk;
        if(tables.find(table_name) != tables.end()){
            tables[table_name].delete_from();
        }//if
        else{
            cout << "Error: " << table_name << " does not name a table in the database\n";
            getline(cin, junk);
        }
    }//delete_from()
    void generate_index(){
        string table_name, type, junk;
        cin >> junk >> table_name >> type >> junk>>junk>>junk;
        if(tables.find(table_name) != tables.end()){
            int col_index = tables[table_name].get_col_index(junk);
            if(col_index == -1){
                cout << "Error: " << junk <<" does not name a column in " << table_name << "\n";
                return;
            }//if
            tables[table_name].generation(type, col_index, true,0,true);
        }//if
        else{
            cout << "Error: " << table_name << " does not name a table in the database\n";
            getline(cin, junk);
        }
    }//generate_index()
    void print(){
        string table_name, junk;
        int num_cols;
        cin>>junk>>table_name >>num_cols;
        if(tables.find(table_name) != tables.end()){
            tables[table_name].print(num_cols, quiet);
        }//if
        else{
            cout << "Error: " << table_name << " does not name a table in the database\n";
            getline(cin, junk);
        }
    }//print()
    void join(){
        string table_name1, table_name2, junk;
        cin >> table_name1 >> junk >> table_name2 >> junk;
        if(tables.find(table_name1) != tables.end()){
            
        }//if
        else{
            cout << "Error: " << table_name1 << " does not name a table in the database\n";
            getline(cin, junk);
        }
        if(tables.find(table_name2) != tables.end()){
            tables[table_name1].join(tables[table_name2],quiet);
        }//if
        else{
            cout << "Error: " << table_name2 << " does not name a table in the database\n";
            getline(cin, junk);
        }
    }//join()
    void remove(){
        string name;
        cin >> name;
        if(tables.find(name) != tables.end()){
            tables.erase(name);
            cout << "Table " << name << " deleted\n";
        }//if
        else{
            cout << "Error: " << name << " does not name a table in the database\n";
        }
    }//remove()
    
private:
    unordered_map<string, tableObject> tables;
    bool quiet;
};
int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    cin >>std::boolalpha;
    cout << std::boolalpha;
    tableReader t1;
    t1.get_options(argc, argv);
    t1.read();
    return 0;
}
