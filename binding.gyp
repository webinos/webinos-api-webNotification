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
                    # tdb
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