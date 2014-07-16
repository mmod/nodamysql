/**
 * package: nodamysql
 * version:  0.1.4
 * author:  Richard B. Winters <a href="mailto:rik@massivelymodified.com">rik At MMOGP</a>
 * copyright: 2011-2014 Massively Modified, Inc.
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */


// Deps

var nodamysql = null;
	
	if( process.arch == 'x64' )
	{
		if( process.platform == 'win32' )
		{
			nodamysql = require( './includables/module/precompiled/win/win8164-mysql561964/opt/nodamysql' );	// Release build
		}
		else
		{
			// Must be linux
			nodamysql = require( './includes/precompiled/lin/deb7564-mysql561964/opt/nodamysql' );
		}
	}
	else
	{	// It's x86
		if( process.platform == 'win32' )
		{	// We don't actually have this yet... personally we'll always use 64-bit here at MMod.
			nodamysql = require( './includables/module/precompiled/win/win732-mysql561932/opt/nodamysql' );
		}
		else
		{
			// Must be linux
			nodamysql = require( './includes/precompiled/lin/deb7532-mysql561932/opt/nodamysql' );
		}
	}

module.exports = exports = nodamysql;


