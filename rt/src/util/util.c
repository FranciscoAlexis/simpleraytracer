/*******************
	util.c (utility functions)
*********************/

FLOAT_T max(FLOAT_T x0, FLOAT_T x1)
{
	return((x0 > x1) ? x0 : x1);
}

FLOAT_T phi(int j)
{
	FLOAT_T x = 0.0;
	FLOAT_T f = 0.5;
	
	while(j)
	{
		x += f * (FLOAT_T)(!j & 1);
		j /= 2;
		f *= 0.5;
	}
	return x;	
}

int getRandomSample()
{
	return rand()/(int)(((unsigned)RAND_MAX + 1) / vp.numSamples);
}

