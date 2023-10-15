#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;

/* Constructor for Priority Queue min Heap */
PQHeap::PQHeap() {
    numItems = 0;
    capacity = INITIAL_CAPACITY;
    elements = new DataPoint[capacity];
}

/*Destructor for a Priority Queue minHeap
 */
PQHeap::~PQHeap() {
    delete[] elements;
}

/* BUBBLE UP:
 * used when a new element is enqueued, high priority (lower integers) values
    "bubble up" to their appropriate position within the heap/tree
*/
void PQHeap::bubbleUp() {
    int index = numItems - 1;
    while (hasParent(index) && parent(index) > elements[index].priority) {
        swap(getParentIndex(index), index);
        index = getParentIndex(index);
    }
}

/* BUBBLE DOWN:
 * When an element is dequeued, it is picked from the root (highest priority value),
 * and is replaced by the last element in the queue.
 * Bubble down demotes this replacement until it is positioned based on priority value
 * and the heap's parent-child relation
    */
void PQHeap::bubbleDown() {
    int index = 0;
    while (hasLeftChild(index)) {
        int smallerChild = getLeftChildIndex(index);
        if (hasRightChild(index) && rightChild(index) < leftChild(index)) {
            smallerChild = getRightChildIndex(index);
        }

        if (elements[index].priority < elements[smallerChild].priority) { break;
        } else {
            swap(index, smallerChild);
        }

        index = smallerChild;
    }
}


/* ENQUEUE:
 * puts new DataPoint(elem) at the bottom left-most location in the tree
 * and subsequently promotes that DataPoint to its proper position.
 */
void PQHeap::enqueue(DataPoint elem) {
    if (numItems == capacity) { expand(); }
    elements[numItems] = elem;
    numItems++;
    bubbleUp();
}

/*
 * simply returns the value, without removing it, of the last element in the array
 * bottom-most, right-most element in the tree
 */
DataPoint PQHeap::peek() const {
    /* TODO: Fill in the remainder of this function. */
    if (numItems == 0) {
        error("cannot peek at empty Heap");
    }
    int lastOccupiedSlot = numItems - 1;
    return elements[lastOccupiedSlot];
}

//returns first element in the array, expected to be the minimum
DataPoint PQHeap::rootpeek() {
    if (numItems == 0) {
        error("cannot peekroot at empty Pqueue");
    }
    return elements[0];
}


/* DEQUEUE:
 * pulls the root(highest priority item) out and replaces the last element at that
 * index location and subsequently bubbles it down.
 */
DataPoint PQHeap::dequeue() {
    if (numItems == 0) {
        error("operation pullRoot cannot execute on empty PQ");
    }
    DataPoint root = elements[0];
    elements[0] = elements[numItems - 1];
    numItems--;
    bubbleDown();
    return root;
}

/*isEMPTY:
 * returns true if number of items is 0, and false otherwise.
 *
 */
bool PQHeap::isEmpty() const {
    return numItems == 0;
}

/*
 * The current size of the queue is stored internally as the member
 * variable numItems. This function returns that value.
 */
int PQHeap::size() const {
    return numItems;
}

/*
 * Updates internal state to reflect that no elements are stored in the
 * queue any longer. No memory deallocation or clearing of previously
 * stored values is necessary, as those will eventually be overwritten
 * by new values stored in the queue.
 */
void PQHeap::clear() {
    numItems = 0;
}

/*
 * Prints out the state of the internal array.
 */
void PQHeap::printDebugInfo() {
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << elements[i] << endl;
    }
}

/*
 * Traverses the internal elements of the array and validates that the
 * elements are stored in decreasing order of priority. Throws an error if
 * any out of order elements are found. Also throws an error if the number
 * of items stored is greater than the allocated capacity.
 */
void PQHeap::validateInternalState(){
    /*
     * If the array only has zero or one elements in it, then they
     * must necessarily be in sorted order.
     */
    if (size() == 0 || size() == 1){
        return;
    }

    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (size() > capacity) error("Too many elements in not enough space!");
    /* This for loop walks over the elements in the array, comparing
     * neighboring pairs to one another. If the current element has larger
     * priority than the last element, then the array has not been maintained
     * in decreasing sorted order, and we should throw an error.
     */
    DataPoint lastElem = elements[0];
    for (int i = 1; i < size(); i++){
        DataPoint cur = elements[i];
        if (cur.priority < lastElem.priority){
            error("Internal state of the array is out of order!");
        }
        lastElem = cur;
    }
}

/* 
 * retrieves the index values
 */
int PQHeap::getParentIndex(int curIndex){
    /* This returns the parent index value from place index curIndex */
    return (curIndex - 1) / 2;
}
int PQHeap::getLeftChildIndex(int curIndex){
    /* To find the left child from parent curIndex */
    return (curIndex * 2 + 1);
}
int PQHeap::getRightChildIndex(int curIndex){
    /* function to find the right child from parent curIndex */
    return (curIndex * 2 + 2);
}


//check to see if values are valid/exist
bool PQHeap::hasLeftChild (int curIndex) {
    return getLeftChildIndex(curIndex) < numItems;
}
bool PQHeap::hasRightChild(int curIndex) {
    return getRightChildIndex(curIndex) < numItems;
}
bool PQHeap::hasParent(int curIndex) {
    return getParentIndex(curIndex) >= 0;
}

//retrieve the values, rather than the index
int PQHeap::leftChild(int curIndex) {
    return elements[getLeftChildIndex(curIndex)].priority;
}
int PQHeap::rightChild(int curIndex) {
    return elements[getRightChildIndex(curIndex)].priority;
}
int PQHeap::parent(int curIndex) {
    return elements[getParentIndex(curIndex)].priority;
}

/*  swaps elements given their indecies     */
void PQHeap::swap(int indexOne, int indexTwo) {
    DataPoint temp = elements[indexOne];
    elements[indexOne] = elements[indexTwo];
    elements[indexTwo] = temp;
}

/*  if the elements in the array fulfill the array's capacity,
 * expand doubles the allocated Capacity of the array and copies the values over*/
void PQHeap::expand() {
    //ask for new space for a new array
    DataPoint* newElements = new DataPoint[capacity * 2];

    //copy values over
    for (int i = 0; i < numItems; i++ ) {
        newElements[i] = elements[i];
    }

    //delete elemnets original copy
    delete[] elements;

    //CRITICAL point to new array
    elements = newElements;

    //update capacity
    capacity = capacity * 2;
}
/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("Track enqueue and bubble up") {
    PQHeap pq;
    for (int i = 0; i < 25; i++) {
        pq.enqueue({ "elem" + integerToString(i), i });
    }
}
STUDENT_TEST("test") {
    PQHeap pq;
    for (int i = 0; i < 100; i++) {
        int randomValue = randomInteger(0, 100);
        DataPoint pt = {"elem" + integerToString(randomValue), randomValue};
        pq.enqueue(pt);
    }
    EXPECT_EQUAL(pq.size(), 100);
    //EXPECT(pq.validateInternalState()));
}



/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Newly-created heap is empty.") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    EXPECT(pq.size() == 0);
}

PROVIDED_TEST("Provided Test: Enqueue / dequeue single element (two cycles)") {
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };
    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.peek(), point);
    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);

    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.peek(), point);
    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Test clear operation works with single element."){
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };

    pq.enqueue(point);
    pq.clear();
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Dequeue / peek on empty priority queue throws error") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Provided Test: Dequeue / peek on recently-cleared priority queue throws error") {
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };

    pq.enqueue(point);
    pq.clear();
    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Provided Test: Enqueue elements in sorted order.") {
    PQHeap pq;
    for (int i = 0; i < 25; i++) {
        pq.enqueue({ "elem" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 25);
    for (int i = 0; i < 25; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            "elem" + integerToString(i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Enqueue elements in reverse-sorted order.") {
    PQHeap pq;
    for (int i = 25; i >= 0; i--) {
        pq.enqueue({ "elem" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 26);
    for (int i = 0; i <= 25; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            "elem" + integerToString(i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Insert ascending and descending sequences.") {
    PQHeap pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), 2 * i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), 2 * i + 1 });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 40; i++) {
        DataPoint removed = pq.dequeue();
        EXPECT_EQUAL(removed.priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Insert random sequence of elements.") {
    Vector<DataPoint> sequence = {
        { "A", 0 },
        { "D", 3 },
        { "F", 5 },
        { "G", 6 },
        { "C", 2 },
        { "H", 7 },
        { "I", 8 },
        { "B", 1 },
        { "E", 4 },
        { "J", 9 },
    };

    PQHeap pq;
    for (DataPoint elem: sequence) {
        pq.enqueue(elem);
    }

    EXPECT_EQUAL(pq.size(), sequence.size());

    for (int i = 0; i < 10; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            charToString('A' + i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}


PROVIDED_TEST("Provided Test: Insert duplicate elements.") {
    PQHeap pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 20; i++) {
        DataPoint one = pq.dequeue();
        DataPoint two = pq.dequeue();

        EXPECT_EQUAL(one.priority, i);
        EXPECT_EQUAL(two.priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Handles data points with empty string name.") {
    PQHeap pq;
    for (int i = 0; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 10);
}

PROVIDED_TEST("Test enqueue/dequeue of longer random sequence") {
    PQHeap pq;

    for (int i = 0; i < 100; i++) {
        int randomValue = randomInteger(0, 100);
        DataPoint pt = {"elem" + integerToString(randomValue), randomValue};
        pq.enqueue(pt);
    }
    EXPECT_EQUAL(pq.size(), 100);
    DataPoint last = {"", -1};
    for (int i = 0; i < 100; i++) {
        DataPoint cur = pq.dequeue();
        EXPECT(cur.priority >= 0 && cur.priority <= 100 && cur.priority >= last.priority);
        last = cur;
    }
    EXPECT_EQUAL(pq.size(), 0);
}


PROVIDED_TEST("Provided Test: Handles data points with negative weights.") {
    PQHeap pq;
    for (int i = -10; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 20);
    for (int i = -10; i < 10; i++) {
        DataPoint removed = pq.dequeue();
        EXPECT_EQUAL(removed.priority, i);
    }
}

PROVIDED_TEST("Provided Test: Interleave enqueues and dequeues.") {
    PQHeap pq;
    int n = 100;
    for (int i = n / 2; i < n; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = n / 2; i < n; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);

    for (int i = 0; i < n / 2; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = 0; i < n / 2; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
}

static void fillAndEmpty(int n) {
    PQHeap pq;
    DataPoint max = {"max", 106106106};
    DataPoint min = {"min", -106106106};

    pq.enqueue(min);
    pq.enqueue(max);
    for (int i = 0; i < n; i++) {
        int randomPriority = randomInteger(-10000, 10000);
        pq.enqueue({ "", randomPriority });
    }
    EXPECT_EQUAL(pq.size(), n + 2);

    EXPECT_EQUAL(pq.dequeue(), min);
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
    EXPECT_EQUAL(pq.dequeue(), max);
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("Provided Test: Stress Test. Time the amount of time it takes to cycle many elements in and out. Should take at most about 5-10 seconds.") {
    TIME_OPERATION(20000, fillAndEmpty(20000));
}
