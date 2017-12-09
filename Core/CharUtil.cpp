
void wtoc(char* Dest, const wchar_t* Source)
{
	int i = 0;

	while (Source[i] != '\0')
	{
		Dest[i] = (char)Source[i];
		++i;
	}

	Dest[i] = '\0';
}

const char *wtoc_s(const wchar_t* Source)
{
	static char szTemp[2048];
	int i = 0;

	while (Source[i] != '\0')
	{
		szTemp[i] = (char)Source[i];
		++i;
	}

	szTemp[i] = '\0';

	return szTemp;
}

void ctow(wchar_t* Dest, const char* Source)
{
	int i = 0;

	while (Source[i] != '\0')
	{
		Dest[i] = (wchar_t)Source[i];
		++i;
	}

	Dest[i] = 0;
}

const wchar_t *ctow_s(const char* Source)
{
	static wchar_t szTemp[2048];
	int i = 0;

	while (Source[i] != '\0')
	{
		szTemp[i] = (wchar_t)Source[i];
		++i;
	}

	szTemp[i] = 0;
	return szTemp;
}