#ifndef __PACKET_POOL_H__
#define __PACKET_POOL_H__

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define PACKET_POOL_ID (0x12345678)
#define ALIGN_TYPE      unsigned long
#define PACKET_ALIGNMENT sizeof(unsigned long)
#define ALIGN_UP(addr, align_type, alignment) ((((align_type)addr + alignment  - 1) / alignment) * alignment)

//extern struct packet_pool_s;

typedef struct packet_s
{
    char *data_start;                     /* the buffer start address */
    char *data_end;                       /* the buffer end address */
    char *prepend_ptr;                    /* used to prepend information in front of the packet. */
    char *append_ptr;                     /* Define the pointer to the byte after the last character written in the buffer.  */
    struct packet_s *packet_next;         /* next packet ptr*/
    struct packet_pool_s *pool_owner;     /* The packet pool of the packet belongs */
} packet_t;


typedef struct packet_pool_s
{
    u32 pool_id;                        /* pool ID used for error checking */
    char *pool_name;                    /* the pool's name */
    char *pool_start;                   /* pool buffer start address */
    u32 pool_size;                      /* pool buffer size */
    u32 payload_size;                   /* payload size, not include packet_t size */
    u32 pool_available;                 /* free packet count int pool */
    u32 pool_total;                     /* total packet count in pool */
    packet_t *packet_list;              /* packet list header */
    struct packet_pool_s *pool_prev;    /* previous pool pointer */
    struct packet_pool_s *pool_next;    /* next pool pointer */
} packet_pool_t;

packet_pool_t *g_packet_pool_list = NULL;
u32 g_packet_pool_cnt = 0;

#endif
