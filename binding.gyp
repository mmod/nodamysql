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
				'defines': [ 'DEBUG', '_DEBUG' ]
			},
			'Release':
			{
				'defines': [ 'NDEBUG' ]
			}
		},
		'conditions':
		[
			[
				'OS=="win"',
				{
					'configurations':
					{
						'Debug':
						{
							'defines': [ 'WIN32' ],
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
							'defines': [ 'WIN32' ],
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
					}
				}
			],
			[
				'OS!="win"',
				{
					'configurations':
					{
						'Debug':
						{
							'cflags': [ '-std=c++11', '-fexceptions', '-frtti', '-Wno-deprecated', '-Wno-unused-variable', '-Wno-unused-but-set-variable', '-Wno-maybe-uninitialized', '-Wno-sign-compare', '-Wno-reorder', '-Wno-extra', '-Wno-switch' ],
							'cflags_cc':
							[
								'-std=c++11',
								'-fexceptions',					# Enable exception handler
								'-frtti',						# Allow dynamic casting
								'-Wno-deprecated',				# Remove deprecated warnings
								'-Wno-unused-variable',			# Remove unused-variable warnings
								'-Wno-unused-but-set-variable',	# Remove unused-but-set-variable warnings
								'-Wno-maybe-uninitialized',		# Remove maybe me used uninitialized warnings (Many false positives in GCC 4.7)
								'-Wno-sign-compare',			# Remove sign compare warnings
								'-Wno-reorder',					# Remove initialize after warnings
								'-Wno-extra',					# Remove explicit initialization warnings
								'-Wno-switch',					# Remove switch warnings
							]
						},
						'Release':
						{
							'cflags': [ '-std=c++11', '-fexceptions', '-frtti', '-Wno-deprecated', '-Wno-unused-variable', '-Wno-unused-but-set-variable', '-Wno-maybe-uninitialized', '-Wno-sign-compare', '-Wno-reorder', '-Wno-extra', '-Wno-switch' ],
							'cflags_cc': [ '-std=c++11', '-fexceptions', '-frtti', '-Wno-deprecated', '-Wno-unused-variable', '-Wno-unused-but-set-variable', '-Wno-maybe-uninitialized', '-Wno-sign-compare', '-Wno-reorder', '-Wno-extra', '-Wno-switch' ]
						}
					}
				}
			]
		]
	},
  	'targets': 
  	[	# Any 'for sure' targets
  		{
			'target_name': 'action_after_build',
			'type': 'none',
			'dependencies': [ 'nodamysql' ],
			'copies':
			[
				{
					'files': [ 'includables/module/index.js' ],
					'destination': '<(module_root_dir)'
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
						'dependencies': [ 'library/mysql/connectorc++/binding.gyp:mysqlcppconn' ],  # The MySQL C++ Connector is automatically linked due to this step
						'include_dirs': [ 'library/mysql/connectorc++/', 'library/mysql/connectorc++/driver/', '<(boost_dir)/' ]
					}
				]
      		}
		],
		[
			'OS!="win"', 
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
						'dependencies': [ 'library/mysql/connectorc++/binding.gyp:libmysqlcppconn' ],
						'include_dirs': [ 'library/mysql/connectorc++/', 'library/mysql/connectorc++/driver/' ],
						'link_settings':
						{
							'ldflags':
							[
								'-Wl,-rpath=<(module_root_dir)/build/Release'	# An issue with shared libraries and GYP require us to set this for runtime linking
							]
						}
					}
				]
      	 	}
      	 ]
	]
}