#if !defined HEAPARRAY_H
#define HEAPARRAY_H

#include "Drawable.h"
#include "QueueLinked.h"
using CSC2110::QueueLinked;
#include "BinaryTree.h"
#include "Integer.h"
using CSC2110::Integer;

#include <iostream>
using namespace std;

template < class T >
class HeapArray : public Drawable
{

   private:
      int max_list;
      T** items;  
      int sze;  
      void heapRebuild();
      void arrayResize(int new_size);

      int (*compare_items) (T* item_1, T* item_2);

   public:
      HeapArray(int (*comp_items) (T* item_1, T* item_2));
      ~HeapArray();

      bool heapIsEmpty();
      void heapInsert(T* item);
      T* heapRemove();

      void draw(wxDC& dc, int width, int height);
      void mouseClicked(int x, int y);

};

// this is a MINHEAP (smallest items to top for alphabetical order)
template < class T >
HeapArray<T>::HeapArray(int (*comp_items) (T* item_1, T* item_2)) : Drawable()
{ 
   max_list = 2;
   items = new T*[max_list];
   sze = 0;

   compare_items = comp_items;
}  

template < class T >
HeapArray<T>::~HeapArray() 
{ 
   delete[] items;
}  

template < class T >
bool HeapArray<T>::heapIsEmpty() 
{
   return sze == 0;
} 

template < class T >
void HeapArray<T>::heapInsert(T* item)
{
   if (sze == max_list) 
   {
      arrayResize(2*max_list); //double the size of the array
   } 

   // place the new item at the end of the heap
   items[sze] = item;

   // trickle new item up to its proper position
   int place = sze;
   int parent = (place - 1)/2; //array location of parent

   while ((parent >= 0) && (*compare_items) (items[place], items[parent]) < 0)
   {
      //swap items[place] and items[parent]
      T* temp = items[parent];
      items[parent] = items[place];
      items[place] = temp;
        
      place = parent;
      parent = (place - 1)/2;
   }  

   sze++;
} 

template < class T >
T* HeapArray<T>::heapRemove()
{
   T* root_item = NULL;
   if (!heapIsEmpty()) 
   {
      root_item = items[0];  //dequeue smallest
      items[0] = items[sze - 1];
      items[sze - 1] = NULL;
      sze--;
      heapRebuild(); 
   }  
   else
   {
      return NULL;
   }

   return root_item;
}  

template < class T >
void HeapArray<T>::heapRebuild() 
{
   bool swap = true;

   int parent = 0;
   int child = 2*parent + 1;  //index of parent's left child, if any

   while (child < sze && swap)
   {
      swap = false;

      //root is not a leaf, so it has a left child at child
      int right_child = child + 1;  //index of right child, if any

      //if parent has a right child, find smaller child
      //note that using a Heap, FIFO order may not be maintained for items with the same priority
      if (right_child < sze)
      {
         if ((*compare_items) (items[right_child], items[child]) < 0)
         {
            child = right_child;  //index of smaller child
         }
      } 

      //if the parent's value is larger than the
      //value in the smaller child, swap values

      if ((*compare_items) (items[parent], items[child]) > 0)
      {
         T* swap_item = items[parent];
         items[parent] = items[child];
         items[child] = swap_item;
         swap = true;
      }

      parent = child;
      child = 2*parent + 1; //if out of bounds, we are done
   }
} 

template < class T >
void HeapArray<T>::arrayResize(int new_size)
{
   max_list = new_size;
   T** temp = new T*[max_list];

   for (int x = 0; x < sze; x++)
   {
      temp[x] = items[x];
   }

   delete[] items;
   items = temp;
}

template < class T >
void HeapArray<T>::draw(wxDC& dc, int width, int height)
{
   if (sze == 0)
   {
      return;
   }

   QueueLinked<Integer>* qI = new QueueLinked<Integer>();
   QueueLinked< BinaryTree<T> >* qBT = new QueueLinked< BinaryTree<T> >();

   //put all of the leaves on the queue at first
   for (int i = sze - 1; i >= sze/2; i--)
   {
      BinaryTree<T>* bt = new BinaryTree<T>(items[i]);
      qI->enqueue(new Integer(i));
      qBT->enqueue(bt);
   }

   //special case
   if (sze % 2 == 0)  //there is a left child with no right child on the bottom row
   {
      Integer* i = qI->dequeue();
      int currIndex = i->getValue();
      delete i;
      BinaryTree<T>* left = qBT->dequeue();
      int parent_index = (currIndex - 1)/2;
      BinaryTree<T>* parent = new BinaryTree<T>(items[parent_index]);
      parent->attachLeftSubtree(left);
      delete left;
      qI->enqueue(new Integer(parent_index));
      qBT->enqueue(parent);
   }

   while (qI->size() > 1)  
   {
      //right child 
      Integer* i = qI->dequeue();
      int rightIndex = i->getValue();
      delete i;

      BinaryTree<T>* right = qBT->dequeue();
      int parent_index = (rightIndex - 1)/2;  //parent index

      //left child
      i = qI->dequeue();
      int leftIndex = i->getValue();
      delete i;

      BinaryTree<T>* left = qBT->dequeue();
      BinaryTree<T>* parent = new BinaryTree<T>(items[parent_index]);
      parent->attachLeftSubtree(left);
      parent->attachRightSubtree(right);
      delete left;
      delete right;
      qI->enqueue(new Integer(parent_index));
      qBT->enqueue(parent);
   }

   Integer* i = qI->dequeue();  //discard the integer
   delete i;
   BinaryTree<T>* bt = qBT->dequeue();
   bt->draw(dc, width, height);
   delete bt;

   delete qI;
   delete qBT;
}

template < class T >
void HeapArray<T>::mouseClicked(int x, int y) {}

#endif
