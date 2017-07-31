
#define FIEA_NEW(heapID,tag) new(heapID,__FILE__,__LINE__,tag,GetTickCount(),4)
#define FIEA_NEW_ALIGNED(heapID,tag,alignment) new(heapID,__FILE__,__LINE__,tag,GetTickCount(),alignment)
#define FIEA_NEW_NOTIMER(heapID,tag) new(heapID,__FILE__,__LINE__,tag,0,4)

#define FIEA_ALLOC(x,id,tag) HeapManagerC::GetHeapManager()->allocateBlock(id,x,__FILE__,__LINE__,tag,0)
#define FIEA_ALLOC_ALIGNED(x,id,tag,alignment) HeapManagerC::GetHeapManager()->allocateBlock(id,x,__FILE__,__LINE__,tag,0,alignment)

#define FIEA_FREE(x) HeapManagerC::GetHeapManager()->freeBlock(x)
#define FIEA_DELETE(x) delete(x)
void *operator new(size_t uiSize,int heapID,char *file, int lineNumber,char *tag, int time, int alignment);
void operator delete(void *pvAddr,int heapID,char *file, int lineNumber,char *tag, int time,int alignment);

//void *operator new(size_t uiSize);
//void *operator new[](size_t uiSize);
//void *operator new[](size_t uiSize,char *file, uint32_t lineNumber);
//void operator delete(void *pvAddr);
//void operator delete[](void *pvAddr);
//void operator delete[](void *pvAddr, char *file, uint32_t lineNumber);
