#include "string.h"

void *kmemcpy(void *dest,
              const void *src,
              size_t size)
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    while (size--)
    {
        *d++ = *s++;
    }

    return dest;
}

void *kmemset(void *ptr,
              int value,
              size_t size)
{
    unsigned char *p = ptr;

    while (size--)
    {
        *p++ = (unsigned char)value;
    }

    return ptr;
}

int kmemcmp(const void *a,
            const void *b,
            size_t size)
{
    const unsigned char *x = a;
    const unsigned char *y = b;

    while (size--)
    {
        if (*x != *y)
        {
            return *x - *y;
        }

        x++;
        y++;
    }

    return 0;
}

void *kmemmove(void *dest,
               const void *src,
               size_t size)
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    if (d == s)
    {
        return dest;
    }

    if (d < s)
    {
        while (size--)
        {
            *d++ = *s++;
        }
    }
    else
    {
        d += size;
        s += size;

        while (size--)
        {
            *(--d) = *(--s);
        }
    }

    return dest;
}

void kmemzero(void *ptr,
              size_t size)
{
    kmemset(ptr, 0, size);
}

size_t kstrlen(const char *str)
{
    size_t len = 0;

    while (str[len])
    {
        len++;
    }

    return len;
}

int kstrcmp(const char *a,
            const char *b)
{
    while (*a && *b)
    {
        if (*a != *b)
        {
            return (unsigned char)*a -
                   (unsigned char)*b;
        }

        a++;
        b++;
    }

    return (unsigned char)*a -
           (unsigned char)*b;
}

int kstrncmp(const char *a,
             const char *b,
             size_t n)
{
    while (n--)
    {
        if (*a != *b)
        {
            return (unsigned char)*a -
                   (unsigned char)*b;
        }

        if (*a == '\0')
        {
            return 0;
        }

        a++;
        b++;
    }

    return 0;
}

int kstr_equal(const char *a,
               const char *b)
{
    return kstrcmp(a, b) == 0;
}

char *kstrcpy(char *dest,
              const char *src)
{
    char *ptr = dest;

    while ((*ptr++ = *src++))
    {
    }

    return dest;
}

char *kstrncpy(char *dest,
               const char *src,
               size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i]; i++)
    {
        dest[i] = src[i];
    }

    while (i < n)
    {
        dest[i++] = '\0';
    }

    return dest;
}

char *kstrcat(char *dest,
              const char *src)
{
    char *ptr = dest;

    while (*ptr)
    {
        ptr++;
    }

    while ((*ptr++ = *src++))
    {
    }

    return dest;
}

char *kstrncat(char *dest,
               const char *src,
               size_t n)
{
    char *ptr = dest;

    while (*ptr)
    {
        ptr++;
    }

    while (n-- && *src)
    {
        *ptr++ = *src++;
    }

    *ptr = '\0';

    return dest;
}

char *kstrchr(const char *str,
              int ch)
{
    while (*str)
    {
        if (*str == (char)ch)
        {
            return (char *)str;
        }

        str++;
    }

    if (ch == '\0')
    {
        return (char *)str;
    }

    return 0;
}

char *kstrrchr(const char *str,
               int ch)
{
    const char *result = 0;

    while (*str)
    {
        if (*str == (char)ch)
        {
            result = str;
        }

        str++;
    }

    if (ch == '\0')
    {
        return (char *)str;
    }

    return (char *)result;
}