// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the Apache 2.0 license.

#pragma once

namespace mDNSUWP
{
    public ref class HomeKit sealed
    {
	private:
		DNSServiceRef dnsServiceRef;

    public:
		HomeKit();
		virtual ~HomeKit();

		int Register(String^ instanceName, String^ instanceType, String^ domain, unsigned short port, Windows::Foundation::Collections::IMap<String^, String^> ^ txtRecords);
		void Close();
    };
}
