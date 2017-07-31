enum {
    DEFAULT_HEAP=2,
    SYSTEM_HEAP,
    OTHER_HEAP,
    TEMP_HEAP,
    BALL_HEAP,
    INVALID_HEAP
};

#ifdef GAME_CPP    
HeapManagerC::HeapdefT heaps[] = {
                    {DEFAULT_HEAP,"Default Heap",5000000},
                    {SYSTEM_HEAP,"System Heap",5000000},
                    {OTHER_HEAP,"Other Heap",5000000},
                    {TEMP_HEAP,"Temp Heap",5000000},
                    {BALL_HEAP,"Ball Heap",5000000},
                   };
#endif
#define NUM_HEAPS (sizeof(heaps)/sizeof(HeapManagerC::HeapdefT))
