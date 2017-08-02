#include <stdio.h>
class HeapManagerC;
//Assignment
//2. Describe HeapC Class
////////////////////////////////////////////////////////////////////////////
// The Heap class manages a heap. It uses blocks, obtained by the user to allocate or free memory.
////////////////////////////////////////////////////////////////////////////
class HeapC
{
public:
    HeapC();
    ~HeapC(){};
    void                    init(char *name, void *startAddress, int heapSize, int id);
    int                     getID() {return mHeapID;}
    int                     getSize() {return mHeapSize;}
    char*                   getHeapName() {return mHeapName;};
    int                     getNumBlocks() {return mNumBlocks;};
    
    void                    unitTest();

    void*                   allocateBlock(int bytes,char *filename,int lineNumber,char* tag, int time, int alignment=4);
#ifdef _DEBUG
    void                    dumpHeapToFile(char *filename);
#endif

    void                    freeBlock(void *ptr);
    bool                    checkHeapConsistency();


//Assignment
//3. Describe memBlockT structure
////////////////////////////////////////////////////////////////////////////
// The memBlockT structure is a header like structure that each block will own, which contains information about the block itself and references to its adjacent blocks.
////////////////////////////////////////////////////////////////////////////
#define FREE_FLAG 0
    typedef struct 
    {
            void    *prevBlock;
            void    *nextBlock;

#ifdef _DEBUG
            char    *filename;
            char    *tag;
            int     lineNumber;
            int     time;
#endif
            int     flags;  //bit 0: free  //bits 1:8 Heap ID 
    }memBlockT;

    static memBlockT*       getMemblockFromPtr(void *ptr);
    static int              getHeapID(memBlockT *block);

private:

    int                     calcBlockSize(memBlockT *addr);
    void                    clearBlock(memBlockT *block);
    void                    combineBlocks(memBlockT *firstBlock, memBlockT *secondBlock);
    inline void             setFlag(int flagNumber,memBlockT *block) {block->flags |= (1<<flagNumber);};
    inline int              getFlag(int flagNumber,memBlockT *block) {return ((block->flags & (1<<flagNumber)));};
    inline void             clearFlag(int flagNumber,memBlockT *block) {block->flags &= 0xFFFFFFFF - (1<<flagNumber);};
    inline int              isFree(memBlockT *block) {return getFlag(FREE_FLAG,block);};
    inline void             makeFree(memBlockT *block) {setFlag(FREE_FLAG,block);};
    
    void                    setHeapID(memBlockT *block, int id);

    static int              powerOfTwo(int val);
    memBlockT*              findBeginningOfMemoryBlockFromPointer(void *ptr);

    void*                   allocateBlock(int bytes, int alignment=4);

    int                     getBlockSize();
    void                    blockConsistencyCheck(memBlockT *block);
    int                     getNumFreeBlocks();
    int                     getTotalFreeMemory();
    int                     getLargestFreeBlock();

    static int              getBlockOverhead();
    static int              getFrontBlockOverhead();
    static int              getRearBlockOverhead();
    static const int        NUM_GUARD_LONGS = 16;

#ifdef _DEBUG
    void                    outputString(char *stringOut,bool toFile, bool toTTY);
    void                    outputMemoryDumpHeader(bool toFile, bool toTTY);
    void                    outputMemoryDumpStats( bool toFile, bool toTTY);
    void                    outputMemoryBlock(memBlockT *block,bool toFile, bool toTTY);
    void                    outputMemoryBlocks(bool toFile, bool toTTY);
    bool                    checkGuardBytes(memBlockT *block);

#endif

    void                    initFirstBlock();
    char                    mHeapName[32];
    void                    *mHeapStart;
    int                     mHeapSize;
    int                     mHeapID;
    int                     mNumBlocks;
    memBlockT               *mFirstFreeBlock;
    memBlockT               *mLastFreeBlock;

#ifdef _DEBUG
    FILE                    *mOutputFile;
#endif
};
#define GUARD_VAL 0xFACEFACE