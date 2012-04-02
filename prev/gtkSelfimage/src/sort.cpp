#include "sort.h"


bool WideStringSorter(const wchar_t *a, const wchar_t *b)
{
int l1, l2;

	l1 = wcslen (a);
	l2 = wcslen (b);

	if (l1 > l2){
		if (wmemcmp(a,b,l1) > 0)
			return false;
		return true;
	}else{
		if (wmemcmp(a,b,l2) > 0)
			return false;
		return true;
	}
}
