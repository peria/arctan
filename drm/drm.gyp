{
  'includes': [
    '../base/common.gypi'
  ],
  'targets': [
    {
      'target_name': 'drm_all',
      'type': 'none',
      'dependencies': [
        'drm_main'
      ]
    },
    {
      'target_name': 'drm_main',
      'type': 'executable',
      'dependencies': [
        'drm',
        '../number/number.gyp:integer'
      ],
      'sources': [
        'drm_main.cc'
      ],
      'libraries': [
        '-lgmp'
      ]
    },
    {
      'target_name': 'drm',
      'type': 'static_library',
      'sources': [
        'drm.cc'
      ],
      'dependencies': [
        '../number/number.gyp:integer'
      ]
    }
  ]
}
