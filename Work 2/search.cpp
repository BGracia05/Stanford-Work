// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;


// TODO: cleanToken takes a string (s) and returns a clean version of it
string cleanToken(string s) {
    string clean;
    string lower = "0123456789abcdefghijklmnopqrstuvwxyz";
        if (s == "") {
            return s;
    }
    s = toLowerCase(s);
    for (char ch : s) {
            if (lower.find(ch) != string::npos) {
                clean += ch;
            }
    }
    return clean;
}

// TODO: takes a string and returns a set of cleaned tokens
Set<string> gatherTokens(string text) {
    Set<string> tokens;
    Vector<string> Words = stringSplit(text, "");
    string clean;
    for (string str : Words) {
            clean = cleanToken(str);
            if (clean != "") {
                tokens.add(clean);
            }
    }
    return tokens;

}

// TODO: buildIndex takes a vector made out of lines and maps which is referred to as an index. with the index it builds a search engine with information extracted from the input
int buildIndex(Vector<string>& lines, Map<string, Set<string>>& index) {
    Set<string> words;
    int i = 1;
    for (string str: lines) {
        if (i % 2 == 1) {
            words = gatherTokens(str);
                i++;
        }
        i++;
        for (string token : words) {
            for (int j = 0; j < index.size(); j += 2) {
                index.put(token, {});
            }
        }
    }
    for (int i = 0; i < lines.size(); i +=1) {
        words = gatherTokens(lines[i + 2]);
        for (string word : words) {
            index.put(word, {});
        }
    }
    for (int i = 0; i < lines.size(); i += 2) {
        words = gatherTokens(lines[i + 1]);
        for (string word : index) {
            if (words.contains(word)) {
                index[word].add(lines[i]);
            }
        }
    }
    return lines.size() / 2;
}

// TODO: takes the index and a query string to search the index based on the query.
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query) {
    Set<string> result;
    Vector<string> task = stringSplit(query, " ");
    Set<string> set1 = index.get(task[0]);
    if (task.size() == 1) {
        return set1;
    }

    for (int i = 1; i < task.size(); i++) {
        if (task[i].find('+') != string::npos) {
            result += set1.intersect(index [task[i].substr(1)]);

        if (task[i].find('+') != string::npos){
            result += set1.difference(index [task[i].substr(1)]);
        }
            else {
                result += set1.unionWith(index[task[i]]);
        }
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(Vector<string>& lines); {
    Map<string, Set<string>> index;
    cout << "Building index...." << endl;
    int numofPages = buildIndex(lines, index);
    cout << "Indexed" << numofPages << " pages containing " << index.size() << " unique terms." << endl;
    cout << endl;
    while (true) {
        string prompt = getLine("Enter your query");
        if (prompt.length() == 0) {
                cout << "completed" << endl;
                break;
        }
        else {
                prompt = cleanToken(prompt);
                Map<string, Set<string>> currentIndex = index;
                buildIndex(lines, currentIndex);
                cout << "Found " << currentIndex.size() << " matching pages" << endl;
                cout << currentIndex << endl;
                cout << endl;
        }
    }
}

/*
 * The given readDatabaseFile function correctly reads a well-formed
 * website database from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readDatabaseFile(string filename, Vector<string>& lines) {
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    readEntireFile(in, lines);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings of letters and digits") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS106B"), "cs106b");
}

PROVIDED_TEST("cleanToken on strings containing punctuation") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
    EXPECT_EQUAL(cleanToken("they're"), "theyre");
}

PROVIDED_TEST("cleanToken on string of only punctuation") {
    EXPECT_EQUAL(cleanToken("#$^@@.;"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 6 unique tokens, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 6);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 12 unique tokens") {
    Vector<string> lines;
    Map<string, Set<string>> index;
    readDatabaseFile("res/tiny.txt", lines);
    int nPages = buildIndex(lines, index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 12);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Vector<string> lines;
    Map<string, Set<string>> index;
    readDatabaseFile("res/tiny.txt", lines);
    buildIndex(lines, index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Vector<string> lines;
    Map<string, Set<string>> index;
    readDatabaseFile("res/tiny.txt", lines);
    buildIndex(lines, index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("cleanToken on string composed of symbols") {
    EXPECT_EQUAL(cleantoken("$#^&*"), "")
}
STUDENT_TEST("cleanToken on empty string") {
    EXPECT_EQUAL(cleantoken(""), "")
}
STUDENT_TEST("cleanToken on weird string") {
    EXPECT_EQUAL(cleantoken("Wjfiogj13241!"), "wjfiogj13241")
}
}
