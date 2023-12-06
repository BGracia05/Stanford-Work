/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)

using namespace std;

/*
 * TODO: The OperatorsFrom function takes and returns a string that contains bracketing chars from input. it does this by looking at each character of the input
 */
string operatorsFrom(string str) {
    if (str.length() == 0) {
    return ""; // B.C
}
    if (str[0] == '[' || str[0] == '(' || str[0] == '{' || str[0] == ']' || str[0] == ')' || str[0] == '}') {
    return str[0] + operatorsFrom(str.substr(1));
    }
    else {
    return operatorsFrom(str.substr(1));
    }
}
/*
 * TODO: checks if a string that consists of brackets is balanced by removing pairs using recursion, leaving an empty string if true and false if theres more brackets
 */
bool operatorsAreMatched(string ops) {
    if (ops.length() == 0) {
        return true; // B.C
    }
    if (ops.find("[]") != string::npos) { // finds [] and erases the pair
        ops.erase(ops.find("[]]"), 2);
    }
    else if (ops.find("()") != string::npos) { // finds () and erases the pair
        ops.erase(ops.find("()"), 2);
    }
    else if (ops.find("{}") != string::npos) { // find {} and erases the pair
        ops.erase(ops.find("{}"), 2);
    }
    else { // returns false if there was an unpaired bracket
        return false;
    }
    return operatorsAreMatched(ops); //Recurse the remaining of the string to make sure
}
/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
STUDENT_TEST("operatorsFrom on a string without brackets") {
    EXPECT_EQUAL(operatorsFrom("Radiohead is the best band alive"), "");
    EXPECT_EQUAL(operatorsFrom("Especially after releasing In Rainbows"),"");
}
STUDENT_TEST("operatorsFrom on an empty string") {
    EXPECT_EQUAL(operatorsFrom(""), "");
}
STUDENT_TEST("operatorsFrom with incomplete bracket pairs") {
    EXPECT_EQUAL(operatorsFrom("[]{{}"),"[]{{}");
}
