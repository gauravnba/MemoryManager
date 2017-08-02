#include <stdio.h>

#include "heapmanager.h"
#include "heap.h"
#include "string.h"
#include "..\common\macros.h"
#include "random.h"

HeapC::HeapC()
{
};



void HeapC::init(char *name,void *startAddress,int sizeInBytes, int id)
{
    //init the heap parameters from the passed in arguments
    strncpy_s(mHeapName,name,32);
    mHeapStart = startAddress;
    mHeapSize = sizeInBytes;
    mHeapID = id;

    //init one large free block to be the size of the heap
    initFirstBlock();

    //we have one free block
    mNumBlocks = 1;
}


void HeapC::unitTest()
{
	//Assignment
	//0.  Explain what each unit test tests
	
	// 
    char *string1 = (char *)allocateBlock(13,4);
    strcpy_s(string1,13,"Hello World!");
    freeBlock(string1);
    
	
	char *randomNumbers = (char *)allocateBlock(100,4);
    char *randomNumbers1 = randomNumbers;
    for (int i=0;i<100;++i)
    {
        *randomNumbers++ = (char)getRangedRandom(0,255);
    }
    freeBlock(randomNumbers1);
    

	
    char *tempBlock1,*tempBlock2,*tempBlock3;

    checkHeapConsistency();
    int size = calcBlockSize(mFirstFreeBlock);
    tempBlock1 = (char *)allocateBlock(500,4);
    checkHeapConsistency();
    size = calcBlockSize(mFirstFreeBlock);
    assert(size < getSize()-500);  //some overhead anyway...
    assert(getNumBlocks() == 2);

    tempBlock2 = (char *)allocateBlock(500,4);
    checkHeapConsistency();
    size = calcBlockSize(mFirstFreeBlock);
    assert(size < getSize()-1000);  //some overhead anyway...
    assert(getNumBlocks() == 3);

    freeBlock(tempBlock2);
    checkHeapConsistency();
    assert(getNumBlocks() == 2);

    freeBlock(tempBlock1);
    checkHeapConsistency();
    assert(getNumBlocks() == 1);


    tempBlock1 = (char *)allocateBlock(500,4);
    tempBlock2 = (char *)allocateBlock(500,4);
    tempBlock3 = (char *)allocateBlock(500,4);
    assert(getNumBlocks() == 4);

    freeBlock(tempBlock1);
    checkHeapConsistency();
    assert(getNumBlocks() == 4);
    freeBlock(tempBlock3);
    checkHeapConsistency();
    assert(getNumBlocks() == 3);
    freeBlock(tempBlock2);
    checkHeapConsistency();
    assert(getNumBlocks() == 1);


    int freeMemory = getLargestFreeBlock();
    assert(getSize()== freeMemory);

    tempBlock1 = (char *)allocateBlock(500,4);
    freeMemory = getLargestFreeBlock();
    assert(getSize()-(500+getBlockOverhead()) == freeMemory);

    tempBlock2 = (char *)allocateBlock(500,4);
    tempBlock3 = (char *)allocateBlock(500,4);
    freeMemory = getLargestFreeBlock();
    assert(getSize()-(1500+(3*getBlockOverhead())) == freeMemory);

    freeBlock(tempBlock1);
    freeMemory = getLargestFreeBlock();
    assert(getSize()-(1500+(3*getBlockOverhead())) == freeMemory);
    freeMemory = getTotalFreeMemory();
    assert(getSize()-(1000+(2*getBlockOverhead())) == freeMemory);
    assert(getNumFreeBlocks() == 2);

    freeBlock(tempBlock2);
    freeMemory = getLargestFreeBlock();
    assert(getSize()-(1500+(3*getBlockOverhead())) == freeMemory);
    freeMemory = getTotalFreeMemory();
    assert(getSize()-(500+(getBlockOverhead())) == freeMemory);
    assert(getNumFreeBlocks() == 2);

    freeBlock(tempBlock3);
    freeMemory = getLargestFreeBlock();
    assert(getSize() == freeMemory);
    freeMemory = getTotalFreeMemory();
    assert(getSize() == freeMemory);
    assert(getNumFreeBlocks() == 1);

#ifdef _DEBUG
    char *tempBlock4,*tempBlock5,*tempBlock6;


    tempBlock1 = (char *)allocateBlock(500,__FILE__,__LINE__,"unit test 1",0,4);
    tempBlock2 = (char *)allocateBlock(500,__FILE__,__LINE__,"unit test 2",0,4);
    tempBlock3 = (char *)allocateBlock(500,__FILE__,__LINE__,"unit test 3",0,4);
    tempBlock4 = (char *)allocateBlock(500,__FILE__,__LINE__,"unit test 4",0,4);
    tempBlock5 = (char *)allocateBlock(500,__FILE__,__LINE__,"unit test 5",0,4);
    tempBlock6 = (char *)allocateBlock(500,__FILE__,__LINE__,"unit test 6",0,4);
    assert(getNumFreeBlocks() == 1);
    dumpHeapToFile("UnitTestDump0");
    
    freeBlock(tempBlock2);
    freeBlock(tempBlock3);
    tempBlock2 = tempBlock3 = NULL;
    assert(getNumFreeBlocks() == 2);
    dumpHeapToFile("UnitTestDump1");

    tempBlock2 = (char *)allocateBlock(800,__FILE__,__LINE__,"unit test 7",0,4);
    dumpHeapToFile("UnitTestDump1a");
    freeBlock(tempBlock4);
    freeBlock(tempBlock5);
    dumpHeapToFile("UnitTestDump1b");
    tempBlock4 = tempBlock5 = NULL;

    tempBlock3 = (char *)allocateBlock(950,__FILE__,__LINE__,"unit test 8",0,4);
    dumpHeapToFile("UnitTestDump2");
    freeBlock(tempBlock1);
    dumpHeapToFile("UnitTestDump2a");
    freeBlock(tempBlock6);
    dumpHeapToFile("UnitTestDump3");
    freeBlock(tempBlock2);
    dumpHeapToFile("UnitTestDump4");
    freeBlock(tempBlock3);
    dumpHeapToFile("UnitTestDump5");

    assert(getSize() == freeMemory);
    assert(getNumFreeBlocks() == 1);



    tempBlock1 = (char *)allocateBlock(500,__FILE__,__LINE__,"unit test 1",0,4);
    tempBlock2 = (char *)allocateBlock(500,__FILE__,__LINE__,"unit test 2",0,4);
    tempBlock3 = (char *)allocateBlock(500,__FILE__,__LINE__,"unit test allocation 3",0,4);
    dumpHeapToFile("UnitTestDump1");
    
    freeBlock(tempBlock2);
    dumpHeapToFile("UnitTestDump2");
    freeBlock(tempBlock1);
    dumpHeapToFile("UnitTestDump3");
    freeBlock(tempBlock3);
    dumpHeapToFile("UnitTestDump4");

//allocate a thousand pointers, then dealloc them
    char *tempPtrs[1000];
    for(int i=0;i<1000;++i)
    {
        tempPtrs[i] = (char *)allocateBlock(getRangedRandom(32,5000),__FILE__,__LINE__,"random alloc",0,4);
    }
    dumpHeapToFile("UnitTestDump5");
    for(int i=0;i<1000;++i)
    {
        int blockID = getRangedRandom(0,999);
        if(tempPtrs[blockID]!= NULL)
        {
            freeBlock(tempPtrs[blockID]);
            tempPtrs[blockID]=NULL;
        }
    }
    dumpHeapToFile("UnitTestDump6");
    for(int i=0;i<1000;++i)
    {
        if(tempPtrs[i]!= NULL)
        {
            freeBlock(tempPtrs[i]);
            tempPtrs[i]=NULL;
        }
    }
    dumpHeapToFile("UnitTestDump7");

    for(int i=0;i<10000;++i)
    {
        int blockID = getRangedRandom(0,999);

        if(tempPtrs[blockID]!= NULL)
        {
            freeBlock(tempPtrs[blockID]);
            tempPtrs[blockID]=NULL;
        }
        else
        {
            tempPtrs[blockID] = (char *)allocateBlock(getRangedRandom(32,5000),__FILE__,__LINE__,"alloc",0,4);
        }
    }
    dumpHeapToFile("UnitTestDump8");
    for(int i=0;i<1000;++i)
    {
        if(tempPtrs[i]!= NULL)
        {
            freeBlock(tempPtrs[i]);
            tempPtrs[i]=NULL;
        }
    }
    dumpHeapToFile("UnitTestDump9");


#endif

}

//Assignment
//4. Describe function
////////////////////////////////////////////////////////////////////////////
// Get the number of blocks in the application that are marked free.
////////////////////////////////////////////////////////////////////////////
int HeapC::getNumFreeBlocks()
{
    int count = 0;
    memBlockT *addr = (memBlockT *)mHeapStart;
    
    do
    {
        if(isFree(addr))
        {
            ++count;    
        }
        addr = (memBlockT *)addr->nextBlock;
    } while(addr != NULL);
    
    return count;
}


//Assignment
//6. Describe function
////////////////////////////////////////////////////////////////////////////
// Get the amount of free memory in the heap in bytes.
////////////////////////////////////////////////////////////////////////////
int HeapC::getTotalFreeMemory()
{
    int bytes = 0;
    memBlockT *addr = (memBlockT *)mHeapStart;
    
    do
    {
        if(isFree(addr))
        {
            bytes += calcBlockSize(addr);    
        }
        addr = (memBlockT *)addr->nextBlock;
    } while(addr != NULL);
    
    return bytes;
}
//Assignment
//7. Describe function
////////////////////////////////////////////////////////////////////////////
// Iterate through all the free blocks and returns the largest one that is free.
////////////////////////////////////////////////////////////////////////////
int HeapC::getLargestFreeBlock()
{
    int largestBlock = 0;
    memBlockT *addr = (memBlockT *)mHeapStart;
    
    do
    {
        if(isFree(addr))
        {
            if(calcBlockSize(addr) > largestBlock)
            {
                largestBlock = calcBlockSize(addr);
            }
        }
        addr = (memBlockT *)addr->nextBlock;
    } while(addr != NULL);
    
    return largestBlock;    
}

//Assignment
//8. Describe function - when is it called and why?
////////////////////////////////////////////////////////////////////////////
// Only called in the init of the heap, because this initializes the first block in the heap ie it allocates the memory for the full heap.
////////////////////////////////////////////////////////////////////////////
void HeapC::initFirstBlock()
{
//Assignment
//9. Describe line
	////////////////////////////////////////////////////////////////////////////
	// Set the start of the heap as the first block.
	////////////////////////////////////////////////////////////////////////////
    memBlockT *addr = (memBlockT *)mHeapStart;
    
//Assignment
//10. Describe 2 lines
	////////////////////////////////////////////////////////////////////////////
	// Set the flag of the first block to free.
	////////////////////////////////////////////////////////////////////////////
    addr->flags = NULL;
    this->setFlag(FREE_FLAG,addr);
    
//Assignment
//11. Describe 2 lines
	////////////////////////////////////////////////////////////////////////////
	// The nextBlock and prevBlock point to nothing, hence initialize as null.
	////////////////////////////////////////////////////////////////////////////
    addr->nextBlock = NULL;
    addr->prevBlock = NULL;
#if _DEBUG
    addr->filename = NULL;
    addr->lineNumber = NULL;
    addr->time = NULL;
    addr->tag = NULL;
#endif
//Assignment
//12. Describe 2 lines
	////////////////////////////////////////////////////////////////////////////
	// Since only the first block exists on the heap, and it is free, both the first and last free blocks should point to the first block.
	////////////////////////////////////////////////////////////////////////////
    mFirstFreeBlock = addr;
    mLastFreeBlock = addr;
}

//Assignment
//12. Describe function
////////////////////////////////////////////////////////////////////////////
// Checks if the number of blocks on the heap and the cached members of the heap are consistent.
////////////////////////////////////////////////////////////////////////////
bool HeapC::checkHeapConsistency()
{
    memBlockT *addr = (memBlockT *)mHeapStart;
    int count = 1;
    void *prev = NULL;

    while(addr->nextBlock != NULL)
    {
        count++;
        assert(prev == addr->prevBlock);
        prev = addr;
        addr = (memBlockT *)addr->nextBlock;
    }
	volatile int count1 = count;
    assert(count == mNumBlocks);

    return true;
}
//Assignment
//13. Describe Function
////////////////////////////////////////////////////////////////////////////
// Given the pointer 'ptr', find the location of the start of the block, considering the overhead.
////////////////////////////////////////////////////////////////////////////
HeapC::memBlockT *HeapC::findBeginningOfMemoryBlockFromPointer(void *ptr)
{
//Assignment
//14. Explain 2 Lines
	////////////////////////////////////////////////////////////////////////////
	// Cast 'ptr' as a long, so that it can be decremented.
	////////////////////////////////////////////////////////////////////////////
    long *tempPtr = (long *)ptr;
    tempPtr--;

//Assignment
//15. Explain loop
	////////////////////////////////////////////////////////////////////////////
	// If 'tempPtr' doesn't point to the start of the block, move to start of the block.
	////////////////////////////////////////////////////////////////////////////
    while(*tempPtr==0)
    {
        tempPtr--;
    }
    tempPtr++;

//Assignment
//16. Explain line
	////////////////////////////////////////////////////////////////////////////
	// Get the pointer to the beginning of the block, by moving it to the start of the blockOverhead.
	////////////////////////////////////////////////////////////////////////////
    memBlockT *retPtr = (memBlockT *)((int)tempPtr - getFrontBlockOverhead());

    return retPtr;
}

#ifdef _DEBUG
//Assignment
//17. Why debug version?
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The debug version of the allocateBlock writes additional debug info to debug file. 
// Since we're running the program in debug mode, we need the debug info. In release mode, the file will not be written to.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void *HeapC::allocateBlock(int bytes,char *filename,int lineNumber,char *tag,int time,int alignment)
{
    void *thisBlock = allocateBlock(bytes, alignment);


    memBlockT *thisMemBlock = findBeginningOfMemoryBlockFromPointer(thisBlock);
    thisMemBlock->tag = tag;
    thisMemBlock->filename = filename;
    thisMemBlock->lineNumber = lineNumber;
    thisMemBlock->time = time;
    return thisBlock;
}
#else
//standard version of allocate does not do debug info
void *HeapC::allocateBlock(int bytes,char *filename,int lineNumber,char *tag,int time, int alignment)
{
    void *thisBlock = allocateBlock(bytes, alignment);
    return thisBlock;
}
#endif


int HeapC::powerOfTwo(int val)
{
    //make sure you're asking for a power of two
    assert(
        (val==(1<<0)) ||
        (val==(1<<1)) ||
        (val==(1<<2)) ||
        (val==(1<<3)) ||
        (val==(1<<4)) ||
        (val==(1<<5)) ||
        (val==(1<<6)) ||
        (val==(1<<7)) ||
        (val==(1<<8)) ||
        (val==(1<<9)) ||
        (val==(1<<10)) ||
        (val==(1<<11)) ||
        (val==(1<<12)) ||
        (val==(1<<13)) ||
        (val==(1<<14)) ||
        (val==(1<<15))
        );

    return val;
}
//Assignment
//18.  Describe function
/////////////////////////////////////////////////////////////////////////////
// Allocates a block of size 'bytes' in this heap, aligned along 'alignment'.
/////////////////////////////////////////////////////////////////////////////
void *HeapC::allocateBlock(int bytes, int alignment)
{
    bytes = (bytes+3)&(0xFFFFFFFF-3);   //always ask for a multiple of 4 bytes just to simplify life
    
    bool blockFound = false;            //init to false
    void *retBlock;

//Assignment
//19.  Describe computation
	////////////////////////////////////////////////////////////////////////////
	// Computes the offset required to align the block.
	// The alignment of bits has to be a power of two.
	////////////////////////////////////////////////////////////////////////////
    int alignmentOffset;  //how many more bytes to ask for in case we need to align
    if(alignment==4)
        alignmentOffset = 0;
    else
        alignmentOffset = powerOfTwo(alignment);
    
//Assignment
//20.  Describe line
	////////////////////////////////////////////////////////////////////////////
	// The actual number of bytes that will be allocated on the heap.
	// Calculated by added the alignmentOffset with the Maximum overhead we are allowing per block.
	////////////////////////////////////////////////////////////////////////////
    int actualBytesNeeded = bytes  + getBlockOverhead() + alignmentOffset;

//Assignment
//21.  Describe line
	////////////////////////////////////////////////////////////////////////////
	// Create memBlockT pointer 'addr' that points to the first free address in the heap.
	// The oldNextBlock is a memBlockT pointer that will be a reference to the next possible block.
	////////////////////////////////////////////////////////////////////////////
    memBlockT *addr = mFirstFreeBlock;
    memBlockT *oldNextBlock;

//Assignment
//22.  Describe loop
	////////////////////////////////////////////////////////////////////////////
	// Loop till a valid block is found.
	////////////////////////////////////////////////////////////////////////////
    while(!blockFound)
    {
//Assignment
//23.  Describe line
		////////////////////////////////////////////////////////////////////////////
		// Check if addr is a free block in the heap.
		////////////////////////////////////////////////////////////////////////////
        if(getFlag(FREE_FLAG,addr))
        {
//Assignment
//24.  Describe compare
			////////////////////////////////////////////////////////////////////////////
			// Check if the 'addr' block size is bigger than the amount of space needed to be allocated.
			////////////////////////////////////////////////////////////////////////////
            if(calcBlockSize(addr) > (actualBytesNeeded + getBlockOverhead()))
            {
                //found a block big enough!
                blockFound = true;

                //at this point you could compute how big the block really needed to be
                //by looking at the start address and alignment and reducing the size
//Assignment
//25. Why would this be a bad thing to do?
				////////////////////////////////////////////////////////////////////////////
				// It would be a bad idea to reduce the block size at this point, because the blockOverhead needed for the block is not yet applied to addr.
				////////////////////////////////////////////////////////////////////////////

//Assignment
//26.  Describe line
				////////////////////////////////////////////////////////////////////////////
				// Since we found a block big enough, assign 'retBlock' the memory address to return as 'addr'.
				////////////////////////////////////////////////////////////////////////////
                retBlock = addr;
//Assignment
//27.  Describe line
				////////////////////////////////////////////////////////////////////////////
				// 'oldNextBlock' is assigned as the block after 'addr'
				////////////////////////////////////////////////////////////////////////////
                oldNextBlock = (memBlockT *)addr->nextBlock;
//Assignment
//28.  Describe line
				////////////////////////////////////////////////////////////////////////////
				// The address that 'nextBlock' is pointing to, is moved to the end of the current block.
				////////////////////////////////////////////////////////////////////////////
                addr->nextBlock = (void *)((int)addr + (int)actualBytesNeeded);
//Assignment
//29.  Describe line
				////////////////////////////////////////////////////////////////////////////
				// Clear the flag of the 'addr' from garbage to free.
				////////////////////////////////////////////////////////////////////////////
                clearFlag(FREE_FLAG,addr);
                //set the heapID
                setHeapID(addr,mHeapID);
                //assert that this wasn't the last block and that it pointed somewhere past or exactly the same as the new next block
                assert((oldNextBlock == NULL) || (int)oldNextBlock >= (int)addr->nextBlock);

                if((int)oldNextBlock != (int)addr->nextBlock)
                {
                    //move to newly created block
                    addr = (memBlockT *)addr->nextBlock;
                    //set previous block to our new block
                    addr->prevBlock = (memBlockT *)retBlock;
                    //set the next block to the old next block
                    addr->nextBlock = oldNextBlock;
                    //call it free
                    setFlag(FREE_FLAG,addr);
                    //if this was the first free block, set the new first free block
                    if(mFirstFreeBlock == retBlock)
                    {
                        mFirstFreeBlock = addr;
                    }
                    //if this wasn't the last block, set the prev block of the next block to be the newly created block
                    if(oldNextBlock!=NULL)
                    {
//Assignment - comment in and what should it = ?                        oldNextBlock->prevBlock = ????;
////////////////////////////////////////////////////////////////////////////
//The prev block of the next block should be the newly created block.
////////////////////////////////////////////////////////////////////////////
						oldNextBlock->prevBlock = addr;
                    }
					//Assignment
					// Increment the number of blocks in the heap.
					++mNumBlocks;
                }
            }
        }
        //go to next block
        addr = (memBlockT *)addr->nextBlock;

        //check if we're at end of list - if so panic
		//Assignment
		//comment 2 lines below in and fill in conditions
        //if((??) && ??)
        //    assert(0); //no large enough blocks

		////////////////////////////////////////////////////////////////////////////
		// If reached the end of the heap and the block was still not found, crash.
		////////////////////////////////////////////////////////////////////////////
		if ((addr == NULL) && !blockFound)
		{
			assert(0);
		}
    }
//Assignment
//30.  Why isn't the retVal just the address of the block?
	////////////////////////////////////////////////////////////////////////////
	// The return value needs to be the usable address, where the user can store the data that they need to.
	////////////////////////////////////////////////////////////////////////////
    int returnPointerVal = ((int)retBlock+getFrontBlockOverhead());
    //setup for clearing memory for alignment
    long *origPtr = (long *)returnPointerVal;
    //check if alignment is necessary, if so push pointer until it's aligned properly
    //and backfill with zeros for freeing
    if(alignmentOffset!=0)
    {
        returnPointerVal+=(alignmentOffset-1);
        returnPointerVal = returnPointerVal & (0xFFFFFFFF-alignmentOffset+1);
        
        long *newPtrVal = (long *)returnPointerVal;
       
        while(origPtr != newPtrVal)
        {
            *origPtr++=0x00000000;  //clear memblock before pointer start so we can free it by searching backwards
        }
    }

    //if this is debug, set the guard longs at the end of the block to a known value
    //avoid zero since it's common to overwrite with zero
#ifdef _DEBUG
    memBlockT *tempBlock = (memBlockT *)retBlock;
    tempBlock = (memBlockT*)tempBlock->nextBlock;

    long *clearPtr =(long *)tempBlock;
    clearPtr-=NUM_GUARD_LONGS;
    for(int i=0;i < NUM_GUARD_LONGS;++i)
    {
        *clearPtr++ = GUARD_VAL;
    }
#endif

    return ((void*)(returnPointerVal));
}

void HeapC::freeBlock(void *ptr)
{
//Assignment
//31.  Describe line
	////////////////////////////////////////////////////////////////////////////
	// Gets the actual beginning of the block, accounting for the blockOffset and assigns to 'thisBlock'
	////////////////////////////////////////////////////////////////////////////
    memBlockT *thisBlock = (memBlockT *)((int)findBeginningOfMemoryBlockFromPointer(ptr));
//Assignment
//32.  Describe what it would mean if this assert fired...
	////////////////////////////////////////////////////////////////////////////
	// If this assert fired, it means the user tried to free an already free block.
	////////////////////////////////////////////////////////////////////////////
    assert(!isFree(thisBlock));
    //check consistency ? not doing anything right now but wanted to have a placeholder in case
    blockConsistencyCheck(thisBlock);

#if _DEBUG
    //check the guard bytes to make sure they haven't changed
    if(!checkGuardBytes(thisBlock))
        assert(0);  //memory overrun
#endif
    //make the block free
    makeFree(thisBlock);
//Assignment
//33.  Describe compare
	////////////////////////////////////////////////////////////////////////////
	// Check if 'mFirstFreeBlock' is ahead of thisBlock. If so, assign it to this block.
	////////////////////////////////////////////////////////////////////////////
    if(thisBlock < mFirstFreeBlock)
    {
        mFirstFreeBlock = thisBlock;
    }

//Assignment
//34.  Describe following 8 lines
	////////////////////////////////////////////////////////////////////////////
	// If the nextBlock is also free, combine the two free blocks into a single free block.
	////////////////////////////////////////////////////////////////////////////
    memBlockT *nextBlock = (memBlockT *)thisBlock->nextBlock;
    if(nextBlock != NULL)
    {
        if(isFree(nextBlock))
        {
            combineBlocks(thisBlock,nextBlock);
        }
    }
//Assignment
//35.  Describe following 11 lines
	////////////////////////////////////////////////////////////////////////////
	// If the prevBlock is also free, combine the two free blocks into a single free block, and move the mFirstFreeBlock to point to the start of prevBlock.
	////////////////////////////////////////////////////////////////////////////
    memBlockT *prevBlock = (memBlockT *)thisBlock->prevBlock;
    if(prevBlock != NULL)
    {
        if(isFree(prevBlock))
        {
            combineBlocks(prevBlock,thisBlock);
            if(prevBlock < mFirstFreeBlock)
            {
                mFirstFreeBlock = prevBlock;
            }
        }
    }
}
#if _DEBUG
//check to see that the guard bytes haven't been modified
bool HeapC::checkGuardBytes(memBlockT *block)
{
    long *checkAddr;
    //calculate address we should start checking bytes
    if(block->nextBlock == NULL)
    {
        checkAddr = (long *)((int)mHeapStart+mHeapSize);
    }
    else
    {
        checkAddr = (long *)block->nextBlock;
    }
    //check them all
    checkAddr -= NUM_GUARD_LONGS;
    for(int i=0;i < NUM_GUARD_LONGS;++i)
    {
        if(*checkAddr++ != GUARD_VAL)
            return false;
    }
    return true;
}
#endif

void HeapC::blockConsistencyCheck(memBlockT *block)
{
    //nothing...yet
    //non-debug consistency checking if needed
}
int HeapC::getHeapID(memBlockT *memBlock)
{
    //extract the heap ID info from the memory block
    int id;
    id = memBlock->flags;
    id >>= 1;
    id &= 0xFF;
    
    return id;
}
void HeapC::setHeapID(memBlockT *memBlock, int id)
{
    //set the heap ID in the appropriate bits
    assert(id >= 0 && id <= 255);

    id <<= 1;

    memBlock->flags &= 0xFFFFF801;  //clear heap bits
    memBlock->flags |= id;
}


HeapC::memBlockT* HeapC::getMemblockFromPtr(void *ptr)
{
    //work back thru nulls until you get to non-null data
    //then jump from end of memblock to beginning
    memBlockT *retVal;
    long* tempPtr = (long *)ptr;
    if(*(tempPtr-1)==0x00000000)
    {
        tempPtr--;
        while(*tempPtr==0x00000000)
            tempPtr--;

        tempPtr++;

        retVal = (memBlockT *)((int)tempPtr-getFrontBlockOverhead());
    }
    else
    {
        retVal = (memBlockT *) ( (int)ptr - getFrontBlockOverhead());
    }
    return retVal;
}
void HeapC::clearBlock(memBlockT *block)
{
    //clear the block with known data - for debugging purposes 
    void *addr = (void *)((int)block + getFrontBlockOverhead());
    void *endAddr;
    if(block->nextBlock == NULL)
    {
        endAddr = (void *)((int)mHeapStart + mHeapSize);
    }
    else
    {
        endAddr = block->nextBlock;
    }
    unsigned char *temp = (unsigned char *)addr;

    while(temp != (unsigned char *)endAddr)
    {
        *temp++ = 0xEA;
    }
}

//Assignment
//36. Describe function
////////////////////////////////////////////////////////////////////////////
// Combines two free, contiguous blocks into a single free block.
////////////////////////////////////////////////////////////////////////////
void HeapC::combineBlocks(memBlockT *firstBlock,memBlockT *secondBlock)
{

//Assignment
//37. Describe asserts
	////////////////////////////////////////////////////////////////////////////
	// Check that the 'firstBlock' is located before the 'secondBlock' and that they are both free, contiguous blocks.
	////////////////////////////////////////////////////////////////////////////
    assert((int)firstBlock < (int)secondBlock);
    assert(firstBlock->nextBlock == secondBlock);
    assert(isFree(firstBlock) && isFree(secondBlock));
//Assignment
//38. Describe line
	////////////////////////////////////////////////////////////////////////////
	// Set the 'nextBlock' of first block, the 'nextBlock' of second block, because the 'nextBlock' of the combined block will point to that one.
	////////////////////////////////////////////////////////////////////////////
    firstBlock->nextBlock = secondBlock->nextBlock;
    if(secondBlock->nextBlock!=NULL)
    {
        //if not, set the previous block of the next block to the first block
        ((memBlockT *)secondBlock->nextBlock)->prevBlock = firstBlock;
    }
    //clear out the newly created block
    clearBlock(firstBlock);
//Assignment
	--mNumBlocks;
}

int HeapC::calcBlockSize(memBlockT *addr)
{
    //figure out the size of a block by looking at
    //next address or heap size if it's the end
    int retSize, endAddress;
    if(addr->nextBlock == NULL)
    {
        endAddress = (int)mHeapStart + (int)mHeapSize;
        retSize = endAddress - (int)addr;
    }
    else
    {
        retSize = (int)addr->nextBlock - (int)addr;
    }
    return retSize;
}
int HeapC::getBlockOverhead()
{
    //how many bytes of overhead are we allowing per block
    return getFrontBlockOverhead()+getRearBlockOverhead();
}
int HeapC::getFrontBlockOverhead()
{
    //how many bytes up front (before actual ptr returned)
    return sizeof(memBlockT);
}
int HeapC::getRearBlockOverhead()
{
    //how many bytes after block
    return NUM_GUARD_LONGS*4;
}
#ifdef _DEBUG
//debug dump routines
void HeapC::outputString(char *stringOut,bool toFile, bool toTTY)
{
    if(toFile)
        fprintf(mOutputFile,stringOut);
    if(toTTY)
        printf(stringOut);
}
void HeapC::outputMemoryDumpStats( bool toFile, bool toTTY)
{
    int numBlocks = 0;
    int numFreeBlocks = 0;
    int numFreeBytes = 0;
    int numAllocatedBytes = 0;
    int numAllocatedBlocks = 0;
    int largestFreeBlock = 0;
    char stringOut[512];

    memBlockT *addr = (memBlockT *)mHeapStart;
    bool first = true;

    do
    {
        if(!first)
        {
            addr = (memBlockT*)addr->nextBlock;
        }
        else
        {
            first = false;
        }
        numBlocks++;
        int size = calcBlockSize(addr);
        if(!isFree(addr))
        {
            numAllocatedBlocks++;
            numAllocatedBytes += size;
        }
        else 
        {
            numFreeBlocks++;
            numFreeBytes+=size;
            if(size > largestFreeBlock)
                largestFreeBlock = size;
        }

    }while(addr->nextBlock != NULL);
    sprintf_s(stringOut,512,"Heap Stats:\nTotal Allocated Blocks: %d\nTotal Free Blocks: %d\nTotalAllocatedBytes: %d\nTotalFreeBytes: %d\nLargest Free Block: %d\n\n",
        numAllocatedBlocks,numFreeBlocks,numAllocatedBytes,numFreeBytes,largestFreeBlock);
    outputString(stringOut,toFile,toTTY);
}
void HeapC::outputMemoryDumpHeader(bool toFile, bool toTTY)
{
    char tempOutput[256];
    outputMemoryDumpStats(toFile,toTTY);
    sprintf_s(tempOutput,256,"Memory Location\t   Alloc Size\t   Actual Size\t          Filename\t   Line Number\t      Tag\n");
    outputString(tempOutput,toFile,toTTY);
}

void HeapC::outputMemoryBlock(memBlockT *block,bool toFile, bool toTTY)
{
    char tempOutput[256];
    int endAddr;

    if(block->nextBlock == NULL)
        endAddr = (int)mHeapStart + (int)mHeapSize;
    else
        endAddr = (int)block->nextBlock;

    if(!isFree(block))
    {
        sprintf_s(tempOutput,256," 0x%08x  \t     0x%05x\t      0x%05x\t%18s\t%10d\t%16s\n",(int)block+getFrontBlockOverhead(),
            (endAddr - (int)block)-getBlockOverhead(),
            (endAddr - (int)block),
            block->filename,block->lineNumber,block->tag);
    }
    else
    {
        sprintf_s(tempOutput,256," 0x%08x  \t    FREE %10d bytes\n",(unsigned int)block,endAddr - (int)block);
    }
    outputString(tempOutput,toFile,toTTY);
}
void HeapC::outputMemoryBlocks(bool toFile, bool toTTY)
{
    memBlockT *addr = (memBlockT *)mHeapStart;
    outputMemoryBlock(addr,toFile,toTTY);
    while(addr->nextBlock != NULL)
    {
        addr = (memBlockT *)addr->nextBlock;
        outputMemoryBlock(addr,toFile,toTTY);
    }
}

void HeapC::dumpHeapToFile(char *filename)
{
    char newFilename[256];
    checkHeapConsistency();
    sprintf_s(newFilename,256,"%s - %s.txt",filename,this->mHeapName);
    fopen_s(&mOutputFile,newFilename,"w+");
    outputMemoryDumpHeader(true,false);
    outputMemoryBlocks(true,false);
    fclose(mOutputFile);
    mOutputFile = NULL;
}

#endif