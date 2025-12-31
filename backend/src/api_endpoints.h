#ifndef API_ENDPOINTS_H
#define API_ENDPOINTS_H

#include <mantisbase/mantisbase.h>
#include <mantisbase/core/router.h>
#include <nlohmann/json.hpp>

using namespace mb;
using json = nlohmann::json;

// Setup all API endpoints
void setupAllEndpoints(Router& router, MantisBase& app);

// User management endpoints
void setupUserEndpoints(Router& router, MantisBase& app);

// Stock management endpoints
void setupStockEndpoints(Router& router, MantisBase& app);

// Sales endpoints
void setupSalesEndpoints(Router& router, MantisBase& app);

// Creditee endpoints
void setupCrediteeEndpoints(Router& router, MantisBase& app);

// Dashboard/Statistics endpoints
void setupDashboardEndpoints(Router& router, MantisBase& app);

// Product/Item endpoints
void setupProductEndpoints(Router& router, MantisBase& app);

#endif // API_ENDPOINTS_H

