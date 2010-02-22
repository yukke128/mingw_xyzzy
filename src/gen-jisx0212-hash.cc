#include "cdecl.h"
#include "charset.h"
#include "jisx0212-width.h"

int
main ()
{
  Char ic[sizeof jisx0212_width_table * 8];
  int nic = 0;
  for (int i = 0; i < sizeof jisx0212_width_table; i++)
    for (int j = 0; j < 8; j++)
      if (!(jisx0212_width_table[i] & (1 << j)))
        ic[nic++] = CCS_JISX0212_MIN + i * 8 + j;

  if (nic >= 255)
    {
      fprintf (stderr, "Too many chars: %d\n", nic);
      return 2;
    }

  int buf[sizeof jisx0212_width_table * 8];
  for (int hashsize = nic;; hashsize++)
    {
      memset (buf, -1, sizeof *buf * hashsize);
      for (i = 0; i < nic; i++)
        {
          if (buf[ic[i] % hashsize] != -1)
            break;
          buf[ic[i] % hashsize] = i;
        }
      if (i == nic)
        break;
    }

  printf ("static const Char jisx0212_half_width_table[] = {");
  for (i = 0; i < nic; i++)
    {
      if (!(i % 8))
        printf ("\n");
      printf (" 0x%04x,", ic[i]);
    }
  printf ("};\n\n");

  printf ("#define JISX0212_HALF_WIDTH_HASH_SIZE %d\n\n", hashsize);

  printf ("static const u_char jisx0212_half_width_hash[] = {");
  for (i = 0; i < hashsize; i++)
    {
      if (!(i % 8))
        printf ("\n");
      printf (" 0x%02x,", buf[i] & 255);
    }
  printf ("};\n\n");

  return 0;
}
