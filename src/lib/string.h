#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t kstrlen(const char *str);

void *kmemcpy(void *dest,
              const void *src,
              size_t size);

void *kmemset(void *ptr,
              int value,
              size_t size);

int kmemcmp(const void *a,
            const void *b,
            size_t size);

void *kmemmove(void *dest,
               const void *src,
               size_t size);

void kmemzero(void *ptr,
              size_t size);

int kstrcmp(const char *a,
            const char *b);

int kstrncmp(const char *a,
             const char *b,
             size_t n);

int kstr_equal(const char *a,
               const char *b);

char *kstrcpy(char *dest,
              const char *src);

char *kstrncpy(char *dest,
               const char *src,
               size_t n);

char *kstrcat(char *dest,
              const char *src);

char *kstrncat(char *dest,
               const char *src,
               size_t n);

char *kstrchr(const char *str,
              int ch);

char *kstrrchr(const char *str,
               int ch);

#endif