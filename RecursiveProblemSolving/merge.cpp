/* 
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/* 
 * TODO: Replace this comment with a descriptive function
 * header comment.
 * a{2, 4, 5}    b{1, 3, 3}
 */


Queue<int> merge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    /* Iterative: until one of the queues are empty, if one is empty, add the rest of the full one to the result queue */
    // , compare the each queue.front and enqueue the min() of the two
    int a1 = a.size();
    int b1 = b.size();
    int length = a1 + b1;
    for (int i = 0; i < length; i++) {
        if (a.isEmpty()) {
            //enqueue the rest of b
            while (!b.isEmpty()) {
                result.enqueue(b.dequeue());
            }
            return result;
        }
        if (b.isEmpty()) {
            //enqueue the rest of a
            while (!a.isEmpty()) {
                result.enqueue(a.dequeue());
            }
            return result;
        }

        //enqueue the minimum value between front of a and front of b to the front of RESULTing QUEUE
        if (a.peek() < b.peek()){
            result.enqueue(a.dequeue());
        } else {
            result.enqueue(b.dequeue());
        }
    }
    return result;
}

/* 
 * This function assumes correct functionality of the previously
 * defined merge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented – 
 * it does not need to modified at all.
 */
Queue<int> multiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    for (Queue<int>& q : all) {
        result = merge(q, result);
    }
    return result;
}

/* 
 * base case, when alll vectors are size 1 or wehen there are no more vectors of size greater than one. when ALL the Vectors cannot be split in half
 * if Vector-all is length 1 it should start to merge with all the other vectors
 * Divide k sequences into two halves, left being first half, right being 1/2 to end,
 * Recursively apply MultiMerge to the left half of the sequences, and then on the right half to create two combined sequences
 * then use binary merge function to combine the final two.
 * return that final sequence.
 * - Base Case ->
 *       {[5, 7, 20, 1]}
 * call merge in base case and once in recursion
 * 3 main while loops
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    int vectorLength = all.size();             // vectorLength is the length of Vector parameter all


    Vector<Queue<int>> firstHalf = all.subList(0, vectorLength/2);     //new Vector of first half of sequences/queues

    // CHECk if length is odd to include the last sequence
    int cvectorLength = vectorLength;
    if (vectorLength % 2 == 1) {
        vectorLength += 1;
    }

    Vector<Queue<int>> secondHalf = all.subList(cvectorLength / 2, vectorLength/2);    //New Vector called secondHalf


    //Base Case HERE :  if the new vectors are totally split, pass them into merge to put them in order
    if (firstHalf.size() == 1 && secondHalf.size() == 1) {
        return merge(firstHalf[0], secondHalf[0]);
    }

    // Recursive cases
    if (all.size() > 1) {
        recMultiMerge(firstHalf);

        recMultiMerge(secondHalf);
    }
    all.clear();            // wipes the recently split Vector clean for trip returning call stack
    //merge must be called through a helper function or with two sorted Queues as parameters?

    //Queue<int> merge(Queue<int> a, Queue<int> b)
    //Base case vectorLength = 0?

    return result;

}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("Test binary merge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

//PROVIDED_TEST("Test multiMerge, small collection of short sequences") {
//    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
//                             {1, 5, 9, 9, 12},
//                             {5},
//                             {},
//                             {-5, -5},
//                             {3402}
//                            };
//    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
//    EXPECT_EQUAL(multiMerge(all), expected);
//}

PROVIDED_TEST("Test recMultiMerge by compare to multiMerge") {
    //int n = 1000;
    //delete after testing ^<>^
    int n = 5;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(multiMerge(all), recMultiMerge(all));
}

//PROVIDED_TEST("Time binary merge operation") {
//    int n = 1500000;
//    Queue<int> a = createSequence(n);
//    Queue<int> b = createSequence(n);
//    TIME_OPERATION(a.size(), merge(a, b));
//}

//PROVIDED_TEST("Time multiMerge operation") {
//    int n = 11000;
//    int k = n/10;
//    Queue<int> input = createSequence(n);
//    Vector<Queue<int>> all(k);
//    distribute(input, all);
//    TIME_OPERATION(input.size(), multiMerge(all));
//}
//PROVIDED_TEST("Time recMultiMerge operation") {
//    int n = 90000;
//    int k = n/10;
//    Queue<int> input = createSequence(n);
//    Vector<Queue<int>> all(k);
//    distribute(input, all);
//    TIME_OPERATION(input.size(), recMultiMerge(all));
//}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].add(input.dequeue());
    }
}

STUDENT_TEST("a has no values") {
    Queue<int> a = {};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 3, 3};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);

}
