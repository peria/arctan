{
  'includes': [
    '../base/common.gypi'
  ],
  'targets': [
    {
      'target_name': 'drm_all',
      'type': 'none',
      'dependencies': [
        'drm_main',
        'drm2_main',
        'drm3_main',
        'drm4_main'
      ]
    },
    {
      'target_name': 'drm_main',
      'type': 'executable',
      'dependencies': [
        'drm',
        '../number/number.gyp:number'
      ],
      'sources': [
        'drm_main.cc'
      ],
      'libraries': [
        '-lgmp'
      ]
    },
    {
      'target_name': 'drm2_main',
      'type': 'executable',
      'dependencies': [
        'drm2',
        '../number/number.gyp:number'
      ],
      'sources': [
        'drm2_main.cc'
      ],
      'libraries': [
        '-lgmp'
      ]
    },
    {
      'target_name': 'drm3_main',
      'type': 'executable',
      'dependencies': [
        'drm3',
        '../number/number.gyp:number'
      ],
      'sources': [
        'drm3_main.cc'
      ],
      'libraries': [
        '-lgmp'
      ]
    },
    {
      'target_name': 'drm4_main',
      'type': 'executable',
      'dependencies': [
        'drm4',
        '../number/number.gyp:number'
      ],
      'sources': [
        'drm4_main.cc'
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
        '../number/number.gyp:number'
      ]
    }
  ]
}
