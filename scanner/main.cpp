#include <cstdio>
#include <dns_sd.h>

void DNSSD_API serviceResolveCallback(
    DNSServiceRef service, DNSServiceFlags flags, uint32_t interfaceIndex,
    DNSServiceErrorType errorCode, const char *fullName, const char *hostTarget,
    uint16_t port, uint16_t txtLen, const unsigned char *txtRecord,
    void *context) {
  if (errorCode == kDNSServiceErr_NoError) {
    printf("Resolved Service: %s\n", fullName);
    printf("Host: %s\n", hostTarget);
    printf("Port: %u\n", ntohs(port)); // Convert the port to host byte order
  } else {
    printf("Failed to resolve service: %d\n", errorCode);
  }
}

void DNSSD_API serviceBrowseCallback(
    DNSServiceRef service, DNSServiceFlags flags, uint32_t interfaceIndex,
    DNSServiceErrorType errorCode, const char *serviceName, const char *regtype,
    const char *replyDomain, void *context) {
  if (errorCode == kDNSServiceErr_NoError) {
    printf("Service found: %s.%s%s\n", serviceName, regtype,
           replyDomain ? replyDomain : "");

    // Now resolve the service to get port and other details
    DNSServiceRef resolveRef;
    DNSServiceErrorType resolveError =
        DNSServiceResolve(&resolveRef, 0, interfaceIndex, serviceName, regtype,
                          replyDomain, serviceResolveCallback, nullptr);

    if (resolveError != kDNSServiceErr_NoError) {
      printf("Error resolving service: %d\n", resolveError);
    } else {
      // Process the resolved service
      DNSServiceProcessResult(resolveRef);
      DNSServiceRefDeallocate(resolveRef);
    }
  }
}

int browseServices() {
  DNSServiceRef serviceRef;
  DNSServiceErrorType error = DNSServiceBrowse(
      &serviceRef, 0, 0, "_flowy._tcp", NULL, serviceBrowseCallback, NULL);

  if (error != kDNSServiceErr_NoError) {
    fprintf(stderr, "Error browsing services: %d\n", error);
    return -1;
  }

  // Process the mDNS service (this will block until services are discovered)
  DNSServiceProcessResult(serviceRef);

  DNSServiceRefDeallocate(serviceRef);
  return 0;
}

int main() {
  browseServices();
  return 0;
}
