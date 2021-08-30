//
//  Machine.hpp
//  Library Catalog
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

#include "Classifier.h"

class Machine {
public:
    
    Machine();
    void execute();
    
    void startMenu();
    
    void favoritesMenu();
    void searchFavorite();
    void addFavorite();
    void favoritesList();
    
    void wishesMenu();
    void searchWish();
    void getRecommendation();
    void addWishFromSearch();
    void addWishFromRec();
    void wishList();
    
    void printBooks(const std::vector<int>& indices);
    
private:
    
    const std::string fileName = "BookData.csv";
    Classifier trainer;
    
};

#endif /* Machine_h */
