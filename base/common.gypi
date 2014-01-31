#export CXXFLAGS+=-O3 -std=c++0x -Wall -I$(CURDIR)
#export LDFLAGS+=-lgmp -L$(CURDIR)
#subdirs=arctan
{
  'target_defaults': {
    'default_configuration': 'Release',
    'cflags': [
      '-std=c++0x',
      '-Wall',
    ],
    'libralies': [
      '-lglog',
      '-lgflags'
    ],
    'include_dirs': [
      '..'
    ],

    'configurations': {
      'Debug': {
        'cflags': ['-g', '-pg'],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'Optimization': '0', # /Od
            'RuntimeLibrary': '1', # /MTd
          },
          'VCLinkerTool': {
            'LinkIncremental': '2',
          },
        },
        'xcode_settings': {
          'GCC_OPTIMIZATION_LEVEL': '0', # -O0
        },
      }, # Debug
      'Release': {
        'cflags+': ['-O3'],
        'msvs_settings':{
          'VCCLCompilerTool': {
            'Optimization': '2', # /O2
            'InlineFunctionExpansion': '2',
            'RuntimeLibrary': '0', # /MT
          },
          'VCLinkerTool': {
            'LinkIncremental': '1',
            'OptimizeReferences': '2',
          },
        },
        'xcode_settings': {
          'GCC_OPTIMIZATION_LEVEL': '3', # -O3
        },
      }, # Release
    },
    'variables': {
      'component%': 'static_library',
    },
  }, # target_defaults
}
