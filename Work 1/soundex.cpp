/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "map.h"
#include "SimpleTest.h"
// IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;
string lettersOnly(string s) {
    string result = charToString(s[0]);
    for (int i = 1; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}
string soundex(string ID)
{
    int x;
    Map<char,char> skey;  { ('A', '0'), ('E', '0'), ('I', '0'), ('O', '0'), ('U', '0'), ('H', '0'), ('W', '0'), ('Y', '0'),
                            ('B', '1'), ('F', '1'), ('P', '1'), ('V', '1'),
                            ('C', '2'), ('G', '2'), ('J', '2'), ('K', '2'), ('Q', '2'), ('S', '2'), ('X', '2'), ('Z', '2'),
                            ('D', '3'), ('T', '3'),
                            ('L', '4'),
                            ('M', '5'), ('N', '5'),
                            ('R', '6'); }
    string result = ID;
    /* rid of non-alphabetic letters and uppercases the resulting string*/
    x = 0;
    while (x < result.length()) {
        if (!isalpha(result[x])) {
            result.erase(x, 1);
        } else {
            result[x] = toupper(result[x]);
            x++;
        }
    }
    char firstLetter = toupper(ID[0]);

    /* Map the characters to digits */
    for (char& c : result) {
        if (skey.containsKey(c)) {
            c = skey[c];
        }
    }

    /* get rid of all duplicate digits and make it one*/
    x = 1;
    while (x < result.length()) {
        if (isdigit(result[x]) && result[x] == result[x - 1]) {
            result.erase(x, 1);
        } else {
            x++;
        }
    }

    /*Replace first letter*/
    result[0] = firstLetter;

    /* get the length to be 4 to ensure it fits soundex form*/
    result += "000";
    result = result.substr(0, 4);

    return result;
}

int main() {
    std::string ID;
    while (true) {
        std::cout << "Enter a word (empty to end): ";
        std::getline(std::cin, ID);
        if (ID.empty())
            break;
        std::cout << "Soundex representation is " << soundex(ID) << endl;
    }
    return 0;
}

/* This function is intended to return a string which
        * includes only the letter characters from the original
        * (all non-letter characters are excluded)
        *
        * WARNING: The provided code is buggy!
                   *
                   * Use test cases to identify which inputs to this function
        * are incorrectly handled.
        * TODO: Remove this comment and replace it with a
            * description of the bug you fixed.

                      * }
*/

void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, allNames);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    // The names read from file are now stored in Vector allNames

    /* TODO: Fill in the remainder of this function. */
}

/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here


