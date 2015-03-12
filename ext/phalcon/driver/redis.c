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
 *     Thanh Huynh      <thanhhb@penlook.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ext.h"
#include "hiredis/hiredis.h"
#include <stdlib.h>

#define REDIS_RESOURCE   "REDIS RESOURCE"
#define REDIS_MODULE_ID  12345

int le_redis;
int le_redis_persist;

static void redis_persist_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    redisContext *context = (redisContext*)rsrc->ptr;

    if (context) {
        if (context->obuf) {
            pefree(context->obuf, 1);
        }
        pefree(context, 1);
    }
}

static void redis_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    redisContext *context = (redisContext*)rsrc->ptr;

    if (context) {
        if (context->obuf) {
            efree(context->obuf);
        }
        efree(context);
    }
}

void redis_connect(zval *return_value, zval *host_, zval *port_) {

	// Convert zend interface
	char* host = Z_STRVAL_P(host_);
	int port = atoi(Z_STRVAL_P(port_));

	// Establish redis connection
	redisContext *context = redisConnect(host, port);
	le_redis = zend_register_list_destructors_ex(NULL, NULL, REDIS_RESOURCE, REDIS_MODULE_ID);

	if (context != NULL && context->err) {
    	printf("Redis error: %s\n", context->errstr);
	} else {
		ZEND_REGISTER_RESOURCE(return_value, context, le_redis);
	}
}

/*
void parseContextFromResource(zval *redis) {
	zval *return_value;
	redisContext *context;
	int le_resource = zend_register_list_destructors_ex(NULL, NULL, REDIS_RESOURCE, 12345);

	if (Z_TYPE_P(redis) == IS_RESOURCE) {
		redisContext *context = (redisContext*) zend_fetch_resource(&redis TSRMLS_CC, -1, REDIS_RESOURCE, NULL, 1, le_resource);
		ZEND_VERIFY_RESOURCE(context);
	} else {
		printf("%s\n", "Redis resource error !");
	}

	/*
	context = (redisContext*) zend_fetch_resource(&redis TSRMLS_CC, -1, REDIS_RESOURCE, NULL, 1, le_resource);

	redisContext *c = redisConnect("127.0.0.1", 6379);
	if (c != NULL && c->err) {
    	printf("Error: %s\n", c->errstr);
    	// handle error
	}

	redisCommand(c, "SET LOINT VAALUE");
	printf("%s\n", "FETCH DONE");
}
*/

void redis_set(zval *return_value, zval *redis, zval *key_, zval *value_) {

	char* key   = Z_STRVAL_P(key_);
	char* value = Z_STRVAL_P(value_);

	printf("%s\n", key);
	printf("%s\n", value);

	char* variable = "Hello";
	ZVAL_STRINGL(return_value, variable, strlen(variable), 1);

	/*
	redisContext *context;
	if (Z_TYPE_P(redis) == IS_RESOURCE) {
		le_redis = zend_register_list_destructors_ex(NULL, NULL, REDIS_RESOURCE, REDIS_MODULE_ID);
		//le_redis_persist = zend_register_list_destructors_ex(NULL, redis_persist_dtor, REDIS_RESOURCE, REDIS_MODULE_ID);
		ZEND_FETCH_RESOURCE(context, redisContext*, &redis, -1, REDIS_RESOURCE, le_redis);



	} else {
		printf("%s\n", "Redis resource error !");
	}
	*/
}

/*
void parseContextFromResource(zval *redis) {

	/*redisContext *context;

	if (zend_parse_parameters(PHP_RINIT_FUNCTION() TSRMLS_CC, "r", &redis, NULL, NULL) == FAILURE) {
        RETURN_FALSE;
    }

    int le_resource = zend_register_list_destructors_ex(NULL, NULL, REDIS_RESOURCE, 12345);
    ZEND_FETCH_RESOURCE(context, redisContext*, &redis, -1, REDIS_RESOURCE, le_resource);

    return context;
}

void redis_set(zval *return_value, zval *key_, zval *value_) {
	RETURN_STRING(return_value, 1);

	// Convert zend interface
	/*char* key   = Z_STRVAL_P(key_);
	char* value = Z_STRVAL_P(value_);
	redisContext *context = parseContextFromResource(*redis)
	char cmd[256];
	snprintf(cmd, sizeof cmd, "SET %s %s", key, value);
	redisCommand(context, cmd);
}
*/