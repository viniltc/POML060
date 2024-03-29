
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    uint32_t seconds;
    uint16_t milliseconds;
} CLOCK_T;

extern volatile CLOCK_T clockk; // (check if this is needed)change from clock to clockk !

/**
 * Get the time
 * @param output Where to write the time.
 */
void CLOCK_GetTheTime(CLOCK_T *output);


#ifdef __cplusplus
}
#endif
