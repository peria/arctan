{
  'target_defaults': {
    'dependencies': [
      '<(DEPTH)/number/number.gyp:number'
    ]
  },
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
        'drm5',
        '<(DEPTH)/number/number.gyp:number'
      ],
      'sources': [
        'drm_main.cc'
      ]
    },
    {
      'target_name': 'drm',
      'type': 'static_library',
      'sources': ['drm.cc'],
    },
    {
      'target_name': 'drm2',
      'type': 'static_library',
      'sources': ['drm2.cc'],
      'dependencies': ['drm']
    },
    {
      'target_name': 'drm3',
      'type': 'static_library',
      'sources': ['drm3.cc'],
      'dependencies': ['drm']
    },
    {
      'target_name': 'drm4',
      'type': 'static_library',
      'sources': [ 'drm4.cc' ],
      'dependencies': [
        'drm',
        '<(DEPTH)/base/base.gyp:prime'
      ]
    },
    {
      'target_name': 'drm5',
      'type': 'static_library',
      'sources': [ 'drm5.cc' ],
      'dependencies': [
        'drm',
        '<(DEPTH)/base/base.gyp:prime'
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
        '-lgtest',
        '-lpthread',
        '-lgtest_main'
      ]
    }
  ]
}
