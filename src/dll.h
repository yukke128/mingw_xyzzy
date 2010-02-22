// -*-C++-*-
#ifndef _dll_h_
# define _dll_h_

class ldll_module: public lisp_object
{
public:
  lisp name;        // DLL�̖��O
  HMODULE handle;   // ���W���[���n���h��
  int loaded;       // LoadLibrary�����ꍇ��1, LoadModule�̏ꍇ��0

  ~ldll_module () {if (handle && loaded) FreeLibrary (handle);}
};

# define dll_module_p(X) typep ((X), Tdll_module)

inline void
check_dll_module (lisp x)
{
  check_type (x, Tdll_module, Qsi_dll_module);
}

inline lisp &
xdll_module_name (lisp x)
{
  assert (dll_module_p (x));
  return ((ldll_module *)x)->name;
}

inline HMODULE &
xdll_module_handle (lisp x)
{
  assert (dll_module_p (x));
  return ((ldll_module *)x)->handle;
}

inline int &
xdll_module_loaded (lisp x)
{
  assert (dll_module_p (x));
  return ((ldll_module *)x)->loaded;
}

# define CTYPE_VOID 0
# define CTYPE_INT8 1
# define CTYPE_UINT8 2
# define CTYPE_INT16 3
# define CTYPE_UINT16 4
# define CTYPE_INT32 5
# define CTYPE_UINT32 6
# define CTYPE_FLOAT 7
# define CTYPE_DOUBLE 8

class ldll_function: public lisp_object
{
public:
  lisp module;        // DLL���W���[���I�u�W�F�N�g
  lisp name;          // DLL���̖��O
  FARPROC proc;       // �֐��̃|�C���^
  u_char *arg_types;  // �����̌^
  u_short arg_size;   // �����S�̂̃T�C�Y
  u_char nargs;       // �����̐�
  u_char return_type; // �߂�l�̌^

  ~ldll_function () {xfree (arg_types);}
};

#define dll_function_p(X) typep ((X), Tdll_function)

inline void
check_dll_function (lisp x)
{
  check_type (x, Tdll_function, Qsi_c_function);
}

inline lisp &
xdll_function_module (lisp x)
{
  assert (dll_function_p (x));
  return ((ldll_function *)x)->module;
}

inline lisp &
xdll_function_name (lisp x)
{
  assert (dll_function_p (x));
  return ((ldll_function *)x)->name;
}

inline FARPROC &
xdll_function_proc (lisp x)
{
  assert (dll_function_p (x));
  return ((ldll_function *)x)->proc;
}

inline u_char *&
xdll_function_arg_types (lisp x)
{
  assert (dll_function_p (x));
  return ((ldll_function *)x)->arg_types;
}

inline u_char &
xdll_function_nargs (lisp x)
{
  assert (dll_function_p (x));
  return ((ldll_function *)x)->nargs;
}

inline u_char &
xdll_function_return_type (lisp x)
{
  assert (dll_function_p (x));
  return ((ldll_function *)x)->return_type;
}

inline u_short &
xdll_function_arg_size (lisp x)
{
  assert (dll_function_p (x));
  return ((ldll_function *)x)->arg_size;
}

class lc_callable: public lisp_object
{
public:
  lisp function;      // �֐�
  u_char *arg_types;  // �����̌^
  u_short arg_size;   // �����S�̂̃T�C�Y
  u_char nargs;       // �����̐�
  u_char return_type; // �߂�l�̌^
  u_char insn[16];    // stub�R�[�h

  ~lc_callable () {xfree (arg_types);}
};

#define c_callable_p(X) typep ((X), Tc_callable)

inline void
check_c_callable (lisp x)
{
  check_type (x, Tc_callable, Qsi_c_callable);
}

inline lisp &
xc_callable_function (lisp x)
{
  assert (c_callable_p (x));
  return ((lc_callable *)x)->function;
}

inline u_char *&
xc_callable_arg_types (lisp x)
{
  assert (c_callable_p (x));
  return ((lc_callable *)x)->arg_types;
}

inline u_char &
xc_callable_nargs (lisp x)
{
  assert (c_callable_p (x));
  return ((lc_callable *)x)->nargs;
}

inline u_char &
xc_callable_return_type (lisp x)
{
  assert (c_callable_p (x));
  return ((lc_callable *)x)->return_type;
}

inline u_short &
xc_callable_arg_size (lisp x)
{
  assert (c_callable_p (x));
  return ((lc_callable *)x)->arg_size;
}

inline u_char *
xc_callable_insn (lisp x)
{
  assert (c_callable_p (x));
  return ((lc_callable *)x)->insn;
}

ldll_module *make_dll_module ();
ldll_function *make_dll_function ();
lc_callable *make_c_callable ();
lisp funcall_dll (lisp fn, lisp arglist);
lisp funcall_c_callable (lisp, lisp);
void init_c_callable (lisp);

#endif
