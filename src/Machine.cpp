//
//  Machine.cpp
//  Library Catalog
//
//  Created by Sisir Potluri on 5/22/21.
//

#include "Machine.h"

using namespace std;

const string CTOR_LOADING = "Loading the catalog...";
const string CTOR_LOADED = "Catalog loaded!";

const string STARTMENU_WELCOME = "Welcome to the library catalog!";
const string STARTMENU_INSTRUCTIONS = "Enter commands as uppercase letters following the '>>' prompt";
const string STARTMENU_OPTIONS = "Here are the options at the start menu:";
const string STARTMENU_OPTION_A = "A: Navigate to your list of favorite books";
const string STARTMENU_OPTION_B = "B: Navigate to your wishlist of books to read";
const string STARTMENU_OPTION_Q = "Q: Quit the program and exit the catalog";

const string FAVORITES_OPTIONS = "Here are the options at the favorites menu:";
const string FAVORITES_OPTION_A = "A: Search for a favorite book";
const string FAVORITES_OPTION_B = "B: View your list of favorite books";
const string FAVORITES_OPTION_Q = "Q: Quit the favorites menu and return to the start menu";

const string SEARCHFAVORITE_INSTRUCTIONS = "Enter the title of one of your favorite books:";
const string ADDFAVORITE_RESULTS = "Here are the matching search results:";
const string ADDFAVORITE_INSTRUCTIONS = "Enter the index of the book to add to your favorites list (enter '0' to not add any)";
const string ADDFAVORITE_ADDED = "Added the book to your favorites list";
const string ADDFAVORITE_REPEAT = "That book is already in your favorites list or isn't a valid index";

const string FAVORITESLIST_RESULTS = "Here is your list of favorite books:";
const string FAVORITESLIST_INSTRUCTIONS = "Enter the index of a book to remove (enter '0' to not remove any)";
const string FAVORITESLIST_REMOVE = "The book was removed";
const string FAVORITESLIST_INVALID = "That isn't a valid index";

const string STARTMENU_EXIT = "Exiting the program...";

const string EMPTY_SEARCH = "No books found in the previous query";
const string EMPTY_FAVORITES = "No books in the favorites list";
const string EMPTY_REC = "No books were recommended (favorites list and wishlist could be empty)";
const string EMPTY_WISH = "No books in the wishlist of books to read";

const string WISHLIST_OPTIONS = "Here are the options at the wishlist menu:";
const string WISHLIST_OPTION_A = "A: Search for a book to read";
const string WISHLIST_OPTION_B = "B: Get recommendations based on your favorite books and wishlist books";
const string WISHLIST_OPTION_C = "C: View your wishlist of books to read";
const string WISHLIST_OPTION_Q = "Q: Quit the wishlist menu and return to the start menu";

const string SEARCHWISH_INSTRUCTIONS = "Enter the title of a book you wish to read:";
const string ADDWISH_SEARCH_RESULTS = "Here are the matching search results:";
const string ADDWISH_SEARCH_INSTRUCTIONS = "Enter the index of the book to add to your wishlist (enter '0' to not add any)";
const string ADDWISH_SEARCH_ADDED = "Added the book to your wishlist";
const string ADDWISH_SEARCH_REPEAT = "That book is already in your wishlist or isn't a valid index";

const string ADDWISH_REC_RESULTS = "Here are the recommendation results:";
const string ADDWISH_REC_INSTRUCTIONS = "Enter the index of the book to add to your wishlist (enter '0' to not add any)";
const string ADDWISH_REC_ADDED = "Added the book to your wishlist";
const string ADDWISH_REC_REPEAT = "That book is already in your wishlist or isn't a valid index";

const string WISHLIST_RESULTS = "Here is your wishlist of books to read:";
const string WISHLIST_INSTRUCTIONS = "Enter the index of a book to remove (enter '0' to not remove any)";
const string WISHLIST_REMOVE = "The book was removed";
const string WISHLIST_INVALID = "That isn't a valid index";

void print(const string& msg) {
    cout << msg << '\n';
}

void Machine::printBooks(const vector<int>& indices) {
    vector<Book> catalog = trainer.linkAllBooks();
    int counter = 1;
    for (int idx : indices) {
        cout << counter++ << ": " << catalog[idx].title << '\n';
    }
    cout << '\n';
}

Machine::Machine() {
    print(CTOR_LOADING);
    trainer.train(fileName);
    print(CTOR_LOADED);
}

void Machine::execute() {
    startMenu();
}

void Machine::startMenu() {
    print(STARTMENU_WELCOME);
    print(STARTMENU_INSTRUCTIONS);
    char command = ' ';
    bool running = true;
    while (running) {
        print(STARTMENU_OPTIONS);
        print(STARTMENU_OPTION_A);
        print(STARTMENU_OPTION_B);
        print(STARTMENU_OPTION_Q);
        cout << ">> ";
        cin >> command; cout << '\n';
        switch (command) {
            case 'A':
                favoritesMenu();
                break;
                
            case 'B':
                wishesMenu();
                break;
                
            case 'Q':
                print(STARTMENU_EXIT);
                running = false;
                break;
                
            default:
                running = false;
                break;
        }
    }
}

void Machine::favoritesMenu() {
    char command = ' ';
    bool running = true;
    while (running) {
        print(FAVORITES_OPTIONS);
        print(FAVORITES_OPTION_A);
        print(FAVORITES_OPTION_B);
        print(FAVORITES_OPTION_Q);
        cout << ">> ";
        cin >> command; cout << '\n';
        switch (command) {
            case 'A':
                searchFavorite();
                break;
                
            case 'B':
                favoritesList();
                break;
                
            case 'Q':
                running = false;
                break;
                
            default:
                running = false;
                break;
        }
    }
}

void Machine::searchFavorite() {
    print(SEARCHFAVORITE_INSTRUCTIONS);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string query = "";
    getline(cin, query); cout << '\n';
    trainer.bookSearch(query);
    addFavorite();
}

void Machine::addFavorite() {
    print(ADDFAVORITE_RESULTS);
    vector<int> resIndices = trainer.linkSearchResults();
    if (resIndices.empty()) {
        print(EMPTY_SEARCH);
        return;
    }
    printBooks(resIndices);
    print(ADDFAVORITE_INSTRUCTIONS);
    int idx = 0;
    cout << ">> ";
    cin >> idx; cout << '\n';
    if (idx == 0) {
        return;
    }
    if (trainer.addToFavorites(idx - 1)) {
        print(ADDFAVORITE_ADDED);
    }
    else {
        print(ADDFAVORITE_REPEAT);
    }
}

void Machine::favoritesList() {
    print(FAVORITESLIST_RESULTS);
    vector<int> resIndices = trainer.linkFavoritesList();
    if (resIndices.empty()) {
        print(EMPTY_FAVORITES);
        return;
    }
    printBooks(resIndices);
    print(FAVORITESLIST_INSTRUCTIONS);
    int idx = 0;
    cout << ">> ";
    cin >> idx; cout << '\n';
    if (idx == 0) {
        return;
    }
    if (trainer.removeFavorite(idx - 1)) {
        print(FAVORITESLIST_REMOVE);
    }
    else {
        print(FAVORITESLIST_INVALID);
    }
}

void Machine::wishesMenu() {
    char command = ' ';
    bool running = true;
    while (running) {
        print(WISHLIST_OPTIONS);
        print(WISHLIST_OPTION_A);
        print(WISHLIST_OPTION_B);
        print(WISHLIST_OPTION_C);
        print(WISHLIST_OPTION_Q);
        cout << ">> ";
        cin >> command; cout << '\n';
        switch (command) {
            case 'A':
                searchWish();
                break;
                
            case 'B':
                getRecommendation();
                break;
                
            case 'C':
                wishList();
                break;
                
            case 'Q':
                running = false;
                break;
                
            default:
                running = false;
                break;
        }
    }
}

void Machine::searchWish() {
    print(SEARCHWISH_INSTRUCTIONS);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string query = "";
    getline(cin, query); cout << '\n';
    trainer.bookSearch(query);
    addWishFromSearch();
}

void Machine::getRecommendation() {
    trainer.updateRecommendations();
    addWishFromRec();
}

void Machine::addWishFromSearch() {
    print(ADDWISH_SEARCH_RESULTS);
    vector<int> resIndices = trainer.linkSearchResults();
    if (resIndices.empty()) {
        print(EMPTY_SEARCH);
        return;
    }
    printBooks(resIndices);
    print(ADDWISH_SEARCH_INSTRUCTIONS);
    int idx = 0;
    cout << ">> ";
    cin >> idx; cout << '\n';
    if (idx == 0) {
        return;
    }
    if (trainer.addToWish(false, idx - 1)) {
        print(ADDWISH_SEARCH_ADDED);
    }
    else {
        print(ADDWISH_SEARCH_REPEAT);
    }
}

void Machine::addWishFromRec() {
    print(ADDWISH_REC_RESULTS);
    vector<int> resIndices = trainer.linkRecommended();
    if (resIndices.empty()) {
        print(EMPTY_REC);
        return;
    }
    printBooks(resIndices);
    print(ADDWISH_REC_INSTRUCTIONS);
    int idx = 0;
    cout << ">> ";
    cin >> idx; cout << '\n';
    if (idx == 0) {
        return;
    }
    if (trainer.addToWish(true, idx - 1)) {
        print(ADDWISH_REC_ADDED);
    }
    else {
        print(ADDWISH_REC_REPEAT);
    }
}

void Machine::wishList() {
    print(WISHLIST_RESULTS);
    vector<int> resIndices = trainer.linkWishList();
    if (resIndices.empty()) {
        print(EMPTY_WISH);
        return;
    }
    printBooks(resIndices);
    print(WISHLIST_INSTRUCTIONS);
    int idx = 0;
    cout << ">> ";
    cin >> idx; cout << '\n';
    if (idx == 0) {
        return;
    }
    if (trainer.removeWish(idx - 1)) {
        print(WISHLIST_REMOVE);
    }
    else {
        print(WISHLIST_INVALID);
    }
}

