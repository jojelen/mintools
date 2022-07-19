project('minparse', ['cpp'], default_options: ['cpp_std=c++17', 'warning_level=3'],)

cc = meson.get_compiler('cpp')

if get_option('buildtype').startswith('release')
    add_project_arguments('-DNDEBUG', language : ['cpp'])
else
    add_project_arguments('-DDEBUG', language : ['cpp'])
endif

inc_dir = include_directories('include')

subdir('test')
