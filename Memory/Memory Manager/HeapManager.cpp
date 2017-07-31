// Header File For Standard Input / Output
#include <stdio.h>	
#include <malloc.h>
#include <string.h>
#include "macros.h"
#include "heapmanager.h"
#include "heap.h"
#include "memory.h"

HeapManagerC theHeap;
HeapC theHeapHeap;

HeapManagerC* HeapManagerC::GetHeapManager()
{
    return &theHeap;
}
void HeapManagerC::init(HeapdefT *heapDefs,int numHeaps, int defaultHeapID)
{
    //heapDefs contains a ptr to the array of info for all heaps
    //numHeaps is the number of heaps initialized
    //init the heap manager 
    mDefaultHeapID = defaultHeapID;
    mCurrentHeapID = mDefaultHeapID;
    mOldHeapID = mDefaultHeapID;

    mNumHeaps = numHeaps;
    //getting memory for the heaps using malloc - could use static memory as well
    //if number of heaps was known to avoid dynamic allocation
    mHeapList = (HeapC**)malloc(sizeof(HeapC *)*mNumHeaps+1); 
    //now init each heap
    initHeaps(heapDefs);
    unitTestHeaps();
}

//accessor function for heap name
char *HeapManagerC::getHeapName(int id) 
{
    return getHeap(id)->getHeapName();
};

//accessor function for num heaps
int HeapManagerC::getNumHeaps()
{
    return mNumHeaps;
}

//accessor function for heap size
int HeapManagerC::getHeapSize(int id)
{
    HeapC* thisHeap = getHeap(id);
    return thisHeap->getSize();
}
//unit test
void HeapManagerC::unitTestHeaps()
{
    for(int i=2;i<getNumHeaps();++i)
    {
        //call unit test on each heap
        getHeap(i)->unitTest();
    }
}

void HeapManagerC::initHeaps(HeapdefT *heapDefs)
{
    assert(mHeapList);
    //create a special heap to hold info about other heaps 
    int heapHeapSize = 10000;
    void *heapHeapStart = malloc(heapHeapSize);
    theHeapHeap.init("TheHeapHeap",heapHeapStart,heapHeapSize,1);
    mHeapList[0] = &theHeapHeap;

    for(int i=0;i<getNumHeaps();++i)
    {
        //go thru each heap and init it
        HeapdefT *thisHeapDef = &heapDefs[i];
        char name[32];
        //copy the name over from the array to the heap data
        strncpy_s(name,heapDefs[i].name,32);
        //get memory for the heap in the new special heap
        HeapC *thisHeap = FIEA_NEW_NOTIMER(1,name) HeapC();
        //set pointer for the heap in the heap manager
        mHeapList[i+1] = thisHeap;
        //allocate the memory for the heap
        void *startHeapPtr = (void*)malloc(thisHeapDef->heapSize);
        //init the heap data
        thisHeap->init(thisHeapDef->name,startHeapPtr,thisHeapDef->heapSize, thisHeapDef->heapID);
    }
}

//accessor function for the heap
HeapC *HeapManagerC::getHeap(int id)
{
    for(int i=0;i<getNumHeaps()+1;++i)
    {
        HeapC *thisHeap = mHeapList[i];
        if(thisHeap->getID() == id)
        {
            return thisHeap;
        }
    }
    assert(0);//heap not found!
    return NULL;
}
//main allocation functions - calls into heap and gets block
void *HeapManagerC::allocateBlock(int id,int bytes,char *filename,int lineNumber,char *tag,int time)
{
    return getHeap(id)->allocateBlock(bytes,filename,lineNumber,tag,time,4);//long word alignment if none specified
}
void *HeapManagerC::allocateBlock(int id,int bytes,char *filename,int lineNumber,char *tag,int time, int alignment)
{
    return getHeap(id)->allocateBlock(bytes,filename,lineNumber,tag,time,alignment);
}
//free a block - gets heap ID from block itself
void HeapManagerC::freeBlock(void *ptr)
{
    HeapC::memBlockT *block = HeapC::getMemblockFromPtr(ptr);
    HeapC *heap = getHeap(HeapC::getHeapID(block));
    heap->freeBlock(ptr);
}

#if _DEBUG
void HeapManagerC::dumpAllHeaps()
{
    for(int i=1;i<mNumHeaps+2;++i)
    {
        printDebugFile(i,getHeap(i)->getHeapName());
    }
}
void HeapManagerC::printDebugFile(int heapID, char *filename)
{
    getHeap(heapID)->dumpHeapToFile(filename);
}
#endif