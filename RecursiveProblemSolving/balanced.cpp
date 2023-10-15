/* 
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/* This function recursively reverses the characters of the provided
 * string and returns the reversed string.
 */
//string reverse(string s) {
//    if (s == "") {
//        return "";
//    }
//    return reverse(s.substr(1)) + s[0]; // Builds up the string by moving
//                                        // the first letter to the end

//    // Builds up the string by moving the last letter to the front
//    //return s[s.length() - 1] + reverse(s.substr(0, s.length() - 1));
//}

/* take s[0] and process that(if its one of the operators) and then recurse on s.substr(1) */
//control reaching non-void function -> helper function, void?
//
string operatorsOnlyHelper(string s, string operators) {
    Set<char> findme = {'(',')','{','}','[',']'};
    //operators = "";
    if (s == "") {
        return operators;
    }
    char process = s[0];
    if (findme.contains(process)) {
        operators += process;
    }
    return operatorsOnlyHelper(s.substr(1), operators);
}


string operatorsOnly(string s) {
    string operators = "";
    return operatorsOnlyHelper(s, operators);
}

/* 
 * TODO: Replace this comment with a descriptive function
 * "{ [ () ] }" true "(()" false
 */
bool checkOperators(string s) {
    if (s == "") {
        return true;
    }
    int index = s.find("()");
    //if (s[index + 1] == ')') {
    if (index != string::npos) {
        return checkOperators(s.substr(0, index) + s.substr(index + 2));
    }
    int index1 = s.find("[]");
    if (index1 != string::npos) {
    //if (s[index1 + 1] == ']') {
        return checkOperators(s.substr(0, index1) + s.substr(index1 + 2));
    }
    int index2 = s.find("{}");
    //if (s[index2 + 1] == '}') {
    if (index2 != string::npos) {
        return checkOperators(s.substr(0, index2) + s.substr(index2 + 2));
    }

    return false;
}

/* 
 * This function assumes correct implementation of the previous 
 * two functions. It uses the operatorsOnly function to strip out
 * all characters from the provided string and then checks to see
 * whether the remaining operators are balanced by using the 
 * checkOperators function. All in all, this function is able to 
 * correctly determine whether a snippet of code has correctly
 * balanced bracketing operators. You should not need to modify this
 * function if the previous two functions have been implemented
 * correctly. 
 */
bool isBalanced(string str) {
    string ops = operatorsOnly(str);
    return checkOperators(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsOnly on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    string only = "(){([])(())}";
    EXPECT_EQUAL(operatorsOnly(example), only);
}

PROVIDED_TEST("checkOperators on example from writeup") {
    string only = "(){([])(())}";
    EXPECT(checkOperators(only));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on illegal examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

// Scott's Tests

STUDENT_TEST("operatorsOnly examples to track") {
    string example ="in(+)it";
    string only = "()";
    EXPECT_EQUAL(operatorsOnly(example), only);
}
STUDENT_TEST("operatorsOnly examples to track") {
    string example1 ="in(whatss) in your head {} [] duhh";
    string only1 = "(){}[]";
    EXPECT_EQUAL(operatorsOnly(example1), only1);
}
STUDENT_TEST("checkOperators() checkers") {
    string examine = "((){}";
    EXPECT(!checkOperators(examine));
}
STUDENT_TEST("checkOperators() checkers") {
    string examiner = "( ( [ a ] )";
    EXPECT(!isBalanced(examiner));
}
STUDENT_TEST("checkOperators() checkers") {
    string examiner = "int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(examiner));
}

