#include "./idt.h"

InterruptDescriptor64 idt_entries[256];
IDTR idtr_ptr;

void fill_idts()
{
    // TODO: :)
    idt_set_entry_ex(&idt_entries[0], (uint64_t)isr_routine_1, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[1], (uint64_t)isr_routine_2, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[2], (uint64_t)isr_routine_3, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[3], (uint64_t)isr_routine_4, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[4], (uint64_t)isr_routine_5, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[5], (uint64_t)isr_routine_6, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[6], (uint64_t)isr_routine_7, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[7], (uint64_t)isr_routine_8, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[8], (uint64_t)isr_routine_9, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[9], (uint64_t)isr_routine_10, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[10], (uint64_t)isr_routine_11, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[11], (uint64_t)isr_routine_12, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[12], (uint64_t)isr_routine_13, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[13], (uint64_t)isr_routine_14, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[14], (uint64_t)isr_routine_15, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[15], (uint64_t)isr_routine_16, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[16], (uint64_t)isr_routine_17, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[17], (uint64_t)isr_routine_18, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[18], (uint64_t)isr_routine_19, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[19], (uint64_t)isr_routine_20, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[20], (uint64_t)isr_routine_21, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[21], (uint64_t)isr_routine_22, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[22], (uint64_t)isr_routine_23, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[23], (uint64_t)isr_routine_24, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[24], (uint64_t)isr_routine_25, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[25], (uint64_t)isr_routine_26, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[26], (uint64_t)isr_routine_27, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[27], (uint64_t)isr_routine_28, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[28], (uint64_t)isr_routine_29, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[29], (uint64_t)isr_routine_30, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[30], (uint64_t)isr_routine_31, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[31], (uint64_t)isr_routine_32, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[32], (uint64_t)isr_routine_33, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[33], (uint64_t)isr_routine_34, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[34], (uint64_t)isr_routine_35, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[35], (uint64_t)isr_routine_36, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[36], (uint64_t)isr_routine_37, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[37], (uint64_t)isr_routine_38, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[38], (uint64_t)isr_routine_39, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[39], (uint64_t)isr_routine_40, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[40], (uint64_t)isr_routine_41, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[41], (uint64_t)isr_routine_42, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[42], (uint64_t)isr_routine_43, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[43], (uint64_t)isr_routine_44, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[44], (uint64_t)isr_routine_45, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[45], (uint64_t)isr_routine_46, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[46], (uint64_t)isr_routine_47, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[47], (uint64_t)isr_routine_48, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[48], (uint64_t)isr_routine_49, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[49], (uint64_t)isr_routine_50, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[50], (uint64_t)isr_routine_51, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[51], (uint64_t)isr_routine_52, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[52], (uint64_t)isr_routine_53, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[53], (uint64_t)isr_routine_54, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[54], (uint64_t)isr_routine_55, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[55], (uint64_t)isr_routine_56, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[56], (uint64_t)isr_routine_57, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[57], (uint64_t)isr_routine_58, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[58], (uint64_t)isr_routine_59, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[59], (uint64_t)isr_routine_60, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[60], (uint64_t)isr_routine_61, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[61], (uint64_t)isr_routine_62, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[62], (uint64_t)isr_routine_63, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[63], (uint64_t)isr_routine_64, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[64], (uint64_t)isr_routine_65, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[65], (uint64_t)isr_routine_66, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[66], (uint64_t)isr_routine_67, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[67], (uint64_t)isr_routine_68, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[68], (uint64_t)isr_routine_69, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[69], (uint64_t)isr_routine_70, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[70], (uint64_t)isr_routine_71, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[71], (uint64_t)isr_routine_72, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[72], (uint64_t)isr_routine_73, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[73], (uint64_t)isr_routine_74, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[74], (uint64_t)isr_routine_75, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[75], (uint64_t)isr_routine_76, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[76], (uint64_t)isr_routine_77, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[77], (uint64_t)isr_routine_78, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[78], (uint64_t)isr_routine_79, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[79], (uint64_t)isr_routine_80, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[80], (uint64_t)isr_routine_81, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[81], (uint64_t)isr_routine_82, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[82], (uint64_t)isr_routine_83, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[83], (uint64_t)isr_routine_84, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[84], (uint64_t)isr_routine_85, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[85], (uint64_t)isr_routine_86, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[86], (uint64_t)isr_routine_87, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[87], (uint64_t)isr_routine_88, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[88], (uint64_t)isr_routine_89, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[89], (uint64_t)isr_routine_90, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[90], (uint64_t)isr_routine_91, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[91], (uint64_t)isr_routine_92, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[92], (uint64_t)isr_routine_93, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[93], (uint64_t)isr_routine_94, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[94], (uint64_t)isr_routine_95, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[95], (uint64_t)isr_routine_96, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[96], (uint64_t)isr_routine_97, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[97], (uint64_t)isr_routine_98, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[98], (uint64_t)isr_routine_99, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[99], (uint64_t)isr_routine_100, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[100], (uint64_t)isr_routine_101, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[101], (uint64_t)isr_routine_102, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[102], (uint64_t)isr_routine_103, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[103], (uint64_t)isr_routine_104, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[104], (uint64_t)isr_routine_105, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[105], (uint64_t)isr_routine_106, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[106], (uint64_t)isr_routine_107, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[107], (uint64_t)isr_routine_108, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[108], (uint64_t)isr_routine_109, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[109], (uint64_t)isr_routine_110, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[110], (uint64_t)isr_routine_111, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[111], (uint64_t)isr_routine_112, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[112], (uint64_t)isr_routine_113, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[113], (uint64_t)isr_routine_114, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[114], (uint64_t)isr_routine_115, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[115], (uint64_t)isr_routine_116, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[116], (uint64_t)isr_routine_117, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[117], (uint64_t)isr_routine_118, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[118], (uint64_t)isr_routine_119, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[119], (uint64_t)isr_routine_120, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[120], (uint64_t)isr_routine_121, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[121], (uint64_t)isr_routine_122, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[122], (uint64_t)isr_routine_123, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[123], (uint64_t)isr_routine_124, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[124], (uint64_t)isr_routine_125, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[125], (uint64_t)isr_routine_126, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[126], (uint64_t)isr_routine_127, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[127], (uint64_t)isr_routine_128, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[128], (uint64_t)isr_routine_129, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[129], (uint64_t)isr_routine_130, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[130], (uint64_t)isr_routine_131, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[131], (uint64_t)isr_routine_132, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[132], (uint64_t)isr_routine_133, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[133], (uint64_t)isr_routine_134, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[134], (uint64_t)isr_routine_135, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[135], (uint64_t)isr_routine_136, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[136], (uint64_t)isr_routine_137, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[137], (uint64_t)isr_routine_138, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[138], (uint64_t)isr_routine_139, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[139], (uint64_t)isr_routine_140, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[140], (uint64_t)isr_routine_141, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[141], (uint64_t)isr_routine_142, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[142], (uint64_t)isr_routine_143, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[143], (uint64_t)isr_routine_144, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[144], (uint64_t)isr_routine_145, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[145], (uint64_t)isr_routine_146, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[146], (uint64_t)isr_routine_147, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[147], (uint64_t)isr_routine_148, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[148], (uint64_t)isr_routine_149, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[149], (uint64_t)isr_routine_150, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[150], (uint64_t)isr_routine_151, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[151], (uint64_t)isr_routine_152, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[152], (uint64_t)isr_routine_153, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[153], (uint64_t)isr_routine_154, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[154], (uint64_t)isr_routine_155, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[155], (uint64_t)isr_routine_156, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[156], (uint64_t)isr_routine_157, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[157], (uint64_t)isr_routine_158, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[158], (uint64_t)isr_routine_159, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[159], (uint64_t)isr_routine_160, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[160], (uint64_t)isr_routine_161, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[161], (uint64_t)isr_routine_162, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[162], (uint64_t)isr_routine_163, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[163], (uint64_t)isr_routine_164, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[164], (uint64_t)isr_routine_165, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[165], (uint64_t)isr_routine_166, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[166], (uint64_t)isr_routine_167, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[167], (uint64_t)isr_routine_168, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[168], (uint64_t)isr_routine_169, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[169], (uint64_t)isr_routine_170, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[170], (uint64_t)isr_routine_171, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[171], (uint64_t)isr_routine_172, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[172], (uint64_t)isr_routine_173, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[173], (uint64_t)isr_routine_174, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[174], (uint64_t)isr_routine_175, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[175], (uint64_t)isr_routine_176, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[176], (uint64_t)isr_routine_177, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[177], (uint64_t)isr_routine_178, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[178], (uint64_t)isr_routine_179, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[179], (uint64_t)isr_routine_180, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[180], (uint64_t)isr_routine_181, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[181], (uint64_t)isr_routine_182, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[182], (uint64_t)isr_routine_183, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[183], (uint64_t)isr_routine_184, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[184], (uint64_t)isr_routine_185, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[185], (uint64_t)isr_routine_186, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[186], (uint64_t)isr_routine_187, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[187], (uint64_t)isr_routine_188, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[188], (uint64_t)isr_routine_189, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[189], (uint64_t)isr_routine_190, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[190], (uint64_t)isr_routine_191, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[191], (uint64_t)isr_routine_192, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[192], (uint64_t)isr_routine_193, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[193], (uint64_t)isr_routine_194, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[194], (uint64_t)isr_routine_195, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[195], (uint64_t)isr_routine_196, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[196], (uint64_t)isr_routine_197, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[197], (uint64_t)isr_routine_198, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[198], (uint64_t)isr_routine_199, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[199], (uint64_t)isr_routine_200, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[200], (uint64_t)isr_routine_201, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[201], (uint64_t)isr_routine_202, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[202], (uint64_t)isr_routine_203, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[203], (uint64_t)isr_routine_204, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[204], (uint64_t)isr_routine_205, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[205], (uint64_t)isr_routine_206, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[206], (uint64_t)isr_routine_207, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[207], (uint64_t)isr_routine_208, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[208], (uint64_t)isr_routine_209, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[209], (uint64_t)isr_routine_210, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[210], (uint64_t)isr_routine_211, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[211], (uint64_t)isr_routine_212, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[212], (uint64_t)isr_routine_213, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[213], (uint64_t)isr_routine_214, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[214], (uint64_t)isr_routine_215, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[215], (uint64_t)isr_routine_216, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[216], (uint64_t)isr_routine_217, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[217], (uint64_t)isr_routine_218, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[218], (uint64_t)isr_routine_219, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[219], (uint64_t)isr_routine_220, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[220], (uint64_t)isr_routine_221, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[221], (uint64_t)isr_routine_222, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[222], (uint64_t)isr_routine_223, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[223], (uint64_t)isr_routine_224, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[224], (uint64_t)isr_routine_225, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[225], (uint64_t)isr_routine_226, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[226], (uint64_t)isr_routine_227, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[227], (uint64_t)isr_routine_228, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[228], (uint64_t)isr_routine_229, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[229], (uint64_t)isr_routine_230, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[230], (uint64_t)isr_routine_231, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[231], (uint64_t)isr_routine_232, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[232], (uint64_t)isr_routine_233, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[233], (uint64_t)isr_routine_234, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[234], (uint64_t)isr_routine_235, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[235], (uint64_t)isr_routine_236, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[236], (uint64_t)isr_routine_237, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[237], (uint64_t)isr_routine_238, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[238], (uint64_t)isr_routine_239, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[239], (uint64_t)isr_routine_240, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[240], (uint64_t)isr_routine_241, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[241], (uint64_t)isr_routine_242, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[242], (uint64_t)isr_routine_243, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[243], (uint64_t)isr_routine_244, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[244], (uint64_t)isr_routine_245, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[245], (uint64_t)isr_routine_246, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[246], (uint64_t)isr_routine_247, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[247], (uint64_t)isr_routine_248, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[248], (uint64_t)isr_routine_249, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[249], (uint64_t)isr_routine_250, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);

    idt_set_entry_ex(&idt_entries[250], (uint64_t)isr_routine_251, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[251], (uint64_t)isr_routine_252, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[252], (uint64_t)isr_routine_253, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[253], (uint64_t)isr_routine_254, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[254], (uint64_t)isr_routine_255, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
    idt_set_entry_ex(&idt_entries[255], (uint64_t)isr_routine_256, 0, 1, PRIVILEGE_RING_0_KERNEL, INTERRUPT_GATE_TYPE, KERNEL_CODE_SELECTOR);
}

void idt_set_entry_ex(InterruptDescriptor64 *entry,
                      uint64_t handler,
                      uint8_t ist,
                      uint8_t present,
                      uint8_t dpl,
                      uint8_t gate_type,
                      uint16_t selector)
{
    entry->offset_1 = handler & 0xFFFF;
    entry->selector = selector;

    entry->ist = ist & 0x07;

    uint8_t type_attr = 0;
    type_attr |= (present & 0x01) << 7;
    type_attr |= (dpl & 0x03) << 5;
    type_attr |= (gate_type & 0x0F);

    entry->type_attributes = type_attr;

    entry->offset_2 = (handler >> 16) & 0xFFFF;
    entry->offset_3 = (handler >> 32) & 0xFFFFFFFF;

    entry->zero = 0;
}

void init_idt()
{
    // TODO: implement the IST for the idt entries
    fill_idts();

    idtr_ptr.offset = (uint64_t)&idt_entries[0];
    idtr_ptr.size = sizeof(idt_entries) - 1;

    load_idtr((void *)&idtr_ptr);
}

void interrupt_handler(uint64_t isr_num)
{
    printf("interrupt! %x\n", isr_num);
    for (;;)
    {
    }
}
