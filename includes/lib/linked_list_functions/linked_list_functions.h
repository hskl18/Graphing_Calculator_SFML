#ifndef LINKED_LIST_FUNCTIONS_H
#define LINKED_LIST_FUNCTIONS_H

#include <cassert>

#include "../node/node.h"

using namespace std;

// Declaration
//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head);

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T>* head);

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key);

template <typename T>
node<T>* _insert_head(node<T>* &head, T insert_this);

//insert after ptr
template <typename T>
node<T>* _insert_after(node<T>* &head, node<T>* after_this, T insert_this);

//insert before ptr
template <typename T>
node<T>* _insert_before(node<T>* &head, node<T>* before_this, T insert_this);

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* head, node<T>* prev_to_this);

//delete, return item
template <typename T>
T _delete_node(node<T>* &head, node<T>* delete_this);

//duplicate the list
template <typename T>
node<T>* _copy_list(node<T>* head);

//duplicate list and return the last node of the copy
template <typename T>
node<T>* _copy_list(node<T>* &dest, node<T>* src);

//delete all the nodes
template <typename T>
void _clear_list(node<T>* &head);

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos);

template <typename T>
node<T>* _insert_tail(node<T>* &head, T insert_this);



//Last node in the list, never use this function.
template <typename T>
node<T>* _last_node(node<T>* head);

// Definition

// TODO


//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head)
{
    node<T>* walker = head;
    while(walker != nullptr)
    {
        cout << *walker << "->";
        walker = walker->_next;
    }
    cout << "|||" << endl;
}

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T>* head)
{
    if(head == nullptr)
    {
        cout << "|||";
        return;
    }

    _print_list_backwards(head->_next);
    cout << "<-" << *head;
}

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key)
{
    if(head == nullptr || head->_item == key) return head;
    return _search_list<T>(head->_next, key);
}

template <typename T>
node<T>* _insert_head(node<T>* &head, T insert_this)
{
    node<T>* newNode = new node<T>(insert_this);
    newNode->_next = head;
    head = newNode;
    return head;
}


template <typename T>
node<T>* _insert_tail(node<T>* &head, T insert_this)
{
    node<T>* lastNode = _last_node(head);
    if(lastNode == nullptr)
    {
        head = _insert_head<T>(head, insert_this);
        return head;
    }
    lastNode->_next = new node<T>(insert_this);
    return lastNode->_next;
}




//insert after ptr
template <typename T>
node<T>* _insert_after(node<T>* &head, node<T>* after_this, T insert_this)
{
    if(head == nullptr)
    {
        head = _insert_head<T>(head, insert_this);
        return head;
    }
    if((head->_next == nullptr && head == after_this) || after_this == _last_node<T>(head))
    {
        node<T>* lastNode = _insert_tail<T>(head, insert_this);
        return lastNode;
    }

    node<T>* newNode = new node<T>(insert_this);
    node<T>* nextNode = after_this->_next;
    newNode ->_next = nextNode;
    after_this->_next = newNode;

    return newNode;
}


//insert before ptr
template <typename T>
node<T>* _insert_before(node<T>* &head, node<T>* before_this, T insert_this)
{
    if(head == nullptr || head == before_this)
    {
        head = _insert_head<T>(head, insert_this);
        return head;
    }
    

    node<T>* newNode = new node<T>(insert_this);
    node<T>* walker = head;
    while(walker->_next != before_this) walker = walker->_next;
    newNode->_next = before_this;
    walker->_next = newNode;
    return newNode;
}


//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* head, node<T>* prev_to_this)
{
    if(prev_to_this == head) return nullptr;
    node<T>* walker = head;
    while(walker->_next != prev_to_this) walker = walker->_next;
    return walker;
}

//delete, return item
template <typename T>
T _delete_node(node<T>* &head, node<T>* delete_this)
{
    if(head == nullptr) return T();

    T val = delete_this->_item;
    if(head == delete_this)
    {
        node<T>* newHead = head->_next;
        head = newHead;
        delete delete_this;
        return val;
    }
    

    node<T>* walker = head;
    while(walker->_next != delete_this) walker = walker->_next;
    node<T>* nextNode = walker->_next->_next;
    walker->_next = nextNode;
    delete delete_this;
    return val;
}


//duplicate the list
template <typename T>
node<T>* _copy_list(node<T>* head)
{
    if(head == nullptr) return nullptr;

    node<T>* headWalker = head;
    node<T>* newNode = new node<T>(head->_item);
    node<T>* newHeadHold = newNode;
    while(headWalker->_next != nullptr)
    {
        newNode->_next = new node<T>(headWalker->_next->_item);
        headWalker = headWalker->_next;
        newNode = newNode->_next;
    }
    return newHeadHold;
}

//duplicate list and return the last node of the copy
template <typename T>
node<T>* _copy_list(node<T>* &dest, node<T>* src)
{
    if(dest != nullptr) _clear_list(dest);
    dest = _copy_list<T>(src);
    return _last_node<T>(dest);
}

//delete all the nodes
template <typename T>
void _clear_list(node<T>* &head)
{
    if (head == nullptr) return;
    node<T>* toDelete = head;
    head = head->_next;
    delete toDelete;
    _clear_list(head);
}


//_item at this position
template <typename T>
T& _at(node<T>* head, int pos)
{
    if(pos == 0) return head->_item;
    pos--;
    return _at<T>(head->_next, pos);
}

//Last node in the list, never use this function.
template <typename T>
node<T>* _last_node(node<T>* head)
{
    if(head == nullptr) return head;
    if(head->_next == nullptr) return head;
    return _last_node<T>(head->_next);
}






#endif // LINKED_LIST_FUNCTIONS_H