cd ~ && mkdir .brew && curl -L https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C .brew
echo 'PATH="$HOME/.brew/bin:$PATH"' >> ~/.bash_profile
echo 'PATH="$HOME/.brew/bin:$PATH"' >> ~/.zshrc
brew install cmake
brew install glfw
