#pragma once

// Copy this file to secrets.h and edit values.

static const char* WIFI_SSID = "YOUR_WIFI_SSID";
static const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

#define NETWATCH_COMPACT_URL "http://netpi:8080/api/monitoring/compact"
#define AQUAPI_COMPACT_URL "http://aquapi:8081/api/monitoring/compact"

#define NETWATCH_FETCH_INTERVAL_SECONDS 30
#define AQUAPI_FETCH_INTERVAL_SECONDS 60
#define AUTO_ROTATION_SECONDS 20
