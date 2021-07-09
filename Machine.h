//
//  Machine.hpp
//  Book List
//
//  Created by Sisir Potluri on 5/22/21.
//

#ifndef Machine_h
#define Machine_h

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Book.h"
#include "Counter.h"

class Machine {
public:
    Machine();
    
    void execute();
    
    void load_books();
    
    void option_E();
    
    void option_S();
    
    void book_search();
    
    void append_all(uint32_t idx);
    
    void append_book(uint32_t idx);
    
    void remove_book(uint32_t list_idx);
    
    void sort_toread_list();
    
    void add_favorite_book();
    
    void view_favorites();
    
    void update_recommendations();
    
private:
    
    bool e_mode = false;
    
    std::vector<Book> all_books;
    std::unordered_map<std::string, std::vector<uint32_t>> title_map;
    std::unordered_map<std::string, std::vector<uint32_t>> desc_map;
    std::unordered_map<std::string, std::vector<uint32_t>> genres_map;
    std::vector<uint32_t> search_results;
    std::vector<uint32_t> finished_list;
    std::vector<uint32_t> toread_list;
    
    struct BookBin {
        uint32_t all_index;
        uint32_t count;
        BookBin() {
            all_index = 0;
            count = 0;
        }
        BookBin(uint32_t index_in) {
            all_index = index_in;
            count = 1;
        }
        void add_count(uint32_t add) {
            count += add;
        }
    };
    
    struct BookBinComparator {
        //return if LHS < RHS
        bool operator()(const BookBin& lhs, const BookBin& rhs) const {
            return lhs.count < rhs.count;
        }
    };
    
    std::vector<BookBin> search_bins;
    std::vector<BookBin> all_bins;
    
};

#endif /* Machine_hpp */
