#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return 2;
    }
    FILE *f = fopen(argv[1], "rb");
    if (!f)
    {
        perror("fopen");
        return 2;
    }
    unsigned char buf[16];
    size_t n = fread(buf, 1, 16, f);
    for (size_t i = 0; i < n; i++)
        printf("%02x ", buf[i]);
    printf("\n");
    for (size_t i = 0; i < n; i++)
        printf("%c", (buf[i] >= 32 && buf[i] <= 126) ? buf[i] : '.');
    printf("\n");
    fclose(f);
    return 0;
}
