#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "sqlite3crypt.h"
#include "defs.h"

// #pragma comment(lib, "SQLiteTea.lib")

typedef unsigned char byte;

// #define  SQLITE3_STATIC
// extern int sqlite3_key(sqlite3 *db, const void *pKey, int nKey);

// extern "C" int sqlite3_key(sqlite3 *db, const void *pKey, int nKey);

/*
extern C {
#include "sqlite3.h"
};

#define  SQLITE3_STATIC

extern int sqlite3_key(sqlite3 *db, const void *pKey, int nKey);
*/

int xxtea_IDA(int *a1, int a2, __int64 a3)
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

#define MX (z>>5^y<<2) + (y>>3^z<<4)^(sum^y) + (k[p&3^e]^z);

long xxtea_tmp(long* v, long n, long* k) {
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


static int _callback_exec(void * notused, int argc, char ** argv, char ** aszColName)
{
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("%s = %s\r\n", aszColName[i], argv[i] == 0 ? "NUL" : argv[i]);
	}

	return 0;
}

void deCrptTea()
{
	FILE* fp = NULL;
	byte bQQ[] = { 0x64, 0x45, 0x3F, 0x3C, 0x4A, 0x46, 0x6C, 0x6F, 0x7C, 0x4B, 0x22, 0x28, 0x28, 0x4E, 0x25, 0x62 };
	fopen_s(&fp, "QQ.db", "rb+");

	fseek(fp, 0x400, SEEK_END);
	long len = ftell(fp);
	int Size = len - 0x400;
	int PageCount = Size / 0x100;

	byte data[0x2000] = { 0 };
	fseek(fp, 0x400, 0);

	//int tmplen = fread(data, 1, 0x400, fp);
	// xxtea_tmp(data, 0x2000, bQQ);
	// xxtea_IDA(data, 0x400, bQQ);

	for (int i = 0; i < PageCount; i++)
	{
		int ilen = fread(data, 1, 0x100, fp);
		if (ilen == 0)
			break;
		// xxtea_tmp(data, 0x300, bQQ);
		xxtea_IDA(data, 0x100, bQQ);
	}
}


int main(int argc, char * argv[])
{
	deCrptTea();

	const char * sSQL;
	char * pErrMsg = 0;
	int ret = 0;
	sqlite3 * db = 0;

	char fileName8[0x40] = "E:\\SourceCode\\QQ\\HackQQ\\HackQQ\\bin\\Cache1.1.db";
	byte bCache[] = { 0x77, 0x52, 0x2A, 0x53, 0x5B, 0x7D, 0x24, 0x60, 0x3C, 0x5D, 0x6F, 0x47, 0x50, 0x39, 0x65, 0x77 };
	byte bQQ[] = { 0x64, 0x45, 0x3F, 0x3C, 0x4A, 0x46, 0x6C, 0x6F, 0x7C, 0x4B, 0x22, 0x28, 0x28, 0x4E, 0x25, 0x62 };
	const char *sqlVacuum = "VACUUM INTO 'Cache2.db';";
	const char* sql = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name";
	char *errMsg = 0;

	//ret = sqlite3_open(fileName8, &db);
	ret = sqlite3_open("QQ.db", &db);

	ret = sqlite3_key(db, bQQ, 0x10);

	// sSQL = "create table class(name varchar(20), student);";
	sqlite3_exec(db, sql, _callback_exec, 0, &pErrMsg);

	//插入数据
	// sSQL = "insert into class values('mem_52911', 'zhaoyun');";
	// sqlite3_exec(db, sSQL, _callback_exec, 0, &pErrMsg);

	//取得数据并显示
	// sSQL = "select * from class;";
	// sqlite3_exec(db, sSQL, _callback_exec, 0, &pErrMsg);

	//关闭数据库
	sqlite3_close(db);
	db = 0;

	return 0;
}