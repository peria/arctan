{
  'targets': [
    {
      'target_name': 'drm_all',
      'type': 'none',
      'dependencies': [
        'drm_main',
        'drm4_test'
      ]
    },
    {
      'target_name': 'drm_main',
      'type': 'executable',
      'dependencies': [
        'drm',
        'drm2',
        'drm3',
        'drm4',
        '../number/number.gyp:number'
      ],
      'sources': [
        'drm_main.cc'
      ],
      'libraries': [
        '-lgmp',
        '-lgflags',
        '-lglog'
      ]
    },
    {
      'target_name': 'drm',
      'type': 'static_library',
      'sources': [
        'drm.cc'
      ],
      'dependencies': [
        '../number/number.gyp:number'
      ]
    },
    {
      'target_name': 'drm2',
      'type': 'static_library',
      'sources': [
        'drm2.cc'
      ],
      'dependencies': [
        'drm',
        '../number/number.gyp:number'
      ]
    },
    {
      'target_name': 'drm3',
      'type': 'static_library',
      'sources': [
        'drm3.cc'
      ],
      'dependencies': [
        'drm',
        '../number/number.gyp:number'
      ]
    },
    {
      'target_name': 'drm4',
      'type': 'static_library',
      'sources': [
        'drm4.cc'
      ],
      'dependencies': [
        'drm',
        '../number/number.gyp:number'
      ]
    },
    {
      'target_name': 'drm4_test',
      'type': 'executable',
      'sources': [
        'drm4_test.cc'
      ],
      'dependencies': [
        'drm4'
      ],
      'libraries': [
        '-lgmp',
        '-lgtest',
        '-lpthread',
        '-lgtest_main'
      ]
    }
  ]
}
