//
//  Book.cpp
//  Book List
//
//  Created by Sisir Potluri on 5/22/21.
//

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

struct Book {
    uint32_t book_id = 0;
    std::string title;
    std::string title_stzd;
    std::string desc;
    std::string desc_stzd;
    std::string genres;
    std::string genres_stzd;
    
    Book() {
        book_id = 0;
        title = "";
        title_stzd = "";
        desc = "";
        desc_stzd = "";
        genres = "";
        genres_stzd = "";
    }
    Book(uint32_t id_in, std::string &title_in, std::string &desc_in, std::string &genres_in) {
        book_id = id_in;
        title = title_in;
        title_stzd = title_in;
        desc = desc_in;
        desc_stzd = desc_in;
        genres = genres_in;
        genres_stzd = genres_in;
        for (int i = 0, len = static_cast<int>(title_stzd.size()); i < len; i++) {
            if (ispunct(title_stzd[i])) {
                title_stzd.erase(i--, 1);
                len = static_cast<int>(title_stzd.size());
            }
            else {
                title_stzd[i] = toupper(title_stzd[i]);
            }
        }
        for (int i = 0, len = static_cast<int>(desc_stzd.size()); i < len; i++) {
            if (ispunct(desc_stzd[i])) {
                desc_stzd.erase(i--, 1);
                len = static_cast<int>(desc_stzd.size());
            }
            else {
                desc_stzd[i] = toupper(desc_stzd[i]);
            }
        }
        for (int i = 0, len = static_cast<int>(genres_stzd.size()); i < len; i++) {
            if (ispunct(genres_stzd[i])) {
                genres_stzd[i] = ' ';
            }
            else if (genres_stzd[i] == ' ') {
                genres_stzd.erase(i--, 1);
                len = static_cast<int>(genres_stzd.size());
            }
            else if (isdigit(genres_stzd[i])) {
                genres_stzd.erase(i--, 1);
                len = static_cast<int>(genres_stzd.size());
            }
            else {
                genres_stzd[i] = toupper(genres_stzd[i]);
            }
        }
    }
    
};

struct Book_Comparator {
    
    //return if LHS < RHS
    bool operator()(const Book& lhs, const Book& rhs) const {
        return lhs.title_stzd < rhs.title_stzd;
    }
    
    bool operator()(const Book& lhs, std::string &rhs) const {
        for (int i = 0, len = static_cast<int>(rhs.size()); i < len; i++) {
                if (ispunct(rhs[i])) {
                    rhs.erase(i--, 1);
                    len = static_cast<int>(rhs.size());
                }
                else {
                    rhs[i] = toupper(rhs[i]);
                }
        }
        return lhs.title_stzd < rhs;
    }
    
};
