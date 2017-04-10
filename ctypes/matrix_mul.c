#include "stdio.h"

int mul(int res[], int ma[], int mb[], int l, int m, int n)
{
	int i = 0;
	int j = 0;
	int k = 0;
	for(i=0; i<l; i++)
		for(j=0; j<n; j++)
		{
			int sum = 0;
			for(k=0; k<m; k++)
				sum += (ma[i*m+k] * mb[k*n+j]);
			res[i*n+j]=sum;
		}

	return 0;
}


