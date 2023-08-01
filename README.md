# Contiguous Memory Allocation

This program simulates contiguous memory allocation by managing a contiguous
region of memory of size `MAX` where addresses may range from `0` ... `MAX − 1`.

+ The program supports six different requests:
  + `1.` Allocate `N` bytes of memory for a process using one of 3 allocation
     algorithms
  + `2.` Free all allocations for a given process
  + `3.` Show the status of the memory pool - all allocated and free blocks
  + `4.` Read a script - a sequence of commands from a file, and execute them
  + `5.` Compact the allocations, making them into one contiguous block
  + `6.` Exit - terminate the program

## Getting started

To compile the program:

+ Clone repo to your local machine.
+ Navigate to the project directory: `contiguous_allocator/`
+ Run `makefile`.

Use the following command to run the program and replace **`<memory size>`**
with the size of the memory pool you want the program to manage

+ **`./allocator.out <memory size>`**

## Allocating memory

Memory is allocated using one of three approaches highlighted in Section `9.2.2`
of OSC 10th edition, chapter 9.

+ **`First-Fit` -** Allocate the first hole that is big enough. Searching can
                    start either at the beginning of the set of holes or at the
                    location where the previous first-fit search ended. We can
                    stop searching as soon as we find a free hole that is large
                    enough.

+ **`Best-Fit` -** Allocate the smallest hole that is big enough. We must search
                   the entire list, unless the list is ordered by size. This
                   strategy produces the smallest leftover hole.

+ **`Worst-Fit` -** Allocate the largest hole. Again, we must search the entire
                    list, unless it is sorted by size. This strategy produces
                    the largest leftover hole, which may be more useful than the
                    smaller leftover hole from a best-fit approach.

## Commands

Commands are case sensitive. When entering a command that takes one or more
args, separate the command and its args with one or more spaces.

### **Allocate**: **`A`** **<`name`>** **<`size`>** **<`algo`>**

+ Allocate **<`size`>** bytes for process **<`name`>** using algorithm
   **<`algo`>**. **<`algo`>** can be any of the of the following:
  + **`F`** - for First-Fit
  + **`B`** - for Best-Fit
  + **`W`** - for Worst-Fit

   Examples:
  + `allocator> A P 20 F`   // Allocate 20 bytes for process P using First-Fit
  + `allocator> A X 14 B`   // Allocate 14 bytes for process X using Best-Fit

---

### **Free**: **`F`** **<`name`>**

+ Free all allocation owned by process **<`name`>**

   Examples:
  + `allocator> F P`   // Free all allocations owned by process P
  + `allocator> F X`   // Free all allocations owned by process X

---

### **Show**: **`S`** or **`STAT`**

+ Show the state of the memory pool.
  + **`S`** print all blocks in memory pool.
  + **`STAT`** gives a list of all unallocated and allocated spaces.

---

### **Read**: **`R`** **<`file`>**

+ Read the script in the file called **<`file`>** and execute each command
   one-by-one.
+ The script file should be formatted as a list of commands to execute.
   Each line should contain one command. For example scripts, look at the
   files in `scripts/` folder.

   Examples:
  + `allocator> R scripts/best.txt`  // Read file `scripts/best.txt` and executes the commands
  + `allocator> R scripts/worst.txt` // Read file `scripts/worst.txt` and executes the commands

---

### **Compact**: **`C`**

+ Compact the memory pool, sliding all allocations to lower addresses so they
   become one contiguous block. After compaction, all free space lies to the
   right as one contiguous block.

---

### **Exit**: **`E`**

+ Terminate program

---

### **Clear**: **`CLEAR`**

+ clear cli screen

---
