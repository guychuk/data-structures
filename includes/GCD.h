#pragma once

unsigned int GCD(unsigned int m, unsigned int n)
{
	// use the Euclidean algorithm
	
	unsigned int r = m % n;

	while (r)
	{
		m = n;
		n = r;
		r = m % n;
	}

	return n;
}
