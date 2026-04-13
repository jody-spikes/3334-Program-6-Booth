//
//  SearchEngine.cpp
//  Program 6
//
//  Created by Jody Spikes on 4/12/26.
//

#include "SearchEngine.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

SearchEngine::SearchEngine() : index_(101, 0.75) {}

//----------
//Clean Word
//----------
string SearchEngine::cleanWord(const string& word) const {
    string result;
    for(char c : word)
        if(isalpha(c))
            result += tolower(c);
    return result;
}

//--------------
//Index Document
//--------------
void SearchEngine::indexDocument(const string& filename) {
    string word;
    ifstream inFile;
    inFile.open(filename);
    if(!inFile) return;

    while(inFile >> word){
        string cleaned = cleanWord(word);
        if(cleaned.empty()) continue;

        if(index_.contains(cleaned)){
            vector<Posting>& postings = index_.at(cleaned);
            bool found = false;
            for(Posting& p : postings){
                if(p.document == filename){
                    p.frequency++;
                    found = true;
                    break;
                }
            }
            if(!found)
                postings.emplace_back(filename, 1);
        }
        else {
            vector<Posting> newList;
            newList.emplace_back(filename, 1);
            index_.insert(cleaned, newList);
        }
    }
}

//------------------
//Build From Corpus
//------------------
bool SearchEngine::buildFromCorpus(const string& corpusFile) {
    ifstream inFile;
    inFile.open(corpusFile);
    if(!inFile) return false;

    string filename;
    while(getline(inFile, filename))
        if(!filename.empty())
            indexDocument(filename);
    return true;
}

//------------
//Query Word
//------------
void SearchEngine::queryWord(const string& term, ostream& out) const {
    if(!index_.contains(term)){
        out << term << " not found in any document.\n";
        return;
    }
    const vector<Posting>& postings = index_.at(term);
    out << term << " found in:\n";
    for(const Posting& p : postings)
        out << p.document << " (" << p.frequency << ")\n";
}

//------------
//Query And
//------------
void SearchEngine::queryAnd(const string& term1, const string& term2, ostream& out) const {
    if(!index_.contains(term1) || !index_.contains(term2)){
        out << "No documents contain both " << term1 << " and " << term2 << ".\n";
        return;
    }
    const vector<Posting>& postings1 = index_.at(term1);
    const vector<Posting>& postings2 = index_.at(term2);
    out << "Documents containing both " << term1 << " and " << term2 << ":\n";
    bool found = false;
    for(const Posting& p1 : postings1){
        for(const Posting& p2 : postings2){
            if(p1.document == p2.document){
                out << p1.document << "\n";
                found = true;
                break;
            }
        }
    }
    if(!found)
        out << "No documents contain both " << term1 << " and " << term2 << ".\n";
}

//----------
//Query Or
//----------
void SearchEngine::queryOr(const string& term1, const string& term2, ostream& out) const {
    bool has1 = index_.contains(term1);
    bool has2 = index_.contains(term2);
    if(!has1 && !has2){
        out << "No documents contain " << term1 << " or " << term2 << ".\n";
        return;
    }
    out << "Documents containing " << term1 << " or " << term2 << ":\n";
    vector<string> printed;
    if(has1){
        const vector<Posting>& postings1 = index_.at(term1);
        for(const Posting& p : postings1){
            out << p.document << "\n";
            printed.push_back(p.document);
        }
    }
    if(has2){
        const vector<Posting>& postings2 = index_.at(term2);
        for(const Posting& p : postings2){
            bool already = false;
            for(const string& s : printed){
                if(s == p.document){
                    already = true;
                    break;
                }
            }
            if(!already)
                out << p.document << "\n";
        }
    }
}

//------------
//Query Freq
//------------
void SearchEngine::queryFreq(const string& term, ostream& out) const {
    if(!index_.contains(term)){
        out << term << " not found in any document.\n";
        return;
    }
    const vector<Posting>& postings = index_.at(term);
    int total = 0;
    for(const Posting& p : postings)
        total += p.frequency;
    out << term << " appears " << total << " times in the corpus.\n";
}

//------------
//Query Docs
//------------
void SearchEngine::queryDocs(const string& term, ostream& out) const {
    if(!index_.contains(term)){
        out << term << " not found in any document.\n";
        return;
    }
    const vector<Posting>& postings = index_.at(term);
    out << term << " appears in " << postings.size() << " documents.\n";
}

//-----------------
//Process Queries
//-----------------
bool SearchEngine::processQueries(const string& queryFile, ostream& out) const {
    ifstream inFile;
    inFile.open(queryFile);
    if(!inFile) return false;

    string line;
    while(getline(inFile, line)){
        if(line.empty()) continue;
        istringstream iss(line);
        string command;
        iss >> command;

        if(command == "WORD"){
            string term;
            iss >> term;
            queryWord(cleanWord(term), out);
        }
        else if(command == "AND"){
            string term1, term2;
            iss >> term1 >> term2;
            queryAnd(cleanWord(term1), cleanWord(term2), out);
        }
        else if(command == "OR"){
            string term1, term2;
            iss >> term1 >> term2;
            queryOr(cleanWord(term1), cleanWord(term2), out);
        }
        else if(command == "FREQ"){
            string term;
            iss >> term;
            queryFreq(cleanWord(term), out);
        }
        else if(command == "DOCS"){
            string term;
            iss >> term;
            queryDocs(cleanWord(term), out);
        }
        else{
            out << "Unknown command: " << command << "\n";
        }
        out << "\n";
    }
    return true;
}

//------------------
//Print Statistics
//------------------
void SearchEngine::printStatistics(ostream& out) const {
    out << "\nHash Table Statistics\n";
    out << "---------------------\n";
    out << "Unique words: "  << index_.size()          << "\n";
    out << "Table size: "    << index_.tableSize()      << "\n";
    out << "Collisions: "    << index_.collisionCount() << "\n";
    out << "Rehashes: "      << index_.rehashCount()    << "\n";
    out << "Load factor: "   << index_.loadFactor()     << "\n";
}
