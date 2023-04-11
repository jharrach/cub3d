#!/bin/bash
cd ~ && mkdir .brew 2>/dev/null && curl -L https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C .brew

grep 'PATH="$HOME/.brew/bin:$PATH"' ~/.bash_profile >/dev/null || echo 'PATH="$HOME/.brew/bin:$PATH"' >> ~/.bash_profile
grep 'PATH="$HOME/.brew/bin:$PATH"' ~/.zshrc >/dev/null || echo 'PATH="$HOME/.brew/bin:$PATH"' >> ~/.zshrc
brew install cmake
brew install glfw
