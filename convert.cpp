#include "convert.hpp"
#include "Wordset.hpp"
#include <iostream>
#include <set>
#include <sstream>
#include <map>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>

// You should not need to change this function.
void loadWordsIntoTable(WordSet & words, std::istream & in)
{
	std::string line, word;
	std::stringstream ss;

	while(	getline(in, line) )
	{
		ss.clear();
		ss << line;
		while( ss >> word )
		{
			words.insert( word );
		}
	}
}



std::vector< std::string > convert(const std::string & s1, const std::string & s2, const WordSet & words)
{
	std::vector<std::string> res;
    std::unordered_map<std::string, std::string> map;
    std::queue<std::string> q;
    q.push(s1);

    while (q.empty() != true && map.size() < words.getCount()) {
        std::string current = q.front();
        q.pop();

        for (int i = 0; i < current.size(); i++) {
            for (char letter = 'a'; letter <= 'z'; letter++) {

                std::string new_word = current;
				
                new_word[i] = letter;

				if (new_word == current) {
					continue;

				} else if (words.contains(new_word) && !map.count(new_word)) {
					
                    if (new_word == s2) {
                        map[new_word] = current;
                        break;
                    }

                    q.push(new_word);
                    map[new_word] = current;
                }
            }
        }
    }

	std::stack<std::string> final_words;
	std::string temp = s2;
	while (temp != s1) {
		final_words.push(temp);
		temp = map[temp];
	}
	final_words.push(s1);
    
	while (final_words.empty() != true) {
		res.push_back(final_words.top());
		final_words.pop();
	}

	return res; 
}
