FROM ubuntu:18.04

RUN apt-get update && apt-get -y install cmake build-essential

RUN mkdir -p /home/app
COPY . /home/app

WORKDIR /home/app/build
RUN cmake ..
RUN make
RUN ./BinaryTree

ENTRYPOINT /bin/bash

