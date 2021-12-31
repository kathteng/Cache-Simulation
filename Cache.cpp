#include "Cache.h"
#include <cmath>

Cache::Cache(string replacement_, int numSets_, int numBlocks_, int numBytes_)
{
	numSets = numSets_;
	numBlocks = numBlocks_;
	numBytes = numBytes_;
	replacement = replacement_;

	count = 0;
	hitCount = 0;
	missCount = 0;
	hitRatio = 0.0f;

	offsetSize = log2(numBytes);
	blockNumber = 32 - offsetSize;
	cacheSize = numSets * numBlocks * numBytes;
	cacheFIFO.resize(numSets * numBlocks, "0");
	cacheLRU.resize(numSets * numBlocks, "0");
}

int Cache::getNumSets()
{
	return numSets;
}

int Cache::getNumBlocks()
{
	return numBlocks;
}

int Cache::binToDecimal(string bin)
{
	int decimal = 0;
	int base = 1;
	for (int i = bin.length() - 1; i >= 0; i--)
	{
		if (bin[i] == '1')
			decimal += base;
		base *= 2;
	}
	return decimal;
}

void Cache::directMapped(string address)
{
	block = address.substr(0, blockNumber);
	int lineSize = log2(numSets);
	int tagSize = 32 - lineSize - offsetSize;
	string tag = block.substr(0, tagSize);
	string line = block.substr(tagSize, lineSize);
	int lineNum = binToDecimal(line);
	count++;

	bool hit = false;
	if (cacheLRU[lineNum].find(tag) != string::npos)
	{
		hitCount++;
		hit = true;
	}
	if (hit == false)
	{
		missCount++;
		cacheLRU[lineNum].replace(0, tagSize, tag);
	}
	
}

void Cache::fullyAssociative(string address)
{
	block = address.substr(0, blockNumber);
	count++;
	
	for (int i = 0; i < cacheLRU.size(); i++)
	{
		if (replacement == "FIFO")
		{
			if (cacheFIFO[i] == block)
			{
				hitCount++;
				return;
			}
		}
		else if (replacement == "LRU")
		{
			if (cacheLRU[i] == block)
			{
				hitCount++;

				string temp = cacheLRU[i];
				for (int j = i; j < cacheLRU.size() - 1; j++)
					cacheLRU[j] = cacheLRU[j + 1];

				cacheLRU[cacheLRU.size() - 1] = temp;

				return;
			}
		}
	}
	missCount++;
	// LRU or FIFO to find line to remove
	if (replacement == "FIFO")
	{
		for (int i = 0; i < cacheFIFO.size(); i++)
		{
			if (cacheFIFO[i] == "0") // empty line
			{
				cacheFIFO[i] = block;
				return;
			}
		}
		// cache is full, remove first line
		for (int i = 0; i < cacheFIFO.size() - 1; i++)
			cacheFIFO[i] = cacheFIFO[i + 1];

		cacheFIFO[cacheFIFO.size() - 1] = block;
	}
	else if (replacement == "LRU")
	{
		for (int i = 0; i < cacheLRU.size(); i++)
		{
			if (cacheLRU[i] == "0")
			{
				cacheLRU[i] = block;
				return;
			}
		}
		for (int i = 0; i < cacheLRU.size() - 1; i++)
			cacheLRU[i] = cacheLRU[i + 1];

		cacheLRU[cacheLRU.size() - 1] = block;
	}
}

void Cache::setAssociative(string address)
{
	block = address.substr(0, blockNumber);
	int setField = log2(numSets);
	int tagSize = block.length() - setField;
	string tag = block.substr(0, tagSize);
	string set = block.substr(tagSize, setField);
	int setDecimal = binToDecimal(set);
	count++;

	int start = numBlocks * setDecimal;
	if (replacement == "FIFO")
	{
		for (int i = start; i < start + numBlocks; i++)
		{
			if (cacheFIFO[i] == tag)
			{
				hitCount++;
				return;
			}
		}
	}
	else if (replacement == "LRU")
	{
		for (int i = start; i < start + numBlocks; i++)
		{
			if (cacheLRU[i] == tag)
			{
				hitCount++;

				string temp = cacheLRU[i];
				for (int j = i; j < start + numBlocks - 1; j++)
					cacheLRU[j] = cacheLRU[j + 1];

				cacheLRU[start + numBlocks - 1] = temp;

				return;
			}
		}
	}
	missCount++;
	// LRU or FIFO to find line to remove
	if (replacement == "FIFO")
	{
		for (int i = start; i < start + numBlocks; i++)
		{
			if (cacheFIFO[i] == "0") // empty line
			{
				cacheFIFO[i] = tag;
				return;
			}
		}
		// set is full, remove first line
		for (int i = start; i < start + numBlocks - 1; i++)
			cacheFIFO[i] = cacheFIFO[i + 1];

		cacheFIFO[start + numBlocks - 1] = tag;
	}
	else if (replacement == "LRU")
	{
		for (int i = start; i < start + numBlocks - 1; i++)
		{
			if (cacheLRU[i] == "0")
			{
				cacheLRU[i] = tag;
				return;
			}
		}
		for (int i = start; i < start + numBlocks - 1; i++)
			cacheLRU[i] = cacheLRU[i + 1];

		cacheLRU[start + numBlocks - 1] = tag;
	}
}

void Cache::displayStats(string fileName, string cacheDesign)
{
	cout << fileName << ", " << cacheDesign << endl;
	if (cacheDesign == "Direct Mapped")
		replacement = "N/A";
	cout << "sets: " << numSets << " | " << "blocks: " << numBlocks << " | " << "bytes: " << numBytes << " | " << replacement << endl;
	cout << "cache size: " << cacheSize << endl;
	cout << "hits: " << hitCount << endl;
	cout << "misses: " << missCount << endl;
	cout << "total accesses: " << count << endl;
	hitRatio = float(hitCount) / count;
	cout << "hit ratio: " << hitRatio << endl;
	cout << endl;
}