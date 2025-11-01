### Custom malloc Implementation - dmalloc
- Block consists of two things --- 1. Header, 2. Data
- Thus we make the structure of the header like that --

```
Header {
    blocksize (to denote the entire block size),
    free (flag to represent if the block is free or not),
    marker (represent the state if the block is allocated from the memory pool, or it is allocated by the system os)
    *next (link to the next block)
}
```

Here we have the following functions & their workings are like ---
1. get_space_from_os(size) - this is the way we can get the memory block from the os 
2. first_fit_free_block_search(size) - returns that block if the block is available in the pool & also free.
3. append_block_to_pool(block) - we are going to append the block at the end of the mempool
4. zmalloc(size, args)
5. free(block)