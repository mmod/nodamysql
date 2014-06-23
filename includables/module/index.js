/**
 * package: nodamysql
 * version:  0.0.1
 * author:  Richard B. Winters <a href="mailto:rik@massivelymodified.com">rik At MMOGP</a>
 * copyright: 2011-2014 Massively Modified, Inc.
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */

// Deps


module.exports = exports = nodamysql = function()
{
	var nkmysql = null;
	
	nkmysql = require( './build/Release/nodamysql' );
	
	return nkmysql;
};

