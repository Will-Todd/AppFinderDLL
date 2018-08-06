#include <algorithm>
#include <Windows.h>
#include <shobjidl.h>
#include <propvarutil.h>
#include <strsafe.h>
#include <Shlobj.h>
#include <iostream>
#include <atlbase.h>

#define DLL_EXPORT __declspec( dllexport )   

extern "C" {
	DLL_EXPORT std::string LowerString(std::string str)
	{
		int length = str.size();
		for (int i = 0; i < length; ++i)
		{
			str[i] = tolower(str[i]);
		}

		return str;
	}

	DLL_EXPORT int SearchAppsForString(const char* game_name)
	{
		std::string name_string(game_name);
		name_string = LowerString(name_string);

		LPITEMIDLIST apps_folder = nullptr;
		HRESULT hr = SHGetKnownFolderIDList(FOLDERID_AppsFolder, 0, nullptr, &apps_folder);

		if (SUCCEEDED(hr))
		{
			CComPtr<IShellFolder> shell_folder;

			CoInitialize(nullptr);

			hr = SHBindToObject(nullptr, apps_folder, nullptr, __uuidof(IShellFolder), (PVOID*)&shell_folder);

			if (SUCCEEDED(hr))
			{
				CComPtr<IEnumIDList> app_enumerator;
				hr = shell_folder->EnumObjects(nullptr, SHCONTF_FOLDERS | SHCONTF_INIT_ON_FIRST_NEXT | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN, &app_enumerator);
				if (SUCCEEDED(hr))
				{
					LPITEMIDLIST pidl = nullptr;

					while (app_enumerator->Next(1, &pidl, nullptr) == S_OK)
					{
						STRRET strret_disp_name;
						WCHAR display_name[MAX_PATH];

						shell_folder->GetDisplayNameOf(pidl, SHGDN_NORMAL, &strret_disp_name);
						StrRetToBufW(&strret_disp_name, pidl, display_name, _countof(display_name));

						CoTaskMemFree(pidl);

						std::wstring ws_disp_name(display_name);
						std::string recovered_name(ws_disp_name.begin(), ws_disp_name.end());
						recovered_name = LowerString(recovered_name);

						if (recovered_name.find(name_string) != std::string::npos)
						{
							return 1;
						}
					}
				}
			}
		}
		return 0;
	}
}