# Package hacking
https://nix-tutorial.gitlabpages.inria.fr/nix-tutorial/first-package.html

# CMake
You first have to generate the project.

## First try
If you try to simply:
'''
mkdir build ; cd build ; cmake ..
'''
You'll have error messages telling you that all the libraries you depend on are missing.

## Build an env
The env building should be done with nix. More specifically, a nix-shell should make all the
necesary dependencies available, so:
'''
nix-shell
'''
Once you've dropped into the nix-shell, you should be able to run the cmake command.

# Build the binary
Then you can actually compile and link your executable.

## Naive attempt
From within your build directory, you can launch the following command to try starting a build:
'''
cmake --build .
'''
You'll see that make (which should be the generator used by default manages to compile your sources
but fails at the linker stage. An example error would look like:
/nix/store/kmqs0wll31ylwbqkpmlgbjrn6ny3myik-binutils-2.35.1/bin/ld: /nix/store/73k2knwhv4zzizsjr1g1dqjvzkamx8bd-glibc-2.33-49/lib/libpthread.so.0: undefined reference to `__libc_dlclose@GLIBC_PRIVATE'
First you see the linker utility which is provided by the nix store. Next you have the piece of
code that is causing the linker to error out, which is the pthread library fetched from the nix
store. Finally, the symbol that can't be found by the linker is provided, here it seems to be
something related to the glibc. My guess is that the nix env provided makes a lot of assuptions
about the libraries available and those assumptions don't hold when manually building.

# Conclusion
That's as far as we're going to go. It is pointless to try to setup the environment manually when
the whole point of our tooling is to do so manually. We ended up going down the flake road even if
that was not our primary intention. Some great documentation was found along the way which should
help with further development.
