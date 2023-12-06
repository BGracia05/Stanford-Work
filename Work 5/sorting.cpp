#include "listnode.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void runSort(ListNode*& front) {
    if (front == nullptr) {
        return;
    }
    if (front->next == nullptr) {
        return;
    }
    ListNode* result = nullptr;

    while (front != nullptr) {
        ListNode* run = front;
        front = front->next;
        run->next = nullptr;

        if (result == nullptr || run->data < result->data) {
            run->next = result;
            result = run;
        } else {
            ListNode* cur = result;
            while (cur->next != nullptr && run->data >= cur->next->data) {
                cur = cur->next;
            }
            run->next = cur->next;
            cur->next = run;
        }
    }
    front = result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void quickSort(ListNode*& front) {
    if (front == nullptr) {
        return;
    }
    if (front->next == nullptr) {
            return;
    }
    int pivot = front->data;

    ListNode* lessFront = nullptr;
    ListNode* equalFront = nullptr;
    ListNode* greaterFront = nullptr;
    ListNode* lessEnd = nullptr;
    ListNode* equalEnd = nullptr;
    ListNode* greaterEnd = nullptr;
    ListNode* cur = front;

    while (cur != nullptr) {
            ListNode* next = cur->next;
            cur->next = nullptr;

            if (cur->data < pivot) {
                if (lessFront == nullptr) {
                    lessFront = lessEnd = cur;
                } else {
                    lessEnd->next = cur;
                    lessEnd = cur;
                }
            } else if (cur->data == pivot) {
                if (equalFront == nullptr) {
                    equalFront = equalEnd = cur;
                } else {
                    equalEnd->next = cur;
                    equalEnd = cur;
            }
            } else {
                if (greaterFront == nullptr) {
                    greaterFront = greaterEnd = cur;
                } else {
                    greaterEnd->next = cur;
                    greaterEnd = cur;
                }
            }
            cur = next;
    }
    quickSort(lessFront);
    quickSort(greaterFront);
}


/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr) {
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
   while (front != nullptr) {
       ListNode* next = front->next;
       delete front;
       front = next;
   }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values) {
   ListNode* front = nullptr;
   for (int i = values.size() - 1; i >= 0; i--) {
       front = new ListNode(values[i], front);
    }
    return front;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v) {
    ListNode* cur = front;
    for (int i = 0; i < v.size(); i++) {
       if (cur == nullptr || cur->data != v[i]) {
           return false;
       }
       cur = cur->next;
    }
    return cur == nullptr;
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test.") {
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    runSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

PROVIDED_TEST("Confirm correctness of test utility functions.") {
    /* Manually construct testList 1 -> 2 -> 3 */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++) {
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
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());
        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}
STUDENT_TEST("Time linked list runSort") {
    int startSize = 10000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        ListNode* listrunSort = nullptr;

        /* Create linked list with a random sequence. */
        for (int i = n-1; i >= 0; i--) {
            int value = randomInteger(-10000, 10000);
            listrunSort = new ListNode(value, listrunSort);
        }

        /* Time runSort for linked list */
        TIME_OPERATION(n, runSort(listrunSort));

        /* Deallocate memory for the list */
        deallocateList(listrunSort);
    }
}
STUDENT_TEST("Time linked list quickSort") {
    int startSize = 10000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        ListNode* listquickSort = nullptr;

        /* Create linked list with a random sequence. */
        for (int i = n-1; i >= 0; i--) {
            int value = randomInteger(-10000, 10000);
            listquickSort = new ListNode(value, listquickSort);
        }

        /* Time runSort for linked list */
        TIME_OPERATION(n, quickSort(listquickSort));

        /* Deallocate memory for the list */
        deallocateList(listquickSort);
    }
}
