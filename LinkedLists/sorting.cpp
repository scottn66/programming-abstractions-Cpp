#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/*  split function finds the midpoint of the linked list by iterating 2 times: one is twice the rate of the other,
 *  so when the faster reaches the end of the list, the slower is at the midpoint.
 *  with the midpoint, it splits list into two approximately equal halves.
 */
void split(ListNode* list, ListNode** frontHalf, ListNode** backHalf) {

    ListNode* right;
    ListNode* left;
    left = list;
    right = list->next;

    /*   Advance 'right':'left' 2:1   */
    while (right != nullptr) {
        right = right->next;
        if (right != NULL) {
            left = left->next;
            right = right->next;
        }
    }

    /* 'left' is before the midpoint in the list, and is split at that point. */
    *frontHalf = list;
    *backHalf = left->next;
    left->next = nullptr;
}


/* -SWAP-
 *  Function is used to swap front nodes between sourceRef to the linkedList destRef
 * assert will throw an error if the source linked list is empty
 */
void swap(ListNode** destRef, ListNode** sourceRef) {
    ListNode* swapper = *sourceRef;
    assert(swapper != nullptr);
    *sourceRef = swapper->next;
    swapper->next = *destRef;
    *destRef = swapper;
}



/* -MERGE-
 *  Takes two lists sorted in increasing
 * order, and merges them together.
 */

ListNode* merge(ListNode* a, ListNode* b) {

    ListNode temp;
    ListNode* tail = &temp;
    temp.next = nullptr;

    while (4) {
        if (a == nullptr) {
            /* if its empty, fill the remaining. */
            tail->next = b;
            break;
        } else if (b == nullptr) {
            tail->next = a;
            break;
        }
        if (a->data <= b->data)
            swap(&(tail->next), &a);
        else
            swap(&(tail->next), &b);

        tail = tail->next;
    }
    return(temp.next);
}

/* -MERGE SORT-
 *  puts all of the parts together, and recursively breaks the linked list down,
 *  and builds it back sorted.
 */
void mergeSort(ListNode** front) {
    ListNode* list = *front;
    ListNode* a;
    ListNode* b;

    //Base Case
    if ((list == nullptr) || (list->next == nullptr)) {
        return;
    }
    split(list, &a, &b);

    mergeSort(&a);
    mergeSort(&b);

    //deallocateList(list);
    *front = merge(a, b);
}



/* -PARTITION NODE-
 * in quickSort(), partition acts as the comparison value(pivot)
 * based on pivot, elements are put into less than pivot list, greater than list, or equal to list
 * the quickSort function utilizes this recursively
 */
void partitionNode(ListNode* &list, ListNode* &less, ListNode* &equal, ListNode* &greater, int pivot) {

    while (list != nullptr) {

        if (list->data > pivot) {
            //add node to less list
            ListNode* temp = list;
            less->next = list;
            list = temp->next;
        } else if (list->data == pivot) {
            ListNode* temp = list;
            equal->next = list;
            list = temp->next;
        } else {
            ListNode* temp = list;
            greater->next = list;
            list = temp->next;
        }
    }
}


/* -CONCATENATE-
 * combines the three sublists:
 * linking them together in order
 */
void concatenate(ListNode* &less, ListNode* &equal, ListNode* &greater) {
    // add less, equal and greater in that order -
    while(equal->next != nullptr) {
        equal = equal->next;
        if (equal->next == nullptr)
            //last node in equal list() points to the first node in greater()
            equal = greater;
    }
    while(less->next != nullptr) {
        less = less->next;
        if (less->next == nullptr)
            less = equal;
    }
}


/* -QUICK SORT-
 * divides the input linked list(front) into sublists sorted in
 * comparision to the pivot variable: in this case it is always the first node in the linked list
 */
//void quickSort(ListNode*& front) {
//    if(lengthOf(front) <= 1) {
//        return;
//    }
//    ListNode* less, equal, greater;
//    int pivot = front->data;
//    partition(front, less, equal, greater, pivot);

//    quickSort(less);
//    quickSort(greater);

//    deallocateList(front);
//    concatenate(front, less, equal, greater);
//}


//this function returns the number of items in the linked list
int lengthOf(ListNode* list) {
    int result = 0;
    while (list != nullptr) {
        result++;
        list = list->next;
    }
    return result;
}

/*This helper function prints the contents of the linked list*/
void printList(ListNode* list) {
    while(list != nullptr) {
        cout << list->data << endl;
        list = list->next;
    }
}


/*
 * This helper function deallocates all of the memory being used
 * to store nodes in that list.
 */
void deallocateList(ListNode* front) {
    while (front != nullptr) {

        //copy next spot
        ListNode* next = front->next;
        delete front;
        front = next;
    }
}

/*
 * This helper function is provided a vector of integer values and
 * returns a pointer to the beginning of a linked list containing
 * those values in the specified order.
 */
ListNode* createList(Vector<int> values){
    ListNode* result = nullptr;
    int vSize = values.size();
    for (int i = vSize; i <= 0; i--) {
        int adder = values[i];
        ListNode* newNode = new ListNode;
        newNode->data = adder;
        newNode->next = result;
        result = newNode;
    }
    return result;
}

/*
 * This helper function is provided a vector of integer values
 * and a pointer to the beginning of a linked list. If these two structures
 * contain the same values in the same order, the function returns
 * true, otherwise it returns false.
 */
bool checkListVectorEquality(ListNode* curPoint, Vector<int> v){
    printList(curPoint);
    Vector<int> frontdata;
    while (curPoint != nullptr) {
        int curItem = curPoint->data;
        frontdata.add(curItem);
        curPoint = curPoint->next;
    }
    cout << frontdata << endl;
    //frontdata.reverse();
    return (v == frontdata);
}



/*
 *      STUDENT TESTS
 */
STUDENT_TEST("Testing capability to print out ListNodes"){
    ListNode* trial = new ListNode;
    trial->data = 5;
    printList(trial);
    delete trial;
}
STUDENT_TEST("first play") {
    //testing createList
    Vector<int> beginning = {1};
    createList(beginning);

}
STUDENT_TEST("checks functionality of createList() ") {
    Vector<int> prelist = {1, 2};
    EXPECT_NO_ERROR(createList(prelist));
    printList(createList(prelist));
}
STUDENT_TEST("checks functionality deallocateList() ") {
    ListNode* trial = new ListNode;
    trial->data = 5;
    EXPECT_NO_ERROR(deallocateList(trial));
}

STUDENT_TEST("This is a skeleton for what your overall student tests should look like."){
    /* Initialize the values and list for this test case. */
    Vector<int> values = {1, 2, 3, 4};
    ListNode* list = createList(values);

    /* Sort the linked list. */
    mergeSort(&list);

    /* Sort the vector. */
    values.sort();

    /* Check to make sure that the list is now in the correct order. */
    EXPECT(checkListVectorEquality(list, values));

    /* Avoid memory leaks by deallocating list. */
    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(checkListVectorEquality(testList, {1, 2, 3}));
    EXPECT(!checkListVectorEquality(testList, {1, 2}));
    EXPECT(!checkListVectorEquality(testList, {1, 2, 3, 4}));
    EXPECT(!checkListVectorEquality(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v;
        ListNode* list = nullptr;

        /* Insert the same random values into linked list and vector. */
        for (int i = 0; i < n; i++) {
            int val = randomInteger(-10000, 10000);
            v.add(val);
            list = new ListNode(val, list);
        }

        /* The vector values are currently in the reverse order than the
         * linked list values – let's fix that so we have a fair comparison. */
        v.reverse();

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to generate a comparison. */
        //TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

