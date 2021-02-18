
#include <Windows.h>
#include <WinHttp.h>
#include <stdio.h>
#include <iostream> //getchar
#include <fstream>
#include <vector>

#pragma comment(lib, "winhttp.lib")

using namespace std;

void get(wstring domain, wstring url)
{	
	BOOL  bResults = FALSE;
	HINTERNET  hSession = nullptr,
		hConnect = nullptr,
		hRequest = nullptr;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP Example/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, domain.c_str(),
			INTERNET_DEFAULT_HTTPS_PORT, 0);

	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET", url.c_str(),
			nullptr, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			WINHTTP_FLAG_SECURE);

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS, 0,
			nullptr, 0,
			0, 0);


	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, nullptr);

	if (bResults) 
	{
		unsigned long sz;
		WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE, nullptr, nullptr, &sz, nullptr);

		std::vector<wchar_t> buf(sz);

		WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE, nullptr, buf.data(), &sz, nullptr);

		std::wcout << "status code: " << buf.data();
	}

	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

}

int main()
{
	get(L"health-infobase.canada.ca", L"/src/data/covidLive/covid19.csv");
}
