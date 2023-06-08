# Memory-Allocator

A memory allocator that implements the core C functions - `malloc`, `calloc`, `realloc` and `free`

The implementations of `malloc`, `calloc`, `realloc` and `free` mainly rely on either the `sbrk/brk` or `mmap` system calls in a UNIX system (`VirtualAlloc` for Windows). The system call 

```c++
void *sbrk(intptr_t increment);
```
 moves the program break by an `increment` amount of bytes and returns either the previous or current address (depending on the internal implementation) of the breakpoint of the running process. 

### What is the program breakpoint? 
Short Answer - The boundary separating the heap’s mapped and unmapped memory regions is the program breakpoint.

Processes have their own virtual address space - an address space the OS makes available for them to use. This virtual address space is divided into several regions to hold the process’s stack, code, constants, globals, local variables, and heap. The program’s heap is what we are concerned about as that is where dynamic memory allocation is supported. 

The process's heap consists of both mapped and unmapped memory addresses. Mapped memory regions, unlike unmapped, contain addresses that have been mapped to physical memory. The translation of an unmapped memory address to a mapped one happens dynamically through the Memory Management Unit (MMU) of the system. 

The boundary separating the heap’s mapped and unmapped memory regions is the system break point.

As `malloc` is used to request some memory, the system breakpoint is moved to enlarge the mapped region - effectively allocating more memory dynamically.  

<img width="1387" alt="Allocator Implementation Diagrams (1)" src="https://github.com/JanatB/Memory-Allocator/assets/76413679/c06ec9db-67b0-4be0-aea4-b00b51e2525a">

> Image created on FigJam </br> 

</br>

```
Note: This project was done following a simple tutorial by Marwan Burelle (site is currently down) and is yet
to be throughly tested. I may update the project in the future to include testing and focus more on the optimization
of the functions. Aside from that, I've found it quite eye-opening how complicated concepts may seem before
diving deep and disecting them. Projects like this help make magic less magical and more understandable💡  
```
