//---------------------------------------------------------------------------

#ifndef JListH
#define JListH
//---------------------------------------------------------------------------

//JList Version 1.02 - 11/01/2004

//If compiled in debug mode, the assert() will be resolved as a function
//If not in debug mode, assert will resolve to a NOP macro

#ifndef _DEBUG           
   #define NDEBUG
#endif

#include <assert.h>


template <class T> class JList
{

private:

	struct ListEntryStruct
	{
		ListEntryStruct* pPrev;
		T* pData;
		ListEntryStruct* pNext;

		ListEntryStruct()
		{
			pPrev=NULL;
			pData=NULL;
			pNext=NULL;
		}

   };

   bool bDelete;

   ListEntryStruct* pFirstEntry;
   ListEntryStruct* pLastEntry;
   ListEntryStruct* pCurrentEntry;
	int nCount;

   T** ppVector;

   void ClearList(const bool bDel);

	void QSort(const int nLeft, const int nRight);
   void BSort();


public:

   JList(const bool bAutoDelete=true)
   {
      bDelete=bAutoDelete;
      pFirstEntry=new ListEntryStruct;
      pFirstEntry->pPrev=NULL;
      pFirstEntry->pData=NULL;
      pFirstEntry->pNext=NULL;
      pLastEntry=pFirstEntry;
      nCount=0;
		ppVector=NULL;
   }

   JList(const int nAutoCreate, const bool bAutoDelete=true);

	JList(const JList<T>& L);

	JList& operator = (const JList<T>& L);

	void Clear();

   void ClearPointers();


   ~JList()
   {
      Clear();
      delete pFirstEntry;
      delete[] ppVector;
	}

	int nGetCount() const
	{
		return nCount;
   }

   T* pExpand()
   {
      pCurrentEntry=pLastEntry;
      pLastEntry->pData=new T;
      ListEntryStruct* pPrev=pLastEntry;
      pLastEntry=new ListEntryStruct;
      pPrev->pNext=pLastEntry;
      pLastEntry->pPrev=pPrev;
      pLastEntry->pNext=NULL;
      ++nCount;
      delete[] ppVector;
      ppVector=NULL;
      return pPrev->pData;
   }

   int nAdd(T* pItem)
   {
      pCurrentEntry=pLastEntry;
      pLastEntry->pData=pItem;
      ListEntryStruct* pPrev=pLastEntry;
      pLastEntry=new ListEntryStruct;
      pPrev->pNext=pLastEntry;
      pLastEntry->pPrev=pPrev;
      pLastEntry->pNext=NULL;
      delete[] ppVector;
      ppVector=NULL;
      return nCount++;
   }

	T* pFirst()
   {
      assert(nCount>0);
      return pFirstEntry->pData;
   }

   T* pLast()
	{
		if (nCount>0)
			return pLastEntry->pPrev->pData;
		else
			return NULL;
   }

   T* pPopFirst()             //NB Does not delete contents of pointer !!!
   {
      assert(nCount>0);
      T* pData=pFirstEntry->pData;
      pCurrentEntry=pFirstEntry;
      bool bDel=bDelete;
      bDelete=false;
      Delete();
      bDelete=bDel;
   }

   //Use as FIFO
   void PopFirst(T& Data)    //NB Returns copy of data and deletes contents of pointer (if set)
   {
      assert(nCount>0);
      Data=*pFirstEntry->pData;
      pCurrentEntry=pFirstEntry;
      Delete();
   }

   void PopFirst()            //NB Deletes contents of pointer (if set) and throws away data
   {
      assert(nCount>0);
      pCurrentEntry=pFirstEntry;
      Delete();
   }

   T* pFind(const int nIndex);

   void Insert(T* pItem)   //Insert at current position
	{
      ListEntryStruct* pEntry=new ListEntryStruct;
      pEntry->pPrev=pCurrentEntry->pPrev;
      if (pEntry->pPrev)
         pEntry->pPrev->pNext=pEntry;
      else
         pFirstEntry=pEntry;  //Inserted new first entry
      pEntry->pNext=pCurrentEntry;
      pEntry->pData=pItem;
      pCurrentEntry->pPrev=pEntry;
      pCurrentEntry=pEntry;
      ++nCount;
      delete[] ppVector;
      ppVector=NULL;
   }

   void Insert(const int nIndex, T* pItem);

   void Delete()  //Delete at current position
   {
      assert(nCount>0);
      ListEntryStruct* pNew=pCurrentEntry->pNext;
      if (pCurrentEntry->pPrev)
         pCurrentEntry->pPrev->pNext=pCurrentEntry->pNext;
      else
         pFirstEntry=pNew;    //Deleted old first entry
      if (pCurrentEntry->pNext)
         pCurrentEntry->pNext->pPrev=pCurrentEntry->pPrev;
      if (bDelete)
         delete pCurrentEntry->pData;
      delete pCurrentEntry;
      pCurrentEntry=pNew;
      --nCount;
      delete[] ppVector;
      ppVector=NULL;
   }

   void Delete(const int nIndex);

	void GoFirst()
	{
		pCurrentEntry=pFirstEntry;
	}

   void GoLast()
   {
      pCurrentEntry=pLastEntry;
   }

   T* pNext()
   {
      assert(pCurrentEntry->pNext!=NULL);
      T* pData=pCurrentEntry->pData;
      pCurrentEntry=pCurrentEntry->pNext;
      return pData;
	}

	void GoNext()
	{
		assert(pCurrentEntry->pNext!=NULL);
		pCurrentEntry=pCurrentEntry->pNext;
	}

	T* pPrev()
	{
		if (pCurrentEntry->pPrev)
			{
			pCurrentEntry=pCurrentEntry->pPrev;
			return pCurrentEntry->pData;
			}
		else
			return NULL;
	}

	void GoPrev()
	{
		if (pCurrentEntry->pPrev)
			pCurrentEntry=pCurrentEntry->pPrev;
	}

	T* pGetPrev() const
	{
		assert(pCurrentEntry->pPrev!=NULL);
		return pCurrentEntry->pPrev->pData;
	}

	T* pGetNext() const
	{
      assert(pCurrentEntry->pNext!=NULL);
      return pCurrentEntry->pNext->pData;
   }

   T* pGetCurrent() const
   {
      assert(pCurrentEntry->pData!=NULL);
      return pCurrentEntry->pData;
   }

	bool bLast() const
	{
      if (pCurrentEntry)
			return (pCurrentEntry->pNext==NULL);
      else
         return true;
	}

	bool bFirst() const
	{
      if (pCurrentEntry)
     	 return (pCurrentEntry->pPrev==NULL);
      else
         return true;
   }

	void DeletePrev()
	{
		GoPrev();
		Delete();
	}

   void Exchange(const int n1, const int n2);

   void SwopWithPrev();
   void SwopWithNext();

   void Pack(const bool bWipeList=false);

   void Unpack(const bool bWipeVector=false);

   bool bPacked() const
   {
      return (ppVector!=NULL);
   }

	T* operator[](const int nIndex)  //Use Only after Pack() !!!
	{
//      assert((nIndex<nCount)&&(ppVector!=NULL));
		assert(nIndex<nCount);
		assert(ppVector!=NULL);
		return ppVector[nIndex];
   }

   T* At(const int nIndex)          //Use Only after Pack() !!!
   {
      assert((nIndex<nCount)&&(ppVector!=NULL));
      return ppVector[nIndex];
   }


   // QuickSort: This needs > and < operators to be defined for the template class
   // BubbleSort: This needs < operator to be defined for the template class
   void Sort(const bool bQuickSort=true)
	{
		if (nGetCount()>1)
			{
			if (!bPacked())
				Pack();
			if (bQuickSort)
				QSort(0,nCount-1);
			else
				BSort();
			}
	}

	bool bEmpty() const
	{
		return (nCount==0);
	}

};

template <class T> JList<T>::JList(const JList<T>& L)
{
	pFirstEntry=new ListEntryStruct;
	pFirstEntry->pPrev=NULL;
	pFirstEntry->pData=NULL;
	pFirstEntry->pNext=NULL;
	pLastEntry=pFirstEntry;
	nCount=0;
	ppVector=NULL;
	*this=L;
};

template <class T> JList<T>& JList<T>::operator = (const JList<T>& L)
{
   Clear();
   bDelete=L.bDelete;
   if (L.ppVector)
      {
      nCount=L.nCount;
		if (nCount>0)
         {
			ppVector=new T*[nCount];
         if (L.bDelete)
            {
            //Memory managed internally
            for (int i=0; i<nCount; i++)
					{
					ppVector[i]=new T;
					*ppVector[i]=*L.ppVector[i];
               }
				}
         else
            {
            //Memory managed externally
				for (int i=0; i<L.nCount; i++)
					ppVector[i]=L.ppVector[i];
				}
         if (L.pFirstEntry->pNext!=NULL)
				Unpack();
			}
		}
	else
		{
		ListEntryStruct* pCurrent=L.pFirstEntry;
		if (L.bDelete)
			{
			//Memory managed internally
			while(pCurrent->pNext)
				{
				T* pData=pCurrent->pData;
				pCurrent=pCurrent->pNext;
				*pExpand()=*pData;
				}
			}
		else
			{
			//Memory managed externally
			while(pCurrent->pNext)
				{
				T* pData=pCurrent->pData;
				pCurrent=pCurrent->pNext;
				nAdd(pData);
				}
			}

/*
		L.GoFirst();
		if (L.bDelete)
			{
			//Memory managed internally
			while(!L.bLast())
				*pExpand()=*L.pNext();
			}
		else
			{
			//Memory managed externally
			while(!L.bLast())
				nAdd(L.pNext());
			}
*/
		}
	return *this;
};


template <class T> JList<T>::JList(const int nAutoCreate, const bool bAutoDelete)
{
   bDelete=bAutoDelete;
   pFirstEntry=new ListEntryStruct;
   pFirstEntry->pPrev=NULL;
   pFirstEntry->pData=NULL;
   pFirstEntry->pNext=NULL;
   pLastEntry=pFirstEntry;
   nCount=0;
   ppVector=NULL;
   for (int i=0; i<nAutoCreate; i++)
      pExpand();
}

template <class T> void JList<T>::ClearPointers()
{
   pFirstEntry=new ListEntryStruct;
   pFirstEntry->pPrev=NULL;
   pFirstEntry->pData=NULL;
   pFirstEntry->pNext=NULL;
   pLastEntry=pFirstEntry;
   pCurrentEntry=NULL;
   ppVector=NULL;
   nCount=0;
   bDelete=false;
}


template <class T> void JList<T>::ClearList(const bool bDel)
{
	if (pLastEntry)
		pCurrentEntry=pLastEntry->pPrev;
	else
		pLastEntry=NULL;
   delete pLastEntry;
	pLastEntry=NULL;
   while(pCurrentEntry)
      {
      ListEntryStruct* pDel=pCurrentEntry->pPrev;
      if (bDel)
         delete pCurrentEntry->pData;
      delete pCurrentEntry;
		pCurrentEntry=pDel;
      }
   pFirstEntry=new ListEntryStruct;
   pFirstEntry->pPrev=NULL;
	pFirstEntry->pData=NULL;
   pFirstEntry->pNext=NULL;
   pLastEntry=pFirstEntry;
}

template <class T> void JList<T>::Clear()
{
	if ((pFirstEntry->pNext==NULL)&&(ppVector)&&(bDelete))
      {
      //The List has been deleted in Pack() -> delete pointers in vector
      for (int i=0; i<nCount; i++)
         delete ppVector[i];
      }
   else
      ClearList(bDelete);
   delete[] ppVector;
   ppVector=NULL;
   nCount=0;
}

template <class T> T* JList<T>::pFind(const int nIndex)
{
	assert((pFirstEntry!=pLastEntry)&&(nIndex<nCount));
   pCurrentEntry=pFirstEntry;
   for (int i=0; i<nIndex; i++)
		pCurrentEntry=pCurrentEntry->pNext;
   return pCurrentEntry->pData;
}

template <class T> void JList<T>::Exchange(const int n1, const int n2)
{
   assert((n1<nCount)&&(n2<nCount));
   if (pFirstEntry->pNext)
      {
      T* p1=pFind(n1);
      T* p2=pFind(n2);
      T Data=*p1;
      *p1=*p2;
      *p2=Data;
      }
   if (ppVector) Pack();
}

template <class T> void JList<T>::SwopWithPrev()
{
   assert(pCurrentEntry->pPrev!=NULL);
   T Prev=*pCurrentEntry->pPrev;
   *pCurrentEntry->pPrev=*pCurrentEntry->pData;
   *pCurrentEntry->pData=Prev;
}

template <class T> void JList<T>::SwopWithNext()
{
   assert(pCurrentEntry->pNext!=NULL);
   T Next=*pCurrentEntry->pNext;
   *pCurrentEntry->pNext=*pCurrentEntry->pData;
   *pCurrentEntry->pData=Next;
}

template <class T> void JList<T>::Insert(const int nIndex, T* pItem)
{
	pFind(nIndex);
   Insert(pItem);
}

template <class T> void JList<T>::Delete(const int nIndex)
{
   pFind(nIndex);
   Delete();
}


template <class T> void JList<T>::Pack(const bool bWipeList)
{
   delete[] ppVector;
   ppVector=NULL;
   if (nCount>0)
      {
      ppVector=new T* [nCount];
      pCurrentEntry=pFirstEntry;
      for (int i=0; i<nCount; i++)
         {
         ppVector[i]=pCurrentEntry->pData;
         pCurrentEntry=pCurrentEntry->pNext;
         }
      }
   if (bWipeList)
      ClearList(false); //do not delete data
}

template <class T> void JList<T>::Unpack(const bool bWipeVector)
{
   assert(ppVector!=NULL);
   if (pFirstEntry->pNext==NULL)
		{
      for (int i=0; i<nCount; i++)
         {
			pCurrentEntry=pLastEntry;
         pLastEntry->pData=ppVector[i];
         ListEntryStruct* pPrev=pLastEntry;
         pLastEntry=new ListEntryStruct;
         pPrev->pNext=pLastEntry;
         pLastEntry->pPrev=pPrev;
         pLastEntry->pNext=NULL;
         }
      if (bWipeVector)
         {
         delete[] ppVector;
         ppVector=NULL;
         }
      }
}

template <class T> void JList<T>::BSort()
{
   T T1;
   T T2;
   if (!ppVector) Pack();
   for (int i=1; i<nCount; i++)
      {
      for (int j=1; j<nCount; j++)
         {
         T1=*ppVector[j-1];
         T2=*ppVector[j];
         if (T2<T1)
            {
            *ppVector[j]=T1;
            *ppVector[j-1]=T2;
            }
         }
		}
}

template <class T> void JList<T>::QSort(const int nLeft, const int nRight)
{
 	int i=nLeft;
   int j=nRight;
   T TTmp;
   T TPivot;
   TPivot=*ppVector[(nLeft+nRight)/2];
   // partition
   while (i<=j)
   	{
      while (*ppVector[i]<TPivot)
      	++i;
      while (*ppVector[j]>TPivot)
      	--j;
      if (i<=j)
      	{
      	TTmp=*ppVector[i];
         *ppVector[i]=*ppVector[j];
         *ppVector[j]=TTmp;
         ++i;
         --j;
         }
      }
   // recursion
   if (nLeft<j)
   	QSort(nLeft,j);
   if (i<nRight)
   	QSort(i,nRight);
}

#endif



