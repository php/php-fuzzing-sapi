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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "fuzzer-sapi.h"

#include "ext/standard/php_var.h"

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
 	unsigned char *data = malloc(Size+1);

	memcpy(data, Data, Size);
	data[Size] = '\0';

	if (php_request_startup()==FAILURE) {
		php_module_shutdown();
		return 0;
	}

	zval result;

	php_unserialize_data_t var_hash;
	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	php_var_unserialize(&result, &data, data + Size, &var_hash);
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	ZVAL_UNDEF(&result);

	php_request_shutdown(NULL);

	free(data);

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

