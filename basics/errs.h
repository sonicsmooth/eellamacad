#ifndef __ERRS_H__
#define __ERRS_H__

#include <iostream>
#include <sstream>
#include <stdio.h>

void warnfn(int line) {
    std::cout << "Warning at " << line << std::endl;
    } 
void warnfn(int line, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "Warning at %d: ", line);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    } 

void fatalfn(int line) {
    std::cout << "Fatal Warning at " << line << std::endl;
    throw;
    } 
void fatalfn(int line, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "Fatal at %d: ", line);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    throw;
    } 




// exp should evaluate to false to show warning
#define assertwarn(exp, ...) \
    do {if (!(exp)) warnfn(__LINE__, __VA_ARGS__); } while(0);

// exp should evaluate to false to show fatal and exception
#define assertfatal(exp, ...) \
    do {if (!(exp)) fatalfn(__LINE__, __VA_ARGS__); } while(0);

#endif __ERRS_H__