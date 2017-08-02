
class HeapC;

//Assignment
//1. Describe HeapManagerC Class
////////////////////////////////////////////////////////////////////////////
// The HeapManager manages multiple heaps for the user and allows debug functionality for the heaps.
////////////////////////////////////////////////////////////////////////////
class HeapManagerC
{
public:

    static HeapManagerC* GetHeapManager();

    typedef struct 
    {
        int             heapID;
        char            *name;
        int             heapSize;
    }HeapdefT;


    void                    init(HeapdefT *heapdefs,int numHeaps, int defaultHeapID);

    int                     getHeapSize(int id);
    char*                   getHeapName(int id);

    void*                   allocateBlock(int heapID, int bytes, char *filename, int lineNumber, char* tag, int time);
    void*                   allocateBlock(int heapID, int bytes, char *filename, int lineNumber, char* tag, int time, int alignment);
    void                    freeBlock(void *ptr);

    void                    setCurrentHeapID(int id){ mOldHeapID = mCurrentHeapID;mCurrentHeapID = id;};
    int                     restoreHeapID() { mCurrentHeapID = mOldHeapID;};

#if _DEBUG
    void                    printDebugFile(int heapID, char *filename);
    void                    dumpAllHeaps();
#endif
	HeapManagerC(){};
	~HeapManagerC(){};

private:

    void                    initHeaps(HeapdefT *heapdefs);
	int                     getNumHeaps();
    HeapC*                  getHeap(int id);
    void                    unitTestHeaps();

    int                     mOldHeapID;
    int                     mCurrentHeapID;
    int                     mDefaultHeapID;

    int                     mNumHeaps;

    HeapC                   **mHeapList;
};

