//test 
#pragma once
#include <unordered_set>
#include <list>
#include <deque>
#include <string>
#include <cstring>
#include <stdio.h>
#include <fstream>
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <openssl/sha.h>
#include <algorithm>
#include "RabinHash.h"
#include "CircularHash.h"
#include "CircularHashPool.h"
#include "Definition.h"

//The maximum block number in the server GPU
const int BLOCK_NUM = 4096;
//The maximum thread nubmer per block in the server GPU
const int THREAD_PER_BLOCK = 512;
//Mention! this is just the number of windows for each thread
//const int NUM_OF_WINDOWS_PER_THREAD = 4;
const uint MAX_KERNEL_INPUT_LEN = BLOCK_NUM * THREAD_PER_BLOCK + WINDOW_SIZE - 1;

class RedundancyEliminator_CUDA {
private:
	RabinHash hashFunc;
	VirtualHash* circHash;

	//Add a new chunk into cache, if hash value queue is full also delete the oldest chunk
	void addNewChunk(uchar* hashValue, char* chunk, uint chunkSize);
	inline uchar* computeChunkHash(char* chunk, uint chunkSize);
public:
	ulong *kernelTA, *kernelTB, *kernelTC, *kernelTD;
	enum Type {MultiFingerprint, NonMultifingerprint};

	//deque<uint> chunking(char* kernelInput, uint inputLen, ulong *resultHost);
	void ChunkHashing(deque<uint> indexQ, char* package, deque<uchar*> &hashValues, deque<tuple<char*, uint>> &chunks);
	uint ChunkMatching(deque<uchar*> &hashValues, deque<tuple<char*, uint>> &chunks);
	/*deque<tuple<uchar*, uint>> is for simulation, deque<uchar*> for real case*/
	void ChunkHashingAscyn(uint* indices, int indicesNum, char* package, deque<tuple<uchar*, uint>> &hashValueQ, mutex &hashValueQMutexs);
	uint fingerPrinting(deque<uint> indexQ, char* package);
	void RabinHashAsync(char* inputKernel, char* inputHost, uint inputLen, ulong* resultKernel, ulong* resultHost, cudaStream_t stream);

	uint eliminateRedundancy(char* package, uint packageSize);
	RedundancyEliminator_CUDA() {}
	RedundancyEliminator_CUDA(Type type);
	~RedundancyEliminator_CUDA();
};

__global__ void Hash(const ulong *TA, const ulong *TB, const ulong *TC, const ulong * TD,
	char *str, const uint windowsNum, ulong *result/*, int *debug*/);
__device__ void SetSubResult(ulong* subResult, uint idx, ulong resultPoint); 
__device__ ulong* GetSubResult(ulong* result, uint blockNum);
__device__ char* GetSubStr(char *str, uint blockNum);
__device__ uint GetUIntFromStr(char* strs, uint idx);
__device__ ulong GetULongFromStr(char* strs, uint idx);
__device__ char GetChar(char* subStr, uint idx);