# URL编码与解码（C语言版本）

static BOOL urlencode(char* *lpszDest, const char* cszSrc, size_t nLen); // 编码
static BOOL urldecode(char* *lpszDest, const char* pszUrl, size_t nLen); // 解码