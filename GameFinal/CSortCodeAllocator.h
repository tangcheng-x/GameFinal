#ifndef __SORT_CODE_ALLOCATE_POOL_CLASS__
#define __SORT_CODE_ALLOCATE_POOL_CLASS__

template<u32 MaxCodeNum>
class CSortCodeAllocator
{
public:
	CSortCodeAllocator()
	{
		memset(mFrequencies, 0, sizeof(mFrequencies));
	}

	u32 allocate()
	{
		u32 code;
		u32 minFrequency = 1000;
		for (u32 i = 1; i <= MaxCodeNum; i++)
		{
			if (mFrequencies[i] == 0)
			{
				code = i;
				break;
			}
			
			if (mFrequencies[i] < minFrequency)
			{
				minFrequency = mFrequencies[i];
				code = i;
			}
		}

		mFrequencies[code]++;
		return code;
	}

	void release(u32 code)
	{
		mFrequencies[code]--;
	}

private:
	u32 mFrequencies[MaxCodeNum + 1]; // 1-MaxCodeNum
};

#endif
