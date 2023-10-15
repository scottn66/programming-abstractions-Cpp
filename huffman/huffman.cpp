#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * bits queue contains a valid sequence of encoded bits.
 *
 * boolean isLeaf checks if the given node is a Leaf i.e. contains a char values and no children.
 */
bool isLeaf(EncodingTreeNode* parent) {
    return (parent->zero == nullptr && parent->one == nullptr);
}

string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string result;
    Bit curBit;
    EncodingTreeNode* treeClimber = tree;
    while(!messageBits.isEmpty()) {
        curBit = messageBits.dequeue();
        if (curBit == 0) {
            treeClimber = treeClimber->zero;
            if (isLeaf(treeClimber)) {
                result += treeClimber->ch;
                treeClimber = tree;
            }
        }
        if (curBit == 1) {
            treeClimber = treeClimber->one;
            if (isLeaf(treeClimber)) {
                result += treeClimber->ch;
                treeClimber = tree;
            }
        }
    }
    return result;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the input Queues are well-formed and represent
 * a valid encoding tree.
 * Bits 0 are leafNodes and locations of chars, while Bits 1 are internal nodes acting as parents
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeBits, Queue<char>& treeLeaves) {
    Bit curBit = treeBits.dequeue();
    if (curBit == 0) {
        return new EncodingTreeNode(treeLeaves.dequeue());
    }
    if (curBit == 1) {
        return new EncodingTreeNode(unflattenTree(treeBits, treeLeaves), unflattenTree(treeBits, treeLeaves));
    }
    return nullptr;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * Deallocation of huffman tree after its use
 */
string decompress(EncodedData& data) {
    EncodingTreeNode* HuffmanTree = unflattenTree(data.treeBits, data.treeLeaves);
    string result = decodeText(HuffmanTree, data.messageBits);
    deallocateTree(HuffmanTree);
    return result;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * frequencyTableHelper creates the table to prioritize chars/data
 *  before constructing the Huffman tree with Priority Queue.
 */

Map<char, int> frequencyTableHelper(string text) {
    Map<char, int> frequencyTable;
    int strLength = text.size();
    for (int i = 0; i < strLength; i++) {
        if (!frequencyTable.containsKey(text[i])) {
            frequencyTable.put(text[i], 1);
        } else {
            //get value of text[i] and add 1
            frequencyTable[text[i]]++;
        }
    }
    if (frequencyTable.size() < 2) {
        error("input text does not contain at least two distinct characters");
    }
    return frequencyTable;
}


EncodingTreeNode* buildHuffmanTree(string text) {
    Map<char, int> freq = frequencyTableHelper(text);
    PriorityQueue<EncodingTreeNode*> PQ;
    for (char elem : freq) {
        EncodingTreeNode* character = new EncodingTreeNode(elem);
        PQ.enqueue(character, freq[elem]);
    }
    while (PQ.size() > 1) {
        int elem1Priority = PQ.peekPriority();
        EncodingTreeNode* elem1 = PQ.dequeue();
        int elem2Priority = PQ.peekPriority();
        EncodingTreeNode* elem2 = PQ.dequeue();
        EncodingTreeNode* newParent = new EncodingTreeNode(elem1, elem2);
        int newPriority = elem1Priority + elem2Priority;
        PQ.enqueue(newParent, newPriority);
    }
    return PQ.dequeue();
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * below helper function tracks the path traversal of the Huffman tree through recursion and pbr&
 * recording each character's unique path to its location in a Vector of Bits
 * encodeText funnels each char:sequence of Bits into the compressed code of Bits
 */

void encodeTextMapperHelper(EncodingTreeNode* tree, Vector<Bit> path, Map<char,Vector<Bit>>& sequences){
    if (isLeaf(tree)) {
        sequences.put(tree->ch, path);
    } else {
    Vector<Bit> leftPath = path;
    leftPath.add(0);
    encodeTextMapperHelper(tree->zero, leftPath, sequences);
    Vector<Bit> rightPath = path;
    rightPath.add(1);
    encodeTextMapperHelper(tree->one, rightPath, sequences);
    }
}


Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    Queue<Bit> result;
    Vector<Bit> path;
    Map<char,Vector<Bit>> sequences;
    encodeTextMapperHelper(tree, path, sequences);
    int iterations = text.size();
    for (int i = 0; i < iterations; i++) {
        Vector<Bit> funnel = sequences.get(text[i]);
        int BitLength = funnel.size();
        for(int i = 0; i < BitLength; i++){
            result.enqueue(funnel[i]);
        }
    }
    return result;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input Queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * Uses recursive traversal preorder method to record treeBits and Leaf Nodes in order
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeBits, Queue<char>& treeLeaves) {
    EncodingTreeNode* treeClimber = tree;
    if(isLeaf(treeClimber)) {
        treeBits.enqueue(0);
        treeLeaves.enqueue(treeClimber->ch);
    } else {
        treeBits.enqueue(1);
        flattenTree(treeClimber->zero, treeBits, treeLeaves);
        flattenTree(treeClimber->one, treeBits, treeLeaves);
    }
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * All-together: frequency map--> huffman tree--> compressed data --> deallocation
 */
EncodedData compress(string messageText) {
    EncodedData result;
    EncodingTreeNode* Huffman = buildHuffmanTree(messageText);
    result.messageBits = encodeText(Huffman, messageText);
    flattenTree(Huffman, result.treeBits, result.treeLeaves);
    deallocateTree(Huffman);
    return result;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    EncodingTreeNode* tryIt = new EncodingTreeNode(new EncodingTreeNode('T'), new EncodingTreeNode(new EncodingTreeNode(new EncodingTreeNode('R'), new EncodingTreeNode('S')), new EncodingTreeNode('E')));
    return tryIt;
}

void deallocateTree(EncodingTreeNode* root) {
    if (root == nullptr) {
        return;
    }
    deallocateTree(root->zero);
    deallocateTree(root->one);
    delete root;
}



bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    //check if a or b is a nullptr
    if (a == nullptr || b == nullptr) {
        return (a == b);
    }
    /* if a and b left children are leaves */
    bool equality = true;
    if (isLeaf(a) && isLeaf(b)){
        if (a->ch != b->ch){
            equality = false;
        }
    } else {
        return areEqual(a->zero, b->zero) && areEqual(a->one, b->one);
    }
    return equality;
}


/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("Testing the deallocation of example Tree Node"){
    EncodingTreeNode* test = createExampleTree();
    deallocateTree(test);
}
STUDENT_TEST("areEqual with two congruent trees") {
    EncodingTreeNode* ex1 = createExampleTree();
    EncodingTreeNode* ex2 = createExampleTree();
    EXPECT(areEqual(ex1, ex2));
    deallocateTree(ex1);
    deallocateTree(ex2);
}
STUDENT_TEST("areEqual test of subtrees") {
    EncodingTreeNode* ex1 = createExampleTree();
    EncodingTreeNode* subtree1 = ex1->one;
    EncodingTreeNode* subtree2 = ex1->one;
    EXPECT(areEqual(subtree1, subtree2));
    deallocateTree(ex1);
}

STUDENT_TEST("testing unflattening a one node tree-- just a leaf") {
    Queue<Bit> baseCase = {0};
    Queue<char> letterS = {'S'};
    EncodingTreeNode* testingBase = new EncodingTreeNode('S');
    EXPECT_EQUAL(unflattenTree(baseCase, letterS), testingBase);
    deallocateTree(testingBase);
}

STUDENT_TEST("testing combination of decode and unflatten in...decompress") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeBits
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };
}
STUDENT_TEST("encodeText Mapper test") {
    Map<char, Vector<Bit>> exampleCode = {};
    exampleCode.put('T', {0});
    exampleCode.put('E', {1, 1});
    exampleCode.put('R', {1, 0, 0});
    exampleCode.put('S', {1, 0, 1});
    EncodingTreeNode* exampleTree = createExampleTree();
    Vector<Bit> pathway = {};
    Map<char,Vector<Bit>> testMap;
    encodeTextMapperHelper(exampleTree, pathway, testMap);
    deallocateTree(exampleTree);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeBits   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeBits, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeBits
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedBits   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeBits;
    Queue<char> treeLeaves;
    flattenTree(reference, treeBits, treeLeaves);

    EXPECT_EQUAL(treeBits,   expectedBits);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeBits    = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeBits, treeBits);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "The job requires extra pluck and zeal from every young wage earner.",
        ":-) :-D XD <(^_^)>",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(output.size(), input.size());

        /* Don't clobber the output with a huge string if there's a mismatch. */
        EXPECT(input == output);
    }
}
