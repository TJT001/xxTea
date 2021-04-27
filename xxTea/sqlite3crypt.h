#pragma once
#ifndef  DCG_SQLITE_CRYPT_FUNC_
#define  DCG_SQLITE_CRYPT_FUNC_
#include <stdio.h>

//#ifdef SQLITE_HAS_CODEC

typedef unsigned char BYTE;
// ���ܽṹ
#define CRYPT_OFFSET 8
typedef struct _CryptBlock {

	BYTE* ReadKey; // �����ݿ��д���������Կ
	BYTE* WriteKey; // д�����ݿ����Կ
	int PageSize; // ҳ�Ĵ�С
	BYTE* Data;

} CryptBlock, *LPCryptBlock;

#ifndef DB_KEY_LENGTH_BYTE        // ��Կ����
#define DB_KEY_LENGTH_BYTE   16   // ��Կ����
#endif

#ifndef DB_KEY_PADDING            // ��Կλ������ʱ������ַ�
#define DB_KEY_PADDING       0x33 // ��Կλ������ʱ������ַ�
#endif

// ���ܺ���
int sqlite3_encrypt(unsigned char * pData, unsigned int data_len, unsigned char * key, unsigned int len_of_key);

// ���ܺ���
int sqlite3_dencrypt(unsigned char * pData, unsigned int data_len, unsigned char * key, unsigned int len_of_key);

// ===
// ��sqlite3���õļ�/���ܺ���
void* sqlite3Codec(void *pArg, void *data, int nPageNum, int nMode);

// �ͷŶ���/���ڴ�Ļص�
void sqlite3CodecFree(void* arg);

// ��֤����ӿ�
unsigned char* DeriveKey(const void *pKey, int nKeyLen);

// ���������һ��ҳ�ļ����㷨����.�˺��������뻺����.
LPCryptBlock CreateCryptBlock(unsigned char* hKey, int nPageSize, LPCryptBlock pExisting);

//#endif // SQLITE_HAS_CODEC
#endif