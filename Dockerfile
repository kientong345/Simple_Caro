FROM ubuntu:22.04
SHELL ["/bin/bash", "-xec"]

RUN apt-get update;\
    apt-get dist-upgrade --no-install-recommends --purge --yes\
        clang-12\
        cmake\
        g++\
        make\
        libncurses5-dev libncursesw5-dev\
        ;\
    apt-get autoremove --purge --yes;\
    apt-get clean;\
    ln -s /usr/bin/clang-12 /usr/bin/clang;\
    ln -s /usr/bin/clang++-12 /usr/bin/clang++;\
    ln -s /usr/bin/clang-tidy-12 /usr/bin/clang-tidy

WORKDIR /app

COPY . .

CMD ["/bin/bash"]