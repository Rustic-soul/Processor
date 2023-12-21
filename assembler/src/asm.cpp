#include <cstdio>
#include <stdlib.h>

#include "../include/asm.h"
#include "../../library/commands.h"
#include "../../library/color.h"
#include "../../library/error.h"

int main(int argc, char *argv[])
{
    PRINT_INFO("\n___%sWORKING ASSEMBLER%s___\n\n", RED, RESET);

    char test_ch = 2;
    printf("<%d><%c>\n", test_ch, (test_ch | 130));

    OPEN_LOG_FIlE();
    FILE *fp_src = fopen(argv[1], "r");
    CHECK_OPEN_FILE(fp_src);   

    FILE *fp_res = fopen(argv[2], "wb");
    CHECK_OPEN_FILE(fp_res);   

    create_byte_code(fp_src, fp_res);

    fclose(fp_res);
    fclose(fp_src);
    
    CLOSE_LOG_FILE();
}

int create_byte_code(FILE *fp_src, FILE *fp_res)
{
    assert(fp_res != NULL);
    assert(fp_src != NULL);

    PRINT_LOG("\n\n\n---NEW_LOG_INFORMATION---\n");
    
    struct Array *src_struct_arr = ctor_struct_arr(fp_src);
    assert(src_struct_arr != NULL);

    struct Label *arr_label = (struct Label*)calloc(100, sizeof(struct Label));
    assert(arr_label != NULL);

    struct Array *res_struct_arr = (struct Array*)calloc(1, sizeof(struct Array));
    assert(res_struct_arr != NULL);

    assembly(src_struct_arr, res_struct_arr, arr_label, 1);
    assembly(src_struct_arr, res_struct_arr, arr_label, 2);

    res_struct_arr->arr_ptr[res_struct_arr->size_arr] = (char)cmd_hlt;
    
    PRINT_LOG("NAME_CMD <%-7s> NUM_CMD <%-2d> " , commands[cmd_hlt] + 4, cmd_hlt);
    PRINT_LOG("PRM<%-9s> CMD_ID <%lu>\n", "---", res_struct_arr->size_arr);

    PRINT_INFO("name_cmd: %s[%4s]%s", RED, "hlt", RESET);
    PRINT_INFO("%s[%2d]%s\n", MAGENTA, cmd_hlt, RESET);

    fwrite(res_struct_arr->arr_ptr, sizeof(char), res_struct_arr->size_arr + 1, fp_res);

    free(src_struct_arr->arr_ptr);
    free(src_struct_arr);

    free(res_struct_arr->arr_ptr);
    free(res_struct_arr);

    free(arr_label);
    return WITHOUT_ERROR;
}

struct Array *ctor_struct_arr(FILE *fp_src)
{
    struct Array *new_struct_arr = (struct Array*)malloc(sizeof(struct Array));

    long start_ptr_file = ftell(fp_src);

    size_t sz_file = search_size_file(fp_src);
    char   *array  = (char*)calloc(sz_file + 1, sizeof(char));

    fread(array, sizeof(char), sz_file, fp_src);

    new_struct_arr->arr_ptr  = array;
    new_struct_arr->size_arr = sz_file;
    
    fseek(fp_src, start_ptr_file, SEEK_SET);
    
    return new_struct_arr;
}

size_t search_size_file(FILE *fp_src)
{
    assert(fp_src != NULL);

	long start_ftell = ftell(fp_src);
    
    fseek(fp_src, 0, SEEK_END);
    
    size_t size_file = (size_t)ftell(fp_src);
    
    fseek(fp_src, start_ftell, SEEK_SET);
    
    return size_file;
}

int assembly(struct Array *src_struct_arr, struct Array *res_struct_arr, struct Label *arr_lab, int pass_num)
{
    #define NEW_INSTRUCTIONS(name, num, ASM_DEF, ...)    \
        if (strcmp(name_cmd, #name + 4) == 0)            \
        {                                                \
            check_cmd = true;                            \
            ASM_DEF(pass_num, num);                      \
        }                   

    size_t sz_res_arr = res_struct_arr->size_arr;
    int    index_lab  = 1;

    size_t pc     = 0;
    size_t src_pc = 0;

    num_t num_user = 0;

    char *src_arr_ptr = src_struct_arr->arr_ptr;
    char *res_arr_ptr = res_struct_arr->arr_ptr;

    while (src_pc < src_struct_arr->size_arr)
    {
        bool check_cmd = false;

        int ncr      = 0;  // ncr - number of characters read}
        int num_cmd  = 0;
        
        char name_cmd[MAX_SIZE_STR] = "VENOM";

        bool read_cmd = sscanf(src_arr_ptr + src_pc, "%s %n", name_cmd, &ncr);
        if (!read_cmd)
        {
            fprintf(stderr, "%s\n", ERROR_TEXT[ERROR_READ_CMD]);
            return ERROR_READ_CMD;
        }
        
        if (strcmp(name_cmd, "#") == 0)
        {
            while(src_arr_ptr[src_pc] != '\n')
                src_pc++;
            src_pc++;
            continue;
        }        
        else if (name_cmd[0] == ':')
        {
            if (pass_num == 1)
            {
                arr_lab[0].jmp_id++;
                printf("количество меток = %d\n", arr_lab[0].jmp_id);

                arr_lab[index_lab].jmp_id = (int)sz_res_arr;
                printf("имя метки = %s\n", name_cmd + 1);
                printf("адрес по метке = %d\n", arr_lab[index_lab].jmp_id);             
                strncpy(arr_lab[index_lab].name_lab, name_cmd + 1, MAX_SIZE_STR - 1);
                index_lab++;
            }
            src_pc += ncr;
            continue;             
        }
        else
            src_pc += ncr;

        #include "../../library/asm_def.h"

        #include "../../library/test_def_cmd.h"

        if (!check_cmd)
        {
            fprintf(stderr, "%s\n", ERROR_TEXT[ERROR_NAME_CMD]);
            return ERROR_NAME_CMD;
        }
    }

    if (pass_num == 1)
    {
        res_struct_arr->size_arr = sz_res_arr;
        res_struct_arr->arr_ptr  = (char*)calloc(sz_res_arr + 2, sizeof(char));
    }

    #undef NEW_INSTRUCTIONS

    return WITHOUT_ERROR;
}

int check_num_reg(const char *str)
{
    switch(str[1])
    {
        case 'a':
            return rax;
        case 'b':
            return rbx;
        case 'c':
            return rcx;
        case 'd':
            return rdx;
        default:
            return ERROR_REG_NAME;
    }
}
