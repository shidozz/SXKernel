#include "../include/kio.h"
// Halt and catch fire function.
void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

void *memchr(const void *str, int c, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        if(((int *)str)[i] == c)
        {
            return (void*)str + i;
        }
    }

    return NULL;
}

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}
int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void io_wait()
{
    asm volatile("outb %%al, $0x80"
                 :
                 : "a"(0));
}

int strlen(const char *str) {
    const char *pStr = str;
    
    while (*pStr) {
        pStr++;
    }
    
    return (int)(pStr - str);
}

void splitString(const char* str, char result[][255], int* count) {
    int i = 0;    // indice du caractère en cours de traitement
    int j = 0;    // indice du caractère dans la sous-chaîne en cours
    int wordIndex = 0;    // indice de la sous-chaîne en cours

    // Parcours de la chaîne de caractères
    while (str[i] != '\0') {
        if (str[i] != ' ') {
            // Copie du caractère dans la sous-chaîne en cours
            result[wordIndex][j] = str[i];
            j++;
        } else if (j > 0) {
            // Fin de la sous-chaîne, ajouter le caractère de fin de chaîne
            result[wordIndex][j] = '\0';
            j = 0;
            wordIndex++;
        }
        i++;
    }

    // Vérification si la dernière sous-chaîne n'a pas été ajoutée
    if (j > 0) {
        result[wordIndex][j] = '\0';
        wordIndex++;
    }

    // Mettre à jour le nombre de sous-chaînes trouvées
    *count = wordIndex;
}

char *strrev(char *str)
{
    if(str == NULL)
    {
        return NULL;
    }

    int length = strlen(str);

    for(int i = 0; i < length / 2; ++i)
    {
        char tmp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = tmp;
    }

    return str;
}