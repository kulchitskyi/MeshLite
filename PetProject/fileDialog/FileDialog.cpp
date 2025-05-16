#include "fileDialog.h"

#include <windows.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

std::string FileDialog::openFile(const char* filter)
{
	OPENFILENAMEA OFN;
	CHAR szFile[260] = { 0 };
	ZeroMemory(&OFN, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = szFile;
	OFN.nMaxFile = sizeof(szFile);
	OFN.lpstrFilter = filter;
	OFN.nFilterIndex = 1;
	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileNameA(&OFN) == TRUE)
	{
		return OFN.lpstrFile;
	}
	return std::string();
}