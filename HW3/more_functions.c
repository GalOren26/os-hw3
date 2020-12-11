#include "Functions.h"
#include <assert.h>
#include < stdlib.h >



//inputs:prime - the number that we want to get it's prime factors
	///prime_component is an int* array with size 30(numbers in that range can't have more
	//prime components than that
	//output: index -this is the number of prime components in the number
	//functionality: this function fils prime_components array with the prime numbers, and the rest
	//will be zeros
	//please notice that the prints are just for check
int FindPrimeComponets(int prime, int* OUT prime_components)
{
	//int n = 999999999;
	//int* arr[30] = { 0 };
	int index = 0;
	// Print the number of 2s that divide prime 
	while (prime % 2 == 0) {
		//printf("%d ", 2);
		prime_components[index] = 2;
		index += 1;
		prime = prime / 2;
	}

	// prime must be odd at this point.  So we can start at i = 3 and there is no need to check
	//for even numbers, so i += 2
	for (int i = 3; i <= sqrt(prime); i += 2) {
		// While i divides prime, prime_components[index]=i and divide prime
		while (prime % i == 0) {
			//printf("%d ", i);
			prime_components[index] = i;
			index += 1;
			prime = prime / i;
		}
	}

	// This condition is to handle the case when prime 
	// is a prime number greater than 2 
	if (prime > 2) {
		//printf("%d ", prime);
		prime_components[index] = prime;
	}
	//for (int j = 0; j < 30; j++)
		//printf("%d", prime_components[j]);
	return index;
}

//I don't implement it eventually since "FindPrimeComponets" returns it sorted 
int sort(int* prime_components)
{
	return 0;
}

void FormatNumberString(int* prime_components, char* OUT prime_factors_by_format, int number_of_components)
{
	int i = 0;
	//the for loop runs until number_of_components-1 since we don't want comma after the last one
	for (int i = 0; i < number_of_components - 1; i++) {
		prime_factors_by_format[i] = prime_components[i] + '0';
		prime_factors_by_format[i + 1] = ',';
		i++;
	}
	prime_factors_by_format[i] = prime_components[i] + '0';
	prime_factors_by_format[i + 1] = '\0';
}
//inputs: number-the input number we want to divide to prime factors
//prime_factors_by_format - is "number" divided to it's prime factors by the specified format
//outputs: none
//funcionallity: this function prints the prime components by the specified format 
void printByFormat(int number, char* prime_factors_by_format) {
	printf("The prime factors of %d are: %s\r\n", number, prime_factors_by_format);
}
