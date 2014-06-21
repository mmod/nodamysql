{
	'variables':
	{	# Any variable names we wish to create and define
	},
	
	'includes':
	[	# Any GYP include - or .gypi - files
		'../../../includes/common-mysql.gypi'	
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
						'ExceptionHandling': 1		# /EHsc
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
						'ExceptionHandling': 1		# /EHsc
					}
				}
			}
		},
		'msvs_settings':
		{
			'VCCLCompilerTool':
			{
				'RuntimeLibrary': 2,		# shared release
				'ExceptionHandling': 1,		# /EHsc, but doesnt seem to work very well
				'AdditionalOptions':
				[
					'/GR',
					'/MD',
					'/EHsc', # Enable unwind semantics for Exception Handling.  This one actually does the trick - and no warning either.
					'/Ddllimport=dllexport',	# This puppy makes sure to redefine the dllimport flag as dllexport for when we are building the dll on windows
				]
			},
			'VCLibrarianTool':
			{
			},
			'VCLinkerTool':
			{
				'GenerateDebugInformation': 'true',
				'AdditionalOptions':
				[
					'/FORCE:MULTIPLE'
				]
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
					'boost_dir%': '<!(IF DEFINED BOOST_ROOT (echo %BOOST_ROOT%) ELSE (echo C:/boost/boost_<(boost_version)))',
					'mysql_dir%': '<!(IF DEFINED MYSQL_DIR (echo %MYSQL_DIR%) ELSE (echo C:/mysql/server/<(mysql_version)))',
					'cppconn_inc_dir': 'win',
					'cppconn_inc_dir_for_driver': 'win/cppconn'
				},
				'targets':
				[
					{
						'target_name': 'mysqlcppconn',
						'type': 'shared_library',
						'sources': 
						[ 
							'win/driver/mysql_art_resultset.cpp',
							'win/driver/mysql_art_rset_metadata.cpp',
							'win/driver/mysql_connection.cpp', 
							'win/driver/mysql_debug.cpp',
							'win/driver/mysql_driver.cpp',
							'win/driver/mysql_metadata.cpp',
							'win/driver/mysql_parameter_metadata.cpp',
							'win/driver/mysql_prepared_statement.cpp',
							'win/driver/mysql_ps_resultset.cpp',
							'win/driver/mysql_ps_resultset_metadata.cpp',
							'win/driver/mysql_resultbind.cpp',
							'win/driver/mysql_resultset.cpp',
							'win/driver/mysql_resultset_metadata.cpp',
							'win/driver/mysql_statement.cpp',
							'win/driver/mysql_uri.cpp',
							'win/driver/mysql_util.cpp',
							'win/driver/mysql_warning.cpp'
						],
						'cflags': [ '-std=c++11', '-DBUILD_SHARED=1' ],
						'include_dirs': [ '<(cppconn_inc_dir)/', '<(cppconn_inc_dir_for_driver)/', '<(mysql_dir)/include/', '<(boost_dir)/' ],
						'libraries': [ '<(mysql_dir)/lib/libmysql' ]
					},
					#{
					#	'target_name': 'action_after_build',
					#	'type': 'none',
					#	'dependencies': [ 'mysqlcppconn' ],
					#	'copies':
					#	[
					#		{
					#			'files': [ '<(PRODUCT_DIR)/mysqlcppconn.dll', '<(PRODUCT_DIR)/mysqlcppconn.lib' ],
					#			'destination': 'build/Release'
					#		}
					#	]
					#}
				]
      		}
		],
		[
			'OS=="linux"', 
			{
				'variables':
				{
					'boost_dir%': '<!(if [ -z $BOOST_ROOT ]; then echo \"/opt/boost_<(boost_version)/\"; else echo $BOOST_ROOT; fi)',
					'mysql_dir%': '<!(if [ -z $MYSQL_DIR ]; then echo \"/opt/mysql/server/<(mysql_version)/\"; else echo $MYSQL_DIR; fi)'
				},
				'targets':
				[
					{
						'target_name': 'libmysqlcppconn',
						'type': 'shared_library',
						'sources': 
						[ 
							'mysql_art_resultset.cpp',
							'mysql_art_rset_metadata.cpp',
							'mysql_connection.cpp', 
							'mysql_debug.cpp',
							'mysql_driver.cpp',
							'mysql_metadata.cpp',
							'mysql_parameter_metadata.cpp',
							'mysql_prepared_statement.cpp',
							'mysql_ps_resultset.cpp',
							'mysql_ps_resultset_metadata.cpp',
							'mysql_resultbind.cpp',
							'mysql_resultset.cpp',
							'mysql_resultset_metadata.cpp',
							'mysql_statement.cpp',
							'mysql_uri.cpp',
							'mysql_util.cpp',
							'mysql_warning.cpp'
						],
						'cflags': [ '-std=c++11' ],
						'include_dirs': [ 'includes/mysqlcppconn/include/', 'includes/boost/include/' ],
						'libraries': [ '../includes/mysqlcppconn/lib/win6481-mysql645619/opt/mysqlcppconn.lib' ]
		      	 	},
					{
						'target_name': 'action_after_build',
						'type': 'none',
						'dependencies': [ 'libmysqlcppconn' ],
						'copies':
						[
							{
								'files': [ ],
								'destination': ''
							}
						]
					}
				]
			}
		]
	]
}