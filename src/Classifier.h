//
//  Classifier.hpp
//  Library Catalog
//
//  Created by Sisir Potluri on 5/22/21.
//

#ifndef Classifier_h
#define Classifier_h

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>

#include "Book.h"

class Classifier {
public:

    Classifier();
    void train(const std::string &dataFile);
    void bookSearch(std::string &query);
    void updateRecommendations();
    
    std::vector<Book>& linkAllBooks();
    
    std::vector<int>& linkSearchResults();
    std::vector<int>& linkRecommended();
    std::vector<int>& linkFavoritesList();
    std::vector<int>& linkWishList();
    
    bool addToFavorites(int idx);
    bool removeFavorite(int idx);
    
    bool addToWish(bool fromRecs, int idx);
    bool removeWish(int idx);

private:

    std::vector<Book> allBooks;
    std::unordered_map<std::string, std::vector<int>> titleMap;
    std::unordered_map<std::string, std::vector<int>> descMap;
    std::unordered_map<std::string, std::vector<int>> genresMap;

    std::vector<int> searchResults;
    std::vector<int> recommended;
    std::vector<int> favoritesList;
    std::vector<int> wishList;
    
    std::unordered_set<std::string> favoritesTitles;
    std::unordered_set<std::string> wishTitles;
    
    struct Bin {
        int index;
        int count;
        Bin() {
            index = 0;
            count = 0;
        }
        Bin(int indexIn) {
            index = indexIn;
            count = 1;
        }
        void addCount(int add) {
            count += add;
        }
    };
    
    struct BinComparator {
    
        bool operator()(const Bin& lhs, const Bin& rhs) const {
            return lhs.count < rhs.count;
        }
        
    };
    
    std::vector<Bin> bins;
    
};

#endif /* Classifier_h */
