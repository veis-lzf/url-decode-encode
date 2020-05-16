#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef enum _BOOL_
{
	FALSE = 0,
	TRUE
} BOOL;

#ifdef __GUN_C__
#define HEX_TO_DEC(ch) \
({ \
	char ret = -1; \
	if (ch >= '0' && ch <= '9') ret = ch - '0'; \
	else if (ch >= 'a' && ch <= 'z') ret = ch - 'a' + 10; \
	else if (ch >= 'A' && ch <= 'Z') ret = ch - 'A' + 10; \
	else ret = -1; \
	ret;\
})
#else
static inline int hex2dec(const char ch)
{
	if (ch >= '0' && ch <= '9') return ch - '0';
	if (ch >= 'a' && ch <= 'z') return ch - 'a' + 10;
	if (ch >= 'A' && ch <= 'Z') return ch - 'A' + 10;
	return -1;
}
#endif

static BOOL urlencode(char* *lpszDest, const char* cszSrc, size_t nLen); // ����
static BOOL urldecode(char* *lpszDest, const char* pszUrl, size_t nLen); // ����

int main()
{
	char str[] = "http://www.baidu.com/s?ie=utf-8&f=8&tn=baidu&wd=��ʱ����";
	char *pUrl = NULL; // �洢url�����׵�ַ
	char *pSrc = NULL; // �洢�����Ԫ���׵�ַ

	printf("����ǰ��%s\n", str);

	urlencode(&pUrl, str, strlen(str)); // ����
	printf("�����%s\n", pUrl);

	urldecode(&pSrc, pUrl, strlen(pUrl)); // ����
	printf("�����%s\n", pSrc);

	if (pUrl)
		free(pUrl); // �ͷ��ڴ�
	if (pSrc)
		free(pSrc); // �ͷ��ڴ�
	_getch();
	return 0;
}

// GB2312����תurl����
static BOOL urlencode(char* *lpszDest, const char* cszSrc, size_t nLen)
{
	size_t i = 0, nIndex = 0;
	size_t len = nLen;
	char hex[] = "0123456789ABCDEF";
	char ch;
	char* pNew = NULL;

	if (!cszSrc) 
		return FALSE;

	*lpszDest = (char *)malloc(nLen * sizeof(char) + 1); // ����һ�����ȴ洢'\0'
	if (NULL == *lpszDest)
		return FALSE;

	for (i = 0; i < nLen; i++) {
		ch = cszSrc[i];
		if ((ch >= '0' && ch <= '9') ||
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z') ||
			ch == '-' || ch == '_' || ch == '.' || ch == '~') {
			(*lpszDest)[nIndex++] = ch;
		}
		else if (ch == ' ') {
			(*lpszDest)[nIndex++] = '+';
		}
		else {
			int tmp = ch;
			if (ch < 0)
				tmp += 256;
			(*lpszDest)[nIndex++] = '%';
			(*lpszDest)[nIndex++] = hex[tmp / 16];
			(*lpszDest)[nIndex++] = hex[tmp % 16];
		}
		if (nIndex >= len - 1) {
			len *= 3;
			pNew = malloc(sizeof(char) * len);
			memset(pNew, '\0', sizeof(char) * len);
			if (!pNew)
				return FALSE;
			memcpy(pNew, *lpszDest, nIndex);
			free(*lpszDest);
			*lpszDest = pNew;
		}
	}
	*lpszDest = (char *)realloc(pNew, sizeof(char) * strlen(pNew) + 1); // �������
	if (!*lpszDest)
	{
		free(*lpszDest);
		return FALSE;
	}
	return TRUE;
}


// url���뺯��
static BOOL urldecode(char* *lpszDest, const char* pszUrl, size_t nLen)
{
	size_t i = 0, nIndex = 0;
	char ch;
	char* pNew = NULL;

	*lpszDest = (char *)malloc(nLen * sizeof(char));
	if (NULL == *lpszDest)
		return FALSE;
	memset(*lpszDest, '\0', nLen);

	for (i = 0; i < nLen; ++i) {
		ch = pszUrl[i];
		if (ch != '%') {
			(*lpszDest)[nIndex++] = ch;
		}
		else {
			char h = pszUrl[++i];
			char l = pszUrl[++i];

#ifdef __GUN_C__
			int num = (HEX_TO_DEC(h) << 4) | HEX_TO_DEC(l);
#else
			int num = (hex2dec(h) << 4) | hex2dec(l);
#endif
			(*lpszDest)[nIndex++] = num;
		}
	}
	pNew = (char*)malloc(sizeof(char) * strlen(*lpszDest) + 1);
	if (!pNew)
		return FALSE;

	strncpy(pNew, *lpszDest, strlen(*lpszDest) + 1); // +1����'\0'
	free(*lpszDest);
	*lpszDest = pNew;

	return TRUE;
}
