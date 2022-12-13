//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <XpuL4Driver.h>

//-------------------------------------------------------------------------------------
XpuL4Driver::XpuL4Driver() {
	init();
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::init() {
//    void* xpu_ptr;
    uint64_t delay;
	unsigned int xpu_status_reg = 0x0;


	int32_t memory_file_descriptor = open("/dev/mem", O_RDWR | O_SYNC);


	void* xpu_ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, memory_file_descriptor, XPU_BASE_ADDR );
	void* dma_ptr = mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, DMA_BASE_ADDR);
	void* data_in_ptr  = mmap(NULL, NR_TRANSACTIONS * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, 0x19000000);
	void* data_out_ptr = mmap(NULL, NR_TRANSACTIONS * sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor, 0x1A000000);



    for(int i = 0 ; i < NR_TRANSACTIONS; i++ )					// data in generation
    {
//    	data_in_ptr[i]= 50;
    }

    memset(data_out_ptr, 0, NR_TRANSACTIONS * sizeof(uint32_t) ); // Clear destination block

    printf("AXI xpu write program + run + dma test.\n");

    printf("Source memory block:      ");
    print_main_mem(data_in_ptr, NR_TRANSACTIONS * sizeof(uint32_t), sizeof(uint32_t));

    printf("Destination memory block: ");
    print_main_mem(data_out_ptr, NR_TRANSACTIONS * sizeof(uint32_t), sizeof(uint32_t));



    dma_reset(dma_ptr);

    xpu_status_reg = *((volatile unsigned *)(xpu_ptr + XPU_STATUS_REG_ADDR_OFFSET));	// write program file
	printf("before loading program file : %x\n", xpu_status_reg);
	printf("xpu: start program_file_load \n");
	XPU_write_program_file_1(xpu_ptr + XPU_FIFO_PROGRAM_ADDR_OFFSET);
	printf("xpu: end program_file_load \n");

    																					// load data in; ddr->dma->xpu
	printf("dma->xpu: start load data in \n");
	DMA_XPU_read(dma_ptr, 0x19000000, NR_TRANSACTIONS * sizeof(uint32_t) );
	printf("dma->xpu: end load data in\n");

																						// interrupt ack
	AXI_LITE_write(xpu_ptr + XPU_WRITE_INT_ACK_ADDR,1);
	for (delay = 0; delay < TIME_DELAY; delay++)
	{
		;
	}
	xpu_status_reg = AXI_LITE_read(xpu_ptr + XPU_STATUS_REG_ADDR_OFFSET);
	printf("after interrupt ack: status reg: %x\n", xpu_status_reg);

																						// get data out; xpu -> dma -> ddr
	printf("xpu->dma: start load data out \n");
	DMA_XPU_write(dma_ptr, 0x1A000000, NR_TRANSACTIONS * sizeof(uint32_t) );
	printf("xpu->dma: end load data out\n");

																						// print results
    printf("Destination memory block: ");
    print_main_mem(data_out_ptr, NR_TRANSACTIONS * sizeof(uint32_t), sizeof(uint32_t));
    printf("\n");

    																					// unmap memory regions
    munmap(dma_ptr,65535);
    munmap(data_in_ptr,NR_TRANSACTIONS * sizeof(uint32_t));
    munmap(data_out_ptr,NR_TRANSACTIONS * sizeof(uint32_t));
    munmap(xpu_ptr,4096);
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::AXI_LITE_write(void * addr, uint32_t value)
{
	*((volatile unsigned *)(addr)) = value;
	return;
}

//-------------------------------------------------------------------------------------
uint32_t XpuL4Driver::AXI_LITE_read(void * addr)
{
	uint32_t return_value;
	return_value = *((volatile unsigned *)(addr));
	return return_value;
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::XPU_write_program_file_1(void * addr) // data in ; ixload+ data in ; data out; addr regs: 0-100
{
	// algorithm:
		// data_out_from_dma = ixload + data_in_from_dma
	AXI_LITE_write(addr, 0x6f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000b);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000e);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000012);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700001e);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x4700001f);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000020);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000021);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000022);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000026);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x1f580000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);

	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x64000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a0001ff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x20000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f500000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60ffffff);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x1f300000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x87000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4777ffff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000001);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000003);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f080000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x12000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x67000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000003);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x00000000);	// function arguments for set addr regs
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x00000000);

	AXI_LITE_write(addr, 0x77000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000006);
	AXI_LITE_write(addr, 0x10000000);

	AXI_LITE_write(addr, 0x00000001);	// function arguments for wait mat
	AXI_LITE_write(addr, 0x00000000);

	AXI_LITE_write(addr, 0x6700000c);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x7f000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000005);
	AXI_LITE_write(addr, 0x10000000);
	return;
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::XPU_write_program_file_2(void * addr) // data in ; no compute ; data out; addr regs: 0-0
{
	AXI_LITE_write(addr, 0x6f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000b);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700000e);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000012);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000015);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700001e);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4700001f);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000020);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000021);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000022);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000026);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x1f580000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x64000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f400000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x5f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a0001ff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x20000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f500000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60ffffff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f300000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x60000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x87000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x4777ffff);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x57000003);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x1f080000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x12000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x6a000000);
	AXI_LITE_write(addr, 0x8f000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x47000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000002);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000003);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x77000000);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000006);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x00000000);
	AXI_LITE_write(addr, 0x67000004);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x7f000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000064);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000001);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x00000040);
	AXI_LITE_write(addr, 0x10000000);
	AXI_LITE_write(addr, 0x67000005);
	AXI_LITE_write(addr, 0x10000000);
	return;
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::dma_mm2s_status(void* dma_ptr)
{
	uint32_t status_reg = AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2));
    printf("MM2S status (addr offset: 0x%x status:0x%x): ", DMA_MM2S_DMASR_OFFSET, status_reg);

    if (status_reg & 0x00000001)
    {
    	printf("halted ");
    }
    else
    {
    	printf("running ");
    }

    if (status_reg & 0x00000002)
    {
    	printf("idle ");
    }
    if (status_reg & 0x00000008)
    {
    	printf("SG enabled ");
    }
    if (status_reg & 0x00000010)
    {
    	printf("DMA interrupt error ");
    }
    if (status_reg & 0x00000020)
    {
    	printf("DMA slave error ");
    }
    if (status_reg & 0x00000040)
    {
    	printf("DMA decode error ");
    }
    if (status_reg & 0x00001000)
    {
    	printf("IOC interrupt request ");
    }
    if (status_reg & 0x00002000)
    {
    	printf("Delay interrupt request ");
    }
    if (status_reg & 0x00004000)
    {
    	printf("Error interrupt request ");
    }

    printf("\n");
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::dma_s2mm_status(void* dma_ptr)
{
	uint32_t status_reg = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2));
    printf("S2MM status (addr offset: 0x%x status: 0x%x): ", DMA_S2MM_DMASR_OFFSET, status_reg);

    if (status_reg & 0x00000001)
	{
		printf("halted ");
	}
	else
	{
		printf("running ");
	}

	if (status_reg & 0x00000002)
	{
		printf("idle ");
	}
	if (status_reg & 0x00000008)
	{
		printf("SG enabled ");
	}
	if (status_reg & 0x00000010)
	{
		printf("DMA interrupt error ");
	}
	if (status_reg & 0x00000020)
	{
		printf("DMA slave error ");
	}
	if (status_reg & 0x00000040)
	{
		printf("DMA decode error ");
	}
	if (status_reg & 0x00001000)
	{
		printf("IOC interrupt request ");
	}
	if (status_reg & 0x00002000)
	{
		printf("Delay interrupt request ");
	}
	if (status_reg & 0x00004000)
	{
		printf("Error interrupt request ");
	}

    printf("\n");
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::print_all_registers_mm2s(void* dma_ptr, int tag)
{
	uint32_t register_read_value;

	printf("Printing all DMA mm2s registers: tag: %d \n", tag);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2) );
	printf("DMA mm2s: control register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2) );
	printf("DMA mm2s: status register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_SA_LSB_OFFSET>>2) );
	printf("DMA mm2s: source addr lsb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_SA_MSB_OFFSET>>2) );
	printf("DMA mm2s: source addr msb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_MM2S_LENGTH_OFFSET>>2) );
	printf("DMA mm2s: transfer length: %x\n", register_read_value);

	printf("Finished printing all DMA mm2s registers: \n");
	return;
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::print_all_registers_s2mm(void* dma_ptr, int tag)
{
	uint32_t register_read_value;

	printf("Printing all DMA s2mm registers: tag: %d\n", tag);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMACR_OFFSET>>2) );
	printf("DMA s2mm: control register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2) );
	printf("DMA s2mm: status register: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DA_LSB_OFFSET>>2) );
	printf("DMA s2mm: source addr lsb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_DA_MSB_OFFSET>>2) );
	printf("DMA s2mm: source addr msb: %x\n", register_read_value);

	register_read_value = AXI_LITE_read(dma_ptr + (DMA_S2MM_LENGTH_OFFSET>>2) );
	printf("DMA s2mm: transfer length: %x\n", register_read_value);

	printf("Finished printing all DMA s2mm registers: \n");
	return;
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::dma_mm2s_wait_transfers_complete(void* dma_ptr)
{
	uint32_t mm2s_status =  AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2) );

    while( ( mm2s_status & (1<<DMA_MM2S_DMASR_X_Idle_LOC) ) == DMA_MM2S_DMASR_X_Idle_X_NOT_IDLE )
    {
        dma_mm2s_status(dma_ptr);
        mm2s_status =  AXI_LITE_read(dma_ptr + (DMA_MM2S_DMASR_OFFSET>>2) );
    }
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::dma_s2mm_wait_transfers_complete(void* dma_ptr)
{
	uint32_t s2mm_status = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2) );

    while( ( s2mm_status & (1<<DMA_S2MM_DMASR_X_Idle_LOC) ) == DMA_S2MM_DMASR_X_Idle_X_NOT_IDLE )
    {
        dma_s2mm_status(dma_ptr);
        s2mm_status = AXI_LITE_read(dma_ptr + (DMA_S2MM_DMASR_OFFSET>>2) );
    }
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::DMA_XPU_read(void* dma_ptr, uint32_t ddr_start_addr, uint32_t transfer_length)
{

	AXI_LITE_write(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2), 0);
    dma_mm2s_status(dma_ptr);
	printf("Writing source address\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_SA_LSB_OFFSET>>2), ddr_start_addr);
	AXI_LITE_write(dma_ptr + (DMA_MM2S_SA_MSB_OFFSET>>2), 0x00000000);
	dma_mm2s_status(dma_ptr);
	printf("Starting MM2S channel\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2), 0x0001);
	dma_mm2s_status(dma_ptr);
	printf("Writing MM2S transfer length\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_LENGTH_OFFSET>>2), transfer_length);
	dma_mm2s_status(dma_ptr);
	printf("Waiting for MM2S to be done\n");
	dma_mm2s_wait_transfers_complete(dma_ptr);
	dma_mm2s_status(dma_ptr);
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::DMA_XPU_write(void* dma_ptr, uint32_t ddr_start_addr, uint32_t transfer_length )
{
	printf("Writing destination address\n");
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DA_LSB_OFFSET>>2), ddr_start_addr);
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DA_MSB_OFFSET>>2), 0x00000000);
	dma_s2mm_status(dma_ptr);
	printf("Starting S2MM channel\n");
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DMACR_OFFSET>>2), 0x0001);
	dma_s2mm_status(dma_ptr);
	printf("Writing S2MM transfer length\n");
	AXI_LITE_write(dma_ptr + (DMA_S2MM_LENGTH_OFFSET>>2), transfer_length);
	dma_s2mm_status(dma_ptr);
	printf("Waiting for S2MM to be done\n");
	dma_s2mm_wait_transfers_complete(dma_ptr);
	dma_s2mm_status(dma_ptr);
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::dma_reset(void* dma_ptr)
{
	printf("Resetting DMA\n");
	AXI_LITE_write(dma_ptr + (DMA_MM2S_DMACR_OFFSET>>2), 1 << DMA_MM2S_DMACR_X_RESET_LOC);
	AXI_LITE_write(dma_ptr + (DMA_S2MM_DMACR_OFFSET>>2), 1 << DMA_S2MM_DMACR_X_RESET_LOC);
	dma_mm2s_status(dma_ptr);
	dma_s2mm_status(dma_ptr);
}

//-------------------------------------------------------------------------------------
void XpuL4Driver::print_main_mem(void* address, int32_t nr_bytes, uint32_t word_size)
{
    char *p = static_cast<char *>(address);

    for (int i = 0; i < nr_bytes; i++)
    {
        if ( (i+1) % word_size == 0)
        {
        	for(int j = 0; j < word_size ; j++)
        	{
        		printf("%02x", p[i-j]);
        	}
        	printf(" ");
        }

        if( (i+1) % (word_size *8) == 0)
		{
			printf("\n");
		}
    }

    printf("\n");
}

//-------------------------------------------------------------------------------------
