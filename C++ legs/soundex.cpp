
//Scott Nelson 1st July 2020. Programming Abstractions in C++

#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include <fstream>
#include <cctype>
#include <string>
#include "simpio.h"
#include "vector.h"
using namespace std;

string removeNonLetters(string s);
string checktoUpper(string s);
string coaleseAdjacent(string s);
string noGooseEgg(string s);
string lengthFour(string s);

/* This function takes a string s and returns a version of
 * the string that has all non-letter characters removed
 * from it.
 *
 * This function could not detect two nonletters in a row, because
 * the loop would increase as each index decreased one, so instead of editing the
 * string being looped, I created a new string made up of only the
 *alphabetic characters from the string
 */
// This function removes any non alphabetic character from the string passed through

string removeNonLetters(string s) {
    int l = s.length();
    string nonLettered ="";
    for (int i = 0; i < l; i++) {
        if (isalpha(s[i])) {
            nonLettered += (s[i]);
        }
    }
    return nonLettered;
}

// # 2, 3, and 5. capitalize each character, saves the 1st letter to newString, and creates the numeric, full code as newString

string checktoUpper(string s) {
   s = toUpperCase(s);
   char firstChar = s[0];
   s.erase(0, 1);
   int l = s.length();
   string newString = charToString(firstChar);
   for (int i = 0; i < l; i++) {
       //implementation of given table for letter -> number

       if (s[i] == 'A' || s[i] == 'E' || s[i] == 'I' || s[i] == 'O' || s[i] == 'U' || s[i] == 'H' || s[i] == 'W' || s[i] == 'Y') {
           newString.append("0");
       }
       if (s[i] == 'B' || s[i] == 'F' ||s[i] == 'P' || s[i] == 'V') {
           newString.append("1");
       }
       if (s[i] == 'C' || s[i] == 'G' || s[i] == 'J' || s[i] == 'K' || s[i] == 'Q' || s[i] == 'S' || s[i] == 'X' || s[i] == 'Z') {
           newString.append("2");
       }
       if (s[i] == 'D' || s[i] == 'T') {
           newString.append("3");
       }
       if (s[i] == 'L') {
           newString.append("4");
       }
       if (s[i] == 'M' || s[i] == 'N') {
           newString.append("5");
       }
       if (s[i] == 'R') {
           newString.append("6");
       }
   }
   return newString;
}


// #4. coalese adjacent duplicates (N222025 -> N2025)

string coaleseAdjacent(string s) {
    int stringLength = s.length();
    string simpler = "";
    for (int i = 0; i < stringLength; i++) {
        if (s[i] != s[i+1]) {
            simpler += s[i];
        }
    }
    return simpler;
}



//6. Remove all Zero's (N2025 -> N225)

string noGooseEgg(string s) {
    int stringLength = s.length();
    string nonzeros = "";
    for (int i = 0; i < stringLength; i++) {
        if (s[i] != '0') {
            nonzeros += s[i];
        }
    }
    return nonzeros;
}


// 7. Format to length 4 by either truncating (i.e. cutting off end) or adding zeros  N5425, N54
// Length must be four(including Letter) for proper code.
string lengthFour(string s) {
    int length = s.length();
    if (length == 4) {
        return s;
    }
    else if (length > 4) {
        s = s.substr(0, 4);
    }
    else if (length < 4) {
        while (s.length() < 4) {
            s = s.append("0");
        }
    }
    return s;

}



/* This soundex function utilizes all of the functions needed to compute the soundex codename for strings
 */
string soundex(string s) {
    string step1 = removeNonLetters(s);
    string step2 = checktoUpper(step1);
    string step3 = coaleseAdjacent(step2);
    string step4 = noGooseEgg(step3);
    string step5 = lengthFour(step4);
    return step5;
}


/* Given a file, soundexSearch will read this file and fill-in vector dataBaseNames with surnames
 *
 */
void soundexSearch(string filepath) {
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames
    
/* Psedocode!
* repeatedly prompt the user for surname with while loop,
* when they are finished they enter an empty string, breaking out of the code and maybe printing all done
*
* compute the soundex code for the given surname. loop through the vector(dataBaseNames) and compute the code for every entry in the vector.
* store any matching code in a new string vector called names.
* when looping through lines completes, .sort() the vector into alphabetical order and cout the vector
*/

    while (true) {
        string inputStr = getLine("Please enter a surname: (HIT RETURN TO QUIT) ");
        if (inputStr == "") break;
        string codename = soundex(inputStr);
        Vector<string> names = {};
        cout << "The soundex code of " << inputStr << " is " << codename << endl;
        for (int i = 0; i < databaseNames.size(); i++) {
            if (soundex(databaseNames[i]) == codename) {
                names += databaseNames[i];
            }
        }
        names.sort();
        cout << "Matches from database" << names << endl;

    }
    cout << "ALL Done!" << endl;

}

/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Master"), "M236");
    EXPECT_EQUAL(soundex("Jue"), "J000");
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

STUDENT_TEST("Test removing puntuation, digits, and spaces") {
    EXPECT_EQUAL(removeNonLetters("5cott"), "cott");
    EXPECT_EQUAL(removeNonLetters(" 55 c:ott"), "cott");
}
STUDENT_TEST("check to Upper - encoding part") {
    EXPECT_EQUAL(checktoUpper("nelson"), "N04205");
}
STUDENT_TEST("Coalese function - adjacent duplicate elimination") {
    EXPECT_EQUAL(coaleseAdjacent("L2209933555"), "L20935");
}
STUDENT_TEST("no goose egg - no zeros in the codename") {
    EXPECT_EQUAL(noGooseEgg("0450"), "45");
}
STUDENT_TEST("Length Four - shortens the code into four elements") {
    EXPECT_EQUAL(lengthFour("N5425"), "N542");
    EXPECT_EQUAL(lengthFour("L"), "L000");
}
STUDENT_TEST("running function through removeNonLetters") {
    string testing123 = "Nelson";
    EXPECT_EQUAL(removeNonLetters(testing123), "Nelson");
}
STUDENT_TEST("like ok") {
    EXPECT_EQUAL(soundex("Nelson"), "N425");
    EXPECT_EQUAL(soundex("Zelenski"), "Z452");
}
STUDENT_TEST("Testing Bowman and my last name") {
    EXPECT_EQUAL(soundex("Bowman"), "B550");
    EXPECT_EQUAL(soundex("nelson"), "N425");
}
