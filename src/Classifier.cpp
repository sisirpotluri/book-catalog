//
//  Classifier.cpp
//  Library Catalog
//
//  Created by Sisir Potluri on 8/23/21.
//

#include "Classifier.h"
#include "csvstream.h"

using namespace std;

void stringSeparator(const string &str, vector<string> &result) {

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

Classifier::Classifier() {
    allBooks.reserve(10000);
}

void Classifier::train(const string &dataFile) {

    csvstream train_file(dataFile);
    vector<pair<string, string>> data;
    unordered_set<string> titles;
    
    int running_id = 0;
    while (train_file >> data) {
        if (titles.find(data[1].second) == titles.end()) {
            allBooks.push_back(Book(running_id++,
                                    data[1].second,
                                    data[4].second,
                                    data[9].second));
            titles.insert(data[1].second);
        }
    }
    
    Book_Comparator comp;
    sort(allBooks.begin(), allBooks.end(), comp);
    
    vector<string> titleWords;
    vector<string> descWords;
    vector<string> genresWords;
    string temp;
    
    for (int i = 0; i < allBooks.size(); ++i) {
        stringSeparator(allBooks[i].titleStandard, titleWords);
        stringSeparator(allBooks[i].descStandard, descWords);
        stringSeparator(allBooks[i].genresStandard, genresWords);
        for (auto word : titleWords) {
            titleMap[word].push_back(i);
        }
        for (auto word : descWords) {
            descMap[word].push_back(i);
        }
        for (auto word : genresWords) {
            genresMap[word].push_back(i);
        }
    }
    
}

void Classifier::bookSearch(string& query) {

    searchResults.clear();
    standardize(query, false);

    bins.clear();
    bins.resize(allBooks.size());
    unordered_map<uint32_t, uint32_t> binsRef;
    for (int i = 0; i < bins.size(); ++i) {
        bins[i].index = i;
        bins[i].count = 0;
    }

    vector<string> queryWords;
    stringSeparator(query, queryWords);
    for (auto word : queryWords) {
        if (titleMap[word].size() < 500) {
            vector<int> entries = titleMap[word];
            for (int i = 0; i < entries.size(); ++i) {
                bins[entries[i]].addCount(1);
            }
        }
        if (titleMap[word].size() < 50) {
            vector<int> entries = titleMap[word];
            for (int i = 0; i < entries.size(); ++i) {
                bins[entries[i]].addCount(3);
            }
        }
    }

    BinComparator comp;
    sort(bins.begin(), bins.end(), comp);
    int binsSize = static_cast<int>(bins.size());
    
    for (int i = binsSize - 1; i > binsSize - 20 && i >= 0; --i) {
        if (bins[i].count > 0) {
            searchResults.push_back(bins[i].index);
        }
        else {
            break;
        }
    }

}

void Classifier::updateRecommendations() {

    recommended.clear();

    bins.clear();
    bins.resize(allBooks.size());
    unordered_map<uint32_t, uint32_t> binsRef;
    for (int i = 0; i < bins.size(); ++i) {
        bins[i].index = i;
        bins[i].count = 0;
    }

    string allTitles = "";
    string allDesc = "";
    string allGenres = "";
    
    for (auto index : favoritesList) {
        allTitles += allBooks[index].titleStandard + " ";
        allDesc += allBooks[index].descStandard + " ";
        allGenres += allBooks[index].genresStandard + " ";
    }
    
    for (auto index : wishList) {
        allTitles += allBooks[index].titleStandard + " ";
        allDesc += allBooks[index].descStandard + " ";
        allGenres += allBooks[index].genresStandard + " ";
    }

    vector<string> titleWords;
    vector<string> descWords;
    vector<string> genresWords;
    
    stringSeparator(allTitles, titleWords);
    stringSeparator(allDesc, descWords);
    stringSeparator(allGenres, genresWords);
    
    for (auto word : titleWords) {
        if (titleMap[word].size() < 500) {
            vector<int> entries = titleMap[word];
            for (int i = 0; i < entries.size(); ++i) {
                bins[entries[i]].addCount(50);
            }
        }
    }
    
    for (auto word : descWords) {
        if (descMap[word].size() < 40) {
            vector<int> entries = descMap[word];
            for (int i = 0; i < entries.size(); ++i) {
                bins[entries[i]].addCount(static_cast<int>(100 - entries.size()));
            }
        }
    }

    for (auto word : genresWords) {
        vector<int> entries = genresMap[word];
        for (int i = 0; i < entries.size(); ++i) {
            bins[entries[i]].addCount(10);
        }
    }

    BinComparator comp;
    sort(bins.begin(), bins.end(), comp);
    int binsSize = static_cast<int>(bins.size());
    
    int repeatedCount = static_cast<int>(favoritesList.size() + wishList.size());
    
    for (int i = binsSize - 1; i > binsSize - repeatedCount - 20 && i >= 0; --i) {
        auto itFavorites = favoritesTitles.find(allBooks[bins[i].index].titleStandard);
        auto itWish = wishTitles.find(allBooks[bins[i].index].titleStandard);
        if (itFavorites == favoritesTitles.end() && itWish == wishTitles.end()) {
            if (bins[i].count > 0) {
                recommended.push_back(bins[i].index);
            }
            else {
                break;
            }
        }
    }

}

vector<Book>& Classifier::linkAllBooks() {
    return allBooks;
}

vector<int>& Classifier::linkSearchResults() {
    return searchResults;
}
vector<int>& Classifier::linkRecommended() {
    return recommended;
}
vector<int>& Classifier::linkFavoritesList() {
    return favoritesList;
}
vector<int>& Classifier::linkWishList() {
    return wishList;
}

bool Classifier::addToFavorites(int idx) {
    if (idx >= searchResults.size()) {
        return false;
    }
    auto it = favoritesTitles.find(allBooks[searchResults[idx]].titleStandard);
    if (it == favoritesTitles.end()) {
        favoritesList.push_back(searchResults[idx]);
        favoritesTitles.insert(allBooks[searchResults[idx]].titleStandard);
        return true;
    }
    else {
        return false;
    }
}

bool Classifier::removeFavorite(int idx) {
    if (idx >= favoritesList.size()) {
        return false;
    }
    favoritesList.erase(favoritesList.begin() + idx);
    return true;
}

bool Classifier::addToWish(bool fromRecs, int idx) {
    if (fromRecs) {
        if (idx >= recommended.size()) {
            return false;
        }
        auto it = wishTitles.find(allBooks[recommended[idx]].titleStandard);
        if (it == wishTitles.end()) {
            wishList.push_back(recommended[idx]);
            wishTitles.insert(allBooks[recommended[idx]].titleStandard);
            return true;
        }
        else {
            return false;
        }
    }
    else {
        if (idx >= searchResults.size()) {
            return false;
        }
        auto it = wishTitles.find(allBooks[searchResults[idx]].titleStandard);
        if (it == wishTitles.end()) {
            wishList.push_back(searchResults[idx]);
            wishTitles.insert(allBooks[searchResults[idx]].titleStandard);
            return true;
        }
        else {
            return false;
        }
    }
}

bool Classifier::removeWish(int idx) {
    if (idx >= wishList.size()) {
        return false;
    }
    wishList.erase(wishList.begin() + idx);
    return true;
}
