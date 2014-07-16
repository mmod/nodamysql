/**
 * package: nodamysql
 * version:  0.1.4
 * author:  Richard B. Winters <a href="mailto:rik@mmogp.com">rik At MassivelyModified</a>
 * copyright: 2013-2014 Massively Modified, Inc.
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */


// INCLUDES
#include <v8.h>
#include <node.h>
#include "driver.h"

using namespace v8;


/**
 * Initializes all object exports
 *
 * @param Handle<Object>	exports		Defines a reference to an exports object (Syntactically taken)
 *
 * @return void
 *
 * @since 0.0.1
 */
void InitAll( Handle<Object> exports )
{
	Driver::Init( exports );
}


// REQUIRED MACRO
NODE_MODULE( nodamysql, InitAll )
