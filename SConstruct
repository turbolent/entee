env = Environment(CC = 'clang', 
                  CCFLAGS = ('-Wall -Werror -Wextra -Winline'
                             ' -Wno-unused-variable'
                             ' -Wno-unused-parameter'
	                     ' -g -O3 -std=c99 -pedantic -march=native'))
env.Append(BUILDERS =
    {'Ragel': Builder(action = 'ragel -C -G2 $SOURCE -o $TARGET',
                      suffix='.c', src_suffix = '.c.rl')})
env.Ragel(Glob('src/*.c.rl'))
env.Program('loader', Glob('src/*.c'), LIBS=['pthread', 'z', 'm'])
