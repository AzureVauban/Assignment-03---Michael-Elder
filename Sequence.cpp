// FILE: Sequence.cpp
// CLASS IMPLEMENTED: sequence (see sequence.h for documentation)
// INVARIANT for the sequence ADT:
//   1. The number of items in the sequence is in the member variable
//      used;
//   2. The actual items of the sequence are stored in a partially
//      filled array. The array is a dynamic array, pointed to by
//      the member variable data. For an empty sequence, we do not
//      care what is stored in any of data; for a non-empty sequence
//      the items in the sequence are stored in data[0] through
//      data[used-1], and we don't care what's in the rest of data.
//   3. The size of the dynamic array is in the member variable
//      capacity.
//   4. The index of the current item is in the member variable
//      current_index. If there is no valid current item, then
//      current_index will be set to the same number as used.
//      NOTE: Setting current_index to be the same as used to
//            indicate "no current item exists" is a good choice
//            for at least the following reasons:
//            (a) For a non-empty sequence, used is non-zero and
//                a current_index equal to used indexes an element
//                that is (just) outside the valid range. This
//                gives us a simple and useful way to indicate
//                whether the sequence has a current item or not:
//                a current_index in the valid range indicates
//                that there's a current item, and a current_index
//                outside the valid range indicates otherwise.
//            (b) The rule remains applicable for an empty sequence,
//                where used is zero: there can't be any current
//                item in an empty sequence, so we set current_index
//                to zero (= used), which is (sort of just) outside
//                the valid range (no index is valid in this case).
//            (c) It simplifies the logic for implementing the
//                advance function: when the precondition is met
//                (sequence has a current item), simply incrementing
//                the current_index takes care of fulfilling the
//                postcondition for the function for both of the two
//                possible scenarios (current item is and is not the
//                last item in the sequence).

#include <cassert>
#include "Sequence.h"
#include <iostream>
using namespace std;

namespace CS3358_SP2024
{
   // CONSTRUCTORS and DESTRUCTOR
   sequence::sequence(size_type initial_capacity) : used(0), current_index(0), capacity(initial_capacity)
                                                                                   sequence::sequence(size_type initial_capacity)
   {
      // check inital_capacity validity
      if (initial_capacity < 1) {this->capacity = 1;}
      // create nwe dynamic sequence array
      this->data = new value_type[this->capacity];
   }

   sequence::sequence(const sequence &source){
      this->data = new value_type[capacity];
      for (size_type index = 0; index < used;index++)
      {
         this->data[index] = source.data[capacity];
      }
   }
      sequence::~sequence()
      {
         delete[] this->data;
         data = nullptr;
      }

   // MODIFICATION MEMBER FUNCTIONS
   void sequence::resize(size_type new_capacity)
   {
      //check validity of new_capacity
      if (new_capacity < 1){new_capacity = 1;}
      if (new_capacity < this->used){this->capacity = this->used;}
      else{this->capacity = new_capacity;}
      //create new dynamic array
      value_type *temp_data = new value_type[capacity];
      //copy contents to new location
      for (size_type index = 0; index < used; ++index)
      {
         temp_data[index] = data[index];
      }
      //clean up (deallocate & move)
      delete[] data;
      data = temp_data;
   }

   void sequence::start()
   {
      //set current index according to 4th invariant 
      this->current_index = 0;
   }

   void sequence::advance()
   {
      //ensure pre-condition is met, if unmet terminate program
      //else continue sequence::advance()
      assert(is_item());
      this->current_index = current_index + 1;
   }

   void sequence::insert(const value_type& entry)
   {
      if (this->used == this->capacity){
         this->resize(size_type(1.25 * this->capacity) + 1);
      }

      if (!this->is_item()){
         //No current item, insert element at the beginning of the 
         //sequence or current_index == 0, starting from this->used+1 shift
         // items towards to accomdate for insertion entry
         this->sequence::start();
         for (size_type index = this->used + 1; index > current_index;--index)
         {
            this->data[index] = data[index - 1];
            this->used += 1;
         }
      } else {
         // current item, insert entry at current_index-1,
         //  starting from used+1 and onwards
         for (size_type index = used + 1; index > current_index; --index){this->data[index] = this->data[index - 1];}
         this->data[current_index] = entry;
         this->used += 1;
   
      }
   }
   
   

   void sequence::attach(const value_type& entry)
   {
      // check if resize of data array is needed
      // satisfy the resize rule via (current_capacity*1.25)+1
      if (this->used == this->capacity)
      {
         this->resize(size_type(1.25 * this->capacity) + 1);
      }

      if (!this->is_item())
      {
         // No current item, attach anetry at end of sequence
         // or current_index used. Make entry the current item
         this->data[this->current_index] = entry;
         this->used += 1;
      }
   }

   void sequence::remove_current()
   {
      //ensure pre-condition is met, if unmet terminate program
      //else continue function execution
      assert(this->is_item());

      //remove current and shift items leftward
      for (size_type index = current_index; index < this->used - 1; index++)
      {
         this->data[index] = this->data[index + 1];
      }
      //update after removing item
      this->used -= 1;
   }

   sequence& sequence::operator=(const sequence& source)
   {
      //self-assignment fail safe, if self 
      //assignment is ppresent return invoking object
      if (this == &source)
      {

      return *this;
      }
      // create temp data array to assign contents
      value_type *temp_data = new value_type[source.capacity];

      // Moved contents of rhs array to temp
      for (size_type index = 0; index < source.used; index++)
      {
         temp_data[index] = source.data[index];
      }

      // Deallocate old dynamic array.
      delete[] this->data;

      // Start assigning member variables from rhs.
      this->data = temp_data;
      this->capacity = source.capacity;
      this->used = source.used;
      this->current_index = source.current_index;

      return *this;
   }

   // CONSTANT MEMBER FUNCTIONS
   sequence::size_type sequence::size() const
   {
      //used is the same as the number of items in container instance
      return this->used;
   }

   bool sequence::is_item() const
   {
      //conditions for invalid item:
      // sequence is NOT empty, used == 0
      // current index is not the last item in the data array
      //aka : current_index == used
      return current_index != used;
   }

   sequence::value_type sequence::current() const
   {
      // ensure pre-condition is met, if unmet terminate program
      // else return current item of the sequence
      assert(this->is_item());
      return this->data[current_index];
   }
}

