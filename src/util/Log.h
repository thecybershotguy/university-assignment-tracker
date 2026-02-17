#define CFG_DEBUG 1

#if CFG_DEBUG
#define CFG_LOG(fmt, ...) Serial.printf("[CFG] " fmt "\n", ##__VA_ARGS__)
#else
#define CFG_LOG(fmt, ...)
#endif