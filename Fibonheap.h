//
//  Hashtags.cpp
//  FibonacciHeap
//
//  Created by Shubh on 11/14/16.
//  Copyright © 2016 Shubham Shukla. All rights reserved.
//
#ifndef Fibonheap_h
#define Fibonheap_h

#define DEBUG_FIBO 0

#endif /* Fibonheap_h */
using namespace std;

//this struct contains all the  required fields of a node in the fibonacci heap.
struct node
{
    public :

    int value;        // holds the value of the hashCount of each hashtag
    bool marked;    // holds the information weathe if the node is marked or not, i.e., if its child has been cut
    int degree;     //holds the degree of node
    string key;     //hold the value of the hashkey which is to be inserted in the Hashtable
    node* prev;  // holds the pointer to the left sibling
    node* next;  // holds the pointer to the right sibling
    node*child;  // holds the pointer to one of the children
    node*parent; // holds the pointer to the parent node of the node
    
 
    node* getParent() { return parent; } //returns the parent of the node
    node* getChild() { return child; }   // returns the child of the node
    node* getPrev() {  return prev; }    // returns the left sibling of the node
    node* getNext() { return next; }     // returns the right sibling of the node
    int getValue()  { return value; }         // returns the value of the node, i.e., the hashCount of that node
    string getKey()  { return key; }        // returns the hashkey of the corresponding node
    bool isMarked() { return marked; }      //return if the node is marked or not
    
    
};

class FibonHeap
{
    
    
    public :
    
    node* heap;
    int itemCount;
    FibonHeap(){
        itemCount = 0;
        heap = NULL;
    }
    ~FibonHeap()
    {
        free(heap);
    }

    //this function inserts a new node in the Fibonacci heap with the data as given in the arguments
    
    node* insert (int value, string key)
    {
        //creates a new node
        node* newNode= new node;
        newNode->prev = newNode->next = newNode;
        newNode->parent = newNode->child = NULL;
        newNode->value = value;
        newNode->marked = false;
        newNode->degree = 0;
        newNode->key = key;
        
        
        if(heap == NULL)
        {
            heap = newNode;
            heap->next = heap->prev = heap;
            
        }
        else   // if heap isn't NUll, it appends the new node to the left of the heap
        {
            heap->prev->next = newNode;
            newNode->next = heap;
            newNode->prev = heap->prev;
            heap->prev = newNode;
            
            if(heap->value < newNode->value)
                heap = newNode;
            
            
            //print all the root nodes
#if DEBUG_FIBO
            
            node* temp = heap;
            
            do{

                cout << temp->value << "----";

                temp = temp->next;
            }
            
            while(temp!=heap);
            
#endif
        }
        
        itemCount++;    //tracks the no of items in the heap
        return newNode;     // returns the newly inserted node
    }
    
    
    //this function increases the key of node pointed by n
    void increaseKey(node* n, int increment)
    {
        if(n == NULL) return;
        
        n->value = n->value + increment;        //increments the value
        
        node* parent = n->parent;
        
        if(parent == NULL)
        {
            if(n->value > heap->value)
                heap = n;
            
            return;
        }
        else        // if heap is not NULL and if the value of the child becomes more than the value of the parent
        {
            if(n->value > parent->value)
            {
                CutChild(n, parent);    // first cut the child n from the parent
                Cascading(parent);      // do cascading towards the node
            }
            
        }
        

        
        if(n->value > heap->value)      //always make sure that heap points to the max value
            heap = n;
        
    }
    
    //this function cuts the child c from the parent p
    void CutChild( node* c, node* p)
    {
        
        if (c == c->prev)
            p->child = NULL;
        else  // if c is not isolated, i.e., it has siblings
        {
            p->child = c->prev;             //connect the other child to the parent
            (c->prev)->next = c->next;
            (c->next)->prev = c->prev;
        }
        
        
        if(p->degree != 0)              //decrease the degree by 1
            p->degree--;
        
        c->next = c;                //isolate the child for further merge with the heap
        c->prev = c;

        (heap->prev)->next = c;         // merge with the heap.
        c->next = heap;
        c->prev = heap->prev;
        heap->prev = c;
        
        
        if (c->value > heap->value )
            heap = c;
        
        c->parent = NULL;
        c->marked = false;              //mark it as false and reome any parent links
        
    }
    
    void Cascading(node* c)             //this function performs the cascading
    {
        node* p = c->parent;
        if (p != NULL)
        {
            if (c->marked == false )        //mark the child as true if parent is not NULL and child is marked false
            {
                c->marked = true;
            }
            else
            {
                CutChild(c,p);              //again cut the new child

                Cascading(p);               //again cascade towards new parent

            }
        }
    }
    
    
    
    //this function removes the node with the maximum value and then returns its pointer to the calling function
    node* removeMax()
    {
        node* oldMax = heap;        //copy the old max node
        
        
        if(heap == NULL)
            return heap;
        
        
        if(heap->child == NULL)             //if no child then connect the siblings of the max node
        {
            heap->prev->next = heap->next;
            heap->next->prev = heap->prev;
            heap = heap->next;
        }
        
        else
        {
            node* child = heap->child;      // if child , then proceed with unmarking all child with previous parent
            if(child == child->next)
            {
                heap->child = NULL;
                
                
                //update the heap
                heap->prev->next = heap->next;
                heap->next->prev = heap->prev;
                
                //integrate the child with the heap
                
                heap->prev->next = child;
                child->prev = heap->prev;
                child->next = heap;
                heap->prev = child;
                child->parent = NULL;
            }
            
            else
            {
                heap->child = NULL;

                
                //update the heap
                heap->prev->next = heap->next;
                heap->next->prev = heap->prev;
                heap = heap->next;
                
                bool sameChild = false;
                
                do
                {
                //ithChild gets the first child
                node* ithChild = child;
                
                //child becomes the next sibling
                child = ithChild->next;
                
                if(child == child->next)
                    sameChild = true;
                    
                child->prev = ithChild->prev;
                ithChild->prev->next = child;
                
                ithChild->next = ithChild->prev = ithChild;
                
                heap->prev->next = ithChild;
                ithChild->prev = heap->prev;
                ithChild->next = heap;
                heap->prev = ithChild;
                ithChild->parent = NULL;
                
                }
                while(sameChild == false);
            }
        }
        
        node* temp = heap;
        node* max = heap;
        
        do
        {
            if(temp->value > max->value)
            {
                max = temp;
            }
            temp = temp->next;
        }
        
        while(temp!=heap);
        
        heap = max;
        temp = heap;
        
        itemCount--;
        
        if(heap != heap->next)
        {
            Meld();            //after all the trees are connected with the heap, perform Meld to remove redundant degree trees
        }
        
        
        return oldMax;
    }
    
    void Meld()
    {
        if(heap == NULL)
            return;
        
        
        node* treesRecorded[50];            // this array holds all the pointers to the root nodes of heap 1-by-1
        int i = 0;
        for(i = 0; i<50; i++)
            treesRecorded[i] = NULL;
        
        node* first = heap;
        node* temp = heap;
        
        bool isSameNode = false;            //set this to false to proceed with scanning all the nodes 1-by-1
        while(isSameNode == false)
        {
            int degree = temp->degree;
            
        
            if(treesRecorded[degree] == NULL)       //if there is no tree with the same degree, save it to the array
            {
                treesRecorded[degree] = temp;
                temp = temp->next;
            }
            else
            {
                //pairwise combination of the two trees
                
                node* t = treesRecorded[degree];
                
                if(t == temp)
                {
                    isSameNode = true;
                    break;
                }
                
                if(t->value > temp->value)      //if the scanned tree has less value than the one stored in the array
                {
                    temp->prev->next = temp->next;          //make the smaller one child of the bigger one
                    temp->next->prev = temp->prev;
                    
                    node* n = temp;
                    n->next = n->prev = n;
                    
                    if(t->child == NULL)
                    {
                        t->child = n;
                    }
                    else                    //if the child of the bigger node has siblings
                    {
                        t->child->prev->next = n;
                        n->prev = t->child->prev;
                        n->next = t->child;
                        t->child->prev = n;
                    }
                    
                    n->parent = t;
                    t->degree++;            //increase the degree by 1
                    
                    temp = t;
                }
                else
                {
                    t->prev->next = t->next;        // if the scanned tree has a bigger node
                    t->next->prev = t->prev;
                    
                    node* n = t;
                    n->next = n->prev = n;
                    
                    if(t == heap)
                        heap = temp;
                    
                    if(temp->child == NULL)
                    {
                        temp->child = n;
                    }
                    else                           //if the child of the bigger node has siblings, connect them with this newNode
                    {
                        temp->child->prev->next = n;
                        n->prev = temp->child->prev;
                        n->next = temp->child;
                        temp->child->prev = n;
                    }
                    n->parent = temp;
                    temp->degree++;
                }
 
                
                node* temp1 = heap;
                node* max = heap;
                //updating the max value
                do
                {
                    if(temp1->value > max->value)
                    {
                        max = temp1;
                    }
                    temp1 = temp1->next;
                }
                
                while(temp1!=heap);
                
                heap = max;
                temp = heap;
                
                //start from scratch after every merge
                
                for(int i = 0; i<50; i++)
                    treesRecorded[i] = NULL;
                
            }
            
            
        }
      


        
        
        
    }
    
};

