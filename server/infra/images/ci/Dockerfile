FROM ubuntu:22.04

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
               gcc-12 \
               g++-12 \
               build-essential \
               libc6-dev \
               make \
               cmake \
               dpkg-dev \
               gdb \
               doxygen \
               valgrind \
               python3-dev \
               git \
               wget \
               ca-certificates \
               unzip \
               libssl-dev \
               openssl \
               rapidjson-dev \
               libboost-all-dev \
               clang-format \
               iproute2 \
               iputils-ping \
    && apt-get clean \
    && rm -rf /tmp/* /var/tmp/*

## install cmake
#RUN wget https://github.com/Kitware/CMake/archive/refs/tags/v3.27.4.zip \
#	&& unzip v3.27.4.zip && cd CMake-3.27.4 && CC=/usr/bin/gcc-12 CXX=/usr/bin/g++-12 ./bootstrap \
#	&& make -j10 && make install && cd ../
#RUN rm -fr v3.27.4.zip && rm -fr CMake-3.27.4

# install gtest
RUN wget https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip \
	&& unzip v1.14.0.zip && cd googletest-1.14.0/ && mkdir build && cd build \
	&& cmake .. -DCMAKE_C_COMPILER=/usr/bin/gcc-12 -DCMAKE_CXX_COMPILER=/usr/bin/g++-12 && make -j8 && make install && cd ../..
RUN rm -fr v1.14.0.zip && rm -fr googletest-1.14.0/

# install glog
RUN wget https://github.com/google/glog/archive/refs/tags/v0.6.0.zip \
	&& unzip v0.6.0.zip && cd glog-0.6.0 && mkdir build && cd build \
	&& cmake .. -DCMAKE_CXX_COMPILER=/usr/bin/g++-12 && make -j8 && make install && cd ../..
RUN rm -fr v0.6.0.zip && rm -fr glog-0.6.0/

# install benchmark
RUN wget https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip \
	&& unzip v1.8.3.zip && cd benchmark-1.8.3 && mkdir build && cd build \
	&& cmake .. -DBENCHMARK_ENABLE_GTEST_TESTS=OFF -DCMAKE_CXX_COMPILER=/usr/bin/g++-12 && make -j8 && make install && cd ../..
RUN rm -fr benchmark-1.8.3/ v1.8.3.zip

WORKDIR /work

ENTRYPOINT ["/bin/bash"]

## ToDo:
## Move devtools (cmake, gdb, openssl, boost, etc) to concrete version!