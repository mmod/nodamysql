/**
 * package: nodamysql
 * version:  0.1.1
 * author:  Richard B. Winters <a href="mailto:rik@massivelymodified.com">rik At MMOGP</a>
 * copyright: 2011-2014 Massively Modified, Inc.
 * license: Apache, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>
 */

// Deps


module.exports = exports = nodamysql = function()
{
	var nkmysql = null;
	
	if( process.arch == 'x64' )
	{
		if( process.platform == 'win32' )
		{
			nkmysql = require( './includables/module/precompiled/win/win6481-mysql645619/opt/nodamysql' );	// Release build
		}
		else
		{
			// Must be linux
			nkmysql = require( './includes/precompiled/lin/deb6475-mysql645619/opt/nodamysql' );
		}
	}
	else
	{	// It's x86
		if( process.platform == 'win32' )
		{	// We don't actually have this yet... personally we'll always use 64-bit here at MMod.
			nkmysql = require( './includables/module/precompiled/win/win327-mysql325619/opt/nodamysql' );
		}
		else
		{
			// Must be linux
			nkmysql = require( './includes/precompiled/lin/deb3275-mysql645619/opt/nodamysql' );
		}
	}
	
	// If the return is null, you'll need to build
	return nkmysql;
};

