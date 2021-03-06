AC_MSG_CHECKING(for CLANG fuzzer SAPI)

PHP_ARG_ENABLE(fuzzer,,
[  --enable-fuzzer          Build PHP as clang fuzzing test module (for developers], no)

dnl For newer clang versions see https://llvm.org/docs/LibFuzzer.html#fuzzer-usage
dnl for relevant flags. 

dnl Macro to define fuzzing target
dnl FUZZER_TARGET(name, target-var)
dnl
AC_DEFUN([FUZZER_TARGET], [
  PHP_FUZZER_BINARIES="$PHP_FUZZER_BINARIES $SAPI_FUZZER_PATH/php-fuzz-$1"
  PHP_SUBST($2)
  PHP_ADD_SOURCES_X([sapi/fuzzer],[fuzzer-$1.c fuzzer-sapi.c],[],$2)
])

if test "$PHP_FUZZER" != "no"; then
  PHP_REQUIRE_CXX()
  PHP_ADD_MAKEFILE_FRAGMENT($abs_srcdir/sapi/fuzzer/Makefile.frag)
  SAPI_FUZZER_PATH=sapi/fuzzer
  PHP_SUBST(SAPI_FUZZER_PATH)
  if test -z "$LIB_FUZZING_ENGINE"; then
    FUZZING_LIB="-lFuzzer"
    FUZZING_CC="$CC"
    AX_CHECK_COMPILE_FLAG([-fsanitize-coverage=edge -fsanitize=address], [
      CFLAGS="$CFLAGS -fsanitize-coverage=edge -fsanitize=address"
      CXXFLAGS="$CXXFLAGS -fsanitize-coverage=edge -fsanitize=address"
      LDFLAGS="$LDFLAGS -fsanitize-coverage=edge -fsanitize=address"
    ],[
      AC_MSG_ERROR(compiler doesn't support -fsanitize flags)
    ])
  else
    FUZZING_LIB="-lFuzzingEngine"
    FUZZING_CC="$CXX -stdlib=libc++"
  fi
  PHP_SUBST(FUZZING_LIB)
  PHP_SUBST(FUZZING_CC)
  
  dnl PHP_SELECT_SAPI(fuzzer-parser, program, $FUZZER_SOURCES, , '$(SAPI_FUZZER_PATH)')

  PHP_ADD_BUILD_DIR([sapi/fuzzer])
  PHP_BINARIES="$PHP_BINARIES fuzzer"
  PHP_FUZZER_BINARIES=""
  PHP_INSTALLED_SAPIS="$PHP_INSTALLED_SAPIS fuzzer"

  FUZZER_TARGET([parser], PHP_FUZZER_PARSER_OBJS)
  FUZZER_TARGET([unserialize], PHP_FUZZER_UNSERIALIZE_OBJS)
  FUZZER_TARGET([exif], PHP_FUZZER_EXIF_OBJS)

  dnl This check doesn't work, as PHP_ARG_ENABLE(json) comes later ...
  if test "x$enable_json" != "xno"; then
    FUZZER_TARGET([json], PHP_FUZZER_JSON_OBJS)
  fi
  if test "x$enable_mbstring" = "xyes"; then
    FUZZER_TARGET([mbstring], PHP_FUZZER_MBSTRING_OBJS)
  fi

  PHP_SUBST(PHP_FUZZER_BINARIES)
fi

AC_MSG_RESULT($PHP_FUZZER)
