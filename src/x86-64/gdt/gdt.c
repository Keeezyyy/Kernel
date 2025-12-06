
#include "./gdt.h"

gdt_ptr_t gdt_ptr;
tss_entry tss_entries[1];
gdt_entry_t gdt_entries[7];

void gdt_set_entry(gdt_entry_t *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    entry->limit_low = limit & 0xFFFF;
    entry->base_low = base & 0xFFFF;
    entry->base_middle = (base >> 16) & 0xFF;
    entry->access = access;
    entry->granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    entry->base_high = (base >> 24) & 0xFF;
}

void gdt_set_tss(int idx, uint64_t base, uint32_t limit)
{
    gdt_entries[idx].limit_low = limit & 0xFFFF;
    gdt_entries[idx].base_low = base & 0xFFFF;
    gdt_entries[idx].base_middle = (base >> 16) & 0xFF;
    gdt_entries[idx].access = 0x89;
    gdt_entries[idx].granularity = ((limit >> 16) & 0x0F);
    gdt_entries[idx].granularity |= 0;
    gdt_entries[idx].base_high = (base >> 24) & 0xFF;

    gdt_entries[idx + 1].limit_low = (base >> 32) & 0xFFFF;
    gdt_entries[idx + 1].base_low = (base >> 48) & 0xFFFF;
    gdt_entries[idx + 1].base_middle = 0;
    gdt_entries[idx + 1].access = 0;
    gdt_entries[idx + 1].granularity = 0;
    gdt_entries[idx + 1].base_high = 0;
}

void init_gdt()
{
    uint64_t tss_base = (uint64_t)&tss_entries[0];
    uint32_t tss_limit = sizeof(tss_entries) - 1;

    
    gdt_entries[0] = (gdt_entry_t){
        .limit_low = 0,
        .base_low = 0,
        .base_middle = 0,
        .access = 0,
        .granularity = 0,
        .base_high = 0};

    
    gdt_entries[1] = (gdt_entry_t){
        .limit_low = 0x0000,
        .base_low = 0x0000,
        .base_middle = 0x00,
        .access = GDT_KERNEL_CODE_ACCESS,     
        .granularity = GDT_KERNEL_CODE_FLAGS, 
        .base_high = 0x00};

    
    gdt_entries[2] = (gdt_entry_t){
        .limit_low = 0x0000,
        .base_low = 0x0000,
        .base_middle = 0x00,
        .access = GDT_KERNEL_DATA_ACCESS,     
        .granularity = GDT_KERNEL_DATA_FLAGS, 
        .base_high = 0x00};

    
    gdt_entries[3] = (gdt_entry_t){
        .limit_low = 0x0000,
        .base_low = 0x0000,
        .base_middle = 0x00,
        .access = GDT_USER_CODE_ACCESS,     
        .granularity = GDT_USER_CODE_FLAGS, 
        .base_high = 0x00};

    
    gdt_entries[4] = (gdt_entry_t){
        .limit_low = 0x0000,
        .base_low = 0x0000,
        .base_middle = 0x00,
        .access = GDT_USER_DATA_ACCESS,     
        .granularity = GDT_USER_DATA_FLAGS, 
        .base_high = 0x00};

    
    gdt_entries[5] = (gdt_entry_t){
        .limit_low = tss_limit & 0xFFFF,
        .base_low = tss_base & 0xFFFF,
        .base_middle = (tss_base >> 16) & 0xFF,
        .access = 0x89, 
        .granularity = (tss_limit >> 16) & 0x0F,
        .base_high = (tss_base >> 24) & 0xFF};

    
    gdt_entries[6] = (gdt_entry_t){
        .limit_low = (tss_base >> 32) & 0xFFFF,
        .base_low = (tss_base >> 48) & 0xFFFF,
        .base_middle = 0,
        .access = 0,
        .granularity = 0,
        .base_high = 0};

        
    gdt_ptr.base = (uint64_t)&gdt_entries[0];
    gdt_ptr.limit = sizeof(gdt_entries) - 1;

    load_gdtr((void *)&gdt_ptr);
    gdt_reload_segments();

    printf("after gdt load\n");
}
