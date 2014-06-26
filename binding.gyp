{
	'variables':
	{	# Any variable names we wish to create and define
	
	},
	'includes':
	[	# Any GYP include - or .gypi - files
		'includables/common-mysql.gypi'	
	],
	'target_defaults':
	{	# Any default configuration settings
	
		'default_configuration': 'Release',
		'cflags':
		[
			'-std=c++11'
		],
		'configurations':
		{
			'Debug': 
			{
				'defines': [ 'DEBUG', '_DEBUG' ],
				'msvs_settings':
				{
					'VCCLCompilerTool':
					{
						'RuntimeLibrary': 3,		# shared debug
						'ExceptionHandling': 1,		# /EHsc
						'AdditionalOptions': 
						[
							'/EHsc' # Enable unwind semantics for Exception Handling.  This one actually does the trick - and no warning either.
						]
					}
				}
			},
			'Release':
			{
				'defines': [ 'NDEBUG' ],
				'msvs_settings':
				{
					'VCCLCompilerTool':
					{
						'RuntimeLibrary': 2,		# shared release
						'ExceptionHandling': 1,		# /EHsc
						'AdditionalOptions': 
						[
							'/EHsc' # Enable unwind semantics for Exception Handling.  This one actually does the trick - and no warning either.
						]
					}
				}
			}
		},
		'conditions':
		[
			[
				'OS == "win"',
				{
					'defines':
					[
						'WIN32'
					]
				}
			]
		]
	},
  	'targets': 
  	[	# Any 'for sure' targets
  		{	# This target copies an index.js file for the module that is designed for loading pre-compiled variants of the nodamysql add-on.  This acts as a fall-back if subsequent rebuilding/updates fail. 
			'target_name': 'action_before_build',
			'type': 'none',
			'copies':
			[
				{
					'files': [ 'includables/module/precompiled/index.js' ],
					'destination': './'
				}
			]
		}
  	],
    'conditions': 
    [	# Conditional targets
		[ 
			'OS=="win"', 
			{
				'variables':
				{
					'boost_dir%': '<!(IF DEFINED BOOST_ROOT (echo %BOOST_ROOT%) ELSE (echo C:/boost/boost_<(boost_version)))'
				},
				'targets':
				[
					{
						'target_name': 'nodamysql',
						'sources': [ 'nodamysql.cpp', 'driver.cpp' ], 
						'dependencies': [ 'action_before_build', 'library/mysql/cppconn/binding.gyp:mysqlcppconn' ],  # The MySQL C++ Connector is automatically linked due to this step
						'cflags': [ '-std=c++11' ],
						'include_dirs': [ 'library/mysql/cppconn/win/', 'library/mysql/cppconn/win/driver/', '<(boost_dir)/' ]
					},
					{	# This target copies an index.js file to the root of the module which will load the recently built variant of the add-on/library, replacing the default which loads a pre-compiled variant.
						'target_name': 'action_after_build',
						'type': 'none',
						'dependencies': [ 'nodamysql' ],
						'copies':
						[
							{
								'files': [ 'includables/module/index.js' ],
								'destination': './'
							}
						]
					}
				]
      		}
		],
		[
			'OS=="linux"', 
			{
				'variables':
				{
					'boost_dir%': '<!(if [ -z $BOOST_ROOT ]; then echo \"/opt/boost_<(boost_version)/\"; else echo $BOOST_ROOT; fi)',
				},
				'targets':
				[
					{
						'target_name': 'nodamysql',
						'sources': [ 'nodamysql.cpp', 'driver.cpp' ],
						'dependencies': [ 'action_before_build', 'library/mysql/cppconn/binding.gyp:mysqlcppconn' ],	# The MySQL C++ Connector is automatically linked due to this step
						'cflags': [ '-std=c++11' ],
						'include_dirs': [ 'includables/mysqlcppconn/include/', 'includables/boost/include/' ],
					},
					{	# This target copies an index.js file to the root of the module which will load the recently built variant of the add-on/library, replacing the default which loads a pre-compiled variant.
						'target_name': 'action_after_build',
						'type': 'none',
						'dependencies': [ 'nodamysql' ],
						'copies':
						[
							{
								'files': [ 'includables/module/index.js' ],
								'destination': './'
							}
						]
					}
				]
      	 	}
      	 ]
	]
}