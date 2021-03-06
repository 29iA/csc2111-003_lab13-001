#if !defined HEAPSKEW_H
#define HEAPSKEW_H

#include "BinaryTree.h"
#include "wx/wx.h"

template < class T >
class HeapSkew : public Drawable
{

   private:
      BinaryTree<T>* bt;
      int sze;
      BinaryTree<T>* merge(BinaryTree<T>* left, BinaryTree<T>* right);

      int (*compare_items) (T* item_1, T* item_2);

   public:
      HeapSkew(int (*comp_items) (T* item_1, T* item_2));
      ~HeapSkew();

      bool heapIsEmpty();
      void heapInsert(T* item);
      T* heapRemove();

      void draw(wxDC& dc, int width, int height);
      void mouseClicked(int x, int y);

};

// this is a MAXHEAP (largest items to top alphabetical order)
template < class T >
HeapSkew<T>::HeapSkew(int (*comp_items) (T* item_1, T* item_2)) : Drawable()
{
   bt = new BinaryTree<T>();
   sze = 0;

   compare_items = comp_items;
}

template < class T >
HeapSkew<T>::~HeapSkew()
{
   delete bt;
}

template < class T >
bool HeapSkew<T>::heapIsEmpty()
{
   return sze == 0;
}

template < class T >
BinaryTree<T>* HeapSkew<T>::merge(BinaryTree<T>* left, BinaryTree<T>* right)
{
   //DO THIS

   //background checks
   if (left->isEmpty())
   {
      delete left;
      return right;
   }

   if (right->isEmpty())
   {
      delete right;
      return left;
   }

   //otherwise main function follows
   T* left_sub = left->getRootItem();
   T* right_sub = right->getRootItem();

   int compare = (*compare_items) (left_sub, right_sub);

   //check comparison
   if (compare < 0)
      return merge(right, left); //calls merge function recursively

   else
   {
      //detach both both subtree on left, prepare for moving
      BinaryTree<T>* left_sub = left->detachLeftSubtree();
      BinaryTree<T>* right_sub = left->detachRightSubtree();

      //LRL
      left->attachRightSubtree(left_sub);
      delete left_sub;

      //check right_sub
      if (right_sub->isEmpty())
      {
         left->attachLeftSubtree(right);

         delete right;
         delete right_sub;
         return left;
      }

      //right_sub not empty, calls merge again, attach existing merger to the left subtree
      else
      {
         BinaryTree<T>* merger = merge(right_sub, right);
         left->attachLeftSubtree(merger);

         delete merger;
         return left;
      }
   }
}

template < class T >
void HeapSkew<T>::heapInsert(T* item)
{
   //DO THIS (calls merge, should be short)
   BinaryTree<T>* insert = new BinaryTree<T>(item);

   //calls merge with new insert item to the right
   bt = merge(bt, insert);
   sze++;
}

template < class T >
T* HeapSkew<T>::heapRemove()
{
   //DO THIS (calls merge, should be short)
   //get the root item for safety to return later
   T* result = bt->getRootItem();

   //calls detach to respective subtree to isolate root
   BinaryTree<T>* left = bt->detachLeftSubtree();
   BinaryTree<T>* right = bt->detachRightSubtree();

   delete bt;

   //calls merge
   bt = merge(left, right);
   sze--;
   return result;
}

template < class T >
void HeapSkew<T>::draw(wxDC& dc, int width, int height)
{
   bt->draw(dc, width, height);
}

template < class T >
void HeapSkew<T>::mouseClicked(int x, int y) {bt->mouseClicked(x, y);}

#endif
