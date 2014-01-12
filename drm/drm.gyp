{
  'includes': [
    '../base/common.gypi'
  ],
  'targets': [
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
      'target_name': 'drm',
      'type': 'static_library',
      'sources': [
        'drm.cc'
      ]
    }
  ]
}
