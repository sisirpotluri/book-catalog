//
//  Book.h
//  Library Catalog
//
//  Created by Sisir Potluri on 5/22/21.
//

#ifndef Book_h
#define Book_h

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

static void standardize(std::string &word, bool genreWord);

struct Book {

    uint32_t book_id = 0;
    std::string title;
    std::string titleStandard;
    std::string desc;
    std::string descStandard;
    std::string genres;
    std::string genresStandard;

    Book() {
        book_id = 0;
        title = "";
        titleStandard = "";
        desc = "";
        descStandard = "";
        genres = "";
        genresStandard = "";
    }

    Book(uint32_t id_in, std::string &title_in, std::string &desc_in, std::string &genres_in) {
        book_id = id_in;
        title = title_in;
        titleStandard = title_in;
        desc = desc_in;
        descStandard = desc_in;
        genres = genres_in;
        genresStandard = genres_in;
        standardize(titleStandard, false);
        standardize(descStandard, false);
        standardize(genresStandard, true);
    }
    
};

struct Book_Comparator {

    bool operator()(const Book& lhs, const Book& rhs) const {
        return lhs.titleStandard < rhs.titleStandard;
    }

    bool operator()(const Book& lhs, std::string &rhs) const {
        standardize(rhs, false);
        return lhs.titleStandard < rhs;
    }

};

static void standardize(std::string &word, bool genreWord) {
    int len = static_cast<int>(word.length());
    for (int i = 0; i < len; i++) {
        if (ispunct(word[i])) {
            if (word[i] == '|') {
                word[i] = ' ';
            }
            else {
                word.erase(i--, 1);
                len = static_cast<int>(word.length());
            }
        }
        else {
            word[i] = toupper(word[i]);
        }
        if (genreWord) {
            /*
            if (word[i] == ' ') {
                word.erase(i--, 1);
                len = static_cast<int>(word.length());
            }
             */
            /*else*/ if (isdigit(word[i])) {
                word.erase(i--, 1);
                len = static_cast<int>(word.length());
            }
        }
    }
}

#endif /* Book_h */

