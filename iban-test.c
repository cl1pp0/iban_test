#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <ctype.h>

#define IBAN_LEN 34

int test_iban(char *iban)
{
    int res = 0, buf_idx = 0;
    mpz_t iban_mpi, rem;

    /*  34 is max IBAN length
        +2 for country code to number conversion
        +1 for null termination byte
      = 37 max string length */
    char iban_buf[IBAN_LEN+3] = {0};

    /* first 4 bytes must follow schema AADD */
    if (   isalpha(iban[0])
        && isalpha(iban[1])
        && isdigit(iban[2])
        && isdigit(iban[3]) )
    {
        /* are remaining chars digits? -> copy them to buffer */
        for (int i = 4; iban[i] != '\0'; i++)
        {
            if (i < IBAN_LEN)
            {
                if (isdigit(iban[i]))
                {
                    buf_idx = i - 4;
                    iban_buf[buf_idx] = iban[i];
                }
                else
                {
                    res = 1;
                    break;
                }
            }
            else /* IBAN too long */
            {
                res = 4;
                break;
            }
        }

        if (!res)
        {
            buf_idx++;
            /* convert country code to numbers (A=10, B=11, ... , Z=35) and copy to buffer */
            for (int i = 0; i < 2; i++)
            {
                /* if lower case -> upper case */
                if (iban[i] >= 97)
                    sprintf(&iban_buf[buf_idx], "%d", (int)iban[i]-32-55);
                else
                    sprintf(&iban_buf[buf_idx], "%d", (int)iban[i]-55);
                
                buf_idx += 2;
            }

            /* copy checksum to buffer */
            iban_buf[buf_idx] = iban[2];
            iban_buf[++buf_idx] = iban[3];
            //printf("iban_buf %s\n", iban_buf);

            /* covert buffer to large integer and check if (N mod 97) == 1 */
            if (!mpz_init_set_str(iban_mpi, iban_buf, 10))
            {
                //gmp_printf("iban_mpi %Zd\n", iban_mpi);
                mpz_init(rem);
                mpz_mod_ui(rem, iban_mpi, 97);
                //gmp_printf("rem %Zd\n", rem);
                if (mpz_cmp_ui(rem, 1))
                    res = 2;
                mpz_clear(rem);
            }
            else
            {
                res = 3;
            }
            mpz_clear(iban_mpi);
        }
    }
    else /* if (isalpha(iban[0]) && isalpha(iban[1]) && isdigit(iban[2]) && isdigit(iban[3])) */
    {
        res = 1;
    }
    
    return res;
}

int main(int argc, char **argv)
{
    int res = -1;
    char *iban;

    if (argc == 2)
    {
        iban = argv[1];
        if (!strcmp("-h", iban) || !strcmp("--help", iban))
        {
            printf("Usage: iban-test <iban>\n");
            res = 0;
            goto end;
        }
        else
        {
            res = test_iban(iban);
            
            /* error handling */
            switch (res)
            {
                case 0:
                    printf("IBAN \"%s\" is valid\n", iban);
                    goto end;
                    break;
                case 1:
                    printf("IBAN \"%s\" does not have a valid syntax\n", iban);
                    goto end;
                    break;
                case 2:
                    printf("IBAN \"%s\" does not have a valid checksum\n", iban);
                    goto end;
                    break;
                case 3:
                    printf("Error: Cannot convert char* to mpz_t\n");
                    goto end;
                    break;
                case 4:
                    printf("IBAN is too long\n");
                    goto end;
                    break;
                default:
                    printf("IBAN \"%s\" is not valid, generic error\n", iban);
                    goto end;
                    break;
            }
        }
    }
    else /* if (argc == 2) */
    {
        printf("Usage: iban-test <iban>\n");
        goto end;
    }

    end:
    return res;
}
