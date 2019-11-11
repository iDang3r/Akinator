//
// Created by Александр on 10.11.2019.
//

#ifndef AKINATOR_COLOR_PRINTF_H
#define AKINATOR_COLOR_PRINTF_H

int printf_green(const char* s) {
    return printf("\033[1;32m%s\033[0m\n", s);
}

int printf_red(const char* s) {
    return printf("\033[1;31m%s\033[0m\n", s);
}

#endif //AKINATOR_COLOR_PRINTF_H
