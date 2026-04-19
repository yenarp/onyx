#ifndef STAGE2_DEV_CMOS_H
#define STAGE2_DEV_CMOS_H

#include <stdint.h>

typedef enum {
    CMOS_DRIVE_STATE_NONE = 0x0,
    CMOS_DRIVE_STATE_360K_525 = 0x1,
    CMOS_DRIVE_STATE_12M_525 = 0x2,
    CMOS_DRIVE_STATE_720K_35 = 0x3,
    CMOS_DRIVE_STATE_144M_35 = 0x4,
    CMOS_DRIVE_STATE_288_35 = 0x5,
} CmosDriveState;

typedef enum {
    CMOS_DISPLAY_EGA_VGA = 0x0,
    CMOS_DISPLAY_CGA_40 = 0x1,
    CMOS_DISPLAY_CGA_80 = 0x2,
    CMOS_DISPLAY_MDA = 0x3,
} CmosDisplayType;

#define CMOS_DRIVE_MASTER 0
#define CMOS_DRIVE_SLAVE 1

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint32_t year;
} RTCTime;

typedef struct {
    CmosDriveState drives[2];
    RTCTime last_read_time;

    uint16_t base_memory_kb;
    uint16_t extended_memory_kb;
    CmosDisplayType display_type;
    bool has_fpu;

    bool nmi_enabled;
} CmosState;

void cmos_init(CmosState *state);
void cmos_read_rtc(CmosState *state);

bool cmos_battery_is_dead(CmosState *state);

#endif /* STAGE2_DEV_CMOS_H */
