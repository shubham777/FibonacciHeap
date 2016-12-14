//
//  HashTable.h
//  FibonacciHeap
//
//  Created by Shubh on 11/12/16.
//  Copyright © 2016 Shubham Shukla. All rights reserved.
//

#include "Fibonheap.h"
#include <functional>


using namespace std;

#ifndef HashTable_h
#define HashTable_h

#define DEBUGG 0
#define HASHTABLE_SIZE 150401




//this struct represents each entry in the hashtable
struct HashNode
{
// Each hashNode contains the hashtag as the key and the heap struct node as the value, also contains the pointer to the next entry in the table.
public :
    
    string key;
    node* value;
    HashNode* next;

};

//This class contains all the operations that are required by the hashtable
class HashMap
{
private :
    HashNode** HashTable; // Hashtable is an array of HashNodes which will contain all the hashtable entiries
    int Size;  // will hold the size of the Hashtable
    
public :
    HashMap()
    {
        Size = (int)HASHTABLE_SIZE;
        HashTable = new HashNode*[Size];
        
        int i  = 0;
        
        for(i = 0; i < Size; i++)
            HashTable[i] = NULL;
    }
    
    HashMap(int size)
    {
        Size = size;
        HashTable = new HashNode*[Size];
        
        int i  = 0;
        
        for(i = 0; i < Size; i++)
            HashTable[i] = NULL;
    }
        
    ~HashMap()
    {
        int i  = 0;
        HashNode* entry;
        HashNode* prev;
        
        for(i = 0; i < Size; i++)
        {
            entry = HashTable[i];
            
            while(entry != NULL)
            {
                prev = entry;
                entry = entry->next;
                delete prev;
            }
            prev = NULL;
            entry = NULL;
        }
        
        delete[] HashTable;
    }

    
    // the hash function which takes the hashkey as the input and gives back the corresponding index in the hashtable
    int hashFunc(string s)
    {
        hash<string> hash_fn;
        
        unsigned int hash = hash_fn(s);
        
        return hash % HASHTABLE_SIZE;

    }
  
   //this function takes the key and the value and inserts it into the hashtable.
    void insertHash(std::string key, node* value)
    {
        int index = hashFunc(key);
        
        HashNode* prev = NULL;
        HashNode* entry = NULL;
        
        entry = HashTable[index];
        
        //this checks if the entry in the hashtable is a linked list
        while(entry != NULL)
        {
            if(entry->key == key)
            {
                entry->value = value;

                return;
                
            }
            else
            {
                prev = entry;
            }
            
            entry = entry->next;

        }
        
        
        entry = new HashNode();
        entry->key = key;
        entry->value = value;
        
        if(prev != NULL)
            prev->next = entry;
        else
            HashTable[index] = entry;
#if DEBUGG
        std :: cout << "Key : " << entry->key << " Value : " << entry->value << "\n";
#endif
        
    }
    
    //this function finds the node in the hashtable which contains the same key
    HashNode* findNodeinTable(std::string key)
    {
        int index = hashFunc(key);
        
        HashNode* entry = HashTable[index];
        
        if(entry)
        {
            while(entry != NULL)
            {
                if(entry->key == key)
                {
                    return entry;
                }
                entry = entry->next;
                
            }
        }
        
        return NULL;
                    
    }
    
    //this function removes the node from the table which contains the same key and returns the key
    string removefromTable(std::string key)
    {
        HashNode* entry = findNodeinTable(key);
        string poppedKey = entry->key;
        entry = NULL;
        
        return poppedKey;
    }
    
};



#endif /* HashTable_h */
