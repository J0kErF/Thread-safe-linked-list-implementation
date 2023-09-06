# (Concurrent List) Thread Safe linkedlist

A concurrent list implementation using pthread mutexes.

## Functions

* `create_list()`: Creates a new empty list.
* `delete_list()`: Deletes a list and frees all its memory.
* `insert_value()`: Inserts a new value into the list.
* `remove_value()`: Removes a value from the list.
* `print_list()`: Prints the contents of the list.
* `count_list()`: Counts the number of elements in the list that satisfy a predicate.

## Usage

To use the concurrent list, first create a new list with create_list(). Then, you can insert and remove values from the list using insert_value() and remove_value(). To print the contents of the list, use print_list(). To count the number of elements in the list that satisfy a predicate, use count_list().

Example
The following code creates a list, inserts some values into it, and then prints the contents of the list:

list* list = create_list();

insert_value(list, 10);
insert_value(list, 20);
insert_value(list, 30);

print_list(list);


This code will print the following output:

10 20 30

Thread Safety
The concurrent list is thread-safe. This means that multiple threads can safely access the list at the same time without corrupting the data.

License
The concurrent list is released under the MIT License.
