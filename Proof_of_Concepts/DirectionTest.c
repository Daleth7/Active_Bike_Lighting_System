#include <stdio.h>
#include <string.h>

typedef char bit;

#define HIGH 1
#define LOW 0
#define RISING 1
#define FALLING 0

	// Usage: ./dirtest.exe 0101011101 010101010111
	//	First bit string is highs and lows for first signal
	//	Second bit string is rising and falling edges for second signal
int main(int argc, char* argv[]) {
	bit* highlow_string = argv[1], *risefall_string = argv[2];
	const unsigned str_sz = strlen(argv[1]);

	/** Begin command line argument parser **/
	if(argc < 3){
		printf("Usage: ./dirtest.exe [First bit string] [second bit string]");
		return 0;
	}

	unsigned i = 0;
	if(str_sz != strlen(argv[2])){
		printf("Expected same length strings. Aborting...");
		return 0;
	}
	for(i = 0; i < str_sz; ++i){
		highlow_string[i] -= '0';
		risefall_string[i] -= '0';
	}
	/** End command line argument parser **/
	
	
	
	
	for (i = 0; i < str_sz; i++)
	{
		printf("Comparing %u and %u --> ", highlow_string[i], risefall_string[i]);
		if (risefall_string[i] == 1 && highlow_string[i] == 0)
		{
			printf("moving away");
		}
		else if (risefall_string[i] == 1 && highlow_string[i] == 1)
		{
			printf("moving towards");
		}
		else if (risefall_string[i] == 0 && highlow_string[i] == 1)
		{
			printf("moving away");
		}
		else if (risefall_string[i] == 0 && highlow_string[i] == 0)
		{
			printf("moving towards");
		}
		printf("\n");
	}
	
	

	return 0;
}