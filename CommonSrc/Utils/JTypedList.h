//---------------------------------------------------------------------------

#ifndef JTypedListH
#define JTypedListH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
                              
template <class T>
class JTypedList : public TList
{

private:
   bool bAutoDelete;

protected:
   T* __fastcall Get(int Index)
   {
      return (T*) TList::Get(Index);
   }

   void __fastcall Put(int Index, T* Item)
   {
      TList::Put(Index,Item);
   }

public:

   __fastcall JTypedList(bool bFreeObjects = false):TList(),bAutoDelete(bFreeObjects)
   {
   }

    // Note: No destructor needed. TList::Destroy calls Clear,
    //       and Clear is virtual, so our Clear runs.

   int __fastcall Add(T* Item)
   {
      return TList::Add(Item);
   }

   void __fastcall Delete(int Index)
   {
      if(bAutoDelete)
         delete Get(Index);
      TList::Delete(Index);
   }

   void __fastcall Clear();

   T* __fastcall First()
   {
      return (T*)TList::First();
   }

   int __fastcall IndexOf(T* Item)
   {
        return TList::IndexOf(Item);
   }

   void __fastcall Insert(int Index, T* Item)
   {
      TList::Insert(Index,Item);
   }

   T* __fastcall Last(void)
   {
      return (T*) TList::Last();
   }

   int __fastcall Remove(T* Item)
   {
      int nIndex=TList::Remove(Item);
      // Should I delete a pointer that is being passed to me.
      // If bAutoDelete is true, then assume that we are always
      // responsible for deleting a pointer that is added to the
      // list. If the item was found, then delete the pointer.
      if (bAutoDelete&&(nIndex!=-1))
         delete Item;
      return nIndex;
   }

    __property T* Items[int Index] = {read=Get, write=Put};

};

template <class T>
void __fastcall JTypedList<T>::Clear()
{
   if (bAutoDelete)
      {
      for (int j=0; j<Count; j++)
         delete Items[j];
      }
   TList::Clear();
}

#endif
