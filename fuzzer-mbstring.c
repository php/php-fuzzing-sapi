/*
  +----------------------------------------------------------------------+
  | Copyright (c) 2019, Stanislav Malyshev <stas@php.net>                |
  | All rights reserved.                                                 |
  +----------------------------------------------------------------------+
  | Redistribution and use in source and binary forms, with or without   |
  | modification, are permitted provided that the conditions which are   |
  | bundled with this package in the file LICENSE.                       |
  | This product includes PHP software, freely available from            |
  | <http://www.php.net/software/>                                       |
  +----------------------------------------------------------------------+
*/

#include "fuzzer.h"

#include "Zend/zend.h"
#include "main/php_config.h"
#include "main/php_main.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "fuzzer-sapi.h"

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
#ifdef HAVE_MBREGEX
	char *args[2];
	char *data = malloc(Size+1);
	memcpy(data, Data, Size);
	data[Size] = '\0';

	if (php_request_startup()==FAILURE) {
		php_module_shutdown();
		return 0;
	}

	args[0] = data;
	args[1] = "test123";
	fuzzer_call_php_func("mb_ereg", 2, args);

	args[0] = data;
	args[1] = "test123";
	fuzzer_call_php_func("mb_eregi", 2, args);

	args[0] = data;
	args[1] = data;
	fuzzer_call_php_func("mb_ereg", 2, args);

	args[0] = data;
	args[1] = data;
	fuzzer_call_php_func("mb_eregi", 2, args);

	php_request_shutdown(NULL);

	free(data);
#else
	fprintf(stderr, "\n\nERROR:\nPHP built without mbstring, recompile with --enable-mbstring to use this fuzzer\n");
	exit(1);
#endif
	return 0;
}

int LLVMFuzzerInitialize(int *argc, char ***argv) {
	fuzzer_init_php();

	/* fuzzer_shutdown_php(); */
	return 0;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

