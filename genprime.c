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

int main(int argc, char *argv[]){
  //declare variables,
  int i;
  int N = atoi(argv[1]);
  double tstart = 0.0, ttaken;
  num_threads = atoi(argv[2]);
  omp_set_num_threads(num_threads);
  primes = (int*) malloc((N+1) *sizeof(int));
  //fill array with all numbers from 0-N
  for (i = 0; i <= (N+1); i++){
    primes[i] = i;
  }
  //start the timer
  tstart = omp_get_wtime();
  findPrimes(N);
  //stop timer and print results
  ttaken = omp_get_wtime() - tstart;
  printf("time taken for the main part %f\n", ttaken);
  writeFile(N);
}

void findPrimes(int num){
  int i, j;
  int cap = floor((num+1)/2);
  //parallel-y remove all multiples of i through floor from the primes array
  #pragma omp parallel for num_threads(num_threads)
  for (i = 2; i < cap; i++){
    #pragma omp parallel for num_threads(num_threads)
    for(j = i; j <=num; j=j+i){
      if (j != i){
	#pragma omp critical 
	primes[j] = -1;
      }
    }
  }
}

void writeFile(int fileName){
  int i;
  int rank = 1;
  int prevPrime = 2;
  int num  = fileName;
        
  //figure out how long the file name should be
  int length = fileName;
  int hum = 1;
  while(length>10){
    length /= 10;
    hum++;
  }
  char file[hum];
  char toAppend[4];
  sprintf(file, "%d", fileName);
  strcpy(toAppend, ".txt");
  strcat(file, toAppend);
  
  //create the file
  FILE *fp;
  fp = fopen(file, "w+");

  for(i = 2; i <= num; i++) {
    if (primes[i] != -1) {
      prevPrime = primes[i] - prevPrime;
      fprintf(fp, "%d %d %d\n",rank, primes[i], prevPrime);
      prevPrime = primes[i];
      rank++; 
    }
  }
  fclose(fp);
}

