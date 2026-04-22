#include "patches.h"

#include <ultra64.h>

extern u8 *gDecompressHeap;
extern void *malloc_game(s32 size);
extern void free_game(void *ptr);
extern void load_from_rom_to_addr(void* start, void* addr, s32 size);

// decompress.c

// We need to add rom addrs to the file array. Luckily, Hudson left us a spare 4 bytes in each file array element
// so we will use that to store the rom start.

/**
 * Decompress a given area and add it to the file array's ID pointer.
 */
RECOMP_PATCH void DecompressFile(u32 id, u32 rom_start, u32 rom_end) {
    u32 heap;
    u32 allocSize;
    u32 size;
    void* buf;

    // align the heap pointer's size.
    heap = (u32)gDecompressHeap;
    if (heap & 0xF) {
        // add 1 so the heap will point to the new blank area in the heap.
        heap = ALIGN16(heap + 1);
        gDecompressHeap = (void*)heap;
    }
    allocSize = rom_end - rom_start; // get size.
    buf = malloc_game(allocSize);         // alloc buf pointer.

    // first load the compressed bin to the buffer.
    load_from_rom_to_addr((void*)rom_start, buf, allocSize);

    // then decompress the bin to the heap.
    size = Decode(buf, gDecompressHeap);
    free_game(buf);                       // free buf, we dont need it anymore.
    gFileArray[id].ptr = gDecompressHeap; // add the decompressed file to the file array.

    // @recomp: add the rom offset to the file array.
    *(u32*)&gFileArray[id].pad4[0] = rom_start;

    gDecompressHeap += size;              // re-set the heap pointer to the new area.
}
