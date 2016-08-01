#ifndef __LINKLIST_H_
#define __LINKLIST_H_
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
	TLinkListItem<T> *next;	
public:		
	inline T *getItem(){ return item;} //GetItem from container
	inline TLinkListItem<T> *getNext(){ return next;} //Next item in linked list
	~TLinkListItem()
	{
		delete item;
	}
	
//Constructor
//p_item : Item to add to list:
//See TLinkList::Append

	TLinkListItem(T *p_item)
	{	
		item=p_item;
		next=nullptr;	
	}
};


//Linkedlist 
//Create:  TLinkList<TMyObject> l_list()
//Append item  l_list->append(myObject) (myObject of type TMyObject)
//Get top:     l_link->getStart()
//When listis is destoyes item is also deleted

template<class T>
class TLinkList
{
private:
	TLinkListItem<T> *start;   //start of linklist
	TLinkListItem<T> *end;     //last in linkedlist
public:
		
	inline TLinkListItem<T> *getStart(){ return start;} //start of linked list
	
	TLinkList(){
		end=nullptr;
		start=nullptr;
	}
	~TLinkList(){
		clear();
	}
	
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
	}
	
	
	void append(T *p_item)
	{
		TLinkListItem<T> *l_item=new TLinkListItem<T>(p_item);
		
		if(start==nullptr){
			start=l_item;
		} else {
			end->next=l_item;
		}
		end=l_item;
	}


	int getLength()
	{
		TLinkListItem<T> *l_current=start;
		int l_length=0;
		while(l_current){
			l_length++;
			l_current=l_current->getNext();
		}
		return l_length;
	}
};


#define LOOPLL(p_type,p_current,p_name) \
{\
	TLinkListItem<p_type> *p_name=(p_current);\
	while(p_name){
		
#define LOOPLLEND(p_name) \
	p_name=p_name->getNext();\
	}\
}


#define LOOPLLN(p_type,p_current,p_name) \
{\
	int l_cnt=0;\
	TLinkListItem<p_type> *p_name=(p_current);\
	while(p_name){
		
#define LOOPLLNEND(p_name) \
	p_name=p_name->getNext();\
	l_cnt++;\
	}\
}




#endif