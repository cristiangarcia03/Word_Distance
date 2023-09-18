#include "Wordset.hpp"
#include <string>


unsigned polynomialHashFunction(const std::string & s, unsigned base, unsigned mod)
{
    unsigned res = 0;

    char letter;
    for (int i = 0; i < s.size(); i++){
        res *= base;
        letter = s[i] - 96;
        res += letter % mod;
    }
    return res % mod;
}



WordSet::WordSet(unsigned initialCapacity, unsigned evictionThreshold)
{
    table_1 = new std::string[initialCapacity];
    table_2 = new std::string[initialCapacity];

    MaxEvictions = evictionThreshold;

    capacity = initialCapacity;
    unqiue = 0;
}


WordSet::~WordSet()
{
    delete[] table_1;
    delete[] table_2;
}



bool WordSet::isPrime(unsigned n){
    if (n <= 1) {
        return false;
    }
    
    for (int i = 2; i * i <= n; i++) {

        if (n % i == 0)
            return false;
    }
    return true;
}

void WordSet::insert(const std::string & s)
{
    if (contains(s)) {
        return;
    }

    unqiue++;
    
    std::string holder1;
    std::string holder2;
    std::string current = s;
    evicted_count = 0;

    unsigned indx = polynomialHashFunction(s, BASE_H1, capacity);
    while (evicted_count < MaxEvictions) {

        if (table_1[indx] == "") { 
            table_1[indx] = current;
            break;
        } else if (table_1[indx] != "") {  //its taken
            holder1 = table_1[indx]; 
            table_1[indx] = current;

            unsigned indx2 = polynomialHashFunction(holder1, BASE_H2, capacity);

            if (table_2[indx2] == "") {
                table_2[indx2] = holder1;
                break; //IMportant
                
            } else {
                evicted_count++;
                holder2 = table_2[indx2];
                table_2[indx2] = holder1;

                //for the next loop
                indx = polynomialHashFunction(holder2, BASE_H1, capacity);
                current = holder2;
            }
        }
    }
    
    if (MaxEvictions == evicted_count) {

        unsigned preCap = capacity; //must save for lopping later

        capacity = capacity * 2 + 1;
        while (isPrime(capacity) == false) {
            capacity++;
        }
        
        std::string *new_table_1 = table_1;
        std::string *new_table_2 = table_2;

        table_1 = new std::string[capacity];
        table_2 = new std::string[capacity];
        
        for (int i = 0; i < preCap; i++) {
            if (new_table_1[i] != ""){
                insert(new_table_1[i]);
            }
        }
        for (int i = 0; i < preCap; i++){  //rehashing
            if (new_table_2[i] != ""){
                insert(new_table_2[i]);
            }
        }

        delete[] new_table_1;
        delete[] new_table_2;
    }
}

bool WordSet::contains(const std::string & s) const
{
    unsigned indx = polynomialHashFunction(s, BASE_H1, capacity);
    if (table_1[indx] == s){
        return true;
    }
    indx = polynomialHashFunction(s, BASE_H2, capacity);
    if (table_2[indx] == s) {
        return true;
    }
    return false;
}


unsigned WordSet::getCount() const
{
    return unqiue;
}


unsigned WordSet::getCapacity() const
{
    return capacity;
}
