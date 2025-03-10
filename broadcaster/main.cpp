#include <cstring>
#include <dns_sd.h>
#include <iostream>

void register_service_callback(DNSServiceRef service, DNSServiceFlags flags,
                               DNSServiceErrorType errorCode, const char *name,
                               const char *regtype, const char *domain,
                               void *context) {
  if (errorCode == kDNSServiceErr_NoError) {
    std::cout << "Service registered successfully!" << std::endl;
    std::cout << "Service Name: " << name << std::endl;
    std::cout << "Service Type: " << regtype << std::endl;
    std::cout << "Domain: " << domain << std::endl;
  } else {
    std::cerr << "Error registering service: " << errorCode << std::endl;
  }
}

int main() {
  DNSServiceRef serviceRef;

  // Register a service with Bonjour
  const char *serviceName = "My HTTP Service";
  const char *serviceType = "_flowy._tcp";
  uint16_t servicePort = 8080;

  // Register the service
  DNSServiceErrorType err = DNSServiceRegister(
      &serviceRef, 0, 0, serviceName, serviceType, nullptr, nullptr,
      htons(servicePort), 0, nullptr, register_service_callback, nullptr);
  if (err != kDNSServiceErr_NoError) {
    std::cerr << "Error registering service: " << err << std::endl;
    return 1;
  }

  // Run the Bonjour service (advertise the service)
  std::cout << "Advertising service: " << serviceName << " on port "
            << servicePort << std::endl;

  // Now keep the event loop running properly to process mDNS queries
  // This will block here while the service is being advertised
  while (true) {
    // Processing the events - this is the blocking call
    DNSServiceProcessResult(serviceRef);
  }

  // Cleanup when done (this line will never be reached in this version)
  DNSServiceRefDeallocate(serviceRef);

  return 0;
}
