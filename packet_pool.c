#include "packet_pool.h"
#include <stdio.h>
#include <string.h>

void packet_pool_init(void)
{
    g_packet_pool_list = NULL;
    g_packet_pool_cnt = 0;
}


int packet_pool_create(packet_pool_t *pool_ptr, char *name, u32 payload_size, void *pool_start, u32 pool_size)
{
    if ((NULL == pool_ptr) || (NULL == name) || (NULL == pool_start) || 
        (0 == payload_size) || (0 == pool_size))
    {
        return -1;
    }

    packet_pool_t *tail_pool_ptr;   /* Working packet pool pointer */
    u32 packet_cnt;                 /* Number of packets in pool   */
    void *align_pool_start;
    char *payload_address;            /* Address of the first payload*/
    char *cur_packet_ptr;             /* Working packet pointer      */
    char *next_packet_ptr;            /* Next packet pointer         */
    u32 header_size;                /* Rounded header size         */
    char *end_of_pool;                /* End of pool area            */
    u32 original_payload_size;      /* Original payload size       */

    /* Save the original payload size.  */
    original_payload_size =  payload_size;

    align_pool_start = (void *)((((ALIGN_TYPE)pool_start + PACKET_ALIGNMENT  - 1) / PACKET_ALIGNMENT) * PACKET_ALIGNMENT);

    pool_size = (u32)(((pool_size - ((ALIGN_TYPE)align_pool_start - (ALIGN_TYPE)pool_start)) / PACKET_ALIGNMENT) * PACKET_ALIGNMENT);

    payload_address = (char *)((ALIGN_TYPE)align_pool_start + sizeof(packet_t));
    payload_address = (char *)((((ALIGN_TYPE)payload_address + PACKET_ALIGNMENT  - 1) / PACKET_ALIGNMENT) * PACKET_ALIGNMENT);

    header_size = (u32)((ALIGN_TYPE)payload_address - (ALIGN_TYPE)align_pool_start);

    payload_size = (u32)(((header_size + payload_size + PACKET_ALIGNMENT  - 1) / PACKET_ALIGNMENT) * PACKET_ALIGNMENT - header_size);

    /* Clear pool fields. */
    memset(pool_ptr, 0, sizeof(packet_pool_t));

    /* Setup the basic packet pool fields.  */
    pool_ptr->pool_name = name;
    pool_ptr->packet_list =  NULL;
    pool_ptr->pool_start = (char *)pool_start;
    pool_ptr->pool_size = pool_size;
    pool_ptr->payload_size = original_payload_size;

    end_of_pool =  ((char *)pool_start) + pool_size;

    packet_cnt = 0;
    cur_packet_ptr = (char *)align_pool_start;
    next_packet_ptr =    cur_packet_ptr + (payload_size + header_size);

    while (next_packet_ptr <= end_of_pool)
    {
        /* Yes, we have another packet.  Increment the packet count.  */
        packet_cnt++;

        ((packet_t *)cur_packet_ptr)->packet_next = (packet_t *)next_packet_ptr;

        ((packet_t *)cur_packet_ptr)->pool_owner = pool_ptr;

        ((packet_t *)cur_packet_ptr)->data_start = (u8 *)(cur_packet_ptr + header_size);

        ((packet_t *)cur_packet_ptr)->data_end = (u8 *)(cur_packet_ptr + header_size + original_payload_size);

        /* advance to the next packet.  */
        cur_packet_ptr =   next_packet_ptr;

        /* update the next packet pointer.  */
        next_packet_ptr =  cur_packet_ptr + (payload_size + header_size);
    }

    /* Backup to the last packet in the pool.  */
    cur_packet_ptr =  cur_packet_ptr - (payload_size + header_size);

    ((packet_t *)cur_packet_ptr) -> packet_next = NULL;

    /* Save the remaining information in the pool control packet.  */
    pool_ptr->pool_available = packet_cnt;
    pool_ptr->pool_total = packet_cnt;

    /* Set the packet pool available list.  */
    pool_ptr->packet_list = (packet_t *)pool_start;

    pool_ptr->pool_id = PACKET_POOL_ID;

    if (g_packet_pool_list)
    {

        /* Pickup tail pointer.  */
        tail_pool_ptr =  g_packet_pool_list->pool_prev;

        /* Place the new packet pool in the list.  */
        g_packet_pool_list->pool_prev = pool_ptr;
        tail_pool_ptr->pool_next = pool_ptr;

        /* Setup this packet pool's created links.  */
        pool_ptr->pool_prev = tail_pool_ptr;
        pool_ptr->pool_next = g_packet_pool_list;
    }
    else
    {

        /* The created packet pool list is empty.  Add packet pool to empty list.  */
        g_packet_pool_list = pool_ptr;
        pool_ptr->pool_next = pool_ptr;
        pool_ptr->pool_prev = pool_ptr;
    }

    /* Increment the number of packet pools created.  */
    g_packet_pool_cnt++;

    return 0;
}

int packet_allocate(packet_pool_t *pool_ptr,  packet_t **packet_ptr, u32 header_size)
{
    packet_t *work_ptr;            /* Working packet pointer  */

    if ((NULL == pool_ptr) || (NULL == packet_ptr))
    {
        return -1;
    }

    if (pool_ptr->payload_size < header_size)
    {
        return -1;
    }

    if (0 == pool_ptr->pool_available)
    {
        return -1;
    }
}