#pragma once

#define NETWATCH_COMPACT_URL "http://netpi:8080/api/monitoring/compact"
#define AQUAPI_COMPACT_URL "http://aquapi:8080/api/monitoring/compact"
#define AQUAPI_LEAK_LATEST_URL "http://aquapi:8080/api/leak/latest"
#define AQUAPI_TANKS_LATEST_URL "http://aquapi:8080/api/tanks/latest"

#define NETWATCH_FETCH_INTERVAL_SECONDS 30
#define AQUAPI_FETCH_INTERVAL_SECONDS 60
#define AQUAPI_ALERT_FETCH_INTERVAL_SECONDS 15

#define AUTO_CALENDAR_SECONDS 30
#define AUTO_NETWATCH_SECONDS 15
#define AUTO_AQUAPI_SECONDS 15

#define ALARM_BEEP_ON_MS 120
#define ALARM_BEEP_OFF_MS 380
#define ALARM_BEEP_FREQUENCY_HZ 2200
#define ALARM_VOLUME 255
