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

#include "fuzzer.h"

#include "Zend/zend.h"
#include "main/php_config.h"
#include "main/php_main.h"
#include "ext/standard/php_var.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fuzzer-sapi.h"

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
	char *filename;
	int filedes, result;
	zend_fcall_info fci = {0};
	zend_fcall_info_cache fci_cache = {0};
	zval args[1], retval, func;

	if (php_request_startup()==FAILURE) {
		php_module_shutdown();
		return 0;
	}

	/* put the data in a file */
	filename = tmpnam(NULL);
	filedes = open(filename, O_CREAT|O_RDWR);
	write(filedes, Data, Size);
	close(filedes);

	ZVAL_STRING(&func, "exif_read_data");

	zend_fcall_info_init(&func, 0, &fci, &fci_cache, NULL, NULL);

	ZVAL_STRING(&args[0], filename);
	fci.retval = &retval;
	fci.param_count = 1;
	fci.params = args;
	fci.no_separation = 0;

	result = zend_call_function(&fci, &fci_cache);
	/* to ensure retval is not broken */
	php_var_dump(&retval, 0);

	/* cleanup */
	ZVAL_UNDEF(&retval);
	ZVAL_UNDEF(&args[0]);
	ZVAL_UNDEF(&func);
	unlink(filename);
	php_request_shutdown(NULL);

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

