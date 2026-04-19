#include "dev/cmos.h"
#include "io.h"

#define REG_CTRL 0x70
#define REG_DATA 0x71

#define REG_DIAGNOSTIC 0xe
#define REG_DDT 0x10 /* Diskette Drive Type */

#define REG_EQUIPMENT 0x14
#define REG_BASE_MEM_LOW 0x15
#define REG_BASE_MEM_HIGH 0x16
#define REG_EXT_MEM_LOW 0x17
#define REG_EXT_MEM_HIGH 0x18

/* RTC */
#define REG_RTC_SECONDS 0x0
#define REG_RTC_MINUTES 0x2
#define REG_RTC_HOURS 0x4
#define REG_RTC_DAY 0x7
#define REG_RTC_MONTH 0x8
#define REG_RTC_YEAR 0x9
#define REG_RTC_STATUS_A 0xa
#define REG_RTC_STATUS_B 0xb
#define REG_RTC_CENTURY 0x32

/* Status B bits */
#define STATUS_BCD 0x4 /* If clear values are BCD encoded */
#define STATUS_12H 0x2 /* If clear 12-hour clock */

#define EQUIPMENT_HAS_FPU 0x2

#define DIAG_RTC_POWER_LOST 0x80

#define extract_bits(drive, value)                                                                 \
    (((drive) == CMOS_DRIVE_MASTER) ? (((value) >> 4) & 0xf) : ((value) & 0xf))
#define cmos_ctrl(state, reg) (((!(state)->nmi_enabled) << 7) | (reg))

static uint8_t cmos_read_reg(CmosState *state, uint8_t reg) {
    outb(REG_CTRL, cmos_ctrl(state, reg));
    io_wait();

    return inb(REG_DATA);
}

static uint8_t bcd_to_bin(uint8_t bcd) {
    __asm__ volatile("movb %%al, %%ah\n"
                     "shrb $4, %%ah\n"
                     "andb $0xf, %%al\n"
                     "aad\n"
                     : "+a"(bcd));

    return bcd;
}

static bool rtc_update_in_progress(CmosState *state) {
    return (cmos_read_reg(state, REG_RTC_STATUS_A) & 0x80);
}

void cmos_init(CmosState *state) {
    uint8_t ddt_byte = cmos_read_reg(state, REG_DDT);

    state->drives[CMOS_DRIVE_MASTER] = extract_bits(CMOS_DRIVE_MASTER, ddt_byte);
    state->drives[CMOS_DRIVE_SLAVE] = extract_bits(CMOS_DRIVE_SLAVE, ddt_byte);

    uint8_t equipment = cmos_read_reg(state, REG_EQUIPMENT);
    state->has_fpu = (equipment & EQUIPMENT_HAS_FPU) != 0;
    state->display_type = (equipment >> 4) & 0x3;

    state->base_memory_kb =
        cmos_read_reg(state, REG_BASE_MEM_LOW) | (cmos_read_reg(state, REG_BASE_MEM_HIGH) << 8);
    state->extended_memory_kb =
        cmos_read_reg(state, REG_EXT_MEM_LOW) | (cmos_read_reg(state, REG_EXT_MEM_HIGH) << 8);
}

bool cmos_battery_is_dead(CmosState *state) {
    return (cmos_read_reg(state, REG_DIAGNOSTIC) & DIAG_RTC_POWER_LOST) != 0;
}

void cmos_read_rtc(CmosState *state) {
    if (cmos_battery_is_dead(state))
        return;

    uint8_t century, yr, mon, day, hr, min, sec, verify_sec;

    while (rtc_update_in_progress(state))
        ;

    do {
        while (rtc_update_in_progress(state))
            ;

        sec = cmos_read_reg(state, REG_RTC_SECONDS);
        min = cmos_read_reg(state, REG_RTC_MINUTES);
        hr = cmos_read_reg(state, REG_RTC_HOURS);
        day = cmos_read_reg(state, REG_RTC_DAY);
        mon = cmos_read_reg(state, REG_RTC_MONTH);
        yr = cmos_read_reg(state, REG_RTC_YEAR);
        century = cmos_read_reg(state, REG_RTC_CENTURY);

        verify_sec = cmos_read_reg(state, REG_RTC_SECONDS);

    } while (sec != verify_sec);

    uint8_t status_b = cmos_read_reg(state, REG_RTC_STATUS_B);

    if ((status_b & STATUS_BCD) == 0) {
        sec = bcd_to_bin(sec);
        min = bcd_to_bin(min);

        bool is_pm = (hr & 0x80);
        hr = bcd_to_bin(hr & 0x7F);
        if (is_pm)
            hr |= 0x80;

        day = bcd_to_bin(day);
        mon = bcd_to_bin(mon);
        yr = bcd_to_bin(yr);
        century = bcd_to_bin(century);
    }

    if ((status_b & STATUS_12H) == 0) {
        bool is_pm = (hr & 0x80);
        hr &= 0x7F;

        if (hr == 12)
            hr = 0;

        if (is_pm)
            hr += 12;
    }

    state->last_read_time.second = sec;
    state->last_read_time.minute = min;
    state->last_read_time.hour = hr;
    state->last_read_time.day = day;
    state->last_read_time.month = mon;

    state->last_read_time.year = (century * 100) + yr;
}
