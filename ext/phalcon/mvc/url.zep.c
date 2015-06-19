
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "phalcon/mvc/url/utils.h"


/**
 * Phalcon\Mvc\Url
 *
 * This components helps in the generation of: URIs, URLs and Paths
 *
 *<code>
 *
 * //Generate a URL appending the URI to the base URI
 * echo $url->get('products/edit/1');
 *
 * //Generate a URL for a predefined route
 * echo $url->get(array('for' => 'blog-post', 'title' => 'some-cool-stuff', 'year' => '2012'));
 *
 *</code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Mvc_Url) {

	ZEPHIR_REGISTER_CLASS(Phalcon\\Mvc, Url, phalcon, mvc_url, phalcon_mvc_url_method_entry, 0);

	zend_declare_property_null(phalcon_mvc_url_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(phalcon_mvc_url_ce, SL("_baseUri"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(phalcon_mvc_url_ce, SL("_staticBaseUri"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(phalcon_mvc_url_ce, SL("_basePath"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(phalcon_mvc_url_ce, SL("_router"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(phalcon_mvc_url_ce TSRMLS_CC, 1, phalcon_mvc_urlinterface_ce);
	zend_class_implements(phalcon_mvc_url_ce TSRMLS_CC, 1, phalcon_di_injectionawareinterface_ce);
	return SUCCESS;

}

/**
 * Sets the DependencyInjector container
 */
PHP_METHOD(Phalcon_Mvc_Url, setDI) {

	zval *dependencyInjector;

	zephir_fetch_params(0, 1, 0, &dependencyInjector);



	zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);

}

/**
 * Returns the DependencyInjector container
 */
PHP_METHOD(Phalcon_Mvc_Url, getDI) {


	RETURN_MEMBER(this_ptr, "_dependencyInjector");

}

/**
 * Sets a prefix for all the URIs to be generated
 *
 *<code>
 *	$url->setBaseUri('/invo/');
 *	$url->setBaseUri('/invo/index.php/');
 *</code>
 */
PHP_METHOD(Phalcon_Mvc_Url, setBaseUri) {

	zval *baseUri_param = NULL, *_0;
	zval *baseUri = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &baseUri_param);

	if (unlikely(Z_TYPE_P(baseUri_param) != IS_STRING && Z_TYPE_P(baseUri_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'baseUri' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(baseUri_param) == IS_STRING)) {
		zephir_get_strval(baseUri, baseUri_param);
	} else {
		ZEPHIR_INIT_VAR(baseUri);
		ZVAL_EMPTY_STRING(baseUri);
	}


	zephir_update_property_this(this_ptr, SL("_baseUri"), baseUri TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_staticBaseUri"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		zephir_update_property_this(this_ptr, SL("_staticBaseUri"), baseUri TSRMLS_CC);
	}
	RETURN_THIS();

}

/**
 * Sets a prefix for all static URLs generated
 *
 *<code>
 *	$url->setStaticBaseUri('/invo/');
 *</code>
 */
PHP_METHOD(Phalcon_Mvc_Url, setStaticBaseUri) {

	zval *staticBaseUri_param = NULL;
	zval *staticBaseUri = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &staticBaseUri_param);

	if (unlikely(Z_TYPE_P(staticBaseUri_param) != IS_STRING && Z_TYPE_P(staticBaseUri_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'staticBaseUri' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(staticBaseUri_param) == IS_STRING)) {
		zephir_get_strval(staticBaseUri, staticBaseUri_param);
	} else {
		ZEPHIR_INIT_VAR(staticBaseUri);
		ZVAL_EMPTY_STRING(staticBaseUri);
	}


	zephir_update_property_this(this_ptr, SL("_staticBaseUri"), staticBaseUri TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Returns the prefix for all the generated urls. By default /
 */
PHP_METHOD(Phalcon_Mvc_Url, getBaseUri) {

	zval *baseUri = NULL, *phpSelf, *uri = NULL, *_SERVER;

	ZEPHIR_MM_GROW();
	zephir_get_global(&_SERVER, SS("_SERVER") TSRMLS_CC);

	ZEPHIR_OBS_VAR(baseUri);
	zephir_read_property_this(&baseUri, this_ptr, SL("_baseUri"), PH_NOISY_CC);
	if (Z_TYPE_P(baseUri) == IS_NULL) {
		ZEPHIR_OBS_VAR(phpSelf);
		if (zephir_array_isset_string_fetch(&phpSelf, _SERVER, SS("PHP_SELF"), 0 TSRMLS_CC)) {
			ZEPHIR_INIT_VAR(uri);
			phalcon_get_uri(uri, phpSelf);
		} else {
			ZEPHIR_INIT_NVAR(uri);
			ZVAL_NULL(uri);
		}
		ZEPHIR_INIT_NVAR(baseUri);
		if (!(zephir_is_true(uri))) {
			ZVAL_STRING(baseUri, "/", 1);
		} else {
			ZEPHIR_CONCAT_SVS(baseUri, "/", uri, "/");
		}
		zephir_update_property_this(this_ptr, SL("_baseUri"), baseUri TSRMLS_CC);
	}
	RETURN_CCTOR(baseUri);

}

/**
 * Returns the prefix for all the generated static urls. By default /
 */
PHP_METHOD(Phalcon_Mvc_Url, getStaticBaseUri) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *staticBaseUri;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(staticBaseUri);
	zephir_read_property_this(&staticBaseUri, this_ptr, SL("_staticBaseUri"), PH_NOISY_CC);
	if (Z_TYPE_P(staticBaseUri) != IS_NULL) {
		RETURN_CCTOR(staticBaseUri);
	}
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "getbaseuri", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Sets a base path for all the generated paths
 *
 *<code>
 *	$url->setBasePath('/var/www/htdocs/');
 *</code>
 */
PHP_METHOD(Phalcon_Mvc_Url, setBasePath) {

	zval *basePath_param = NULL;
	zval *basePath = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &basePath_param);

	if (unlikely(Z_TYPE_P(basePath_param) != IS_STRING && Z_TYPE_P(basePath_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'basePath' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(basePath_param) == IS_STRING)) {
		zephir_get_strval(basePath, basePath_param);
	} else {
		ZEPHIR_INIT_VAR(basePath);
		ZVAL_EMPTY_STRING(basePath);
	}


	zephir_update_property_this(this_ptr, SL("_basePath"), basePath TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Returns the base path
 */
PHP_METHOD(Phalcon_Mvc_Url, getBasePath) {


	RETURN_MEMBER(this_ptr, "_basePath");

}

/**
 * Generates a URL
 *
 *<code>
 * //Generate a URL appending the URI to the base URI
 * echo $url->get('products/edit/1');
 *
 * //Generate a URL for a predefined route
 * echo $url->get(array('for' => 'blog-post', 'title' => 'some-cool-stuff', 'year' => '2012'));
 *</code>
 */
PHP_METHOD(Phalcon_Mvc_Url, get) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool local, _0, _1, _5;
	zval *uri = NULL, *args = NULL, *local_param = NULL, *baseUri = NULL, *router = NULL, *dependencyInjector = NULL, *routeName, *route = NULL, *queryString = NULL, *_2, *_3 = NULL, _4 = zval_used_for_init, *_6, *_7 = NULL, _8 = zval_used_for_init, *_9, *_10 = NULL, *_11 = NULL, *_12 = NULL, _13, _14, *_15, *_16;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 4, &uri, &args, &local_param, &baseUri);

	if (!uri) {
		ZEPHIR_CPY_WRT(uri, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(uri);
	}
	if (!args) {
		args = ZEPHIR_GLOBAL(global_null);
	}
	if (!local_param) {
		local = 0;
	} else {
		local = zephir_get_boolval(local_param);
	}
	if (!baseUri) {
		ZEPHIR_CPY_WRT(baseUri, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(baseUri);
	}


	if (local == 0) {
		_0 = Z_TYPE_P(uri) == IS_STRING;
		if (_0) {
			_1 = zephir_memnstr_str(uri, SL("//"), "phalcon/mvc/url.zep", 178);
			if (!(_1)) {
				_1 = zephir_memnstr_str(uri, SL(":"), "phalcon/mvc/url.zep", 178);
			}
			_0 = _1;
		}
		if (_0) {
			ZEPHIR_INIT_VAR(_2);
			ZEPHIR_INIT_VAR(_3);
			ZEPHIR_SINIT_VAR(_4);
			ZVAL_STRING(&_4, "#^[a-zA-Z\\:]*//#", 0);
			zephir_preg_match(_3, &_4, uri, _2, 0, 0 , 0  TSRMLS_CC);
			_5 = zephir_is_true(_3);
			if (!(_5)) {
				ZEPHIR_INIT_VAR(_6);
				ZEPHIR_INIT_VAR(_7);
				ZEPHIR_SINIT_VAR(_8);
				ZVAL_STRING(&_8, "#^[a-zA-Z]+:#", 0);
				zephir_preg_match(_7, &_8, uri, _6, 0, 0 , 0  TSRMLS_CC);
				_5 = zephir_is_true(_7);
			}
			if (_5) {
				local = 0;
			} else {
				local = 1;
			}
		} else {
			local = 1;
		}
	}
	if (Z_TYPE_P(baseUri) != IS_STRING) {
		ZEPHIR_CALL_METHOD(&baseUri, this_ptr, "getbaseuri", NULL, 0);
		zephir_check_call_status();
	}
	if (Z_TYPE_P(uri) == IS_ARRAY) {
		ZEPHIR_OBS_VAR(routeName);
		if (!(zephir_array_isset_string_fetch(&routeName, uri, SS("for"), 0 TSRMLS_CC))) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_mvc_url_exception_ce, "It's necessary to define the route name with the parameter 'for'", "phalcon/mvc/url.zep", 196);
			return;
		}
		_9 = zephir_fetch_nproperty_this(this_ptr, SL("_router"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(router, _9);
		if (Z_TYPE_P(router) != IS_OBJECT) {
			_9 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
			ZEPHIR_CPY_WRT(dependencyInjector, _9);
			if (Z_TYPE_P(dependencyInjector) != IS_OBJECT) {
				ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_mvc_url_exception_ce, "A dependency injector container is required to obtain the 'router' service", "phalcon/mvc/url.zep", 208);
				return;
			}
			ZEPHIR_INIT_NVAR(_3);
			ZVAL_STRING(_3, "router", ZEPHIR_TEMP_PARAM_COPY);
			ZEPHIR_CALL_METHOD(&_10, dependencyInjector, "getshared", NULL, 0, _3);
			zephir_check_temp_parameter(_3);
			zephir_check_call_status();
			ZEPHIR_CPY_WRT(router, _10);
			zephir_update_property_this(this_ptr, SL("_router"), router TSRMLS_CC);
		}
		ZEPHIR_CALL_METHOD(&_10, router, "getroutebyname", NULL, 0, routeName);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(route, _10);
		if (Z_TYPE_P(route) != IS_OBJECT) {
			ZEPHIR_INIT_NVAR(_3);
			object_init_ex(_3, phalcon_mvc_url_exception_ce);
			ZEPHIR_INIT_VAR(_11);
			ZEPHIR_CONCAT_SVS(_11, "Cannot obtain a route using the name '", routeName, "'");
			ZEPHIR_CALL_METHOD(NULL, _3, "__construct", NULL, 9, _11);
			zephir_check_call_status();
			zephir_throw_exception_debug(_3, "phalcon/mvc/url.zep", 220 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_INIT_NVAR(_3);
		ZEPHIR_CALL_METHOD(&_10, route, "getpattern", NULL, 0);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_12, route, "getreversedpaths", NULL, 0);
		zephir_check_call_status();
		phalcon_replace_paths(_3, _10, _12, uri TSRMLS_CC);
		ZEPHIR_CPY_WRT(uri, _3);
	}
	if (local) {
		ZEPHIR_SINIT_NVAR(_4);
		ZVAL_LONG(&_4, -1);
		ZEPHIR_INIT_NVAR(_3);
		zephir_substr(_3, baseUri, -1 , 0, ZEPHIR_SUBSTR_NO_LENGTH);
		_0 = ZEPHIR_IS_STRING(_3, "/");
		if (_0) {
			ZEPHIR_SINIT_NVAR(_8);
			ZVAL_LONG(&_8, 0);
			ZEPHIR_SINIT_VAR(_13);
			ZVAL_LONG(&_13, 1);
			ZEPHIR_INIT_NVAR(_7);
			zephir_substr(_7, uri, 0 , 1 , 0);
			_0 = ZEPHIR_IS_STRING(_7, "/");
		}
		if (_0) {
			ZEPHIR_SINIT_VAR(_14);
			ZVAL_LONG(&_14, 1);
			ZEPHIR_INIT_VAR(_15);
			zephir_substr(_15, uri, 1 , 0, ZEPHIR_SUBSTR_NO_LENGTH);
			ZEPHIR_INIT_LNVAR(_11);
			ZEPHIR_CONCAT_VV(_11, baseUri, _15);
			ZEPHIR_CPY_WRT(uri, _11);
		} else {
			ZEPHIR_INIT_LNVAR(_11);
			ZEPHIR_CONCAT_VV(_11, baseUri, uri);
			ZEPHIR_CPY_WRT(uri, _11);
		}
	}
	if (zephir_is_true(args)) {
		ZEPHIR_CALL_FUNCTION(&queryString, "http_build_query", NULL, 363, args);
		zephir_check_call_status();
		_0 = Z_TYPE_P(queryString) == IS_STRING;
		if (_0) {
			_0 = (zephir_fast_strlen_ev(queryString)) ? 1 : 0;
		}
		if (_0) {
			ZEPHIR_SINIT_NVAR(_4);
			ZVAL_STRING(&_4, "?", 0);
			ZEPHIR_INIT_NVAR(_3);
			zephir_fast_strpos(_3, uri, &_4, 0 );
			if (!ZEPHIR_IS_FALSE_IDENTICAL(_3)) {
				ZEPHIR_INIT_LNVAR(_11);
				ZEPHIR_CONCAT_SV(_11, "&", queryString);
				zephir_concat_self(&uri, _11 TSRMLS_CC);
			} else {
				ZEPHIR_INIT_VAR(_16);
				ZEPHIR_CONCAT_SV(_16, "?", queryString);
				zephir_concat_self(&uri, _16 TSRMLS_CC);
			}
		}
	}
	RETVAL_ZVAL(uri, 1, 0);
	RETURN_MM();

}

/**
 * Generates a URL for a static resource
 *
 *<code>
 * // Generate a URL for a static resource
 * echo $url->getStatic("img/logo.png");
 *
 * // Generate a URL for a static predefined route
 * echo $url->getStatic(array('for' => 'logo-cdn'));
 *</code>
 */
PHP_METHOD(Phalcon_Mvc_Url, getStatic) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *uri = NULL, *_0 = NULL, *_1, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &uri);

	if (!uri) {
		uri = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getstaticbaseuri", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_1);
	ZVAL_NULL(_1);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_NULL(_2);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "get", NULL, 0, uri, _1, _2, _0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Generates a local path
 */
PHP_METHOD(Phalcon_Mvc_Url, path) {

	zval *path_param = NULL, *_0;
	zval *path = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &path_param);

	if (!path_param) {
		ZEPHIR_INIT_VAR(path);
		ZVAL_EMPTY_STRING(path);
	} else {
		zephir_get_strval(path, path_param);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_basePath"), PH_NOISY_CC);
	ZEPHIR_CONCAT_VV(return_value, _0, path);
	RETURN_MM();

}

