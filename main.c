#include "packet_pool.h"
#include <stdio.h>
#include <string.h>

char buf[2047];


void print_pool_info(packet_pool_t *pool_ptr)
{
    if (NULL == pool_ptr)
    {
        return;
    }

    printf("pool info:\n");
    printf("pool_id:%#x\n", pool_ptr->pool_id);
    printf("pool_name:%s\n", pool_ptr->pool_name);
    printf("pool_start:%p\n", pool_ptr->pool_start);
    printf("pool_size:%d\n", pool_ptr->pool_size);
    printf("payload_size:%d\n", pool_ptr->payload_size);
    printf("pool_available:%d\n", pool_ptr->pool_available);
    printf("pool_total:%d\n", pool_ptr->pool_total);
    printf("packet_list:%p\n", pool_ptr->packet_list);
}

void print_packet_info(packet_t *packet_ptr)
{
    if (NULL == packet_ptr)
    {
        return;
    }

    printf("\npacket info:\n");
    printf("data_start:%p\n", packet_ptr->data_start);
    printf("data_end:%p\n", packet_ptr->data_end);
    printf("prepend_ptr:%p\n", packet_ptr->prepend_ptr);
    printf("append_ptr:%p\n", packet_ptr->append_ptr);
    printf("data_size:%d\n", packet_ptr->data_size);
    printf("packet_size:%d\n", packet_ptr->packet_size);
    printf("packet_next:%p\n", packet_ptr->packet_next);
    printf("pool_owner:%p\n\n", packet_ptr->pool_owner);
}

int main(void)
{
    int ret;
    packet_pool_t pool;
    packet_t *packet_ptr0 = NULL;
    packet_t *packet_ptr1 = NULL;
    packet_t *packet_ptr2 = NULL;
    packet_t *packet_ptr3 = NULL;
    packet_t *packet_ptr4 = NULL;
    packet_t *packet_ptr5 = NULL;
    packet_t *packet_ptr6 = NULL;

    printf("pool_ptr:%p\n", &pool);

    ret = packet_pool_create(&pool, "pool_test", 261, buf, 2047);
    print_pool_info(&pool);

    ret = packet_allocate(&pool, &packet_ptr0, 10);
    printf("\npacket_ptr0 L%d ret:%d\n", __LINE__, ret);
    print_pool_info(&pool);
    print_packet_info(packet_ptr0);

    ret = packet_allocate(&pool, &packet_ptr1, 10);
    printf("packet_ptr1 L%d ret:%d\n", __LINE__, ret);
    print_pool_info(&pool);
    print_packet_info(packet_ptr1);

    ret = packet_allocate(&pool, &packet_ptr2, 10);
    printf("packet_ptr2 L%d ret:%d\n", __LINE__, ret);
    print_pool_info(&pool);
    print_packet_info(packet_ptr2);

    ret = packet_allocate(&pool, &packet_ptr3, 10);
    printf("packet_ptr3 L%d ret:%d\n", __LINE__, ret);
    print_pool_info(&pool);
    print_packet_info(packet_ptr3);

    ret = packet_allocate(&pool, &packet_ptr4, 10);
    printf("packet_ptr4 L%d ret:%d\n", __LINE__, ret);
    print_pool_info(&pool);
    print_packet_info(packet_ptr4);
    
    ret = packet_allocate(&pool, &packet_ptr5, 10);
    printf("packet_ptr5 L%d ret:%d\n", __LINE__, ret);
    print_pool_info(&pool);
    print_packet_info(packet_ptr5);

    ret = packet_allocate(&pool, &packet_ptr6, 10);
    printf("packet_ptr6 L%d ret:%d\n", __LINE__, ret);
    print_pool_info(&pool);
    print_packet_info(packet_ptr6);

    packet_ptr3->prepend_ptr -= 10;
    memset(packet_ptr3->data_start, 1, packet_ptr3->packet_size);
    packet_ptr3->append_ptr = packet_ptr3->data_start + packet_ptr3->packet_size;
    ret = packet_release(packet_ptr3);
    printf("\nafter release L%d ret:%d\n", __LINE__, ret);
    print_pool_info(&pool);

    ret = packet_allocate(&pool, &packet_ptr6, 10);
    printf("\npacket_ptr6 L%d ret:%d\n", __LINE__, ret);
    print_pool_info(&pool);
    print_packet_info(packet_ptr6);

    return 0;
}
