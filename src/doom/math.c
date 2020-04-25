////////////////////////////////////////////////////////////
//
//  MATH.C:  Math functions including our new random engine
//
////////////////////////////////////////////////////////////

#include "marshmallow.h"


// - rndindex uniquely dedicated for Marshmallow
// - seeded via time by D_DoomMain()
int marshmallow_rndindex = 0;		// [ceben 2017]

static const int maxRandomAttempts = 8;

extern const unsigned char rndtable[256];

#define MAX_RANDOMIZED_ARRAY_SIZE		1024


//-----------------------------------------------------------------------------
// fmix()														[ceben 2017]
//
//		floating point linear interpolation between [a,b] via s
//-----------------------------------------------------------------------------
float fmix(float a, float b, float s)
{
	float x;
	x = a + s * (b - a);
	return x;
}


//-----------------------------------------------------------------------------
// imix()														[ceben 2017]
//
//		integer linear interpolation between [a,b] via s
//-----------------------------------------------------------------------------
int imix(int a, int b, float s)
{
	int x;
	x = a + (int)(s * (float)(b - a));
	return x;
}

//-----------------------------------------------------------------------------
// favg()														[ceben 2017]
//
//		averages elements of array
//-----------------------------------------------------------------------------
float favg(float *a, int size)
{
	float sum;
	int k;
	
	sum = 0;
	for (k=0; k < size; k++)
		sum += a[k];

	sum /= size;
	return sum;
}

//-----------------------------------------------------------------------------
// iavg()														[ceben 2017]
//
//		averages elements of array
//-----------------------------------------------------------------------------
int iavg(int *a, int size)
{
	int sum, k;
	
	sum = 0;
	for (k=0; k < size; k++)
		sum += a[k];

	sum /= size;
	return sum;
}

//-----------------------------------------------------------------------------
// Marshmallow_M_Random()										[ceben 2017]
//
//		- Marshmallow version of M_Random() functionality,
//		  to use new time-seeded random index
//		- returns random integer via rndtable, within [0,255] range
//-----------------------------------------------------------------------------
int Marshmallow_M_Random()
{
	int value;

	if (realnetgame)
		return P_Random();  // was M_

    marshmallow_rndindex = (marshmallow_rndindex+1)&0xff;
    value = rndtable[marshmallow_rndindex];
	return value;
}


//-----------------------------------------------------------------------------
// F_Random()													[ceben 2017]
//
//		- returns float in [0.0, 1.0] range inclusive
//		- ensures that consecutive calls return different values
//-----------------------------------------------------------------------------
float F_Random()
{
	static float last = 2.0;	// arbitrary initial value

	int randi;
	int attempts = maxRandomAttempts;		// safety against infinite loop
	float randf;

	do {
		randi = Marshmallow_M_Random();
		randf = (float)randi / (float)256;
	} while (randf == last && attempts-- > 0);
	last = randf;
	return randf;
}


//-----------------------------------------------------------------------------
// GetRandomFloatInRange()										[ceben 2017]
//
//		returns random float in [lo, hi] range inclusive
//-----------------------------------------------------------------------------
float GetRandomFloatInRange(float lo, float hi)
{
	float randf, result;
	randf = F_Random();
	result = fmix(lo, hi, randf);
	return result;
}


//-----------------------------------------------------------------------------
// GetRandomIntegerInRange()									[ceben 2017]
//
//		- returns random integer in [lo, hi] range inclusive
//		- ensures that consecutive calls return different values
//-----------------------------------------------------------------------------
int GetRandomIntegerInRange(int lo, int hi)
{
	static int last = 0xff;
	int attempts = maxRandomAttempts;		// safety against infinite loop
	int result;
	int randi;
	float randf;

	do
	{
		randf = F_Random();
		result = imix(lo, hi + 1, randf);
	}
	while (result == last && attempts-- > 0);	// necessary since integers may repeat due to float-to-int casting
	last = result;
	return result;
}


//-----------------------------------------------------------------------------
// GetRandomArrayIndex()										[ceben 2017]
//
//		returns a random index into an array of size 'arraySize'
//-----------------------------------------------------------------------------
int GetRandomArrayIndex(int arraySize)
{
	return GetRandomIntegerInRange(0, arraySize - 1);
}


//-----------------------------------------------------------------------------
// RandomizeIntArrayHelper()									[ceben 2017]
//
//		shuffles the elements of the 'remaining' array and stores
//		them in the 'result' array
//-----------------------------------------------------------------------------
static void RandomizeIntArrayHelper(int *remaining, int remaining_size, int *result)
{
	int result_size;

	if (remaining_size == 0)
		return;

	result_size = 0;
	while (remaining_size > 0)
	{
		int src_idx, dst_idx;
		int randomIndex, randomElement;
		int temp[MAX_RANDOMIZED_ARRAY_SIZE];
		int temp_size;

		// get random element from list
		randomIndex = GetRandomArrayIndex(remaining_size);
		randomElement = remaining[randomIndex];

		// add it to result
		result[result_size] = randomElement;
		result_size++;

		// remove random element from remaining by first storing into temp
		dst_idx = 0;	// going through and into temp
		src_idx = 0;	// reading from remaining
		temp_size = 0;
		while (src_idx < remaining_size)
		{
			if (src_idx != randomIndex)
			{
				temp[dst_idx] = remaining[src_idx];
				dst_idx++;
				temp_size++;
			}

			src_idx++;
		}

		remaining_size--;

		// rebuild remaining by copying from temp
		for (src_idx = 0; src_idx < remaining_size; src_idx++)
			remaining[src_idx] = temp[src_idx];
	}
}

//-----------------------------------------------------------------------------
// RandomizeIntArray()										[ceben 2017]
//
//		shuffles the elements of an array
//-----------------------------------------------------------------------------
void RandomizeIntArray(int *a, int size)
{
	int k;
	int result[MAX_RANDOMIZED_ARRAY_SIZE];
	int remaining[MAX_RANDOMIZED_ARRAY_SIZE];
	for (k=0; k < size; k++)
		remaining[k] = a[k];

	RandomizeIntArrayHelper(remaining, size, result);
	for (k=0; k < size; k++)
		a[k] = result[k];
}


//-----------------------------------------------------------------------------
// Test_Random()										[ceben 2017]
//
//		very necessary
//-----------------------------------------------------------------------------


#define TEST_RANDOM_N 3

void Test_Random()
{
	const int numTrials = 100;
	const int lo = 0;
	const int hi = 9;
	// const int n = hi - lo;
	int k;
	int buckets[TEST_RANDOM_N + 3];
	for (k = 0; k < TEST_RANDOM_N + 3; k++)
	{
		buckets[k] = 0;
	}
	
	for (k = 0; k < numTrials; k++)
	{
		// int m = GetRandomIntegerInRange(lo, hi);
		int m/* = Bot_GetRandomWeapon(true)*/;
		// OK int m = GetRandomIntegerInRange(0, TEST_RANDOM_N);
		buckets[m]++;
		if (!(0 <= m && m <= TEST_RANDOM_N))
		{
			continue;
		}

	}

	k = 0;
	return;
}
