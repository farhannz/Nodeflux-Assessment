FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8
ENV TZ="Asia/Jakarta"

RUN apt-get -y update && \
    apt-get install -y build-essential gdb wget git libssl-dev software-properties-common curl libcurl4-openssl-dev tzdata && \ 
    add-apt-repository universe && \
    apt-get install -y libboost-all-dev


COPY . /root

EXPOSE 3000
WORKDIR /root
RUN cd ~ && \
    wget https://github.com/nlohmann/json/releases/download/v3.10.5/json.hpp && \
    g++ main.cpp -lpthread -lcurl

CMD [ "./a.out" ]
