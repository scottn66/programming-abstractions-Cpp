// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "timer.h"
using namespace std;

/* This function takes in a number n and calculates the sum
 * of all proper divisors of n, excluding itself.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function is provided a number n and returns a boolean
 * (true/false) value representing whether or not the number is
 * perfect. A perfect number is a non-zero positive number whose
 * sum of its proper divisors is equal to itself.
 */
bool isPerfect (long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function performs an exhaustive search for perfect numbers.
 * It takes as input a number called `stop` and searches for perfect
 * numbers between 1 and `stop`. Any perfect numbers that are found will
 * be printed out to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush;
    }
    cout << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 * checks numbers between one and the squareroot of n, and adds all of the divisors.
 * it also finds the complementary divisors of numbers of n. ig ( 3 = 6/2) or 28: (1, 2 28/2 = 14, 4 28/4 = 7
 * add complements except for 1's complement
 * in the case of squareroot functions like 25, 5, 25/5 so 5 only should be added once
 */
long smarterSum(long n) {
    long total = 1;
    for (long divisor = 1; divisor < sqrt(n); divisor++) {
        if (n % divisor == 0) {
            if (sqrt(n) != divisor) {
                if (divisor != 1){
                    int complementary = n/divisor;
                    total += complementary;
                    total += divisor;
                }
            }
        }
    }
    return total;
}

/* isPerfectSmarter checks if the iterative n number is equal to the sum of all its divisors (excluding n itself)
 * if they are equal returns true
 */
bool isPerfectSmarter(long n){
    return (n == smarterSum(n));
}

/* function implements both smarter functions to find sum of divisors and check if that sum is a perfect number
 */
void findPerfectsSmarter(long stop){
     for (long num = 1; num < stop; num++) {
         if (isPerfectSmarter(num)) {
             cout << "Found (smarter) Perfect number: " << num << endl;
         }
     }
     cout << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */


bool isPrime(long n) {
    bool grul = true;
    if (n >= 2) {
        while (grul == false) for (int i = 2; i <= sqrt(n); i++) {
            if (n % i == 0) {
                grul = false;
            }
        }
    }
    return grul;
}
/* if, for some k > 1, 2^k -1 is prime. then (2^(k-1)) * (2^k -1) is a perfect number
 * findNtPerfectEuclid loops through values of k until n finding perfect numbers through Euclid's formula
 * when the counter is equal to the nth perfect number you need, then return the nth number you needed
 */

long solution(long k) {
    return (pow(2, k-1) * (pow(2, k) - 1));
}


//long findNthPerfectEuclid(long x){
//    int counter = 0;
//    for (int k = 0; x == counter; k++) {
//        long m = (pow(2, k) - 1);
//        if (isPrime(m)) {
//            long perfect = solution(k);
//            counter += 1;
//            if (x == counter) {
//                cout << "the " << x << "th perfect number is: " << perfect << endl;
//            }
//        }
//    }
//    return 0;
//}
long findNthPerfectEuclid(long nth) {
    int counter = 0;
    long perfect = 0;
    for (int k = 0; nth != counter; k++) {
        long m = pow(2, k) - 1;
        if (isPrime(m)) {
            long perfect = solution(m);
            if (isPerfectSmarter(perfect)) {
                counter += 1;
            }
        }
    }
     return perfect;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

//PROVIDED_TEST("Time multiple trials of findPerfects function on doubling input sizes."){
//    cout << endl;
//    TIME_OPERATION(20000, findPerfects(20000));
//    TIME_OPERATION(40000, findPerfects(40000));
//    TIME_OPERATION(80000, findPerfects(80000));
//    TIME_OPERATION(160000, findPerfects(160000));
//    TIME_OPERATION(320000, findPerfects(320000));
//    TIME_OPERATION(640000, findPerfects(640000));
//}

PROVIDED_TEST("Check divisor sums of small numbers") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Check 6 and 28, should be perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Imperfect numbers") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Oddballs, zero, one, negative") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
    EXPECT(!isPerfect(-6));
}

PROVIDED_TEST("33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

//TODO: add your test cases here

STUDENT_TEST("Testing for perfect numbers") {
    EXPECT(isPerfectSmarter(6));
    EXPECT(isPerfectSmarter(28));
}
STUDENT_TEST("TIME TRIALS") {
    TIME_OPERATION(20000, isPerfectSmarter(20000));
    TIME_OPERATION(40000, isPerfectSmarter(40000));
    TIME_OPERATION(80000, isPerfectSmarter(80000));
    TIME_OPERATION(160000, isPerfectSmarter(160000));
    TIME_OPERATION(320000, isPerfectSmarter(320000));
    TIME_OPERATION(640000, isPerfectSmarter(640000));
}
STUDENT_TEST("33550336 is perfect") {
    EXPECT(isPerfectSmarter(33550336));
}

STUDENT_TEST("Oddballs, zero, one, negative") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
    EXPECT(!isPerfect(-6));
}

STUDENT_TEST("Check divisor sums of small numbers") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}
STUDENT_TEST("tests isPrime function") {
    EXPECT(isPrime(7));
    EXPECT(!isPrime(4));
    EXPECT(!isPrime(32));
    EXPECT(!isPrime(-1));
    EXPECT(!isPrime(0));
    EXPECT(isPrime(3));
}

STUDENT_TEST("checks the Mersenne Prime test") {
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
    EXPECT_EQUAL(findNthPerfectEuclid(2), 28);
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
    EXPECT_EQUAL(findNthPerfectEuclid(4), 8128);
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
}
