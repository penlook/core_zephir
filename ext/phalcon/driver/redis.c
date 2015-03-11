/**
 * Penlook Project
 *
 * Copyright (c) 2015 Penlook Development Team
 *
 * --------------------------------------------------------------------
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * Authors:
 *     Loi Nguyen       <loint@penlook.com>
 *     Tin Nguyen       <tinntt@penlook.com>
 *     Nam Vo           <namvh@penlook.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ext.h"
#include "hiredis/hiredis.h"
#include <stdlib.h>

#define REDIS_RESOURCE "REDIS RESOURCE"

void redis_connect(zval *return_value, zval *host_, zval *port_) {

	// Convert zend interface
	char* host = Z_STRVAL_P(host_);
	int port = atoi(Z_STRVAL_P(port_));

	// Establish redis connection
	redisContext *context = redisConnect(host, port);
	int le_resource = zend_register_list_destructors_ex(NULL, NULL, REDIS_RESOURCE, 12345);

	if (context != NULL && context->err) {
    	printf("Redis error: %s\n", context->errstr);
    	ZEND_REGISTER_RESOURCE(return_value, 0, le_resource);
	} else {
		ZEND_REGISTER_RESOURCE(return_value, context, le_resource);
	}
}

redisContext *parseContextFromResource(zval *redis) {

	redisContext *context;

	if (zend_parse_parameters(PHP_RINIT_FUNCTION() TSRMLS_CC, "r", &redis) == FAILURE) {
        RETURN_FALSE;
    }

    int le_resource = zend_register_list_destructors_ex(NULL, NULL, REDIS_RESOURCE, 12345);
    ZEND_FETCH_RESOURCE(context, redisContext*, &redis, -1, REDIS_RESOURCE, le_resource);

    return *context;
}

void redis_set(zval *return_value, zval *redis, zval *key_, zval *value_) {

	// Convert zend interface
	char* key   = Z_STRVAL_P(key_);
	char* value = Z_STRVAL_P(value_);
	redisContext *context = parseContextFromResource(*redis);
	char cmd[256];
	snprintf(cmd, sizeof cmd, "SET %s %s", key, value);
	redisCommand(context, cmd);
}