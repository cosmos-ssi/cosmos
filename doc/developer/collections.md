
# Collections

CosmOS provides collections in order to easy writing kernel code and to prevent creating bugs. The collections include iterators, and checks for out of range indices.


## Array

A simple in-memory array. 


## List

A list


## Ringbuffer

A ringbuffer

## String

A string

## DTable

Allows simulating sparse arrays, in a manner that does not provide all the space-efficiency of a sparse array but is nevertheless uses much less memory for large indices than would be needed for a normal array, while also involving fewer complications than a hash table.

Imagine, for example, an array of 64-bit pointers, and it becomes necessary to store a pointer at index 0x1122334455667788.  A flat array would be infeasible--over eight quintillion bytes of storage would be necessary just to hold the (possibly NULL) array contents leading up to that index!  A deep table, instead, uses a multidimensional array of pointers, at each level only allocating space on an as-needed basis.  For example, in a deep table, instead of addressing:

----table[0x1122334455667788]

you would address

----table[0x11][0x22][0x33][0x44][0x55][0x66][0x77][0x88]

At initialization, `table` would be a pointer to an allocated region of 256 pointers (on a 64-bit system, this would be 2 kilobytes).  These pointers would initially be set to NULL.  Whenever an item is added, the two most significant hex digits would be an index into the first level of the table.  If there are no other items in the table at that top-level index, another region of 256 pointers (two kilobytes) would be allocated and the element at the top-level index (in this case, table[0x11]) would be set to a pointer to that region, again set to null.  This would then be repeated for the next two most-significant hex digits for the second level of the table, and on down the line.  The result is that inserting an element at any arbitrary 64-bit index will add, at most, 16 kilobytes of overhead.  This is very inefficient for arrays that will only ever have small indices (though the inefficiency goes down as the array fills up even then) but does allow for arbitrary indices throughout the entire 64-bit range of numbers.  It also does not require potentially time-consuming krealloc() calls (which may involve moving the entire array to a different location, a process that becomes slower and slower as the array grows) every time the array needs to increase in size.  At any level, if all entries ever become unused then the memory is freed.

A deep table is best for situations where it may be desirable to avoid reusing indices or searching for unused indices, but elements may get removed as well.  Examples of this is a process table--once a process terminates, it will be removed from the process table, but continuing to monotonically increase process IDs might be desirable to avoid the computational expense of having to search for the lowest unused ID.

