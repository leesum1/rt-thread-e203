/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-30     lizhirui     first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "board.h"
#include "tick.h"

#include "drv_uart.h"
#include "encoding.h"
#include "stack.h"
#include "riscv.h"
#include "stack.h"
#include "riscv_io.h"
#include "interrupt.h"
#ifdef CONFIG_RISCV_S_MODE
#include "sbi.h"
#endif

void primary_cpu_entry(void)
{
    extern void entry(void);

    /* disable global interrupt */
    // rt_memset(&__bss_start, 0x0, (rt_uint8_t*)&__bss_end - (rt_uint8_t*)&__bss_start);

    rt_hw_interrupt_disable();
    entry();
}


void rt_hw_board_init(void)
{
    /* initalize interrupt */
    rt_hw_interrupt_init();
    /* initialize hardware interrupt */
    
    rt_hw_uart_init();

#ifdef RT_USING_HEAP
    /* initialize memory system */
    rt_system_heap_init(RT_HW_HEAP_BEGIN, RT_HW_HEAP_END);
#endif

#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
    /* set console device */
    if (rt_console_set_device("uart") ==RT_NULL){

    }

    rt_kprintf("rt_console_set_device\n");
#endif

    rt_hw_tick_init();
    rt_kprintf("heap: [0x%08x - 0x%08x]\n", (rt_ubase_t) RT_HW_HEAP_BEGIN, (rt_ubase_t) RT_HW_HEAP_END);

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
}

#ifdef CONFIG_RISCV_S_MODE
static void cmd_shutdown(void)
{
    sbi_shutdown();
    while(1);
}
MSH_CMD_EXPORT_ALIAS(cmd_shutdown, shutdown, shutdown qemu-virt64);
#endif
