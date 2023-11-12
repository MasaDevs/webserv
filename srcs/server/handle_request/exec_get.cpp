#include "handle_request.hpp"

namespace server {

namespace handle_request {
    HttpResponse executeGet(const HttpRequest& request, const ServerDirective& server_directive) {
        HttpResponse response;
        std::map<std::string, LocationDirective> locations = server_directive.getLocations();
        const std::map<std::string, LocationDirective>::iterator it = locations.find(request.getRequestPath());
        if (it == locations.end())
            executeError(request, server_directive);
        return (response);
    }
};
};