
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

#define DEBUG_Print(...) do{printf(__VA_ARGS__); fflush(stdout); }while(0)

#define DEBUG_WriteBytes(buffer, num, blocking) (void)(printf("%.*s", (num), (buffer)), (num))

typedef struct
{
    bool guiAlphabetTest;
    bool guiMessagesToDebug;
    bool guiMessagesToGUI;
    bool ignore_power_off;
    bool print_message_headers;
    uint32_t alphabet_repeats;
} DEBUG_OPTIONS_T;

extern DEBUG_OPTIONS_T debugOptions;

extern FILE *logfile;

#ifdef __cplusplus
}
#endif
