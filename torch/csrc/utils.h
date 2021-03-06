#ifndef THP_UTILS_H
#define THP_UTILS_H

#include <string>
#include <type_traits>
#include <vector>

#include "torch/csrc/utils/object_ptr.h"
#include "torch/csrc/utils/python_numbers.h"

#define THPUtils_(NAME) TH_CONCAT_4(THP, Real, Utils_, NAME)

#define THPUtils_typename(obj) (Py_TYPE(obj)->tp_name)

static Py_complex c_ccomplex_to_py_complex(cx cx) {
  Py_complex x;
  x.real = (double)creal(cx);
  x.imag = (double)cimag(cx);
  return x;
}
static Py_complex c_zcomplex_to_py_complex(zx cx) {
  Py_complex x;
  x.real = creal(cx);
  x.imag = cimag(cx);
  return x;
}
static cx py_complex_to_c_ccomplex(Py_complex val) {
  union {
    float x[2];
    cx y;
  } v;
  v.x[0] = val.real;
  v.x[1] = val.imag;
  return v.y;
}
static zx py_complex_to_c_zcomplex(Py_complex val) {
  union {
    double x[2];
    cx y;
  } v;
  v.x[0] = val.real;
  v.x[1] = val.imag;
  return v.y;
}

#if PY_MAJOR_VERSION == 2
#define THPUtils_checkReal_FLOAT(object)                                       \
  (PyFloat_Check(object) || PyLong_Check(object) || PyInt_Check(object))
#define THPUtils_checkReal_COMPLEX(object) (PyComplex_Check(object))

#define THPUtils_unpackReal_FLOAT(object)                                      \
  (PyFloat_Check(object)                                                       \
       ? PyFloat_AsDouble(object)                                              \
       : PyLong_Check(object)                                                  \
             ? PyLong_AsLongLong(object)                                       \
             : PyInt_Check(object)                                             \
                   ? PyInt_AsLong(object)                                      \
                   : (throw std::runtime_error("Could not parse real"), 0))

#define THPUtils_unpackReal_ZCOMPLEX(object)                                   \
  (PyComplex_Check(object)                                                     \
       ? py_complex_to_c_zcomplex(PyComplex_AsCComplex(object))                \
       : (throw std::runtime_error("Could not parse complex"), 0))

#define THPUtils_unpackReal_CCOMPLEX(object)                                   \
  (PyComplex_Check(object)                                                     \
       ? py_complex_to_c_ccomplex(PyComplex_AsCComplex(object))                \
       : (throw std::runtime_error("Could not parse complex"), 0))

#define THPUtils_checkReal_INT(object)                                         \
  (PyLong_Check(object) || PyInt_Check(object))

#define THPUtils_unpackReal_INT(object)                                        \
  (PyLong_Check(object)                                                        \
       ? PyLong_AsLongLong(object)                                             \
       : PyInt_Check(object)                                                   \
             ? PyInt_AsLong(object)                                            \
             : (throw std::runtime_error("Could not parse real"), 0))
#else /* PY_MAJOR_VERSION == 2 */
#define THPUtils_checkReal_FLOAT(object)                                       \
  (PyFloat_Check(object) || PyLong_Check(object))
#define THPUtils_checkReal_COMPLEX(object) (PyComplex_Check(object))

#define THPUtils_unpackReal_FLOAT(object)                                      \
  (PyFloat_Check(object)                                                       \
       ? PyFloat_AsDouble(object)                                              \
       : PyLong_Check(object)                                                  \
             ? PyLong_AsLongLong(object)                                       \
             : (throw std::runtime_error("Could not parse real"), 0))

#define THPUtils_unpackReal_ZCOMPLEX(object)                                   \
  (PyComplex_Check(object)                                                     \
       ? py_complex_to_c_zcomplex(PyComplex_AsCComplex(object))                \
       : (throw std::runtime_error("Could not parse complex"), 0))

#define THPUtils_unpackReal_CCOMPLEX(object)                                   \
  (PyComplex_Check(object)                                                     \
       ? py_complex_to_c_ccomplex(PyComplex_AsCComplex(object))                \
       : (throw std::runtime_error("Could not parse complex"), 0))
#define THPUtils_checkReal_INT(object) PyLong_Check(object)

#define THPUtils_unpackReal_INT(object)                                        \
  (PyLong_Check(object)                                                        \
       ? PyLong_AsLongLong(object)                                             \
       : (throw std::runtime_error("Could not parse real"), 0))
#endif

#define THPUtils_newReal_FLOAT(value) PyFloat_FromDouble(value)
// TODO: handle int overflows for py2
#define THPUtils_newReal_INT(value) PyInt_FromLong(value)
#define THPUtils_newReal_CCOMPLEX(value)                                       \
  PyComplex_FromCComplex(c_ccomplex_to_py_complex(value))
#define THPUtils_newReal_ZCOMPLEX(value)                                       \
  PyComplex_FromCComplex(c_zcomplex_to_py_complex(value))

#define THPDoubleUtils_checkReal(object) THPUtils_checkReal_FLOAT(object)
#define THPDoubleUtils_unpackReal(object)                                      \
  (double)THPUtils_unpackReal_FLOAT(object)
#define THPDoubleUtils_newReal(value) THPUtils_newReal_FLOAT(value)
#define THPDoubleUtils_checkAccreal(object) THPUtils_checkReal_FLOAT(object)
#define THPDoubleUtils_unpackAccreal(object)                                   \
  (double)THPUtils_unpackReal_FLOAT(object)
#define THPDoubleUtils_newAccreal(value) THPUtils_newReal_FLOAT(value)

#define THPDoubleUtils_checkPart(object) THPUtils_checkReal_FLOAT(object)
#define THPDoubleUtils_unpackPart(object)                                      \
  (double)THPUtils_unpackReal_FLOAT(object)
#define THPDoubleUtils_newPart(value) THPUtils_newReal_FLOAT(value)

#define THPFloatUtils_checkReal(object) THPUtils_checkReal_FLOAT(object)
#define THPFloatUtils_unpackReal(object)                                       \
  (float)THPUtils_unpackReal_FLOAT(object)
#define THPFloatUtils_newReal(value) THPUtils_newReal_FLOAT(value)
#define THPFloatUtils_checkAccreal(object) THPUtils_checkReal_FLOAT(object)
#define THPFloatUtils_unpackAccreal(object)                                    \
  (double)THPUtils_unpackReal_FLOAT(object)
#define THPFloatUtils_newAccreal(value) THPUtils_newReal_FLOAT(value)

#define THPFloatUtils_checkPart(object) THPUtils_checkReal_FLOAT(object)
#define THPFloatUtils_unpackPart(object)                                       \
  (float)THPUtils_unpackReal_FLOAT(object)
#define THPFloatUtils_newPart(value) THPUtils_newReal_FLOAT(value)

#define THPZDoubleUtils_checkReal(object) THPUtils_checkReal_COMPLEX(object)
#define THPZDoubleUtils_unpackReal(object) THPUtils_unpackReal_ZCOMPLEX(object)
#define THPZDoubleUtils_newReal(value) THPUtils_newReal_ZCOMPLEX(value)
#define THPZDoubleUtils_checkAccreal(object) THPUtils_checkReal_COMPLEX(object)
#define THPZDoubleUtils_unpackAccreal(object)                                  \
  THPUtils_unpackReal_ZCOMPLEX(object)
#define THPZDoubleUtils_newAccreal(value) THPUtils_newReal_ZCOMPLEX(value)

#define THPZDoubleUtils_checkPart(object) THPUtils_checkReal_FLOAT(object)
#define THPZDoubleUtils_unpackPart(object) (double)THPUtils_unpackReal_FLOAT(object)
#define THPZDoubleUtils_newPart(value) THPUtils_newReal_FLOAT(value)

#define THPZFloatUtils_checkReal(object) THPUtils_checkReal_COMPLEX(object)
#define THPZFloatUtils_unpackReal(object) THPUtils_unpackReal_CCOMPLEX(object)
#define THPZFloatUtils_newReal(value) THPUtils_newReal_CCOMPLEX(value)
#define THPZFloatUtils_checkAccreal(object) THPUtils_checkReal_COMPLEX(object)
#define THPZFloatUtils_unpackAccreal(object)                                   \
  THPUtils_unpackReal_ZCOMPLEX(object)
#define THPZFloatUtils_newAccreal(value) THPUtils_newReal_CCOMPLEX(value)

#define THPZFloatUtils_checkPart(object) THPUtils_checkReal_FLOAT(object)
#define THPZFloatUtils_unpackPart(object) THPUtils_unpackReal_FLOAT(object)
#define THPZFloatUtils_newPart(value) THPUtils_newReal_FLOAT(value)

#define THPHalfUtils_checkReal(object) THPUtils_checkReal_FLOAT(object)
#define THPHalfUtils_checkPart(object) THPUtils_checkReal_FLOAT(object)
#ifndef THP_HOST_HALF
#define THPHalfUtils_unpackReal(object)                                        \
  (half) THC_float2half(THPUtils_unpackReal_FLOAT(object))
#define THPHalfUtils_newReal(value) PyFloat_FromDouble(THC_half2float(value))

#define THPHalfUtils_unpackPart(object)                                        \
  (half) THC_float2half(THPUtils_unpackReal_FLOAT(object))
#define THPHalfUtils_newPart(value) PyFloat_FromDouble(THC_half2float(value))
#else
#define THPHalfUtils_unpackReal(object)                                        \
  TH_float2half(THPUtils_unpackReal_FLOAT(object))
#define THPHalfUtils_newReal(value) PyFloat_FromDouble(TH_half2float(value))

#define THPHalfUtils_unpackPart(object)                                        \
  TH_float2half(THPUtils_unpackReal_FLOAT(object))
#define THPHalfUtils_newPart(value) PyFloat_FromDouble(TH_half2float(value))
#endif

#define THPHalfUtils_checkAccreal(object) THPUtils_checkReal_FLOAT(object)
#define THPHalfUtils_unpackAccreal(object)                                     \
  (double)THPUtils_unpackReal_FLOAT(object)
#define THPHalfUtils_newAccreal(value) THPUtils_newReal_FLOAT(value)

#define THPLongUtils_checkReal(object) THPUtils_checkReal_INT(object)
#define THPLongUtils_unpackReal(object) (long)THPUtils_unpackReal_INT(object)
#define THPLongUtils_newReal(value) THPUtils_newReal_INT(value)
#define THPLongUtils_checkAccreal(object) THPUtils_checkReal_INT(object)
#define THPLongUtils_unpackAccreal(object) (long)THPUtils_unpackReal_INT(object)
#define THPLongUtils_newAccreal(value) THPUtils_newReal_INT(value)

#define THPLongUtils_checkPart(object) THPUtils_checkReal_INT(object)
#define THPLongUtils_unpackPart(object) (long)THPUtils_unpackReal_INT(object)
#define THPLongUtils_newPart(value) THPUtils_newReal_INT(value)

#define THPIntUtils_checkReal(object) THPUtils_checkReal_INT(object)
#define THPIntUtils_unpackReal(object) (int)THPUtils_unpackReal_INT(object)
#define THPIntUtils_newReal(value) THPUtils_newReal_INT(value)
#define THPIntUtils_checkAccreal(object) THPUtils_checkReal_INT(object)
#define THPIntUtils_unpackAccreal(object) (long)THPUtils_unpackReal_INT(object)
#define THPIntUtils_newAccreal(value) THPUtils_newReal_INT(value)

#define THPIntUtils_checkPart(object) THPUtils_checkReal_INT(object)
#define THPIntUtils_unpackPart(object) (int)THPUtils_unpackReal_INT(object)
#define THPIntUtils_newPart(value) THPUtils_newReal_INT(value)

#define THPShortUtils_checkReal(object) THPUtils_checkReal_INT(object)
#define THPShortUtils_unpackReal(object) (short)THPUtils_unpackReal_INT(object)
#define THPShortUtils_newReal(value) THPUtils_newReal_INT(value)
#define THPShortUtils_checkAccreal(object) THPUtils_checkReal_INT(object)
#define THPShortUtils_unpackAccreal(object)                                    \
  (long)THPUtils_unpackReal_INT(object)
#define THPShortUtils_newAccreal(value) THPUtils_newReal_INT(value)

#define THPShortUtils_checkPart(object) THPUtils_checkReal_INT(object)
#define THPShortUtils_unpackPart(object) (short)THPUtils_unpackReal_INT(object)
#define THPShortUtils_newPart(value) THPUtils_newReal_INT(value)

#define THPCharUtils_checkReal(object) THPUtils_checkReal_INT(object)
#define THPCharUtils_unpackReal(object) (char)THPUtils_unpackReal_INT(object)
#define THPCharUtils_newReal(value) THPUtils_newReal_INT(value)
#define THPCharUtils_checkAccreal(object) THPUtils_checkReal_INT(object)
#define THPCharUtils_unpackAccreal(object) (long)THPUtils_unpackReal_INT(object)
#define THPCharUtils_newAccreal(value) THPUtils_newReal_INT(value)

#define THPCharUtils_checkPart(object) THPUtils_checkReal_INT(object)
#define THPCharUtils_unpackPart(object) (char)THPUtils_unpackReal_INT(object)
#define THPCharUtils_newPart(value) THPUtils_newReal_INT(value)

#define THPByteUtils_checkReal(object) THPUtils_checkReal_INT(object)
#define THPByteUtils_unpackReal(object)                                        \
  (unsigned char)THPUtils_unpackReal_INT(object)
#define THPByteUtils_newReal(value) THPUtils_newReal_INT(value)
#define THPByteUtils_checkAccreal(object) THPUtils_checkReal_INT(object)
#define THPByteUtils_unpackAccreal(object) (long)THPUtils_unpackReal_INT(object)
#define THPByteUtils_newAccreal(value) THPUtils_newReal_INT(value)

#define THPByteUtils_checkPart(object) THPUtils_checkReal_INT(object)
#define THPByteUtils_unpackPart(object)                                        \
  (unsigned char)THPUtils_unpackReal_INT(object)
#define THPByteUtils_newPart(value) THPUtils_newReal_INT(value)

#define THPUtils_assert(cond, ...) THPUtils_assertRet(NULL, cond, __VA_ARGS__)
#define THPUtils_assertRet(value, cond, ...)                                   \
  if (__builtin_expect(!(cond), 0)) {                                          \
    THPUtils_setError(__VA_ARGS__);                                            \
    return value;                                                              \
  }
THP_API void THPUtils_setError(const char *format, ...);
THP_API void THPUtils_invalidArguments(PyObject *given_args,
                                       PyObject *given_kwargs,
                                       const char *function_name,
                                       size_t num_options, ...);

#ifdef _THP_CORE

bool THPUtils_checkIntTuple(PyObject *arg);
std::vector<int> THPUtils_unpackIntTuple(PyObject *arg);

void THPUtils_addPyMethodDefs(std::vector<PyMethodDef> &vector,
                              PyMethodDef *methods);

int THPUtils_getCallable(PyObject *arg, PyObject **result);
// https://bugsfiles.kde.org/attachment.cgi?id=61186
#if PY_VERSION_HEX >= 0x03020000
#define THPUtils_parseSlice(SLICE, LEN, START, STOP, LENGTH, STEP)             \
  (PySlice_GetIndicesEx(SLICE, LEN, START, STOP, LENGTH, STEP) == 0)
#else
#define THPUtils_parseSlice(SLICE, LEN, START, STOP, LENGTH, STEP)             \
  (PySlice_GetIndicesEx((PySliceObject *)SLICE, LEN, START, STOP, LENGTH,      \
                        STEP) == 0)
#endif

#define THStoragePtr TH_CONCAT_3(TH, Real, StoragePtr)
#define THTensorPtr TH_CONCAT_3(TH, Real, TensorPtr)
#define THPStoragePtr TH_CONCAT_3(THP, Real, StoragePtr)
#define THPTensorPtr TH_CONCAT_3(THP, Real, TensorPtr)
#define THPPartTensorPtr TH_CONCAT_3(THP, Part, TensorPtr)
#define THSTensorPtr TH_CONCAT_3(THS, Real, TensorPtr)
#define THSPTensorPtr TH_CONCAT_3(THSP, Real, TensorPtr)

typedef THPPointer<THPGenerator> THPGeneratorPtr;

template <typename T> struct THPUtils_typeTraits {};

#include "generic/utils.h"
#include <TH/THGenerateAllTypes.h>

#include "generic/utils.h"
#include <TH/THGenerateHalfType.h>

THLongStoragePtr THPUtils_unpackSize(PyObject *arg);
bool THPUtils_tryUnpackLongs(PyObject *arg, THLongStoragePtr &result);
bool THPUtils_tryUnpackLongVarArgs(PyObject *args, int ignore_first,
                                   THLongStoragePtr &result);
PyObject *THPUtils_dispatchStateless(PyObject *tensor, const char *name,
                                     PyObject *args, PyObject *kwargs);

template <typename _real, typename = void> struct mod_traits {};

template <typename _real>
struct mod_traits<_real, typename std::enable_if<
                             std::is_floating_point<_real>::value>::type> {
  static _real mod(_real a, _real b) { return fmod(a, b); }
};

template <typename _real>
struct mod_traits<
    _real, typename std::enable_if<std::is_integral<_real>::value>::type> {
  static _real mod(_real a, _real b) { return a % b; }
};

void setBackCompatBroadcastWarn(bool warn);
bool getBackCompatBroadcastWarn();

void setBackCompatKeepdimWarn(bool warn);
bool getBackCompatKeepdimWarn();
bool maybeThrowBackCompatKeepdimWarn(char *func);

#endif /* _THP_CORE */

#endif
