# Implementing a Minix Character Device Driver

This driver operates a `poly list` which is a polymorphic list that can act as both a stack and a queue.

## Working of the list
The list implemented by the driver acts as an array of strings. The list can hold a maximum of 50 strings. The default (on start) behavior of the poly_list is as a queue. It can be converted to a stack and back only when the list is empty. This list supports all the push() and pop() operations of a stack and enqueue() and dequeue() operations of a queue. The driver also handles the underflow and overflow of the list. This driver is mounted on `/dev/poly_list` with a major/minor numbers 20 and 0.
