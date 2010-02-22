#include <windows.h>
#include <stdio.h>

#define REQ_RANGE_MIN 0x0080
#define REQ_RANGE_MAX 0x33ff

typedef struct tagWCRANGE {
  WCHAR  wcLow;
  USHORT cGlyphs;
} WCRANGE, *PWCRANGE;

typedef struct tagGLYPHSET {
  DWORD    cbThis;
  DWORD    flAccel;
  DWORD    cGlyphsSupported;
  DWORD    cRanges;
  WCRANGE  ranges[1];
} GLYPHSET, *PGLYPHSET;

/*
DWORD GetFontUnicodeRanges(
  HDC hdc,         // handle to DC
  LPGLYPHSET lpgs  // glyph set
);
*/

typedef DWORD (WINAPI *GETFONTUNICODERANGES)(HDC, GLYPHSET *);

static void
print (HDC hdc, const GLYPHSET *g, const char *name)
{
  printf ("static const struct {ucs2_t c; u_char w;} %s[] = {", name);
  for (int i = 0, n = 0; i < g->cRanges; i++)
    for (int j = 0; j < g->ranges[i].cGlyphs; j++)
      {
        wchar_t wc = g->ranges[i].wcLow + j;
        if (wc >= REQ_RANGE_MIN && wc <= REQ_RANGE_MAX)
          {
            if (!(n % 4))
              printf ("\n");
            SIZE sz;
            GetTextExtentPoint32W (hdc, &wc, 1, &sz);
            printf (" {0x%04x, %d},", wc, sz.cx);
            n++;
          }
      }
  printf ("\n};\n");
}

int
main ()
{
  GETFONTUNICODERANGES GetFontUnicodeRanges =
    (GETFONTUNICODERANGES)GetProcAddress (GetModuleHandle ("GDI32"), "GetFontUnicodeRanges");
  if (!GetFontUnicodeRanges)
    {
      fprintf (stderr, "Cannot get GetFontUnicodeRanges (w2k only).\n");
      return 2;
    }

  char tem[sizeof (GLYPHSET) + sizeof (WCRANGE) * 65536];
  GLYPHSET *g = (GLYPHSET *)tem;
  g->cbThis = sizeof tem;

  HDC hdc = GetDC (0);

  LOGFONT lf;
  memset (&lf, 0, sizeof lf);
  lf.lfHeight = 16;
  strcpy (lf.lfFaceName, "Courier New");
  HGDIOBJ of = SelectObject (hdc, CreateFontIndirect (&lf));
  DWORD r = GetFontUnicodeRanges (hdc, g);
  if (r)
    print (hdc, g, "courier_new_range");
  DeleteObject (SelectObject (hdc, of));

  strcpy (lf.lfFaceName, "�l�r �S�V�b�N");
  of = SelectObject (hdc, CreateFontIndirect (&lf));
  r = GetFontUnicodeRanges (hdc, g);
  if (r)
    print (hdc, g, "ms_gothic_range");
  DeleteObject (SelectObject (hdc, of));

  ReleaseDC (0, hdc);
  return 0;
}
