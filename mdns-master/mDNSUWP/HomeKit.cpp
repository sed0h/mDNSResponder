// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the Apache 2.0 license.

#include "pch.h"
#include "dns_sd.h"

#include "collection.h"

using namespace std;
using namespace Platform;
using namespace Platform::Collections;

#include "HomeKit.h"

using namespace mDNSUWP;

std::string wstos(std::wstring ws);
std::wstring stows(std::string s);
Platform::String ^stops(std::string s);
std::string pstos(Platform::String^ ps);

HomeKit::HomeKit()
{
	dnsServiceRef = NULL;
}

HomeKit::~HomeKit()
{
	Close();
}

void HomeKit::Close()
{
	if (NULL != dnsServiceRef)
	{
		DNSServiceRefDeallocate(dnsServiceRef);
		dnsServiceRef = NULL;
	}
}

/*
	DNSServiceRegisterReply callback function
*/
static void DNSSD_API HomeKitDNSServiceRegisterReply
(
	DNSServiceRef						sdRef,
	DNSServiceFlags						flags,
	DNSServiceErrorType					errorCode,
	const char                          *name,
	const char                          *regtype,
	const char                          *domain,
	void                                *context
)
{
	if (errorCode == kDNSServiceErr_NoError)
	{
		OutputDebugString(L"Successfuly registered");
	}
	else
	{ 
		OutputDebugString(L"Failed to register");
	}
}

int HomeKit::Register(String^ instanceName, String^ instanceType, String^ domainName, unsigned short port, 
													Windows::Foundation::Collections::IMap<String^,String^> ^ txtRecords)
{
	uint16_t servicePort = ((port & 0x00ff) << 8) | ((port & 0xff00) >> 8);

	// Passing an UWP Object^this to an unmanaged DLL function? Bang candidate
	// Platform::Object^ thisObject = reinterpret_cast<Object^>(this);
	// void * context = (void*)reinterpret_cast<IInspectable*>(thisObject);

	std::string instance	= pstos(instanceName);
	std::string type		= pstos(instanceType);
	std::string domain		= pstos(domainName);

	uint32_t		opinterface = kDNSServiceInterfaceIndexAny;
	DNSServiceFlags	flags		= 0;

	uint16_t			txtLen = 0;
	void * DNSSD_API	pData = NULL;
	TXTRecordRef		txtRef;

	if (nullptr != txtRecords)
	{
		TXTRecordCreate(&txtRef, 0, NULL);

		for each(auto item in txtRecords)
		{
			std::string key = pstos(item->Key);
			std::string value = pstos(item->Value);

			TXTRecordSetValue(&txtRef, key.c_str(), value.length(), value.c_str());
		};

		pData = (void *) TXTRecordGetBytesPtr(&txtRef);
		txtLen = TXTRecordGetLength(&txtRef);
	}

	DNSServiceErrorType serviceError = DNSServiceRegister(&dnsServiceRef, flags, opinterface, 
		instance.c_str(), type.c_str(), domain.c_str(), NULL,
		servicePort, txtLen, pData, HomeKitDNSServiceRegisterReply, NULL);

	if (nullptr != txtRecords)
		TXTRecordDeallocate(&txtRef);

	return (int)serviceError;
}

// string helpers
std::wstring stows(std::string s)
{
	std::wstring ws;
	ws.assign(s.begin(), s.end());
	return ws;
}

std::string wstos(std::wstring ws)
{
	std::string s;
	s.assign(ws.begin(), ws.end());
	return s;
}

Platform::String ^stops(std::string s)
{
	return ref new Platform::String(stows(s).c_str());
}

std::string pstos(Platform::String^ ps)
{
	return wstos(std::wstring(ps->Data()));
}

Platform::String ^atops(const char *text)
{
	return stops(std::string(text));
}
