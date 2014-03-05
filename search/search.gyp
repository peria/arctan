{
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
        'search',
        '<(DEPTH)/base/base.gyp:time'
      ]
    },
    {
      'target_name': 'search',
      'type': 'static_library',
      'sources': [
        'search.cc',
      ],
      'dependencies': [
        '<(DEPTH)/base/base.gyp:prime',
        '<(DEPTH)/base/base.gyp:modulo'
      ]
    }
  ]
}
