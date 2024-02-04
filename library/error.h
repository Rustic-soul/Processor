#ifndef ERROR_H
#define ERROR_H

#define ошибок_нет                                      0
#define ошибка_без_имени                                1
#define ошибка_открытия_файла                           2
#define ошибка_чтения_команды                           3
#define ошибка_в_имени_команды                          4
#define ошибка_в_имени_регистра                         5  
#define ошибка_в_номере_регистра                        6
#define ошибка_в_директиве_org                          7
#define ошибка_количество_аргументов_командной_строки   8
#define ошибка_обработки_команды_процессором            9

// enum ERROR{
//     ERROR_REG_NAME = -1,
//     WITHOUT_ERROR,
//     ERROR,
//     ERROR_OPEN_FILE,
//     ERROR_READ_CMD,
//     ERROR_NAME_CMD,
//     VENOM_ENUM,
//     ERROR_REG_NUM,
//     ERROR_ORG_DIR
// };

static const char *ERROR_TEXT[] = {
    "Ошибка: Её нет -- ОТЛИЧНАЯ РАБОТА --",
    "Ошибка: Имя отсутствует",
    "Ошибка: Не получилось открыть файл",
    "Ошибка: Имя команды не было прочитано",
    "Ошибка: Команда не распознана",
    "Ошибка: Некорректное имя регистра",
    "Ошибка: Некорректный номер регистра",
    "Ошибка: Директива org указывает на задействованную область памяти",
    "Ошибка: Недостаточное количество аргументов командной строки",
    "Ошибка: Процессор не смог обработать команду"
};

#define CHECK_OPEN_FILE(file_ptr)                                          \
    if (file_ptr == NULL)                                                  \
    {                                                                      \
        PRINT_ERROR("%s\n", ERROR_TEXT[ошибка_открытия_файла]);            \
        return ошибка_открытия_файла;                                      \
    }

#define CHECK_ARGC(num, right_num)                                                               \
    if (num < right_num)                                                                         \
    {                                                                                            \
        PRINT_ERROR("%s\n", ERROR_TEXT[ошибка_количество_аргументов_командной_строки]);          \
        return ошибка_количество_аргументов_командной_строки;                                    \
    }
    
#endif // !ERROR_H
