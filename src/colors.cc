#include "ed.h"
#include "colors.h"
#include "conf.h"
#include "Filer.h"
#include "mainframe.h"

static XCOLORREF xcolors[MC_NCOLORS];
static struct {const char *name, *disp;} xnames[] =
{
  {cfgTextColor, "�t�@�C�������F"},
  {cfgBackColor, "�t�@�C���w�i�F"},
  {"highlightTextColor", "�t�@�C���I�𕶎��F"},
  {"highlightBackColor", "�t�@�C���I��w�i�F"},
  {cfgCursorColor, "�t�@�C���J�[�\���F"},
  {"buftabSelFg", "�I���o�b�t�@�^�u�����F"},
  {"buftabSelBg", "�I���o�b�t�@�^�u�w�i�F"},
  {"buftabDispFg", "�\���o�b�t�@�^�u�����F"},
  {"buftabDispBg", "�\���o�b�t�@�^�u�w�i�F"},
  {"buftabFg", "�o�b�t�@�^�u�����F"},
  {"buftabBg", "�o�b�t�@�^�u�w�i�F"},
  {"tabSelFg", "�I���^�u�����F"},
  {"tabSelBg", "�I���^�u�w�i�F"},
  {"tabFg", "�^�u�����F"},
  {"tabBg", "�^�u�w�i�F"},
};

const char *
misc_color_name (int i)
{
  return xnames[i].disp;
}

XCOLORREF
get_misc_color (int i)
{
  return xcolors[i];
}

static void
load_default ()
{
  xcolors[MC_FILER_FG] = XCOLORREF (sysdep.window_text, COLOR_WINDOWTEXT);
  xcolors[MC_FILER_BG] = XCOLORREF (sysdep.window, COLOR_WINDOW);
  xcolors[MC_FILER_HIGHLIGHT_FG] = XCOLORREF (sysdep.highlight_text, COLOR_HIGHLIGHTTEXT);
  xcolors[MC_FILER_HIGHLIGHT_BG] = XCOLORREF (sysdep.highlight, COLOR_HIGHLIGHT);
  xcolors[MC_FILER_CURSOR] = RGB (192, 0, 192);

  for (int i = MC_BUFTAB_SEL_FG; i <= MC_TAB_FG; i += 2)
    {
      xcolors[i] = XCOLORREF (sysdep.btn_text, COLOR_BTNTEXT);
      xcolors[i + 1] = XCOLORREF (sysdep.btn_face, COLOR_BTNFACE);
    }
}

void
load_misc_colors ()
{
  load_default ();

  int c;
  for (int i = MC_FILER_FG; i <= MC_FILER_CURSOR; i++)
    if (read_conf (cfgFiler, xnames[i].name, c))
      xcolors[i] = c;

  for (; i < MC_NCOLORS; i++)
    if (read_conf (cfgColors, xnames[i].name, c))
      xcolors[i] = c;
}

void
modify_misc_colors (const XCOLORREF *colors, int save)
{
  memcpy (xcolors, colors, sizeof xcolors);
  if (save)
    {
      for (int i = MC_FILER_FG; i <= MC_FILER_CURSOR; i++)
        write_conf (cfgFiler, xnames[i].name, xcolors[i].rgb, 1);
      for (; i < MC_NCOLORS; i++)
        write_conf (cfgColors, xnames[i].name, xcolors[i].rgb, 1);
    }

  Filer::modify_colors ();
  g_frame.color_changed ();
}
