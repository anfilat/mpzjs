{
  'targets': [
    {
      'target_name': 'libgmp',
      'type': 'none',
      'actions': [{
         'action_name': 'build_gmp_lib',
         'inputs': [''],
         'outputs': [''],
         'action': ['sh', 'scripts/build.sh']
      }]
    },
    {
      'target_name': 'mpzjs',
      'dependencies': ['libgmp'],
      'cxxflags': ['-fPIC'],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'xcode_settings': { 'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7',
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      },
      'sources': [ 'src/mpzjs.cc', 'src/mpz.cc' ],
      'include_dirs': [
        '<!@(node -p "require(\'node-addon-api\').include")',
        '<(module_root_dir)/deps/gmp-build-6.2.0/include',
      ],
      'conditions': [
        ['OS=="linux"',
          {
            'link_settings': {
              'libraries': [
                '<(module_root_dir)/deps/gmp-build-6.2.0/lib/libgmp.a'
              ]
            }
          }
        ],
        ['OS=="mac"',
          {
            'link_settings': {
              'libraries': [
                '<(module_root_dir)/deps/gmp-build-6.2.0/lib/libgmp.a'
              ]
            }
          }
        ]
      ]
    }
  ]
}
