FROM ubuntu:18.04

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone  

RUN apt-get -y update  
RUN apt-get install qt5-default -y  
RUN apt-get install qtbase5-dev -y  
RUN apt-get install qt5-qmake 
RUN apt-get install build-essential -y

WORKDIR /root/
RUN mkdir server
WORKDIR /root/server/
COPY * /root/server/

RUN qmake echoServer.pro
RUN make

ENTRYPOINT ["./echoServer"]