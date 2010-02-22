#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

#if 0
static const u_char h_20_7e[] = (" !\"#$%&'()*+,-./"
                                 "0123456789:;<=>?"
                                 "@ABCDEFGHIJKLMNO"
                                 "PQRSTUVWXYZ[\\]^_"
                                 "`abcdefghijklmno"
                                 "pqrstuvwxyz{|}~");

//                                0123456789abcdef
static const u_char h_a1_df[] = ( "���������������"   // A
                                 "����������������"   // B
                                 "����������������"   // C
                                 "����������������"); // D
#endif

static const u_char f_20_7e[] = ("�@�I�h���������f�i�j���{�C�|�D�^"
                                 "�O�P�Q�R�S�T�U�V�W�X�F�G�������H"
                                 "���`�a�b�c�d�e�f�g�h�i�j�k�l�m�n"
                                 "�o�p�q�r�s�t�u�v�w�x�y�m���n�O�Q"
                                 "�e������������������������������"
                                 "�����������������������o�b�p�P");

static const u_char fk_a1_df[] = ("�B�u�v�A�E���@�B�D�F�H�������b"
                                  "�[�A�C�E�G�I�J�L�N�P�R�T�V�X�Z�\"
                                  "�^�`�c�e�g�i�j�k�l�m�n�q�t�w�z�}"
                                  "�~���������������������������J�K");

static const u_char fh_a1_df[] = ("�B�u�v�A�E�����������������"
                                  "�[������������������������������"
                                  "�����ĂƂȂɂʂ˂̂͂Ђӂւق�"
                                  "�݂ނ߂������������J�K");

static const u_char fh_voiced_b6_ce[] = ("�������������������������Âł�"
					 "          �΂тԂׂ�");

static const u_char fk_voiced_b3_ce[] = ("��    �K�M�O�Q�S�U�W�Y�[�]"
					 "�_�a�d�f�h          �o�r�u�x�{");

static const u_char fh_semi_voiced_ca_ce[] = "�ς҂Ղ؂�";

static const u_char fk_semi_voiced_ca_ce[] = "�p�s�v�y�|";

#define F(T, N) (u_short (((T) [(N) * 2] << 8) + (T) [(N) * 2 + 1]))

static void
soutput (u_char *buf, int n)
{
  int min, max;
  int i, j;

  for (min = 0; min < 256 && !buf[min]; min++)
    ;
  for (max = 255; max > min && !buf[max]; max--)
    ;
  if (min == max)
    return;
  printf ("#define VOICED_SOUND%x_MIN 0x%04x\n", n, (n << 8) + min);
  printf ("#define VOICED_SOUND%x_MAX 0x%04x\n", n, (n << 8) + max);
  printf ("static const u_char voiced_sound_%x[] = {\n", n);
  for (i = min, j = 0; i <= max; i++, j++)
    {
      printf ("0x%02x,", buf[i]);
      if ((j & 7) == 7)
        printf ("\n");
    }
  printf ("};\n\n");
}

static void
make_ss_table (void)
{
  u_char buf[256];
  int i;

  memset (buf, 0, sizeof buf);
  for (i = 0xb6; i <= 0xce; i++)
    {
      if (F (fh_voiced_b6_ce, i - 0xb6) >= 0x8200
          && F (fh_voiced_b6_ce, i - 0xb6) < 0x8300)
        buf[F (fh_voiced_b6_ce, i - 0xb6) - 0x8200] = i;
    }
  for (i = 0xca; i <= 0xce; i++)
    buf[F (fh_semi_voiced_ca_ce, i - 0xca) - 0x8200] = i & 0x7f;
  soutput (buf, 0x82);

  memset (buf, 0, sizeof buf);
  for (i = 0xb3; i <= 0xce; i++)
    {
      if (F (fk_voiced_b3_ce, i - 0xb3) >= 0x8300
          && F (fk_voiced_b3_ce, i - 0xb3) < 0x8400)
        buf[F (fk_voiced_b3_ce, i - 0xb3) - 0x8300] = i;
    }
  for (i = 0xca; i <= 0xce; i++)
    buf[F (fk_semi_voiced_ca_ce, i - 0xca) - 0x8300] = i & 0x7f;
  soutput (buf, 0x83);
}

static void
fill (u_short *buf, u_short from)
{
  u_short to = from + 0x100;
  int i;

  memset (buf, 0, sizeof *buf * 256);
  for (i = 0x20; i <= 0x7e; i++)
    {
      if (F (f_20_7e, i - 0x20) >= from
          && F (f_20_7e, i - 0x20) < to)
        buf[F (f_20_7e, i - 0x20) - from] = i;
    }
  for (i = 0xa1; i <= 0xdf; i++)
    {
      if (F (fk_a1_df, i - 0xa1) >= from
          && F (fk_a1_df, i - 0xa1) < to)
        buf[F (fk_a1_df, i - 0xa1) - from] = i;
    }
  for (i = 0xa1; i <= 0xdf; i++)
    {
      if (F (fh_a1_df, i - 0xa1) >= from
          && F (fh_a1_df, i - 0xa1) < to)
        buf[F (fh_a1_df, i - 0xa1) - from] = i;
    }
}

static void
output (u_short *buf, int n)
{
  int min, max;
  int i, j;

  for (min = 0; min < 256 && !buf[min]; min++)
    ;
  for (max = 255; max > min && !buf[max]; max--)
    ;
  if (min == max)
    return;
  printf ("#define TO_HALF_WIDTH%x_MIN 0x%04x\n", n, (n << 8) + min);
  printf ("#define TO_HALF_WIDTH%x_MAX 0x%04x\n", n, (n << 8) + max);
  printf ("static const u_char to_half_width_%x[] = {\n", n);
  for (i = min, j = 0; i <= max; i++, j++)
    {
      printf ("0x%02x,", buf[i]);
      if ((j & 7) == 7)
        printf ("\n");
    }
  printf ("};\n\n");
}

int
main ()
{
  int i;
  u_short buf[256];

  printf ("static const Char to_full_20_7e[] = {\n");
  for (i = 0x20; i <= 0x7e; i++)
    {
      printf ("0x%04x,", F (f_20_7e, i - 0x20));
      if ((i & 7) == 7)
	printf ("\n");
    }
  printf ("};\n\n");

  printf ("static const Char to_fullkata_a1_df[] = {\n");
  for (i = 0xa1; i <= 0xdf; i++)
    {
      printf ("0x%04x,", F (fk_a1_df, i - 0xa1));
      if ((i & 7) == 7)
	printf ("\n");
    }
  printf ("};\n\n");

  printf ("static const Char to_fullhira_a1_df[] = {\n");
  for (i = 0xa1; i <= 0xdf; i++)
    {
      printf ("0x%04x,", F (fh_a1_df, i - 0xa1));
      if ((i & 7) == 7)
	printf ("\n");
    }
  printf ("};\n\n");

  printf ("static const Char to_fullhira_voiced_b6_ce[] = {\n");
  for (i = 0xb6; i <= 0xce; i++)
    {
      printf ("0x%04x,",
              (fh_voiced_b6_ce[(i - 0xb6) * 2] == ' '
               ? 0 : F (fh_voiced_b6_ce, i - 0xb6)));
      if ((i & 7) == 7)
	printf ("\n");
    }
  printf ("};\n\n");

  printf ("static const Char to_fullkata_voiced_b3_ce[] = {\n");
  for (i = 0xb3; i <= 0xce; i++)
    {
      printf ("0x%04x,",
              (fk_voiced_b3_ce[(i - 0xb3) * 2] == ' '
               ? 0 : F (fk_voiced_b3_ce, i - 0xb3)));
      if ((i & 7) == 7)
	printf ("\n");
    }
  printf ("};\n\n");

  printf ("static const Char to_fullhira_semi_voiced_ca_ce[] = {\n");
  for (i = 0xca; i <= 0xce; i++)
    printf ("0x%04x,", F (fh_semi_voiced_ca_ce, i - 0xca));
  printf ("};\n\n");

  printf ("static const Char to_fullkata_semi_voiced_ca_ce[] = {\n");
  for (i = 0xca; i <= 0xce; i++)
    printf ("0x%04x,", F (fk_semi_voiced_ca_ce, i - 0xca));
  printf ("};\n\n");

  printf ("#define VOICED_SOUND_MARK 0x%04x\n", F ("�J", 0));
  printf ("#define SEMI_VOICED_SOUND_MARK 0x%04x\n", F ("�K", 0));
  printf ("\n");

  for (i = 0; i < 3; i++)
    {
      fill (buf, u_short (0x8100 + i * 0x100));
      output (buf, 0x81 + i);
    }

  make_ss_table ();

  printf ("#define FULL_WIDTH_HIRAGANA_MIN 0x829f\n");
  printf ("#define FULL_WIDTH_HIRAGANA_MAX 0x82f1\n");
  printf ("#define FULL_WIDTH_KATAKANA_MIN 0x8340\n");
  printf ("#define FULL_WIDTH_KATAKANA_MAX 0x8396\n");

  return 0;
}

