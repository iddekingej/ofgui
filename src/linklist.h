#ifndef _LINKLIST_H_
#define _LINKLIST_H_
#include <stdio.h>

template<class T>
class TLinkList;

// Container class for item with T
// Container is added to TLinklist

template<class T>
class TLinkListItem
{
	friend class TLinkList<T>;
private:
	T *item;   //Item
protected:
	/**
	 * Next element in linklist.
	 */
	TLinkListItem<T> *next;	
public:		
	inline T *getItem(){ return item;} //GetItem from container
	inline TLinkListItem<T> *getNext(){ return next;} //Next item in linked list
/**
 *  Destructor
 *  When container is desturcted the containing element is also deleted
 */	

	~TLinkListItem()
	{
		delete item;
	}
	
/**
 *Constructor
 *\param p_item : Item to add to list: (see \ref TLinkList::append)
 *
 */

	TLinkListItem(T *p_item)
	{	
		item=p_item;
		next=nullptr;	
	}
};


/**
 * Linkedlist 
 * Create:  TLinkList<TMyObject> l_list()
 * Append item  l_list->append(myObject) (myObject of type TMyObject)
 * Get top:     l_link->getStart()
* When list is is destroyed item is also deleted
*/

template<class T>
class TLinkList
{
private:
	/**
	 *  Start of linklist
	 */
	TLinkListItem<T> *start;   
	
	/**
	 * Top of linked list. 
	 * All items are added tot the end
	 */
	TLinkListItem<T> *end;     
	
	/**
	 * Length of linklist
	 */
	long             length=0;
public:
		
	inline TLinkListItem<T> *getStart(){ return start;} //start of linked list
	inline TLinkListItem<T> *getEnd(){ return end;} //end of linked list
	inline long getLength(){ return length;}
	inline bool isEmpty(){ return start==nullptr;}
	
	TLinkList(){
		end=nullptr;
		start=nullptr;
	}
	
	/**
	 * Clear all items from link list.
	 * Also all containing items are deleted.
	 */
	void clear()
	{
		TLinkListItem<T> *l_current=start;
		TLinkListItem<T> *l_next;
		while(l_current !=nullptr){
			l_next=l_current->next;
			delete l_current;
			l_current=l_next;
		}
		end=nullptr;
		start=nullptr;
		length=0;
	}
	
	/**
	 *  Destructor
	 *  All items on the lists are deleted
	 */
	
	~TLinkList(){
		clear();
	}
	
	/**
	 * Append item to list
	 * 
	 * \param p_item item to append
	 */
	
	void append(T *p_item)
	{
		TLinkListItem<T> *l_item=new TLinkListItem<T>(p_item);
		
		if(start==nullptr){
			start=l_item;
		} else {
			end->next=l_item;
		}
		end=l_item;
		length++;
	}

};

/**
 * Java like iterator for linklist
 */

template<class T>
class TLinkListIterator{
private:
	TLinkListItem<T> *current;
	
public:
	inline TLinkListIterator(TLinkList<T> &p_list){
		current=p_list.getStart();
	}
	
	inline TLinkListIterator(TLinkList<T> *p_list){
		current=p_list->getStart();
	}
	
	/**
	 *  Returns true when there is a next item.
	 */
	
	inline bool hasNext(){
		return current != nullptr;
	}
	
	/**
	 *  returns the current item and advances to the next item.
	 */
	
	inline T* next(){
		if(current != nullptr){
			T *l_item=current->getItem();
			current=current->getNext();
			return l_item;
		}
		return nullptr;
	}
		
};




#endif

