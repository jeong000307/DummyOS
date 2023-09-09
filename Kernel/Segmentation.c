#include "Segmentation.h"

static union SegmentDescriptor GDT[3 + 2 * MAX_CPU_CORE];

code InitialzeSegmentation(void)    {
    size index;

    GDT[0].data = 0;
    SetCodeSegment(&GDT[1], ExecuteRead, 0, 0, 0xfffff);
    SetDataSegment(&GDT[2], ReadWrite, 0, 0, 0xfffff);

    /*for (index = 0; index < MAX_CPU_CORE; ++index) {
        SetCodeSegment(&GDT[2 + 2 * index], ExecuteRead, 0, 0, 0xfffff);
        SetDataSegment(&GDT[2 + 2 * index + 1], ReadWrite, 0, 0, 0xfffff);
    }*/
    LoadGDT(sizeof(GDT) - 1, (addr)GDT);

    InitializeSegmentRegister(1 << 3, 2 << 3, 0);

    return SUCCESS;
}