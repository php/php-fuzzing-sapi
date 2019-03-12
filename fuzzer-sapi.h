/*
  +----------------------------------------------------------------------+
  | Copyright (c) 2017, Johannes Schlüter <johannes@schlueters.de>       |
  | All rights reserved.                                                 |
  +----------------------------------------------------------------------+
  | Redistribution and use in source and binary forms, with or without   |
  | modification, are permitted provided that the conditions which are   |
  | bundled with this package in the file LICENSE.                       |
  | This product includes PHP software, freely available from            |
  | <http://www.php.net/software/>                                       |
  +----------------------------------------------------------------------+
*/

int fuzzer_init_php();
void fuzzer_call_php_func(const char *func_name, int nargs, char **params);
void fuzzer_call_php_func_zval(const char *func_name, int nargs, zval *args);
