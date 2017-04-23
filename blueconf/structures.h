#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct device {
    char *name;
    char *mac;

    struct device *next;
} __attribute__ ((packed)) Device;

typedef struct record {
    char *username;
    uint size;
    Device *dListHead;

    struct record *next;
} __attribute__ ((packed)) Record;

typedef struct {
    uint size;
    Record *head;
    int buffhead;
} __attribute__ ((packed)) RecordList;

#ifdef __cplusplus
}
#endif

#endif // STRUCTURES_H_
