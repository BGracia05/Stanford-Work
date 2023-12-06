#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
this creates a new empty priority queue. the initial capacity is set and the count is  0
 */
PQHeap::PQHeap() {
    capacity = INITIAL_CAPACITY;
    count = 0;
    heapArray = new DataPoint[capacity];
}

/*
clears heapArray
 */
PQHeap::~PQHeap() {
    delete[] heapArray;
}

/*
adds the elem to the priority queue and stores it in the correct position,
if the array fills, the capacity doubles when expandCapacity is called.
 */
void PQHeap::enqueue(DataPoint elem) {
    if (count >= capacity) {
        expandCapacity();
    }
    heapArray[count] = elem;
    bubbleUp(count);
    count++;
}

/*
returns the element with the highest priority, gives an error if not present
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("No Priority Queue Present");
    }
    return heapArray[0];
}

/*
removes and return the highest priority element(first element in heapArray), then the last
element replaces the removed highest priority and then it is bubbled down to its position.
 */
DataPoint PQHeap::dequeue() {
    if (isEmpty()) {
        error("No Priority Queue Present");
    }
    DataPoint minElement = heapArray[0];
    heapArray[0] = heapArray[count - 1];
    count--;
    bubbleDown(0);
    return minElement;
}

/*
returns true if the priority queue is empty
 */
bool PQHeap::isEmpty() const {
    return count == 0;
}

/*
returns the number of elements in the queue
 */
int PQHeap::size() const {
    return count;
}

/*
 removes all elements from queue
 */
void PQHeap::clear() {
    count = 0;
}

/*
prints the elements in the array alongisde the priorities
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < count; i++) {
        cout << heapArray[i].label << " (" << heapArray[i].priority << ")";
    }
    cout << endl;
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() const {
    for (int i = 0; i < count; i++) {
        int leftChildIndex = getLeftChildIndex(i);
        int rightChildIndex = getRightChildIndex(i);

        if(leftChildIndex != NONE && heapArray[i].priority > heapArray[leftChildIndex].priority) {
                error(" LeftChildPriority is less than the parent priority");
        }
        if (rightChildIndex != NONE && heapArray[i].priority > heapArray[rightChildIndex].priority) {
                error("RighChildPriority is less than parent priority");
        }
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    if (child == 0) return NONE;
    return (child - 1) / 2;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    int leftChildIndex = (2 * parent) + 1;
    return (leftChildIndex < count) ? leftChildIndex : NONE;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    int rightChildIndex = (2 * parent) + 2;
    return (rightChildIndex < count) ? rightChildIndex : NONE;
    return 0;
}

/*
 * We strongly recommend implementing this helper function that swaps
 * array elements. There is a swapElements member function implemented
 * in PQAarray that you start from. Be sure to include error bounds-checking
 * it is an invaluable defense against indexing mistakes!
 */
void PQHeap::swapElements(int indexA, int indexB) {
    DataPoint temp = heapArray[indexA];
    heapArray[indexA] = heapArray[indexB];
    heapArray[indexB] = temp;
}
void PQHeap::expandCapacity() {
    capacity *= 2;
    DataPoint* newHeapArray = new DataPoint[capacity];
    for (int i = 0; i < count; i++) {
        newHeapArray[i] = heapArray[i];
    }
    delete[] heapArray;
    heapArray = newHeapArray;
}

void PQHeap::bubbleUp(int index) {
    int parentIndex = getParentIndex(index);
    while (index > 0 && heapArray[index].priority < heapArray[parentIndex].priority) {
        swapElements(index, parentIndex);
        index = parentIndex;
        parentIndex = getParentIndex(index);
    }
}

void PQHeap::bubbleDown(int index) {
    int leftChildIndex = getLeftChildIndex(index);
    int rightChildIndex = getRightChildIndex(index);
    int smallerChildIndex = index;

    if (leftChildIndex != NONE && heapArray[leftChildIndex].priority < heapArray[smallerChildIndex].priority) {
        smallerChildIndex = leftChildIndex;
    }

    if (rightChildIndex != NONE && heapArray[rightChildIndex].priority < heapArray[smallerChildIndex].priority) {
        smallerChildIndex = rightChildIndex;
    }

    if (smallerChildIndex != index) {
        swapElements(index, smallerChildIndex);
        bubbleDown(smallerChildIndex);
    }
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */
STUDENT_TEST("PQHeap testing the enqueue and dequeue") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "", 8 }, { "", 6 }, { "", 10 }, { "", 3 }, { "", 1 }
    };
    for (DataPoint dp : input) {
        pq.enqueue(dp);
    }
    int prevPriority = pq.dequeue().priority;
    while (!pq.isEmpty()) {
        int currentPriority = pq.dequeue().priority;
        EXPECT(currentPriority >= prevPriority);
        prevPriority = currentPriority;
    }
}
STUDENT_TEST("PQHeap Peek Test") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "", 8 }, { "", 6 }, { "", 10 }, { "", 3 }, { "", 1 }
    };

    for (DataPoint dp : input) {
        pq.enqueue(dp);
    }

    DataPoint maxPriorityElem = { "", 1 };
    EXPECT_EQUAL(pq.peek().priority, maxPriorityElem.priority);
}
STUDENT_TEST("PQHeap isEmpty") {
    PQHeap pq;
    EXPECT(pq.isEmpty());

    pq.enqueue({ "", 5 });
    EXPECT(!pq.isEmpty());

    while (!pq.isEmpty()) {
        pq.dequeue();
    }
    EXPECT(pq.isEmpty());
}

STUDENT_TEST("PQHeap Size Accuracy Test") {
    PQHeap pq;

    EXPECT_EQUAL(pq.size(), 0);

    pq.enqueue({ "", 24 });
    pq.enqueue({ "", 1 });
    pq.enqueue({ "", 7 });
    EXPECT_EQUAL(pq.size(), 3);

    pq.dequeue();
    EXPECT_EQUAL(pq.size(), 2);
    pq.dequeue();
    pq.dequeue();
    EXPECT_EQUAL(pq.size(), 0);
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
