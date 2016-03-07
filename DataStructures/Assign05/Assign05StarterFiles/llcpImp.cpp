#include <iostream>
#include <cstdlib>
#include "llcpInt.h"
using namespace std;

int FindListLength(Node* headPtr)
{
   int length = 0;

   while (headPtr != 0)
   {
      ++length;
      headPtr = headPtr->link;
   }

   return length;
}

bool IsSortedUp(Node* headPtr)
{
   if (headPtr == 0 || headPtr->link == 0) // empty or 1-node
      return true;
   while (headPtr->link != 0) // not at last node
   {
      if (headPtr->link->data < headPtr->data)
         return false;
      headPtr = headPtr->link;
   }
   return true;
}

void InsertAsHead(Node*& headPtr, int value)
{
   Node *newNodePtr = new Node;
   newNodePtr->data = value;
   newNodePtr->link = headPtr;
   headPtr = newNodePtr;
}

void InsertAsTail(Node*& headPtr, int value)
{
   Node *newNodePtr = new Node;
   newNodePtr->data = value;
   newNodePtr->link = 0;
   if (headPtr == 0)
      headPtr = newNodePtr;
   else
   {
      Node *cursor = headPtr;

      while (cursor->link != 0) // not at last node
         cursor = cursor->link;
      cursor->link = newNodePtr;
   }
}

void InsertSortedUp(Node*& headPtr, int value)
{
   Node *precursor = 0,
        *cursor = headPtr;

   while (cursor != 0 && cursor->data < value)
   {
      precursor = cursor;
      cursor = cursor->link;
   }

   Node *newNodePtr = new Node;
   newNodePtr->data = value;
   newNodePtr->link = cursor;
   if (cursor == headPtr)
      headPtr = newNodePtr;
   else
      precursor->link = newNodePtr;

   ///////////////////////////////////////////////////////////
   /* using-only-cursor (no precursor) version
   Node *newNodePtr = new Node;
   newNodePtr->data = value;
   //newNodePtr->link = 0;
   //if (headPtr == 0)
   //   headPtr = newNodePtr;
   //else if (headPtr->data >= value)
   //{
   //   newNodePtr->link = headPtr;
   //   headPtr = newNodePtr;
   //}
   if (headPtr == 0 || headPtr->data >= value)
   {
      newNodePtr->link = headPtr;
      headPtr = newNodePtr;
   }
   //else if (headPtr->link == 0)
   //   head->link = newNodePtr;
   else
   {
      Node *cursor = headPtr;
      while (cursor->link != 0 && cursor->link->data < value)
         cursor = cursor->link;
      //if (cursor->link != 0)
      //   newNodePtr->link = cursor->link;
      newNodePtr->link = cursor->link;
      cursor->link = newNodePtr;
   }

   ////////////////// commented lines removed //////////////////

   Node *newNodePtr = new Node;
   newNodePtr->data = value;
   if (headPtr == 0 || headPtr->data >= value)
   {
      newNodePtr->link = headPtr;
      headPtr = newNodePtr;
   }
   else
   {
      Node *cursor = headPtr;
      while (cursor->link != 0 && cursor->link->data < value)
         cursor = cursor->link;
      newNodePtr->link = cursor->link;
      cursor->link = newNodePtr;
   }
   */
   ///////////////////////////////////////////////////////////
}

bool DelFirstTargetNode(Node*& headPtr, int target)
{
   Node *precursor = 0,
        *cursor = headPtr;

   while (cursor != 0 && cursor->data != target)
   {
      precursor = cursor;
      cursor = cursor->link;
   }
   if (cursor == 0)
   {
      cout << target << " not found." << endl;
      return false;
   }
   if (cursor == headPtr) //OR precursor == 0
      headPtr = headPtr->link;
   else
      precursor->link = cursor->link;
   delete cursor;
   return true;
}

bool DelNodeBefore1stMatch(Node*& headPtr, int target)
{
   if (headPtr == 0 || headPtr->link == 0 || headPtr->data == target) return false;
   Node *cur = headPtr->link, *pre = headPtr, *prepre = 0;
   while (cur != 0 && cur->data != target)
   {
      prepre = pre;
      pre = cur;
      cur = cur->link;
   }
   if (cur == 0) return false;
   if (cur == headPtr->link)
   {
      headPtr = cur;
      delete pre;
   }
   else
   {
      prepre->link = cur;
      delete pre;
   }
   return true;
}

void ShowAll(ostream& outs, Node* headPtr)
{
   while (headPtr != 0)
   {
      outs << headPtr->data << "  ";
      headPtr = headPtr->link;
   }
   outs << endl;
}

void FindMinMax(Node* headPtr, int& minValue, int& maxValue)
{
   if (headPtr == 0)
   {
      cerr << "FindMinMax() attempted on empty list" << endl;
      cerr << "Minimum and maximum values not set" << endl;
   }
   else
   {
      minValue = maxValue = headPtr->data;
      while (headPtr->link != 0)
      {
         headPtr = headPtr->link;
         if (headPtr->data < minValue)
            minValue = headPtr->data;
         else if (headPtr->data > maxValue)
            maxValue = headPtr->data;
      }
   }
}

double FindAverage(Node* headPtr)
{
   if (headPtr == 0)
   {
      cerr << "FindAverage() attempted on empty list" << endl;
      cerr << "An arbitrary zero value is returned" << endl;
      return 0.0;
   }
   else
   {
      int sum = 0,
          count = 0;

      while (headPtr != 0)
      {
         ++count;
         sum += headPtr->data;
         headPtr = headPtr->link;
      }

      return double(sum) / count;
   }
}

void ListClear(Node*& headPtr, int noMsg)
{
   int count = 0;

   Node *cursor = headPtr;
   while (headPtr != 0)
   {
      headPtr = headPtr->link;
      delete cursor;
      cursor = headPtr;
      ++count;
   }
   if (noMsg) return;
   clog << "Dynamic memory for " << count << " nodes freed"
        << endl;
}

// definition of RemBadSplitGood of Assignment 5 Part 1

void RemBadSplitGood(Node*& headPtr1, Node*& headPtr2, Node*& headPtr3)
{
   int countlist1 = 0;
   bool list2Empty = true;
   bool list3Empty = true;
   Node *cursor1 = headPtr1;
   Node *cursor2, *cursor3, *pre = 0;
   if(headPtr1 == 0)
     {
       Node *newNodePtr1 = new Node;
       Node *newNodePtr2 = new Node;
       Node *newNodePtr3 = new Node;
       newNodePtr1->data = -99;
       newNodePtr1->link = headPtr1;
       headPtr1 = newNodePtr1;
       newNodePtr2->data = -99;
       newNodePtr2->link = 0;
       headPtr2 = newNodePtr2;
       newNodePtr3->data = -99;
       newNodePtr3->link = 0;
       headPtr3 = newNodePtr3;
       return;
     }
   while(cursor1->link != 0)
     {
	if(cursor1->data == 6)
          {
	    cursor1 = cursor1->link;
            countlist1++;
          }
	if(cursor1->data >= 0 && cursor1->data <= 5)
	  {
	   if(list2Empty)
	     {
               headPtr2 = cursor1;
	       cursor2 = headPtr2;
	       cursor1 = cursor1->link;
	       list2Empty = false;
	     }
	    else
	     {
	        cursor2->link = cursor1;
		cursor1 = cursor1->link;
	     }
	   }
	 if(cursor1->data >= 7 && cursor1->data <= 9)
	   {
	     if(list3Empty)
	       {
		 headPtr3 = cursor1;
	         cursor3 = headPtr3;
                 cursor1 = cursor1->link;
	         list3Empty = false;
               }
              else
               {
                 cursor3->link = cursor1;
	         cursor1 = cursor1->link;
               }
            }
          if(cursor1->data < 0 || cursor1->data > 9)
            {
	      pre = cursor1;
              cursor1 = cursor1->link;

	      if(cursor1 == headPtr1)
	        headPtr1 = headPtr1->link;
	      else if(cursor1->link ==  0)
		delete pre;
	      else
	        pre->link = cursor1->link;
                delete pre;
             }
     }
   if(countlist1 == 0)
     {
       Node *newNodePtr1 = new Node;
       newNodePtr1->data = -99;
       newNodePtr1->link = 0;
       headPtr1 = newNodePtr1;
     }
   if(list2Empty)
     {
       Node *newNodePtr2 = new Node;
       newNodePtr2->data = -99;
       newNodePtr2->link = 0;
       headPtr2 = newNodePtr2;
     }
   if(list3Empty)
     {
       Node *newNodePtr3 = new Node;
       newNodePtr3->data = -99;
       newNodePtr3->link = 0;
       headPtr3 = newNodePtr3;
     }
 
} 
