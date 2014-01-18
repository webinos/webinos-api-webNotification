{
  'variables': {
    'module_name': 'notifications'
  },
    'targets': [
        {
            'target_name': '<(module_name)',
            'product_name':'notifications',
            'sources': [
              "src/notifications.cc"
            ],
            'conditions': [
                ["OS==\"win\"", {
                    'include_dirs': [
                        "src/"
                    ],
					'sources': [
                        "src/platform/win8/notifications.cpp",
                        "src/platform/win8/StringReferenceWrapper.h",
                        "src/platform/win8/win8Toast.h",
                        "src/platform/win8/win8Toast.cpp"
                    ],
					'msvs_configuration_attributes': {
						'CharacterSet': '1',  # Unicode
					},
					'link_settings': {
						'libraries': [
						  '-lruntimeobject.lib',
						  '-lshlwapi.lib',
						  '-lwinspool.lib',
						],
					},
                }],
                ["OS==\"mac\"", {
                    'include_dirs': [
                        "src/"
                    ],
                    'xcode_settings': {
                        "OTHER_CFLAGS": [
                            "-ObjC++"
                        ]
                    },
                    'link_settings' : {
                        "libraries" : [
                            "-framework Cocoa"
                        ]
                    },
                    'sources': [
                        "src/platform/mac/FakeBundle.m",
                        "src/platform/mac/notifications.cpp"
                    ]
                }, {
                    # tdb
                }]
            ]
        },
        {
            'target_name': 'webinos_wrt',
            'type': 'none',
            'toolsets': ['host'],
            'copies': [
              {
                'files': [
                  '<(PRODUCT_DIR)/notifications.node',
                ],
                'destination': 'node_modules/',
              }],
        }
    ]
}