/*
 Name: Rohaan Kamaraj
 Email: rkamara1@asu.edu
 Description: This file contains a Node implementation, as well as a LinkedList implementation
 with basic methods
 */

#include <string>
#include <iostream>

using namespace std;

//NODE IMPLEMENTATION
class Node {
private:
    //private instance data
    string information;
    string pubYear;
    int weight;
    Node* next;

public:
    // constructor + accessor/mutator methods
    Node(string, string);
    Node* getNext();
    string getInformation();
    string getPubYear();
    void setNext(Node*);

};

//constructor implementation
Node::Node(string userInformation, string pubYear) {
    information = userInformation;
    this->pubYear = pubYear;
    weight = 1;
    next = NULL;
}

//accessor for next node
Node* Node::getNext() {
    return next;
}

//accessor for node's information
string Node::getInformation() {
    return information;
}

//accessor for node's publication year
string Node::getPubYear() {
    return pubYear;
}

//mutator to point to next node
void Node::setNext(Node* userNext) {
    next = userNext;
}

//LINKEDLIST IMPLEMENTATION
class LinkedList{
private:
    //private instance data
    Node* head;
    Node* tail;

public:
    //public methods for LinkedList
    LinkedList();
    void insertEdge(string, string);
    Node* getHead();
    int getLength();
    void removeAll();
};

// default constructor
LinkedList::LinkedList() {
    head = NULL;
    tail = NULL;
}

//adds node to the end of linked list
void LinkedList::insertEdge(string information, string pubYear) {
    Node* node = new Node(information, pubYear);
    if (head == NULL && tail == NULL) { // if the list is empty
        head = node;
        tail = node;
    } else { // if the list isn't empty
        tail->setNext(node);
        tail = node;
    }
}

//gets the first element (the head) of the LinkedList
Node* LinkedList::getHead() {
    return head;
}

//returns the number of nodes in the list
int LinkedList::getLength() {
    Node* node = head;
    int count = 0;
    while (node != NULL) {
        count++;
        node = node->getNext();
    }
    return count;
}

//removes all nodes from the list
void LinkedList::removeAll() {
    Node* node = head;

    while (node != NULL) {
        Node* node2 = node->getNext();
        delete node2;
        node = node2;
    }
}