#pragma once
typedef unsigned int uint;
typedef unsigned long long ulong;
typedef unsigned char uchar;
const uint BYTES_IN_INT = sizeof(int);
const uint BYTES_IN_UINT = sizeof(unsigned int);
const uint BYTES_IN_ULONG = sizeof(unsigned long long);
const uint MAX_CHUNK_NUM = 5000000;
const int WINDOW_SIZE = 12;
//while P = 2^k, fingerprint % P means fingerprint & P_MINUS (P - 1). We set P = 32 here
const int P_MINUS = 0x1F;		