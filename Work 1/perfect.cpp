/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "SimpleTest.h"
#include "console.h"
#include <iostream>
 // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* The divisorSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division using the modulus operator %
 *
 * Note: the C++ long type is a variant of int that allows for a
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

/* The isPerfect function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) {
            cout << "." << flush;
        } // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* By using smarter sum, I can make the code more simple by only using
 * sqrt numbers for n since the divisor will always be less than the sqrt
 */
long smarterSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < sqrt(n); divisor++) {
        if (n % divisor == 0) {
            total += divisor;
            total += n / divisor;
        }
    }
    return total;
}

/* TODO: The isPerfectSmarter function works exactly like isPerfect but it allows
 * more efficiency
 */
bool isPerfectSmarter(long n) {
   return (n != 0) && (n == smarterSum(n));
}

/* TODO: works exactly like findperfects but more efficient
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number using Smarter: " << num << endl;
        }
        if (num % 10000 == 0) {
            cout << "." << flush;
        } // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}



/* TODO: By writing down this function, I am setting variable k to 1
 * and m to (2^k) -1. if the divisor is equal to 1, that means a prime
 * number is found so therefore the m value is multiplied by our
 * perfect number value 2^k-1 in order to return the perfect number.
 * if fail,-1 will be returned.
 */
long findNthPerfectEuclid(long n) {
    long k = 1, m = pow(2,k) - 1;
    for (long divisor = 1; divisor < sqrt(n); divisor++) {
        if (divisor == 1) {
            long perfectnumber = pow(2,k-1) * m;
            return perfectnumber;


        }
}
    return -1;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}

// TODO: add your student test cases here
/* this student test is testing findperfects speed by starting with the int 15,000 with the largest being set to
 * 120,000. in my forloop, i made it so when i is smaller than 15,000 , it is to be
 * multiplied by 2 after a time operation is done. This effectively gives time operations
 * of 15,000 , 30,000 , 60,000 , and 120,000
*/

STUDENT_TEST("Four time trials of findPerfects on doubling input sizes") {

    int smallest = 15000, largest = 120000;

    for (int i = smallest; i <= largest; i *= 2) {
        TIME_OPERATION(i, findPerfects(i));
    }
}
/* by making the smallest int a negative number, I can run the student test in order to find if an error would pop up at the sight of a negative value*/
STUDENT_TEST("Negative perfect numbers test") {
    int smallest = -20000, largest = 15000;

    for (int i = smallest; i <= largest; i += 5000) {
        TIME_OPERATION(i, findPerfects(i));
    }
}
/*
 * This student test is testing smartersum's accuracy by comparing it to the divisorsum
 * values to make sure they are equal.
*/

STUDENT_TEST("smarterSum expected value test") {
    EXPECT_EQUAL(smarterSum(1), divisorSum(1));
    EXPECT_EQUAL(smarterSum(0), divisorSum(0));
    EXPECT_EQUAL(smarterSum(-1), divisorSum(-1));
}
/* this student test is testing findperfectssmarter's speed by starting with the int 15,000 with the largest being set to
 * 120,000. in my forloop, i made it so when i is smaller than 15,000 , it is to be
 * multiplied by 2 after a time operation is done. This effectively gives time operations
 * of 15,000 , 30,000 , 60,000 , and 120,000
*/
STUDENT_TEST("Four time trials of findPerfectsSmarter on doubling input sizes") {

    int smallest = 15000, largest = 120000;

    for (int i = smallest; i <= largest; i *= 2) {
        TIME_OPERATION(i, findPerfectsSmarter(i));
    }
}

     /* This student test is testing the values of findNthPerfectEuclid by comparing them
      * to ispefect
 */
STUDENT_TEST("four Nth perfect euclid trials") {
    EXPECT_EQUAL(isPerfect(6), findNthPerfectEuclid(6));
    EXPECT_EQUAL(isPerfect(28), findNthPerfectEuclid(28));
    EXPECT_EQUAL(isPerfect(496), findNthPerfectEuclid(496));
    EXPECT_EQUAL(isPerfect(8128), findNthPerfectEuclid(8128));
}
