#include "sqlite3crypt.h"
#include "sqlite3.h"
#include <memory.h>
#include <stdlib.h>
#include "defs.h"

/***********************************************************/
/*
int xxtea(int *a1, int a2, __int64 a3)
{
	__int64 v4; // r10
	unsigned int v5; // ecx
	unsigned int v6; // er11
	signed int v7; // eax
	int v8; // er14
	int v9; // er15
	__int64 i; // r13
	unsigned int v11; // er12
	int v12; // er15
	unsigned int v14; // er8
	int v15; // eax
	unsigned int v16; // er11
	unsigned int v17; // edx
	__int64 v18; // r10
	int v19; // er14
	int v20; // er15
	__int64 v21; // rax
	int v22; // er12
	unsigned int v23; // er13
	unsigned int v24; // ebx
	int v25; // ecx
	int v26; // edx
	int v27; // er15

	if (a2 < 2)
	{
		v14 = 1;
		if (a2 <= -2)
		{
			v14 = 0;
			v15 = 52 / -a2;
			v16 = 0x9E3779B9 * v15 - 0x4AB325AA;
			if (0x9E3779B9 * v15 != 0x4AB325AA)
			{
				v17 = *a1;
				v18 = ~a2;
				do
				{
					v19 = (v16 >> 2) & 3;
					v20 = a1[v18];
					v21 = ~a2;
					do
					{
						v22 = 4 * v17;
						v23 = v17;
						v24 = a1[v21 - 1];
						v25 = (v16 ^ v17) + (v24 ^ *(_DWORD *)(a3 + 4LL * (v19 ^ (unsigned int)(v21 & 3))));
						v26 = v20;
						v20 = v24;
						v17 = v26 - ((((v24 >> 5) ^ v22) + ((v23 >> 3) ^ (16 * v24))) ^ v25);
						a1[v21--] = v17;
					} while (v21 > 0);
					v27 = *a1
						- (((((unsigned int)a1[v18] >> 5) ^ (4 * v17)) + ((v17 >> 3) ^ (16 * a1[v18]))) ^ ((v16 ^ v17)
							+ (*(_DWORD *)(a3 + 4LL * (v19 ^ (unsigned int)(v21 & 3))) ^ a1[v18])));
					*a1 = v27;
					v17 = v27;
					v16 += 1640531527;
				} while (v16);
			}
		}
	}
	else
	{
		v4 = (unsigned int)(a2 - 1);
		v5 = a1[(int)v4];
		v6 = 0;
		v7 = 0x34u / a2 + 5;
		do
		{
			v6 -= 0x61C88647;
			v8 = (v6 >> 2) & 3;
			if ((int)v4 <= 0)
			{
				v12 = v5;
				LOBYTE(i) = 0;
			}
			else
			{
				v9 = *a1;
				for (i = 0LL; i != v4; ++i)
				{
					v11 = a1[i + 1];
					v5 = v9
						+ ((((v5 >> 5) ^ (4 * v11)) + ((v11 >> 3) ^ (16 * v5))) ^ ((v6 ^ v11)
							+ (*(_DWORD *)(a3
								+ 4LL * (v8 ^ (unsigned int)(i & 3))) ^ v5)));
					a1[i] = v5;
					v9 = v11;
				}
				v12 = a1[(int)v4];
			}
			v5 = v12
				+ ((((v5 >> 5) ^ (4 * *a1)) + (((unsigned int)*a1 >> 3) ^ (16 * v5))) ^ ((v6 ^ *a1)
					+ (*(_DWORD *)(a3
						+ 4LL
						* (v8 ^ (unsigned int)(i & 3))) ^ v5)));
			a1[(int)v4] = v5;
		} while (v7-- > 0);
		v14 = 0;
	}
	return v14;
}
*/
/***********************************************************/

#define MX (z>>5^y<<2) + (y>>3^z<<4)^(sum^y) + (k[p&3^e]^z);

long xxtea(long* v, long n, long* k) {
	unsigned long z = v[n - 1], y = v[0], sum = 0, e, DELTA = 0x9e3779b9;
	long p, q;
	if (n > 1) {
		q = 6 + 52 / n;
		while (q-- > 0) {
			sum += DELTA;
			e = (sum >> 2) & 3;
			for (p = 0; p < n - 1; p++) y = v[p + 1], z = v[p] += MX;
			y = v[0];
			z = v[n - 1] += MX;
		}
		return 0;
	}
	else if (n < -1) {
		n = -n;
		q = 6 + 52 / n;
		sum = q * DELTA;
		while (sum != 0) {
			e = (sum >> 2) & 3;
			for (p = n - 1; p > 0; p--) z = v[p - 1], y = v[p] -= MX;
			z = v[n - 1];
			y = v[0] -= MX;
			sum -= DELTA;
		}
		return 0;
	}
	return 1;
}

/**********************************************************/
//int xxtea(int * v, int n, int * k) {
//	unsigned int z/*=v[n-1]*/, y = v[0], sum = 0, e, DELTA = 0x9e3779b9;
//	int m, p, q;
//	if (n > 1) {
//		/* Coding Part */
//		z = v[n - 1];
//		q = 6 + 52 / n;
//		while (q-- > 0) {
//			sum += DELTA;
//			e = sum >> 2 & 3;
//			for (p = 0; p < n - 1; p++) {
//				y = v[p + 1],
//					z = v[p] += (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum^y) + (k[p & 3 ^ e] ^ z);
//			}
//			y = v[0];
//			z = v[n - 1] += (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum^y) + (k[p & 3 ^ e] ^ z);
//		}
//		return 0;
//
//		/* Decoding Part */
//	}
//	else if (n < -1) {
//		n = -n;
//		q = 6 + 52 / n;
//		sum = q * DELTA;
//		while (sum != 0) {
//			e = sum >> 2 & 3;
//			for (p = n - 1; p > 0; p--) {
//				z = v[p - 1],
//					y = v[p] -= (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum^y) + (k[p & 3 ^ e] ^ z);
//			}
//			z = v[n - 1];
//			y = v[0] -= (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum^y) + (k[p & 3 ^ e] ^ z);
//			sum -= DELTA;
//		}
//		return 0;
//	}
//
//	return 1;
//}


// 其它算法，未实现
 // ==============================================================================
int sqlite3_encrypt(unsigned char * pData, unsigned int data_len, unsigned char * key, unsigned int len_of_key)
{
	return 0;
}

// 其它算法，未实现
// ==============================================================================
int sqlite3_dencrypt(unsigned char * pData, unsigned int data_len, unsigned char * key, unsigned int len_of_key)
{

	return 0;
}

// ==============================================================================
void * sqlite3Codec(void *pArg, void *data, int nPageNum, int nMode) {

	LPCryptBlock pBlock = (LPCryptBlock)pArg;
	unsigned int dwPageSize = 0;
	int len = 0;

	if (!pBlock)
		return data;
	// 确保pager的页长度和加密块的页长度相等.如果改变,就需要调整.
	if (nMode != 2) {
		int a = 0;
		a++;
		// PgHdr *pageHeader;

		// pageHeader = DATA_TO_PGHDR(data);
		// if (pageHeader->pPager->pageSize != pBlock->PageSize) {
			   // CreateCryptBlock(0, pageHeader->pPager, pBlock);
		 //}
	}

	switch (nMode) {
	case 0: // Undo a "case 7" journal file encryption
	case 2: //重载一个页
	case 3: //载入一个页  
		if (!pBlock->ReadKey) break;

		xxtea(data, len, pBlock->ReadKey);

		break;
	case 6:

		//加密一个主数据库文件的页  
		if (!pBlock->WriteKey) break;

		memcpy(pBlock->Data + CRYPT_OFFSET, data, pBlock->PageSize);
		data = pBlock->Data + CRYPT_OFFSET;


		len = pBlock->PageSize / 4;
		// xxtea(data, len, pBlock->WriteKey);
		break;

	case 7:
		// 加密事务文件的页
		if (!pBlock->ReadKey) break;
		memcpy(pBlock->Data + CRYPT_OFFSET, data, pBlock->PageSize);
		data = pBlock->Data + CRYPT_OFFSET;
		len = pBlock->PageSize / 4;
		xxtea(data, len, pBlock->ReadKey);
		break;
		/*
	case 3: //载入一个页
		   if (!pBlock->ReadKey)
				  break;

		   memcpy(pBlock->Data + CRYPT_OFFSET, data, pBlock->PageSize);
		   data = pBlock->Data + CRYPT_OFFSET;
		   dwPageSize = pBlock->PageSize;

		   // 解密
		   sqlite3_dencrypt((BYTE*)data, dwPageSize, pBlock->ReadKey, DB_KEY_LENGTH_BYTE);

		   break;

	case 6: //加密一个主数据库文件的页
		   if (!pBlock->WriteKey)
				  break;

		   memcpy(pBlock->Data + CRYPT_OFFSET, data, pBlock->PageSize);
		   data = pBlock->Data + CRYPT_OFFSET;
		   dwPageSize = pBlock->PageSize;

		   // 加密
		   sqlite3_encrypt((BYTE*)data, dwPageSize, pBlock->WriteKey, DB_KEY_LENGTH_BYTE);

		   break;

	case 7:
		   // 加密事务文件的页
		   // 在正常环境下, 读密钥和写密钥相同. 当数据库是被重新加密的,读密钥和写密钥未必相同.
		   // 回滚事务必要用数据库文件的原始密钥写入.因此,当一次回滚被写入,总是用数据库的读密钥,
		   // 这是为了保证与读取原始数据的密钥相同.
		   //
		   if (!pBlock->ReadKey)
				  break;

		   memcpy(pBlock->Data + CRYPT_OFFSET, data, pBlock->PageSize);
		   data = pBlock->Data + CRYPT_OFFSET;
		   dwPageSize = pBlock->PageSize;

		   sqlite3_encrypt((BYTE*)data, dwPageSize, pBlock->ReadKey, DB_KEY_LENGTH_BYTE);/*调用我的加密函数*/

		   // break;*/
	}

	return data;
}

// ==============================================================================
void sqlite3CodecFree(void* pArg) {
	if (pArg)
	{
		LPCryptBlock pBlock = (LPCryptBlock)pArg;
		//销毁读密钥.
		if (pBlock->ReadKey) {
			sqlite3_free(pBlock->ReadKey);
		}

		//如果写密钥存在并且不等于读密钥,也销毁.

		if (pBlock->WriteKey && pBlock->WriteKey != pBlock->ReadKey) {
			sqlite3_free(pBlock->WriteKey);
		}

		if (pBlock->Data) {
			sqlite3_free(pBlock->Data);
		}

		//释放加密块.
		sqlite3_free(pBlock);
	}
}


// =====================================================================================
LPCryptBlock CreateCryptBlock(unsigned char* hKey, int nPageSize, LPCryptBlock pExisting)
{
	LPCryptBlock pBlock;

	if (!pExisting) //创建新加密块
	{
		pBlock = sqlite3_malloc(sizeof(CryptBlock));
		memset(pBlock, 0, sizeof(CryptBlock));
		pBlock->ReadKey = hKey;
		pBlock->WriteKey = hKey;
		pBlock->PageSize = nPageSize;
		pBlock->Data = (unsigned char*)sqlite3_malloc(
			pBlock->PageSize + CRYPT_OFFSET);
	}
	else //更新存在的加密块
	{
		pBlock = pExisting;

		if (pBlock->PageSize != nPageSize && !pBlock->Data) {
			sqlite3_free(pBlock->Data);
			pBlock->PageSize = nPageSize;

			pBlock->Data = (unsigned char*)sqlite3_malloc(
				pBlock->PageSize + CRYPT_OFFSET);
		}
	}

	memset(pBlock->Data, 0, pBlock->PageSize + CRYPT_OFFSET);

	return pBlock;
}

// 从用户提供的缓冲区中得到一个加密密钥
// =====================================================================================
unsigned char * DeriveKey(const void *pKey, int nKeyLen)
{
	unsigned char * hKey = 0;
	int j;

	if (pKey == 0 || nKeyLen == 0)
	{
		return 0;
	}

	hKey = sqlite3_malloc(DB_KEY_LENGTH_BYTE + 1);
	if (hKey == 0)
	{
		return 0;
	}

	hKey[DB_KEY_LENGTH_BYTE] = 0;

	if (nKeyLen < DB_KEY_LENGTH_BYTE)
	{
		memcpy(hKey, pKey, nKeyLen); //先拷贝得到密钥前面的部分
		j = DB_KEY_LENGTH_BYTE - nKeyLen;

		//补充密钥后面的部分
		memset(hKey + nKeyLen, DB_KEY_PADDING, j);
	}
	else
	{
		//密钥位数已经足够,直接把密钥取过来
		memcpy(hKey, pKey, DB_KEY_LENGTH_BYTE);
	}

	return hKey;
}