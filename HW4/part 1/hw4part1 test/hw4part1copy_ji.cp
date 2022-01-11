
#include <stdio.h>
#include <stdlib.h>

#define MSize 2048

void copyij(long int src[MSize][MSize], long int dst[MSize][MSize])
{
  long int i,j;
  for (i = 0; i < MSize; i++)
    for (j = 0; j < MSize; j++)
      dst[j][i] = src[j][i];
}

void copyji(long int src[MSize][MSize], long int dst[MSize][MSize])
{
  long int i,j;
  for (j = 0; j < MSize; j++)
    for (i = 0; i < MSize; i++)
      dst[j][i] = src[j][i];
}

long int m1[MSize][MSize];
long int m2[MSize][MSize];

int main(void)
{
  copyji(m1, m2);
  return 0;
}
