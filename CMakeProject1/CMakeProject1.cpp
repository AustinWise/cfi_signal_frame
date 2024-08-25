// CMakeProject1.cpp : Defines the entry point for the application.
//

#include "CMakeProject1.h"

extern "C" void returnFromSignalHandlerWorker(void* failtingPc)
{
	puts("in returnFromSignalHandlerWorker");
	exit(1);
}

static char* sUnwriteableMem;

static void segv_handler(int code, siginfo_t* siginfo, void* context)
{
	ucontext_t* fault_context = (ucontext_t*)context;
	// See first argument to faulting PC
	fault_context->uc_mcontext.gregs[REG_RDI] = fault_context->uc_mcontext.gregs[REG_RIP];
	fault_context->uc_mcontext.gregs[REG_RIP] = (greg_t) returnFromSignalTramp;
}

__attribute__((noinline)) static void causeFault(int a)
{
	printf("int: %d char: %c\n", a, *sUnwriteableMem);
}

int main()
{
	int rc;

	puts("hello there");

	sUnwriteableMem = (char*)mmap(NULL, 4096, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (sUnwriteableMem == MAP_FAILED)
	{
		perror("mmap");
		return EXIT_FAILURE;
	}

	struct sigaction myNewAction;
	myNewAction.sa_handler = NULL;
	myNewAction.sa_sigaction = segv_handler;
	myNewAction.sa_flags = SA_SIGINFO | SA_ONSTACK;
	rc = sigaction(SIGSEGV, &myNewAction, NULL);
	if (rc != 0)
	{
		perror("sigaction(SIGSEGV)");
		return EXIT_FAILURE;
	}

	causeFault(42);

	return EXIT_SUCCESS;
}
