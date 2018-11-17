#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <lab/vector.h>

typedef enum lab_vm_registers_e_t {

    LAB_VM_REG_ZERO, // Zero
    LAB_VM_REG_RA,   // Return address for jump
    LAB_VM_REG_SP,   // Stack pointer
    LAB_VM_REG_GP,   // Global pointer
    LAB_VM_REG_TP,   // Thread pointer

    LAB_VM_REG_T0,   // Temporary register
    LAB_VM_REG_T1,   // Temporary register
    LAB_VM_REG_T2,   // Temporary register

    LAB_VM_REG_FP,   // Saved register or frame pointer
    LAB_VM_REG_S0    // Saved register or frame pointer
    =LAB_VM_REG_FP,  // Saved register or frame pointer
    LAB_VM_REG_S1,   // Saved register

    LAB_VM_REG_A0,   // Function argument
    LAB_VM_REG_A1,   // Function argument
    LAB_VM_REG_A2,   // Function argument
    LAB_VM_REG_A3,   // Function argument
    LAB_VM_REG_A4,   // Function argument
    LAB_VM_REG_A5,   // Function argument
    LAB_VM_REG_A6,   // Function argument
    LAB_VM_REG_A7,   // Function argument

    LAB_VM_REG_S2,   // Saved register 
    LAB_VM_REG_S3,   // Saved register 
    LAB_VM_REG_S4,   // Saved register 
    LAB_VM_REG_S5,   // Saved register 
    LAB_VM_REG_S6,   // Saved register 
    LAB_VM_REG_S7,   // Saved register 
    LAB_VM_REG_S8,   // Saved register 
    LAB_VM_REG_S9,   // Saved register 
    LAB_VM_REG_S10,  // Saved register 
    LAB_VM_REG_S11,  // Saved register 

    LAB_VM_REG_T3,   // Temporary register
    LAB_VM_REG_T4,   // Temporary register
    LAB_VM_REG_T5,   // Temporary register
    LAB_VM_REG_T6,   // Temporary register

} lab_vm_registers_e_t;

typedef enum lab_vm_op_e_t {

    // Math operators

    LAB_VM_OP_ADD,  // dest, src1, src2
    LAB_VM_OP_ADDI, // dest, src1, I ( dest = src1 + I )
    LAB_VM_OP_SUB,  // dest, src1, src2
    LAB_VM_OP_MUL,  // dest, src1, src2
    LAB_VM_OP_DIV,  // dest, src1, src2
    LAB_VM_OP_REM,  // dest, src1, src2

    // Jump operators

    LAB_VM_OP_JAL,  // dest, J

    // Conditional jump operators

    LAB_VM_OP_BEQ,  // src1, src2, I ==
    LAB_VM_OP_BNE,  // src1, src2, I !=
    LAB_VM_OP_BLT,  // src1, src2, I <
    LAB_VM_OP_BGE,  // src1, src2, I >=

} lab_vm_op_e_t;

typedef struct lab_vm_t {

    lab_vec_t stack;
    lab_vec_t instructions;
    size_t    ip; // Instruction pointer

} lab_vm_t;

bool lab_vm_init(lab_vm_t* vm) {
    if(!lab_vec_init(&vm->stack, sizeof(uint8_t), 32 * sizeof(size_t))) {
        return false;
    }
    if(!lab_vec_init(&vm->instructions, sizeof(size_t), 0)) {
        return false;
    }

    lab_vec_push_back_arr(&vm->stack, NULL, 32* sizeof(size_t));

    for(size_t i = 0; i < lab_vec_size(&vm->stack); i += sizeof(size_t)) {
        *(size_t*)lab_vec_at(&vm->stack, i) = 0;
    }

    vm->ip = 0;

    return true;
}

void lab_vm_free(lab_vm_t* vm) {
    lab_vec_free(&vm->stack);
}

bool lab_vm_eval_op(lab_vm_t* vm, size_t opcode) {
    switch ((lab_vm_op_e_t)opcode) {
        case LAB_VM_OP_ADD: {

            size_t* dest = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* a    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* b    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));

            *dest = *a + *b;
        }
        break;

        case LAB_VM_OP_ADDI: {

            size_t* dest = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* a    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* b    =                                 (++vm->ip,  (size_t*)lab_vec_at(&vm->instructions, vm->ip));

            *dest = *a + *b;
        }
        break;

        case LAB_VM_OP_SUB: {

            size_t* dest = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* a    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* b    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));

            *dest = *a - *b;

        }
        break;

        case LAB_VM_OP_MUL: {

            size_t* dest = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* a    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* b    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));

            *dest = *a * *b;

        }
        break;

        case LAB_VM_OP_DIV: {

            size_t* dest = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* a    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* b    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));

            *dest = *a / *b;

        }
        break;

        case LAB_VM_OP_REM: {

            size_t* dest = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* a    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* b    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));

            *dest = *a % *b;

        }
        break;


        case LAB_VM_OP_JAL: {

            size_t* a    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* dest =                                 (++vm->ip,  (size_t*)lab_vec_at(&vm->instructions, vm->ip));

            vm->ip = *dest;
            return true;

        }
        break;

        case LAB_VM_OP_BEQ: {

            size_t* a    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* b    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* dest =                                 (++vm->ip,  (size_t*)lab_vec_at(&vm->instructions, vm->ip));

            if(*a == *b) {
                vm->ip = *dest;
                return true;
            }

        }
        break;

        case LAB_VM_OP_BNE: {

            size_t* a    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* b    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* dest =                                 (++vm->ip,  (size_t*)lab_vec_at(&vm->instructions, vm->ip));

            if(*a != *b) {
                vm->ip = *dest;
                return true;
            }

        }
        break;

        case LAB_VM_OP_BLT: {

            size_t* a    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* b    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* dest =                                 (++vm->ip,  (size_t*)lab_vec_at(&vm->instructions, vm->ip));

            if(*a < *b) {
                vm->ip = *dest;
                return true;
            }

        }
        break;

        case LAB_VM_OP_BGE: {

            size_t* a    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* b    = (size_t*)lab_vec_at(&vm->stack, (++vm->ip, *(size_t*)lab_vec_at(&vm->instructions, vm->ip)));
            size_t* dest =                                 (++vm->ip,  (size_t*)lab_vec_at(&vm->instructions, vm->ip));

            if(*a >= *b) {
                vm->ip = *dest;
                return true;
            }

        }
        break;

        default:
            return false;
    }

    ++vm->ip;

    return true;
}

bool lab_vm_run(lab_vm_t* vm) {
    for(vm->ip = 0; vm->ip < lab_vec_size(&vm->instructions);) {
        if(!lab_vm_eval_op(vm, *(size_t*)lab_vec_at(&vm->instructions, vm->ip))) {
            return false;
        }
    }
    return true;
}

bool lab_vm_load_instructions(lab_vm_t* vm, size_t* instructions, size_t count) {
    return lab_vec_push_back_arr(&vm->instructions, instructions, count) != NULL;
}

bool lab_vm_unload_instructions(lab_vm_t* vm) {
    return lab_vec_resize(&vm->instructions, 0);
}

int main() {

    lab_vm_t vm;
    lab_vm_init(&vm);

    size_t bytecode[] = {
        LAB_VM_OP_ADDI, LAB_VM_REG_A0 * sizeof(size_t), LAB_VM_REG_ZERO * sizeof(size_t), 15, // addI a0, zero, 15
        LAB_VM_OP_ADDI, LAB_VM_REG_A1 * sizeof(size_t), LAB_VM_REG_A1   * sizeof(size_t), 1,  // addI a1, a1, 1
        LAB_VM_OP_BLT,  LAB_VM_REG_A1 * sizeof(size_t), LAB_VM_REG_A0   * sizeof(size_t), 4,  // blt a1, a0, 4
    };

    lab_vm_load_instructions(&vm, bytecode, sizeof(bytecode) / sizeof(size_t));
    lab_vm_run(&vm);
    lab_vm_unload_instructions(&vm);

    lab_successln("%d", *(size_t*)lab_vec_at(&vm.stack, LAB_VM_REG_A1 * sizeof(size_t)));

    lab_vm_free(&vm);

}