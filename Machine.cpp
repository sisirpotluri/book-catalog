//
//  Machine.cpp
//  Book List
//
//  Created by Sisir Potluri on 5/22/21.
//

#include "Machine.h"
#include "csvstream.h"

using namespace std;

void string_separator(const string &str, vector<string> &result);

Machine::Machine() {
    all_books.reserve(10000);
}

void Machine::execute() {
    cout << "Loading library catalog..." << '\n';
    load_books();
    cout << "Finished Loading!" << '\n' << "------------------------" << '\n';
    cout << "Choose an action from the following options:" << '\n'
    << "E | Enter your favorite books to get recommendations" << '\n'
    << "S | Search for a book across the catalog" << '\n'
    << "Q | Close the catalog" << '\n'
    << "Enter your choice by typing the corresponding character (ie. E): " << '\n' << ">> ";
    char selection = ' ';
    cin >> selection;
    do {
        switch (selection) {
            case 'E':
                option_E();
                break;
            case 'S':
                option_S();
                break;
            case 'C':
                cout << "Exiting the program..." << '\n';
                return;
                break;
            default:
                cerr << "Invalid Selection. Exiting Program." << '\n';
                exit(1);
        }
        cout << "Choose an action from the following options:" << '\n'
        << "E | Enter your favorite books to get recommendations" << '\n'
        << "S | Search for a book across the catalog" << '\n'
        << "C | Close the catalog" << '\n'
        << "Enter your choice by typing the corresponding character (ie. E): " << '\n' << ">> ";
        cin >> selection;
    } while (selection != 'C');
    cout << "Exiting the program..." << '\n';
    return;
    
}

void Machine::load_books() {
    csvstream train_file("BookData.csv");
    vector<pair<string, string>> data;
    uint32_t running_id = 0;
    while (train_file >> data) {
        all_books.push_back(Book(running_id++, data[1].second, data[4].second, data[9].second));
    }
    Book_Comparator comp;
    sort(all_books.begin(), all_books.end(), comp);
    vector<string> title_words;
    vector<string> desc_words;
    vector<string> genres_words;
    string temp;
    for (uint32_t i = 0; i < all_books.size(); ++i) {
        string_separator(all_books[i].title_stzd, title_words);
        string_separator(all_books[i].desc_stzd, desc_words);
        string_separator(all_books[i].genres_stzd, genres_words);
        for (auto word : title_words) {
            if (word == "Lightning" || word == "Thief") {
                int x = 0;
                x++;
            }
            title_map[word].push_back(i);
        }
        for (auto word : title_words) {
            desc_map[word].push_back(i);
        }
        for (auto word : title_words) {
            genres_map[word].push_back(i);
        }
    }
}

void Machine::option_E() {
    e_mode = true;
    cout << "Choose an action from the following options:" << '\n'
    << "S | Search for a favorite book to add to your list" << '\n'
    << "V | View and/or modify your list of favorite books" << '\n'
    << "Q | Quit and return to the main options" << '\n';
    
    char command = ' ';
    cout << ">> ";
    cin >> command;
    cout << '\n';
    do {
        switch (command) {
            case 'S':
                book_search();
                add_favorite_book();
                break;
            case 'V':
                finished_list.push_back(3);
                finished_list.push_back(5);
                finished_list.push_back(11);
                view_favorites();
                break;
            case 'Q':
                return;
            default:
                cerr << "Invalid Selection. Exiting Program." << '\n';
                exit(1);
        }
        cout << "Choose an action from the following options:" << '\n'
        << "S | Search for a favorite book to add to your list" << '\n'
        << "V | View and/or modify your list of favorite books" << '\n'
        << "Q | Quit and return to the main options" << '\n';
        cout << ">> ";
        cin >> command;
        cout << '\n';
    } while (command != 'Q');
}

void Machine::option_S() {
    e_mode = false;
    char command = ' ';
    cout << "Choose an action from the following options:" << '\n';
    if (!finished_list.empty()) {
        cout << "R | Get book recommendations based on your favorites list" << '\n';
    }
    cout << "S | Search for a book to read to add to your list" << '\n'
    << "V | View and/or modify your list of books to read" << '\n'
    << "L | Log your book list to a file" << '\n'
    << "Q | Quit and return to the main options" << '\n';
    cout << ">> ";
    cin >> command;
    cout << '\n';
    
    /*
     R: update recommendations, allow to add books
     S: same as in option E
     V: same as in Option E
     L: ofstream to make and write to file
     */
}

void Machine::book_search() {
    string query = "";
    cout << "Enter the title of the book are looking for: ";
    cin >> query;
    query = "A Kiss at Midnight";
    search_results.clear();
    
    for (int i = 0, len = static_cast<int>(query.size()); i < len; i++) {
        if (ispunct(query[i])) {
            query.erase(i--, 1);
            len = static_cast<int>(query.size());
        }
        else {
            query[i] = toupper(query[i]);
        }
    }
    
    all_bins.clear();
    all_bins.resize(all_books.size());
    unordered_map<uint32_t, uint32_t> bins_ref;
    for (uint32_t i = 0; i < all_bins.size(); ++i) {
        all_bins[i].all_index = i;
        all_bins[i].count = 0;
    }
    
    vector<string> query_words;
    string_separator(query, query_words);
    for (auto word : query_words) {
        if (title_map[word].size() < 500) {
            auto it = title_map[word];
            for (uint32_t i = 0; i < it.size(); ++i) {
                all_bins[it[i]].add_count(1);
            }
        }
    }
    
    BookBinComparator comp;
    sort(all_bins.begin(), all_bins.end(), comp);
    
    for (uint32_t i = 9999; i > 9979; --i) {
        search_results.push_back(all_bins[i].all_index);
    }
    
    /*
    stringstream query_ss;
    query_ss << query;
    string temp;
    
    //temp stores each word
    
    search_bins.clear();
    unordered_map<uint32_t, uint32_t> bin_reference;
    uint32_t bin_counter = 0;
    
    while (query_ss >> temp) {
        //it has the indices
        if (title_map[temp].size() > 500) {
            continue;
        }
        auto it = title_map[temp];
        for (uint32_t i = 0; i < it.size(); ++i) {
            auto it_2 = bin_reference.find(it[i]);
            if (it_2 == bin_reference.end()) {
                search_bins.push_back(BookBin(it[i]));
                bin_reference[it[i]] = bin_counter;
                bin_counter++;
            }
            else {
                search_bins[bin_reference[it[i]]].add_count(1);
            }
        }
    }
    
    BookBinComparator comp;
    sort(search_bins.begin(), search_bins.end(), comp);
    
    for (auto idx : search_bins) {
        search_results.push_back(idx.all_index);
    }
    
    
    unordered_map<uint32_t, uint32_t> results_map;
    vector<pair<uint32_t, uint32_t>> results;
    
    while (query_ss >> temp) {
        if (title_map[temp].size() < 500) {
            auto it = title_map[temp];
            for (uint32_t i = 0; i < it.size(); ++i) {
                results_map[it[i]]++;
            }
        }
    }
    for (auto idx : results_map) {
        results.push_back(make_pair(idx.first, idx.second));
    }
    sort(results.begin(), results.end(), [](pair<uint32_t, uint32_t> lhs,
                                            pair<uint32_t, uint32_t> rhs) {
        if (lhs.second == rhs.second) {
            return lhs.first < rhs.first;
        }
        return lhs.second > rhs.second;
    });
    
    
    for (auto idx : results) {
        search_results.push_back(idx.first);
    }
     
     */

}

void Machine::append_all(uint32_t idx) {
    if (!search_results.empty()) {
        for (auto idx : search_results) {
            toread_list.push_back(idx);
        }
    }
}

void Machine::append_book(uint32_t idx) {
    for (uint32_t i = 0; i < toread_list.size(); ++i) {
        if (toread_list[i] == idx) {
            cout << all_books[idx].title << " is already in your list." << '\n';
            return;
        }
    }
    toread_list.push_back(idx);
}

void Machine::remove_book(uint32_t list_idx) {
    if (list_idx < toread_list.size()) {
        toread_list.erase(toread_list.begin() + list_idx, toread_list.begin() + list_idx + 1);
        cout << all_books[toread_list[list_idx]].title << " was removed from your list." << '\n';
    }
    cout << all_books[toread_list[list_idx]].title << " is not in your list." << '\n';
}

void Machine::sort_toread_list() {
    sort(toread_list.begin(), toread_list.end());
}

void Machine::add_favorite_book() {
    uint32_t fav_index;
    cout << "Search results:" << '\n';
    for (uint32_t i = 0; i < search_results.size(); ++i) {
        cout << i << ": " << all_books[search_results[i]].title;
        if (i == 0) {
            cout << " (Best Match)";
        }
        cout << '\n';
    }
    cout << "Enter the index of the book you want to select: ";
    cin >> fav_index;
    auto it = find(finished_list.begin(), finished_list.end(), search_results[fav_index]);
    if (it == finished_list.end()) {
        finished_list.push_back(search_results[fav_index]);
        cout << "The book was appended to your list of favorite books" << '\n';
    }
    else {
        cout << "The book is already in your list of favorite books" << '\n';
    }
    cout << "Returning to the previous menu." << '\n';
}

void Machine::view_favorites() {
    cout << "These are the books in your favorites list: " << '\n';
    for (uint32_t i = 0; i < finished_list.size(); ++i) {
        cout << i << ": " << all_books[finished_list[i]].title << '\n';
    }
    char command = ' ';
    cout << "Choose an action from the following options:" << '\n'
    << "D | Delete a book from your favorites list " << '\n'
    << "R | Return to the previous options" << '\n' << ">> ";
    cin >> command;
    uint32_t index_delete = 0;
    do {
        switch (command) {
            case 'D':
                cout << "Enter the index of the book to delete: ";
                cin >> index_delete;
                if (index_delete >= finished_list.size()) {
                    cerr << "That isn't a valid index." << '\n';
                    break;
                }
                cout << "Deleted " << all_books[finished_list[index_delete]].title
                << " from your favorites list";
                finished_list.erase(finished_list.begin() + index_delete, finished_list.begin() + index_delete + 1);
                if (finished_list.size() > 0) {
                    cout << "These are the books remaining in your favorites list: " << '\n';
                    for (uint32_t i = 0; i < finished_list.size(); ++i) {
                        cout << i << ": " << all_books[finished_list[i]].title << '\n';
                    }
                }
                break;
            case 'R':
                return;
            default:
                cerr << "Invalid Selection. Exiting Program." << '\n';
                exit(1);
        }
        cout << "Choose an action from the following options:" << '\n'
        << "D | Delete a book from your favorites list " << '\n'
        << "R | Return to the previous options" << '\n' << ">> ";
        cin >> command;
        cout << '\n';
        if (command == 'D' && finished_list.empty()) {
            cout << "There are no more books in your favorites list. Returning to the previous options." << '\n';
            return;
        }
    } while (command != 'R');
}

void Machine::update_recommendations() {
    finished_list.push_back(3);
    finished_list.push_back(883);
    finished_list.push_back(235);
    all_bins.clear();
    all_bins.resize(all_books.size());
    unordered_map<uint32_t, uint32_t> bins_ref;
    for (uint32_t i = 0; i < all_bins.size(); ++i) {
        all_bins[i].all_index = i;
        all_bins[i].count = 0;
    }
    vector<string> words;
    string temp;
    for (uint32_t i = 0; i < finished_list.size(); ++i) {
        string_separator(all_books[finished_list[i]].desc_stzd, words);
        for (auto word : words) {
            if (desc_map[word].size() < 500) {
                auto it = desc_map[word];
                for (uint32_t i = 0; i < it.size(); ++i) {
                    all_bins[it[i]].add_count(1);
                }
            }
        }
        string_separator(all_books[finished_list[i]].genres_stzd, words);
        for (auto word : words) {
            if (genres_map[word].size() < 500) {
                auto it = genres_map[word];
                for (uint32_t i = 0; i < it.size(); ++i) {
                    all_bins[it[i]].add_count(2);
                }
            }
        }
        string_separator(all_books[finished_list[i]].title_stzd, words);
        for (auto word : words) {
            if (title_map[word].size() < 500) {
                auto it = title_map[word];
                for (uint32_t i = 0; i < it.size(); ++i) {
                    all_bins[it[i]].add_count(3);
                }
            }
        }
    }
    BookBinComparator comp;
    sort(all_bins.begin(), all_bins.end(), comp);
}

void string_separator(const string &str, vector<string> &result) {
    result.clear();
    string current = "";
    for (uint32_t i = 0; i < str.size(); ++i) {
        if (str[i] != ' ') {
            current += str[i];
        }
        else {
            if (current != "") {
                result.push_back(current);
                current.clear();
            }
        }
    }
    if (current != "") {
        result.push_back(current);
        current.clear();
    }
}
