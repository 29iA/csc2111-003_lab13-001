#if !defined (PQHEAP_H)
#define PQHEAP_H

#include "HeapSkew.h"

template < class T >
class PQHeap
{

   private:
      //HeapSkew<T>* hs;
      HeapArray<T>* ha;

   public:
      PQHeap(int (*compare_item) (T* item_1, T* item_2));
      ~PQHeap();

      bool pqIsEmpty();
      void pqInsert(T* item);
      T* pqRemove();

};

template < class T >
PQHeap<T>::PQHeap(int (*compare_item) (T* item_1, T* item_2))
{
   ha = new HeapArray<T>(compare_item);
}

template < class T >
PQHeap<T>::~PQHeap()
{
   delete ha;
}

template < class T >
bool PQHeap<T>::pqIsEmpty()
{
   return ha->heapIsEmpty();
}

template < class T >
void PQHeap<T>::pqInsert(T* item)
{
   ha->heapInsert(item);
}

template < class T >
T* PQHeap<T>::pqRemove()
{
   T* item = ha->heapRemove();
   return item;
}

#endif
