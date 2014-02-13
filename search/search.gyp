{
  'includes': [
    '../base/common.gypi'
  ],
  'targets': [
    {
      'target_name': 'search_all',
      'type': 'none',
      'dependencies': [
        'search_main'
      ]
    },
    {
      'target_name': 'search_main',
      'type': 'executable',
      'sources': [
        'search_main.cc',
      ],
      'dependencies': [
        'search'
      ]
    },
    {
      'target_name': 'search',
      'type': 'static_library',
      'sources': [
        'search.cc',
      ],
      'dependencies': [
        '../base/base.gyp:prime'
      ]
    }
  ]
}
