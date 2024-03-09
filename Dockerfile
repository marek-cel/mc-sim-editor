FROM ubuntu:22.04

RUN apt update
RUN apt install -y \
    build-essential \
    cmake \
    gdb \
    git \
    googletest \
    googletest-tools \
    lcov \
    libgmock-dev \
    libgtest-dev \
    libopenscenegraph-dev \
    libqt5opengl5-dev \
    libqt5svg5-dev \
    libqt5x11extras5-dev \
    libxml2-dev \
    python3-all \
    qtbase5-dev \
    qtbase5-dev-tools

RUN git clone --branch main https://github.com/marek-cel/libmcutils.git /src
RUN cd /src; cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DBUILD_TESTING=Off -B build
RUN cd /src; cmake --build build --config Release -j 4
RUN cd /src; cmake --build build --config Release --target install
RUN rm -r /src
RUN ldconfig

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME
RUN useradd --uid $USER_UID --gid $USER_GID -m $USERNAME

USER $USERNAME
