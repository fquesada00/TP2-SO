#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include "video_driver.h"
#include "Interrupciones/idtLoader.h"
#include "Interrupciones/time.h"
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];
	puts("[x64BareBones]");
	newLine();
	puts("CPU Vendor:");
	puts(cpuVendor(buffer));
	newLine();

	puts("[Loading modules]");
	newLine();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	puts("[Done]");
	newLine();
	newLine();

	puts("[Initializing kernel's binary]");
	newLine();
	clearBSS(&bss, &endOfKernel - &bss);
	puts("  text: 0x");
	ncPrintHex((uint64_t)&text);
	newLine();
	puts("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	newLine();
	puts("  data: 0x");
	ncPrintHex((uint64_t)&data);
	newLine();
	puts("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	newLine();
	puts("[Done]");
	newLine();
	newLine();
	return getStackBase();
}

int main(){
	load_idt();
	puts("[Kernel Main]");
	newLine();
	puts("Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	newLine();
	puts("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	newLine();
	newLine();

	puts("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	newLine();
	puts("  Sample data module contents: ");
	puts((char*)sampleDataModuleAddress);
	newLine();
	puts("[Finished]");
	write_pixel(1024,0,255,0,0);
	clear();
	//write_pixel(1023,0,255,0,0);
	while(1);
	
	
}
