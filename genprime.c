#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <string.h>

//declare global variables and functions
int *primes;
int num_threads;

void findPrimes(int num);
void writeFile(int nval);

//fill the primes array with all numbers from 2 - N
int main(int argc, char *argv[]){
  int i;
  int N = atoi(argv[1]);

  num_threads = atoi(argv[2]);

  omp_set_num_threads(num_threads);

  primes = (int*) malloc((N+1) *sizeof(int));

  for (i = 0; i <= (N+1); i++){
  	primes[i] = i;
  }
  findPrimes(N);

  writeFile(N);
}

void findPrimes(int num){
	int i;
	int j;

	if(threads == 1){
		#pragma omp parallel for num_threads(num_threads)
		for (i = 2; i < floor((num+1)/2); i++){
			#pragma omp parallel for num_threads(num_threads)
			for(j = i; j <=num; j=j+i){
				if (j != i){
					#pragma omp critical 
					primes[j] = 0;
				}
			}
		}
	}

	else{
		#pragma omp parallel for num_threads(num_threads/2)
		for (i =2; i < floor((num+1)/2); i++){
			#pragma omp parallel for num_threads(num_threads/2)
			for(j = 1; j <=num; j = j+i){
				if(j != i){
					#pragma omp critical
					primes[j] = 0;
				}
			}
		}
	}
}

void writeFile(int name){
	int i;
	int rank = 0;
	int lastPrime = 2;
	int num  = name;

	char fileTemp[6];
	char txt[10];

	sprintf(fileTemp, "%d", name);

	//copy the number name 
	strncpy(txt, ".txt", 4);

	//put together
	strcat(fileTemp, txt);

	FILE *fp;

	fp = fopen(fileTemp, "w+");


    //now lets see what is in there
	 for( i = 2; i <= num; i++) {
	 	if (primes[i] != 0 ) {
	 		lastPrime = primes[i] - lastPrime;
	 		rank++;
	 	
			fprintf(fp, "%d %d %d\n",rank, primes[i], lastPrime);
			lastPrime = primes[i];
	 	}
	 }


    fclose(fp);
}

