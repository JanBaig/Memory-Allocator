# Memory-Allocator

A Memory Allocator that implements the core C functions - `malloc`, `calloc` and `free`

The implementations of `malloc`, `calloc` and `free` mainly rely on either the `sbrk/brk` or `mmap` system calls in a UNIX system (`VirtualAlloc` for Windows). The system call `void *sbrk(intptr_t increment);` moves the program break by *increment* amount of bytes. 

What is the program break? Processes have their own virtual address space - an address space the OS makes available for them to use. This virtual address space consists of both mapped and unmapped memory addresses. Mapped memory regions, unlike unmapped, are addresses that have been mapped to physical memory. The translation of an unmapped memory address to a mapped one happens dynamically through the Memory Management Unit (MMU) of the system. 

This virtual address space is divided into several regions to hold the process’s stack, code, constants, globals, local variables, and heap. The program’s heap is what we are concerned about as that is where dynamic memory allocation is supported. 

The boundary separating the heap’s mapped and unmapped memory regions is the system break point. As malloc is used to request some memory, the system break point is moved to enlarge the mapped region - effectively allocating more memory dynamically.  

<img width="1105" alt="Allocator Implementation Diagrams" src="https://github.com/JanatB/Memory-Allocator/assets/76413679/fea4aa43-dc79-4f3d-ac82-eeda36d7aba8">

> Image created on FigJam
