dep_x11 = dependency('x11')

matrix_dependencies = dependencies + [dep_x11]

matrix_src = [
  'main.cpp',
  '../shared/sndsdl.cpp',
  '../shared/ctrlssdl.cpp',
  '../shared/matrix_client.cpp',
]

# TODO: why do we have to redeclare it here with one more fs level?
includes = include_directories(
  '../../../../src',
  '../../../../src/libretro-common/include',
  '../../../../src/frontend',
)

executable('desmume-matrix',
  matrix_src,
  dependencies: matrix_dependencies,
  include_directories: includes,
  link_with: libdesmume,
  install: true,
)

install_man('doc/desmume-matrix.1')
