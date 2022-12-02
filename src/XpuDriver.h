//-------------------------------------------------------------------------------------
//
//                             The XPU-ORT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
/*
	example of basic program for the xpu system.
		write program file into xpu
		get data in into the xpu system using dma mechanism
		do computations (done automatically)
		get data out from the xpu system using dma mechanism
 */
//-------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/mman.h>

//-------------------------------------------------------------------------------------
// XPU defines
#define XPU_NR_CELLS 64
#define XPU_BASE_ADDR 0x40000000
#define XPU_STATUS_REG_ADDR_OFFSET 0x0
#define XPU_FIFO_PROGRAM_ADDR_OFFSET 0x0

#define XPU_WRITE_INT_ACK_ADDR 4
#define XPU_READ_SEL_READABLE_REGS_ADDR 4
#define XPU_WRITE_SEL_READABLE_REGS_ADDR 8
#define XPU_READABLE_REGS_PROGRAM_COUNTER_ADDR 0
#define XPU_READABLE_REGS_CYCLE_COUNTER_ADDR 1
#define XPU_READABLE_REGS_ACCELERATOR_ID 2

															// test related defines
#define NR_TRANSACTIONS XPU_NR_CELLS
#define TIME_DELAY 10000000

                                // DMA defines
#define DMA_BASE_ADDR 0x40400000

#define DMA_MM2S_DMACR_OFFSET 0x00
#define DMA_MM2S_DMASR_OFFSET 0x04
#define DMA_MM2S_SA_LSB_OFFSET 0x18
#define DMA_MM2S_SA_MSB_OFFSET 0x1C
#define DMA_MM2S_LENGTH_OFFSET 0x28
#define DMA_S2MM_DMACR_OFFSET 0x30
#define DMA_S2MM_DMASR_OFFSET 0x34
#define DMA_S2MM_DA_LSB_OFFSET 0x48
#define DMA_S2MM_DA_MSB_OFFSET 0x4C
#define DMA_S2MM_LENGTH_OFFSET 0x58

															// MM2S
		//	MM2S_DMACR - control register
	// rw
#define DMA_MM2S_DMACR_X_RS_LOC 0
#define DMA_MM2S_DMACR_X_RS_SIZE 1
#define DMA_MM2S_DMACR_X_RS_X_STOP 0
#define DMA_MM2S_DMACR_X_RS_X_RUN 1
	// rw
#define DMA_MM2S_DMACR_X_RESET_LOC 2
#define DMA_MM2S_DMACR_X_RESET_SIZE 1
#define DMA_MM2S_DMACR_X_RESET_X_PAUSED 0
#define DMA_MM2S_DMACR_X_RESET_X_START 1
	// rw
#define DMA_MM2S_DMACR_X_IOC_IrqEn_LOC 12
#define DMA_MM2S_DMACR_X_IOC_IrqEn_SIZE 1
#define DMA_MM2S_DMACR_X_IOC_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_IOC_IrqEn_X_ENABLED 1
	// rw
#define DMA_MM2S_DMACR_X_Dly_IrqEn_LOC 13
#define DMA_MM2S_DMACR_X_Dly_IrqEn_SIZE 1
#define DMA_MM2S_DMACR_X_Dly_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_Dly_IrqEn_X_ENABLED 1
	// rw
#define DMA_MM2S_DMACR_X_Err_IrqEn_LOC 14
#define DMA_MM2S_DMACR_X_Err_IrqEn_SIZE 1
#define DMA_MM2S_DMACR_X_Err_IrqEn_X_DISABLED 0
#define DMA_MM2S_DMACR_X_Err_IrqEn_X_ENABLED 1


		//	MM2S_DMASR - status register
	// ro
#define DMA_MM2S_DMASR_X_Halted_LOC 0
#define DMA_MM2S_DMASR_X_Halted_SIZE 1
#define DMA_MM2S_DMASR_X_Halted_X_RUNNING 0
#define DMA_MM2S_DMASR_X_Halted_X_HALTED 1
	// ro
#define DMA_MM2S_DMASR_X_Idle_LOC 1
#define DMA_MM2S_DMASR_X_Idle_SIZE 1
#define DMA_MM2S_DMASR_X_Idle_X_NOT_IDLE 0
#define DMA_MM2S_DMASR_X_Idle_X_IDLE 1
	// ro
#define DMA_MM2S_DMASR_X_SGIncld_LOC 3
#define DMA_MM2S_DMASR_X_SGIncld_SIZE 1
#define DMA_MM2S_DMASR_X_SGIncld_X_SG_EN 1
#define DMA_MM2S_DMASR_X_SGIncld_X_SG_not_EN 0
	// ro
#define DMA_MM2S_DMASR_X_DMASlvErr_LOC 5
#define DMA_MM2S_DMASR_X_DMASlvErr_SIZE 1
#define DMA_MM2S_DMASR_X_DMASlvErr_X_NO_ERR_DETECTED 0
#define DMA_MM2S_DMASR_X_DMASlvErr_X_ERR_DETECTED 1
	// ro
#define DMA_MM2S_DMASR_X_DMADecErr_LOC 6
#define DMA_MM2S_DMASR_X_DMADecErr_SIZE 1
#define DMA_MM2S_DMASR_X_DMADecErr_X_NO_ERR_DETECTED 0
#define DMA_MM2S_DMASR_X_DMADecErr_X_ERR_DETECTED 1
	// R/W "1" to Clear
#define DMA_MM2S_DMASR_X_IOC_Irq_LOC 12
#define DMA_MM2S_DMASR_X_IOC_Irq_SIZE 1
#define DMA_MM2S_DMASR_X_IOC_Irq_X_NO_IOC_INT_DETECTED 0
#define DMA_MM2S_DMASR_X_IOC_Irq_X_IOC_INT_DETECTED 1
	// R/W "1" to Clear
#define DMA_MM2S_DMASR_X_Err_Irq_LOC 14
#define DMA_MM2S_DMASR_X_Err_Irq_SIZE 1
#define DMA_MM2S_DMASR_X_Err_Irq_X_NO_ERR_INT_DETECTED 0
#define DMA_MM2S_DMASR_X_Err_Irq_X_ERR_INT_DETECTED 1


		// MM2S_SA_LSB - source address
	// R/W
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_LOC 0
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_SIZE 32
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0
#define DMA_MM2S_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0


		// MM2S_SA_MSB - source address
	// R/W
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_LOC 0
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_SIZE 32
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR 0
#define DMA_MM2S_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR  0


		// MM2S_LENGTH
	// R/W
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_LOC 0
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_SIZE 26
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_X_LENGTH 0
#define DMA_MM2S_DMACR_X_MM2S_LENGTH_X_LENGTH 0


															// S2MM
		//	S2MM_DMACR - control register
	// rw
#define DMA_S2MM_DMACR_X_RS_LOC 0
#define DMA_S2MM_DMACR_X_RS_SIZE 1
#define DMA_S2MM_DMACR_X_RS_X_STOP 0
#define DMA_S2MM_DMACR_X_RS_X_RUN 1
	// rw
#define DMA_S2MM_DMACR_X_RESET_LOC 2
#define DMA_S2MM_DMACR_X_RESET_SIZE 1
#define DMA_S2MM_DMACR_X_RESET_X_PAUSED 0
#define DMA_S2MM_DMACR_X_RESET_X_START 1
	// rw
#define DMA_S2MM_DMACR_X_IOC_IrqEn_LOC 12
#define DMA_S2MM_DMACR_X_IOC_IrqEn_SIZE 1
#define DMA_S2MM_DMACR_X_IOC_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_IOC_IrqEn_X_ENABLED 1
	// rw
#define DMA_S2MM_DMACR_X_Dly_IrqEn_LOC 13
#define DMA_S2MM_DMACR_X_Dly_IrqEn_SIZE 1
#define DMA_S2MM_DMACR_X_Dly_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_Dly_IrqEn_X_ENABLED 1
	// rw
#define DMA_S2MM_DMACR_X_Err_IrqEn_LOC 14
#define DMA_S2MM_DMACR_X_Err_IrqEn_SIZE 1
#define DMA_S2MM_DMACR_X_Err_IrqEn_X_DISABLED 0
#define DMA_S2MM_DMACR_X_Err_IrqEn_X_ENABLED 1


		//	S2MM_DMASR - status register
	// ro
#define DMA_S2MM_DMASR_X_Halted_LOC 0
#define DMA_S2MM_DMASR_X_Halted_SIZE 1
#define DMA_S2MM_DMASR_X_Halted_X_RUNNING 0
#define DMA_S2MM_DMASR_X_Halted_X_HALTED 1
	// ro
#define DMA_S2MM_DMASR_X_Idle_LOC 1
#define DMA_S2MM_DMASR_X_Idle_SIZE 1
#define DMA_S2MM_DMASR_X_Idle_X_NOT_IDLE 0
#define DMA_S2MM_DMASR_X_Idle_X_IDLE 1
	// ro
#define DMA_S2MM_DMASR_X_SGIncld_LOC 3
#define DMA_S2MM_DMASR_X_SGIncld_SIZE 1
#define DMA_S2MM_DMASR_X_SGIncld_X_SG_EN 1
#define DMA_S2MM_DMASR_X_SGIncld_X_SG_not_EN 0
	// ro
#define DMA_S2MM_DMASR_X_DMASlvErr_LOC 5
#define DMA_S2MM_DMASR_X_DMASlvErr_SIZE 1
#define DMA_S2MM_DMASR_X_DMASlvErr_X_NO_ERR_DETECTED 0
#define DMA_S2MM_DMASR_X_DMASlvErr_X_ERR_DETECTED 1
	// ro
#define DMA_S2MM_DMASR_X_DMADecErr_LOC 6
#define DMA_S2MM_DMASR_X_DMADecErr_SIZE 1
#define DMA_S2MM_DMASR_X_DMADecErr_X_NO_ERR_DETECTED 0
#define DMA_S2MM_DMASR_X_DMADecErr_X_ERR_DETECTED 1
	// R/W "1" to Clear
#define DMA_S2MM_DMASR_X_IOC_Irq_LOC 12
#define DMA_S2MM_DMASR_X_IOC_Irq_SIZE 1
#define DMA_S2MM_DMASR_X_IOC_Irq_X_NO_IOC_INT_DETECTED 0
#define DMA_S2MM_DMASR_X_IOC_Irq_X_IOC_INT_DETECTED 1
	// R/W "1" to Clear
#define DMA_S2MM_DMASR_X_Err_Irq_LOC 14
#define DMA_S2MM_DMASR_X_Err_Irq_SIZE 1
#define DMA_S2MM_DMASR_X_Err_Irq_X_NO_ERR_INT_DETECTED 0
#define DMA_S2MM_DMASR_X_Err_Irq_X_ERR_INT_DETECTED 1


		// S2MM_SA_LSB - source address
	// R/W
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_LOC 0
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_SIZE 32
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0
#define DMA_S2MM_DMACR_X_MM2S_SA_LSB_X_SOURCE_ADDR 0


		// S2MM_SA_MSB - source address
	// R/W
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_LOC 0
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_SIZE 32
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR 0
#define DMA_S2MM_DMACR_X_MM2S_SA_MSB_X_SOURCE_ADDR  0


		// S2MM_LENGTH
	// R/W
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_LOC 0
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_SIZE 26
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_X_LENGTH 0
#define DMA_S2MM_DMACR_X_MM2S_LENGTH_X_LENGTH 0

//-------------------------------------------------------------------------------------
class XpuDriver {

public:
	XpuDriver();

 	~XpuDriver();

	void write(void * addr, uint32_t value);
	uint32_t read(void * addr);

};
//-------------------------------------------------------------------------------------






