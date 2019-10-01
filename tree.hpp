/*
 tree.hpp -- source/header file for a huffman tree implementation

 Copyright (c) 2019 Lucas V. Araujo <lucas.vieira.ar@protonmail.com>

 	This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
	 
 	This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 GNU General Public License for more details.

 	You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <cstdio>

using std::cout;
using std::endl;

#ifndef  _TREE_H
#define  _TREE_H

    typedef unsigned long u64;

    class Node
    {
        private:
        
        char myChar;
        u64  frequency;
        Node* left;
        Node* right;
        
        public:

        Node(char ch, u64 freq)
        {
            
            setChar(ch);
            setFrequency(freq);
            setLeft(NULL);
            setRight(NULL);
        }

        Node(Node* lnode, Node* rnode)
        {
            
            u64 freq = (lnode) ? lnode->getFrequency() : 0;
            freq    += (rnode) ? rnode->getFrequency() : 0;
            setLeft(lnode);
            setRight(rnode);
            setFrequency(freq);
            setChar(0);
        }

        bool
        hasLeft()
        {
            return (left != NULL);
        }

        bool
        hasRight()
        {
            return (right != NULL);
        }

        bool
        isLeaf()
        {
            return (hasRight() || hasLeft()) ? false : true;
        }

        bool
        isValidChar()
        {
            return (myChar != 0);
        }

        void
        setChar(char ch)
        {
            myChar = ch;
        }

        char
        getChar()
        {
            return myChar;
        }

        void
        setFrequency(u64 freq)
        {
            frequency = freq;
        }

        u64
        getFrequency()
        {
            return frequency;
        }

        void
        setLeft(Node *lnode)
        {
            
            left = lnode;
        }

        Node*
        getLeft()
        {
            return left;
        }

        void
        setRight(Node *rnode)
        {
            
            right = rnode;
        }

        Node*
        getRight()
        {
            return right;
        }

        bool
        operator>(Node &n)
        {
            return getFrequency() > n.getFrequency();
        }

        bool
        operator<(Node &n)
        {
            return getFrequency() < n.getFrequency();
        }

        bool
        operator==(Node &n)
        {
            return getFrequency() == n.getFrequency();
        }

    };

    class SortedList
    {
        private:
        
        struct Item
        {
            Node* value;
            Item* next;
        };

        Item* start;
        
        public:

        SortedList()
        {
            start       = new Item;
            start->next = NULL;
        }

        ~SortedList()
        {
            clear();
        }

        bool
        empty()
        {
            return (start->next == NULL);
        }

        bool
        lonely()
        {
            if (empty())
            {
                return true;
            }
            return (start->next->next == NULL);
        }

        void
        clear()
        {
            Item *i = start;
            Item *n;
            while (i)
            {
                n = i->next;
                delete i;
                i = n;
            }
        }

        void
        addItem(Node *data)
        {
            if (!start->next)
            {
                Item *item  = new Item;
                item->value = data;
                item->next  = NULL;
                start->next = item;
            }
            else
            {
                Item *i = start;
                while (i->next)
                {
                    if (i->next->value->getFrequency() >= data->getFrequency())
                    {
                        break;
                    }
                    i = i->next;
                }
                Item*  item = new Item;
                item->value = data;
                item->next  = i->next;
                i->next     = item;
            }
        }

        Node*
        shiftItem()
        {
            if (empty())
            {
                /* throw exception */
                return NULL;
            }
            Item* item  = start->next;
            start->next = item->next;
            Node* node  = item->value;
            delete item;
            return node;
        }
    };

    
    class HuffmanTree
    {
        private:
        
        Node *root;
        Node *current;
        
        void
        deleteNode(Node *node)
        {
            if (node == NULL)
            {
                return ;
            }
            deleteNode(node->getLeft());
            node->setLeft(NULL);
            deleteNode(node->getRight());
            node->setRight(NULL);
            delete node;
        }

        void
        showNode(Node* tree, int space)
        {
            if (tree == NULL)  
                return ;  
            
            space += 20;  
        
            showNode(tree->getRight(), space);  
        
            cout << endl;
            int i;
            for (i = 20; i < space; i++)  
                cout << " ";  
            printf("(%c : %lu)\n",
            (tree->getChar()) ? tree->getChar() : '0', tree->getFrequency());
            
            showNode(tree->getLeft(), space);
        }

        public:
        
        HuffmanTree(SortedList* sorted)
        {
            while (!sorted->lonely())
            {
                Node *node1 = sorted->shiftItem();
                /*cout << "Node1: " << node1->getChar()<< " - "
                       << node1->getFrequency() << endl;*/
                Node *node2 = sorted->shiftItem();
                /*cout << "Node2: " << node2->getChar() << " - "
                       << node2->getFrequency() << endl; */
                Node *fathr = new Node(node1, node2);
                sorted->addItem(fathr);
            }
            root    = sorted->shiftItem();
            /*cout << "Root: " << root->getChar() << " - "
                   << root->getFrequency() << endl;*/
            current = root;
            sorted->clear();
        }

        ~HuffmanTree()
        {
            clear();
        }

        void
        clear()
        {
            deleteNode(root);
        }
        bool
        canWalkLeft()
        {
            if (!current)
            {
                return false;
            }
            return current->hasLeft();
        }

        bool
        canWalkRight()
        {
            if (!current)
            {
                return false;
            }
            return current->hasRight();
        }

        void
        walkLeft()
        {
            if (canWalkLeft())
            {
                current = current->getLeft();
            }
            else
            {
                /* TODO: Implement an exception to be thrown */
            }
        }

        void
        walkRight()
        {
            if (canWalkRight())
            {
                current = current->getRight();
            }
            else
            {
                /* TODO: Throw an exception */
            }
        }

        void
        backToRoot()
        {
            current = root;
        }

        bool
        isOnLeaf()
        {
            return current->isLeaf();
        }

        char
        getCurrentChar()
        {
            return current->getChar();
        }
        char
        getCurrentFrequency()
        {
            return current->getFrequency();
        }
        Node*
        getRoot()
        {
            return root;
        }

        void
        showTree()
        {
            showNode(root, 0);
        }

    };

#endif /*_TREE_H*/