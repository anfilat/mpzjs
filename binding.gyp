{
  'targets': [
    {
      'target_name': 'gmpbigint',
      'sources': [ 'src/gmpbigint.cc' ],
      'include_dirs': [
        "<!(node -e \"require('nan')\")"
      ],
      'conditions': [
        ['OS=="linux"',
          {
            'link_settings': {
              'libraries': [
                '-lgmp'
              ]
            }
          }
        ],
        ['OS=="mac"',
          {
            'link_settings': {
              'libraries': [
                '-lgmp'
              ]
            }
          }
        ],
        ['OS=="win"',
          {
            'link_settings': {
              'libraries': [
                '-lgmp.lib'
              ],
            }
          }
        ]
      ]
    }
  ]
}
