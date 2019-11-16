////////////////////////////////////////////////////////////////////////////////
// Module Name:  skip_list.h/hpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

// !!! DO NOT include skip_list.h here, 'cause it leads to circular refs. !!!

#include <cstdlib>
#include "time_stamp.h"

//==============================================================================
// class NodeSkipList
//==============================================================================

template <class Value, class Key, int numLevels>
void NodeSkipList<Value, Key, numLevels>::clear(void)
{
    for (int i = 0; i < numLevels; ++i)
        Base::nextJump[i] = 0;

    Base::levelHighest = -1;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(void)
{
    clear();
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey)
{
    clear();

    Base::Base::key = tkey;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey, const Value& val)
{
    clear();

    Base::Base::key = tkey;
    Base::Base::value = val;
}


//==============================================================================
// class SkipList
//==============================================================================

template <class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::SkipList(double probability)
{
    _probability = probability;

    // Lets use m_pPreHead as a final sentinel element
    for (int i = 0; i < numLevels; ++i)
        Base::_preHead->nextJump[i] = Base::_preHead;

    Base::_preHead->levelHighest = numLevels - 1;
}


template<class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::~SkipList()
{

}

template<class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::insert(const Value &val, const Key &key)
{
    Node* n_node = new Node(key, val);

    int head = -1;
    double current = rand() / (double)RAND_MAX;

    while (current < _probability && head < numLevels - 1)
    {
        head++;
        current = rand() / (double)RAND_MAX;
    }

    n_node->levelHighest = head;

    Node* temp = Base::_preHead;

    for (int i = Base::_preHead->levelHighest; i >= 0; i--)
    {
        while (temp->nextJump[i] != Base::_preHead && temp->nextJump[i]->key < key)
            temp = temp->nextJump[i];

        if (i <= n_node->levelHighest)
        {
            n_node->nextJump[i] = temp->nextJump[i];
            temp->nextJump[i] = n_node;
        }
    }

    while (temp->next != Base::_preHead && temp->next->key <= key)
        temp = temp->next;

    n_node->next = temp->next;
    temp->next = n_node;
}

template<class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::removeNext(SkipList::Node *nodeBefore)
{
    Node* temp = Base::_preHead;
    bool was = false;

    for(int i = 0; i < Base::_preHead->levelHighest; i++)
    {
        temp = temp->next;
        if (temp == nodeBefore)
            was = true;
    }

    if(!was)
        throw std::invalid_argument("");
    else
        nodeBefore->next = nodeBefore->next->next;
}

template<class Value, class Key, int numLevels>
typename SkipList<Value, Key, numLevels>::Node *SkipList<Value, Key, numLevels>::findLastLessThan(const Key &key) const
{
    Node* temp = Base::_preHead;

    for (int i = Base::_preHead->levelHighest; i >= 0; i--)
        while (temp->nextJump[i] != Base::_preHead && temp->nextJump[i]->key < key)
            temp = temp->nextJump[i];

    while (temp->next != Base::_preHead && temp->next->key < key)
        temp = temp->next;

    return temp;
}

template<class Value, class Key, int numLevels>
typename SkipList<Value, Key, numLevels>::Node *SkipList<Value, Key, numLevels>::findFirst(const Key &key) const
{
    Node* temp = findLastLessThan(key);
    temp = temp->next;

    if (temp->key == key && temp != Base::_preHead)
        return temp;
    return nullptr;

}
