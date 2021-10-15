#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include "seq_loop.hpp"


#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {

  if (argc < 7) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }

  int functionID = std::stoi (argv[1]);
  int a = std::stoi (argv[2]);
  int b = std::stoi(argv[3]);
  int n = std::stoi(argv[4]);
  int intensity =std::stoi( argv[5]);
  int nbthreads = std::stoi(argv[6]);

  // printf("%i %i %i %i %i %i\n", functionID, a, b, n, intensity, nbthreads);

  float ban = 1.0 * (b-a) / n;
  //printf("%i    %f\n", bMinusA, ban);
  float result = 0;

  SeqLoop sl;
  int sum = 0;

  
  auto start = std::chrono::steady_clock::now();
  
  switch(functionID)
  {
  case 1 :
    //Use f1
    sl.parfor<int>(0, nbthreads-1, 1,
		 [&](int& tls) -> void{
		   tls = 0;
		 },
		 [&](int i, int& tls) -> void{
		   //tls += b[i];

		   int chunk = n/nbthreads;
		   int pass = 0; //How do I determine this?
		   int start = (pass * chunk);
		   int end = ((pass + 1) * chunk) - 1;
		   
		   for (int i = start; i <= end; i++)
		     {
		       float x = a + ((i + 0.5) * ban);
		       printf("%f\n", x);
		       result += f1(x, intensity);
		     }
		   result *= ban;
		   //tls += b[i];

		   
		  },
		 [&](int tls) -> void{
		   sum += tls;
		 }
		 );

    std::cout<<sum<<"\n";
    
    for (int i = 0; i <= n-1; i++)
      {
	float x = a + ((i + 0.5) * ban);
	printf("%f\n", x);
	result += f1(x, intensity);
      }
    result *= ban;
    break;
  case 2 :
    //Use f2
    for (int i = 0; i <= n-1; i++)
      {
	float x = a + ((i + 0.5) * ban);
	result += f2(x, intensity);
      }
    result *= ban;    
    break;
  case 3 :
    //use f3
    for (int i = 0; i <= n-1; i++)
      {
	float x = a + ((i + 0.5) * ban);
	result += f3(x, intensity);
      }
    result *= ban;
   break;
  case 4 :
    //use f4
    for (int i = 0; i <= n-1; i++)
      {
	float x = a + ((i + 0.5) * ban);
	//printf("%f   %i    %f\n", ban, i,  x);
	result += f4(x, intensity);
      }
    result *= ban;
    break;
  default :
    std::cerr<<"invalid functionID";
      return -1;
  }

  auto end = std::chrono::steady_clock::now();
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  float elapsed = milliseconds / 1000.0;
  
  //printf("Result = %f     Elapsed = %f seconds\n", result, elapsed);

  std::cout << std::to_string(result) << std::endl;
  std::cerr << std::to_string(elapsed) << std::endl;
  
  return 0;
}
