{
	'variables':
	{	# Any variable names we wish to create and define
	
	},
	
	'includes':
	[	# Any GYP include - or .gypi - files
		'includes/common-mysql.gypi'	
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
						'RuntimeLibrary': 1		# shared debug
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
						'RuntimeLibrary': 0		# shared release
					}
				}
			}
		},
		'msvs_settings':
		{
			'VCCLCompilerTool':
			{
			},
			'VCLibrarianTool':
			{
			},
			'VCLinkerTool':
			{
				'GenerateDebugInformation': 'true'
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
  	],
    'conditions': 
    [	# Conditional targets
		[ 
			'OS=="win"', 
			{
				'variables':
				{
					'boost_dir%': '<! (IF DEFINED BOOST_ROOT (echo %BOOST_ROOT%) ELSE ( echo <(local_boost_path)/includes/boost_<(boost_version)/include ))'
				},
				'targets':
				[
					{
						'target_name': 'nodamysql',
						'sources': [ 'nodamysql.cpp', 'driver.cpp' ], 
						'dependencies': [ 'library/mysql/cppconn/binding.gyp:mysqlcppconn' ],
						'cflags': [ '-std=c++11' ],
						'include_dirs': [ 'library/mysql/cppconn/win/', 'library/mysql/cppconn/win/driver/', '<(boost_dir)/' ]
						#'libraries': [ '../includes/mysqlcppconn/lib/win6481-mysql645619/opt/mysqlcppconn.lib' ]
					}
				]
      		}
		],
		[
			'OS=="linux"', 
			{
				'sources': [ 'nodamysql.cpp', 'driver.cpp' ],
				'dependencies': [ 'library/mysql/binding.gyp:mysqlcppconn' ],
				'cflags': [ '-std=c++11' ],
				'include_dirs': [ 'includes/mysqlcppconn/include/', 'includes/boost/include/' ],
				'libraries': [ '../includes/mysqlcppconn/lib/win6481-mysql645619/opt/mysqlcppconn.lib' ]
      	 	}
      	 ]
	]
}