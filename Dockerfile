FROM ubuntu:20.04 as builder

WORKDIR /build
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install build-essential cmake libncurses5-dev libncursesw5-dev -y
COPY src src
COPY CMakeLists.txt CMakeLists.txt

RUN mkdir build && cd build && cmake .. && make && mv snake /build/snake

FROM ubuntu:20.04
WORKDIR /app
COPY --from=builder /build/snake /app/snake
ENTRYPOINT ["/snake"]