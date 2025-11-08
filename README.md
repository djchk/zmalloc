# zmalloc

A custom memory allocation header library implementing different allocation strategies in C++.
## Overview

zmalloc is a drop-in (or near drop-in) replacement for the standard `malloc` / `free` that allows you to experiment with custom memory-allocation strategies. It is written in C++ (with some C compatibility) and provides an extensible framework for allocation behaviour.

The repository currently contains:  
- `zmalloc.h` – header interface  
- `zmalloc.cpp` – implementation of the allocator backbone  
- `test.cpp` – example usage and basic tests  
- `Makefile` – build support  
- `outline.md` – design outline and notes  

## Design & Allocation Strategies
- Used `First_fit` techniue to allocate the first fit memory block as requested.
- Used `Best_fit` technique to allocate the best fitted memory block to reduce the internal fragmantetion.
- Future Plans: To integrate the `Buddy Allocator Algorithm`.

## Testing
The repository includes a basic test.cpp which demonstrates usage and may check for correct allocations and frees.
