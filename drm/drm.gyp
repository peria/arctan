{
  'includes': [
    '../base/common.gypi'
  ],
  'targets': [
    {
      'target_name': 'all',
      'type': 'none',
      'dependencies': [
        'drm_main',
	'drm2_main'
      ]
    },
    {
      'target_name': 'drm_main',
      'type': 'executable',
      'dependencies': [
        'drm'
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
        'drm2'
      ],
      'sources': [
        'drm_main.cc'
      ],
      'libraries': [
        '-lgmp'
      ],
      'define': {
        # -DDRM2
      }
    },
    {
      'target_name': 'drm',
      'type': 'static_library',
      'sources': [
        'drm.cc'
      ]
    },
    {
      'target_name': 'drm2',
      'type': 'static_library',
      'sources': [
        'drm2.cc'
      ],
      'dependencies': [
        'number'
      ]
    },
    {
      'target_name': 'number',
      'type': 'static_library',
      'sources': [
        'number.cc'
      ]
    }
  ]
}
