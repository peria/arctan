{
  'includes': [
    '../base/common.gypi'
  ],
  'targets': [
    {
      'target_name': 'search',
      'type': 'static_library',
      'sources': [
        'search.cc',
      ],
      'dependencies': [
        '../base/base.gyp::prime'
      ]
    }
  ]
}
