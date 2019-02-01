#ifndef _BILLING_H_
#define _BILLING_H_

typedef struct {
    unsigned char u8Version;
    unsigned char u8FreeTime;
    unsigned char u8TimingCycle;
    unsigned char u8Price;
    unsigned short u16MaxTiming;
    unsigned char u8VIPDiscount;
} _strBilling;

#endif
