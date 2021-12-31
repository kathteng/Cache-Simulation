#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Cache
{
	int numSets;
	int numBlocks;
	int numBytes;
	int cacheSize;
	int offsetSize;
	int blockNumber;
	string block;
	string replacement;
	vector<string> cacheFIFO; // FIFO: index 0 is first in
	vector<string> cacheLRU; // LRU: index 0 is least recently used

	int count;
	int hitCount;
	int missCount;
	float hitRatio;

public:
	Cache(string replacement_, int numSets_, int numBlocks_, int numBytes_);
	int getNumSets();
	int getNumBlocks();
	int binToDecimal(string bin);

	void directMapped(string address);
	void fullyAssociative(string address);
	void setAssociative(string address);
	void displayStats(string fileName, string cacheDesign);
};

