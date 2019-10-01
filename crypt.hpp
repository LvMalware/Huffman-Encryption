/*
 crypt.hpp -- source/header file for the huffman encryption implementation

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
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>

#include "tree.hpp"

using std::string;
using std::cout;
using std::vector;
using std::stoi;

#ifndef  _CRYPT_H
#define  _CRYPT_H

    u64*
    getFrequencys(string &text)
    {
        /*
            given an input string, get the frequency of each character on it
         */

        /* using calloc() to create an array of unsigned long integers (u64),
           initialized with 0s, where each of the 256 positions represents a
           character on the ascii table. The number at the position i will then
           hold the frequency of the ascii character i */
        u64 *frequencys = (u64 *) calloc(256, sizeof(u64));

        for (u64 i = 0; i < text.size(); i++)
        {
            /* for each character on input string, its frequency is incremented
               by 1 */
            frequencys[text[i]] ++;
        }
        return frequencys;
    }

    HuffmanTree*
    buildTree(u64 *frequencys)
    {
        /*
           given the frequencys of the characters, create a huffman tree that
           represents the text
         */

        /* create a new SortedList which will hold the sorted frequencys */
        SortedList *sorted = new SortedList;

        for (int i = 0; i < 256; i++)
        {
            /* for each of the 256 characters */
            if (frequencys[i])
            {
                /* if the frequency of the character is not 0, create a new
                   Node that holds the character and its frequency */
                Node* node = new Node((char) i, frequencys[i]);
                /* then, add this Node to the sorted list, which will be in
                   charge of keeping the nodes sorted by its frequency */
                sorted->addItem(node);
            }
        }

        /* initialize a new instance of HuffmanTree from the SortedList */
        HuffmanTree *tree = new HuffmanTree(sorted);
        return tree;
    }

    HuffmanTree*
    buildTree(string &text, string &keyf)
    {
        /* 
           given an input string (TEXT) and the name of the file to export the
           key (KEYF), count the frequency of each character on the string and
           export the frequencys to the file. Create and return the tree by
           calling the correct buildTree() implementation
         */
        

        u64* frequencys = getFrequencys(text);

        HuffmanTree* tree = buildTree(frequencys);

        /* create a pointer to the file KEYF, using write-binary mode */
        FILE *keyfile = fopen(keyf.c_str(), "wb");
        /* write the frequencys to that file pointer */
        fwrite(frequencys, sizeof(u64), 256, keyfile);
        /* close the file */
        fclose(keyfile);
        /* delete the frequencys, though it will not be necessary anymore */
        delete[] frequencys;
        /* return the tree */
        return tree;
    }

    void
    DSF(vector<char>* path, Node *root, vector<char> &stack)
    {

        /*
           Deep Search First algorithm - make a deep search on the tree,
           storing the route used to reach each leaf.
           path - an array of 256 positions, where the vector<char> at position
           i represents the route to reach the character i .
           root - the root node of the tree to search in .
           stack - a vector<char> structure to store the route of every char in
           runtime
         */
        if (root->isLeaf())
        {
            /* if the current node is a leaf, store its route on PATH[] */
            path[root->getChar()] = vector<char>(stack);
            /* exit the function */
            return;
        }
        if (root->hasLeft())
        {
            /* if the current sub-tree has an left node, store an 0 on stack */
            stack.push_back(0);
            /* make a DSF on the left sub-tree */
            DSF(path, root->getLeft(), stack);
            /* removes the element at the top of the stack */
            stack.pop_back();
        }
        if (root->hasRight())
        {
            /* if the current sub-tree has an right node, store 1 on stack */
            stack.push_back(1);
            /* make a DSF on the right sub-tree */
            DSF(path, root->getRight(), stack);
            /* removes the element at the top of the stack */
            stack.pop_back();
        }
    }

    vector<char>*
    getPaths(HuffmanTree *tree)
    {
        /*
           given an input HuffmanTree, find and return the path for each
           character on it.
           OBS: a path is a binary word (0s and 1s) that represents the
           directions taken from the root node to a leaf node. The bit 1
           means 'go to right', and the bit 0 means 'go to left'.
         */

        /* create an array with 256 positions to store the paths
           (each vector<char> is a different one) */
        vector<char>* path = new vector<char>[256];
        /* create a vector<char> to work as a stack */
        vector<char> stack;
        /* make a DSF on the tree, starting with the root node */
        DSF(path, tree->getRoot(), stack);
        /* clear the stack */
        stack.clear();
        /* return the paths */
        return path;
    }

    unsigned char
    getChar(vector<char> binary)
    {
        /*
            given an input vector<char> that represents a binary word,
            get the corresponding integer number as a char.
         */

        int b = 0;
        unsigned char v = 0;
        while (!binary.empty())
        {
            /* repeats while the binary word is not empty, converting it to a
               decimal base number. At each iteration, the less significant bit
               is multiplied by its index (from right to left), and is added to
               the variable v, which will contain the acumulated values of the
               previous added bits. */
            v += (unsigned char) binary[binary.size() - 1 ] * (int) pow(2, b++);
            /* removes the less significant bit */
            binary.pop_back();
        }
        return v;
    }

    vector<char>
    getBinary(unsigned char ch)
    {
        
        /*
            given an input character (ch), gets a 8-bit length binary word
            that represents that char.
         */

        
        vector<char> binary;
        for (int i = 0; i < 8; i++)
        {
            binary.insert(binary.begin(), (char)((ch >> i) & 1));
        }
        while (binary[0] == 0)
        {
            binary.erase(binary.begin());
        }
        return binary;
    }

    void
    showPath(vector<char> &path)
    {

        cout << "Path: ";
        for (int i = 0; i < path.size(); i++)
        {
            printf("%d ", path[i]);
        }
        cout << endl;
    }

    string
    hexChar(unsigned char ch)
    {
        char hex[3];
        sprintf(hex, ((ch < 16) ? "0%x" : "%x"), (int) ch);
        return string(hex);
    }

    string
    hexDecode(string &hex)
    {
        string str;
        for (u64 i = 0; i < hex.size(); i+=2)
        {
            str += (char) stoi(hex.substr(i, 2), 0, 16);
        }
        return str;
    }

    string
    encrypt(string &text, string &keyf)
    {
        HuffmanTree* tree   = buildTree(text, keyf);
        vector<char>* paths = getPaths(tree);
        
        /* tree->showTree(); */
        
        delete tree;
        string encrypted;
        u64 index = 0;
        u64 used  = 0;
        vector<char> oldChar = paths[text[index]];
        vector<char> newChar;
        
        while (used < text.size())
        {
            if (newChar.empty())
            {
                newChar.push_back(1);
            }
            while (newChar.size() < 8)
            {
                if (oldChar.empty())
                {
                    used += 1;
                    if (index < (text.size()-1))
                    {
                        index += 1;
                        oldChar = paths[text[index]];
                    }
                    else
                    {
                        break;
                    }
                    
                }
                
                newChar.push_back(oldChar[0]);
                oldChar.erase(oldChar.begin());
            }
            encrypted += hexChar(getChar(newChar));
            /* showPath(newChar); */
            newChar.clear();
        }
        
        return encrypted;
    }

    string
    decrypt(string &encrypted, string &keyf)
    {
        string decoded    = hexDecode(encrypted);
        
        u64 *frequencys   = new u64[256];
        FILE *keyfile     = fopen(keyf.c_str(), "rb");
        fread(frequencys, sizeof(u64), 256, keyfile);
        fclose(keyfile);
        HuffmanTree *tree = buildTree(frequencys);
        
        /* tree->showTree(); */
        
        string decrypted;
        u64 index = 0;
        u64 used  = 0;
        vector<char> oldC = getBinary(decoded[index]);
        /* showPath(oldC); */
        oldC.erase(oldC.begin());
        while (used < decoded.size())
        {
            if (oldC.size() == 8)
            {
                oldC.erase(oldC.begin());
            }
            while (!tree->isOnLeaf())
            {
                if (oldC.empty())
                {
                    used += 1;
                    if (index < (decoded.size()-1))
                    {
                        index += 1;
                        oldC = getBinary(decoded[index]);
                        /* showPath(oldC); */
                        oldC.erase(oldC.begin());
                    }
                    else
                    {
                        break;
                    }
                    
                }
                if (oldC.empty())
                {
                    break;
                }
                char dir = oldC[0];
                oldC.erase(oldC.begin());
                if (dir == 1)
                {
                    
                    tree->walkRight();
                }
                else
                {
                    tree->walkLeft();
                }
            }
            decrypted += tree->getCurrentChar();
            tree->backToRoot();
            
        }

        return decrypted;
    }

#endif /*_CRYPT_H*/
