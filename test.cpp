#include "Wordset.hpp"
#include <string>
#include <cmath>

#include <iostream>


#include "convert.hpp"
#include <iostream>
#include <set>
#include <sstream>
#include <map>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_map>


// returns s, as a number in the given base, mod the given modulus
unsigned polynomialHashFunction(const std::string & s, unsigned base, unsigned mod)
{
    unsigned res = 0;

    char letter;

    for (int i = 0; i < s.length(); i++) {
        res *= base;
        letter = s[i]-96;
        res += letter % mod;
    }
    
    return res % mod;
	
}



WordSet::WordSet(unsigned initialCapacity, unsigned evictionThreshold)
{
    maxEviction = evictionThreshold;
    capacity = initialCapacity;
    evicition_cout = 0;
    unqiue = 0;

    table_1 = new std::string[initialCapacity];
    table_2 = new std::string[initialCapacity];
}

WordSet::~WordSet()
{
    delete[] table_1;
    delete[] table_2;

}

bool isPrime(unsigned n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;   
    }
    return true;
}


void WordSet::insert(const std::string & s)
{
    evicition_cout = 0;
    if (contains(s)) {
        return;
    }
    unqiue += 1;

    unsigned indx = polynomialHashFunction(s, BASE_H1, capacity);
    std::string holder1;
    std::string holder2;
    std::string current = s;
    
    while (evicition_cout < this->maxEviction) {
        if (table_1[indx] == "") {
            table_1[indx] = s;
            break;
        }
        else if (table_1[indx] != "") {
            holder1 = table_1[indx];
            table_1[indx] = s;

            unsigned second_indx = polynomialHashFunction(holder1, BASE_H2, capacity);
            if (table_2[second_indx] == "") {
                table_2[second_indx] = holder1;
                break;
            } else {
                evicition_cout += 1;
                holder2 = table_2[second_indx];
                table_2[second_indx] = holder1;
                indx = polynomialHashFunction(holder2, BASE_H1, capacity);
                current = holder2;
            }
        }
    }

    if (evicition_cout == this->maxEviction) {
        unsigned prevCap = capacity;
        capacity = capacity * 2 + 1;
        while (isPrime(capacity) != true) {
            capacity++;
        }
        std::string *new_table_1 = table_1;
        std::string *new_table_2 = table_2;
        table_1 = new std::string[capacity];
        table_2 = new std::string[capacity];

        for (int i = 0; i < prevCap; i++) {
            if (new_table_1[i] != "") {
                insert(new_table_1[i]);
            }
        }
        for (int i = 0; i < prevCap; i++) {
            if (new_table_2[i] != "") {
                insert(new_table_2[i]);
            }
        }
        delete[] new_table_1;
        delete[] new_table_2;
        evicition_cout = 0;
        insert(s);
    } 
}



bool WordSet::contains(const std::string & s) const
{
	unsigned indx = polynomialHashFunction(s, BASE_H1, capacity);
    if (table_1[indx] == s) {
        return true;
    }

    indx = polynomialHashFunction(s, BASE_H2, capacity);
    if (table_2[indx] == s) {
        return true;
    }
    return false;
}

// return how many distinct strings are in the set
unsigned WordSet::getCount() const
{
	return unqiue; // this is not the right answer. 
}

// return how large the underlying array is.
unsigned WordSet::getCapacity() const
{
	return capacity; // that isn't even a prime number!
}




std::vector< std::string > convert(const std::string & s1, const std::string & s2, const WordSet & words)
{
    std::unordered_map<std::string, std::string> map;
	std::vector< std::string > res;

    std::queue<std::string> q;
    q.push(s1);
    map[s1] = "";

    while(q.size() > 0 && map.size() < words.getCount()) {

        std::string current = q.front();
        q.pop();
            
        for (int i = 0; i<current.length(); i++) {
            for (char c='a'; c <= 'z'; c++) {
                std::string temp = current;
                temp[i]= c;

                if (temp != current && words.contains(temp) && !map.count(temp)) {
                    if (temp == s2) {
                        map[temp] = current;
                        break;
                    }
                    q.push(temp);
                    map[temp] = current;
                }
            }
        }
    }

    for (std::string word = s2; word != ""; word = map[word]) {
        res.push_back(word);
    }
    std::reverse(res.begin(), res.end());

	return res;
}




int main() {

    WordSet ws(11);
    ws.insert("ant");
    ws.insert("BBB");
    ws.insert("art");
    ws.insert("are");

    std::vector<std::string> res = convert("ant", "art", ws);
    std::cout << "size :" << res.size() << std::endl;


    return 0;
}

// g++ Wordset.cpp -std=c++11 -o p3
